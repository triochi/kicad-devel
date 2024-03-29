/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2012 Jean-Pierre Charras, jean-pierre.charras@ujf-grenoble.fr
 * Copyright (C) 2012 SoftPLC Corporation, Dick Hollenbeck <dick@softplc.com>
 * Copyright (C) 2012 Wayne Stambaugh <stambaughw@verizon.net>
 * Copyright (C) 1992-2012 KiCad Developers, see AUTHORS.txt for contributors.
*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @file deltrack.cpp
 */

#include <fctsys.h>
#include <class_drawpanel.h>
#include <confirm.h>
#include <wxPcbStruct.h>
#include <macros.h>
#include <ratsnest_data.h>

#include <class_board.h>
#include <class_track.h>

#include <pcbnew.h>
#include <protos.h>

#ifdef PCBNEW_WITH_TRACKITEMS
#include "trackitems/trackitems.h"
#endif


TRACK* PCB_EDIT_FRAME::Delete_Segment( wxDC* DC, TRACK* aTrack )
{
    if( aTrack == NULL )
        return NULL;

    if( aTrack->IsNew() )  // Trace in progress, erase the last segment
    {
        if( g_CurrentTrackList.GetCount() > 0 )
        {
            PCB_LAYER_ID previous_layer = GetActiveLayer();

            DBG( g_CurrentTrackList.VerifyListIntegrity(); )

            // Delete the current trace
            ShowNewTrackWhenMovingCursor( m_canvas, DC, wxDefaultPosition, false );

#ifdef PCBNEW_WITH_TRACKITEMS
            GetBoard()->TrackItems()->Teardrops()->RouteCreate_Stop();
            GetBoard()->TrackItems()->RoundedTracksCorners()->RouteCreate_Stop();
            for( TRACK* track = g_FirstTrackSegment; track;  )
            {
                TRACK* next_t = track->Next();
                if(track->Type() == PCB_ROUNDEDTRACKSCORNER_T)
                    GetBoard()->TrackItems()->RoundedTracksCorners()->Remove(track, false, true);
                if(track->Type() == PCB_TEARDROP_T)
                    GetBoard()->TrackItems()->Teardrops()->Remove(track, false, true);
                track = next_t;
            }
#endif
            // delete the most recently entered
            delete g_CurrentTrackList.PopBack();

            if( g_TwoSegmentTrackBuild )
            {
                // if in 2 track mode, and the next most recent is a segment
                // not a via, and the one previous to that is a via, then
                // delete up to the via.
                if( g_CurrentTrackList.GetCount() >= 2
                    && g_CurrentTrackSegment->Type() != PCB_VIA_T
                    && g_CurrentTrackSegment->Back()->Type() == PCB_VIA_T )
                {
                    delete g_CurrentTrackList.PopBack();
                }
            }

            while( g_CurrentTrackSegment && g_CurrentTrackSegment->Type() == PCB_VIA_T )
            {
                delete g_CurrentTrackList.PopBack();

                if( g_CurrentTrackSegment && g_CurrentTrackSegment->Type() != PCB_VIA_T )
                    previous_layer = g_CurrentTrackSegment->GetLayer();
            }

            // Correct active layer which could change if a via
            // has been erased
            SetActiveLayer( previous_layer );

            UpdateStatusBar();

            if( g_TwoSegmentTrackBuild )   // We must have 2 segments or more, or 0
            {
                if( g_CurrentTrackList.GetCount() == 1
                    && g_CurrentTrackSegment->Type() != PCB_VIA_T )
                {
                    delete g_CurrentTrackList.PopBack();
                }
            }

            if( g_CurrentTrackList.GetCount() == 0 )
            {
                m_canvas->SetMouseCapture( NULL, NULL );

                if( GetBoard()->IsHighLightNetON() )
                    HighLight( DC );

                SetCurItem( NULL );
                return NULL;
            }
            else
            {
#ifdef PCBNEW_WITH_TRACKITEMS
                PICKED_ITEMS_LIST aUndoRedoTempList;
                GetBoard()->TrackItems()->Teardrops()->Add(g_CurrentTrackSegment, &aUndoRedoTempList);
                TRACK* track = g_CurrentTrackSegment->Back();
                while( track )
                {
                    GetBoard()->TrackItems()->Teardrops()->Add(track, &aUndoRedoTempList);
                    track = track->Back();
                    if(track)
                        GetBoard()->TrackItems()->RoundedTracksCorners()->Add(track);
                }
                GetBoard()->TrackItems()->Teardrops()->RouteCreate_Start();
                GetBoard()->TrackItems()->RoundedTracksCorners()->RouteCreate_Start();
#endif
                if( m_canvas->IsMouseCaptured() )
                    m_canvas->CallMouseCapture( DC, wxDefaultPosition, false );

                return g_CurrentTrackSegment;
            }
        }
        return NULL;
    }

    int netcode = aTrack->GetNetCode();

#ifdef PCBNEW_WITH_TRACKITEMS
    PICKED_ITEMS_LIST pickedItems;
    ITEM_PICKER       picker( NULL, UR_DELETED );
    GetBoard()->TrackItems()->Teardrops()->Remove( aTrack, &pickedItems, true );
    GetBoard()->TrackItems()->RoundedTracksCorners()->Remove( aTrack, &pickedItems, true );
#endif

    // Remove the segment from list, but do not delete it (it will be stored i n undo list)
    GetBoard()->Remove( aTrack );

    GetBoard()->GetRatsnest()->Remove( aTrack );

    // redraw the area where the track was
    m_canvas->RefreshDrawingRect( aTrack->GetBoundingBox() );

#ifdef PCBNEW_WITH_TRACKITEMS
    picker.SetItem( aTrack );
    pickedItems.PushItem( picker );
    SaveCopyInUndoList( pickedItems, UR_DELETED );
#else
    SaveCopyInUndoList( aTrack, UR_DELETED );
#endif
    OnModify();
    TestNetConnection( DC, netcode );
    SetMsgPanel( GetBoard() );

    return NULL;
}


void PCB_EDIT_FRAME::Delete_Track( wxDC* DC, TRACK* aTrack )
{
    if( aTrack != NULL )
    {
        int netcode = aTrack->GetNetCode();
        Remove_One_Track( DC, aTrack );
        OnModify();
        TestNetConnection( DC, netcode );
    }
}


void PCB_EDIT_FRAME::Delete_net( wxDC* DC, TRACK* aTrack )
{
    if( aTrack == NULL )
        return;

    if( !IsOK( this, _( "Delete NET?" ) ) )
        return;

    PICKED_ITEMS_LIST itemsList;
    ITEM_PICKER       picker( NULL, UR_DELETED );
    int    netcode = aTrack->GetNetCode();

#ifdef PCBNEW_WITH_TRACKITEMS
    //Delete teardrops at this net code.
    GetBoard()->TrackItems()->Teardrops()->Remove( netcode, TEARDROPS::ALL_TYPES_T, &itemsList, true);
    GetBoard()->TrackItems()->RoundedTracksCorners()->Remove( netcode, &itemsList, true );
#endif

    /* Search the first item for the given net code */
    TRACK* trackList = GetBoard()->m_Track->GetStartNetCode( netcode );

    /* Remove all segments having the given net code */
    int    ii = 0;
    TRACK* next_track;
    for( TRACK* segm = trackList;  segm; segm = next_track, ++ii )
    {
        next_track = segm->Next();
        if( segm->GetNetCode() != netcode )
            break;

#ifdef PCBNEW_WITH_TRACKITEMS
        //Not stitch vias
        if(segm && dynamic_cast<VIA*>(segm))
            if( dynamic_cast<VIA*>(segm)->GetThermalCode() )
                continue;

        GetBoard()->TrackItems()->NetCodeFirstTrackItem()->Remove( segm );
#endif
        GetBoard()->GetRatsnest()->Remove( segm );
        GetBoard()->m_Track.Remove( segm );

        // redraw the area where the track was
        m_canvas->RefreshDrawingRect( segm->GetBoundingBox() );
        picker.SetItem( segm );
        itemsList.PushItem( picker );
    }

    SaveCopyInUndoList( itemsList, UR_DELETED );
    OnModify();
    TestNetConnection( DC, netcode );
    SetMsgPanel( GetBoard() );
}


void PCB_EDIT_FRAME::Remove_One_Track( wxDC* DC, TRACK* pt_segm )
{
    int segments_to_delete_count;

    if( pt_segm == NULL )
        return;

#ifdef PCBNEW_WITH_TRACKITEMS
    int net_code = pt_segm->GetNetCode();
    PICKED_ITEMS_LIST itemsList;
    ITEM_PICKER       picker( NULL, UR_DELETED );
    //Delete teardrops from whole net.
    GetBoard()->TrackItems()->Teardrops()->Remove( net_code, TEARDROPS::ALL_TYPES_T, &itemsList, true);
    GetBoard()->TrackItems()->RoundedTracksCorners()->Remove( net_code, &itemsList, true );
#endif

    TRACK* trackList = GetBoard()->MarkTrace( pt_segm, &segments_to_delete_count,
                                              NULL, NULL, true );

    if( segments_to_delete_count == 0 )
        return;

#ifndef PCBNEW_WITH_TRACKITEMS
    int net_code = pt_segm->GetNetCode();
    PICKED_ITEMS_LIST itemsList;
    ITEM_PICKER       picker( NULL, UR_DELETED );
#endif

    int               ii = 0;
    TRACK*            tracksegment = trackList;
    TRACK*            next_track;

    for( ; ii < segments_to_delete_count; ii++, tracksegment = next_track )
    {
        next_track = tracksegment->Next();
        tracksegment->SetState( BUSY, false );

        DBG( std::cout << __func__ << ": track " << tracksegment << " status=" \
                     << TO_UTF8( TRACK::ShowState( tracksegment->GetStatus() ) ) \
                     << std::endl; )

#ifdef PCBNEW_WITH_TRACKITEMS
        GetBoard()->TrackItems()->NetCodeFirstTrackItem()->Remove( tracksegment );
#endif

        GetBoard()->GetRatsnest()->Remove( tracksegment );
        GetBoard()->m_Track.Remove( tracksegment );

        // redraw the area where the track was
        m_canvas->RefreshDrawingRect( tracksegment->GetBoundingBox() );
        picker.SetItem( tracksegment );
        itemsList.PushItem( picker );
    }

#ifdef PCBNEW_WITH_TRACKITEMS
    //Add teardrops back those net tracks witch where not deleted.
    GetBoard()->TrackItems()->Teardrops()->Recreate( net_code, &itemsList );
    GetBoard()->TrackItems()->RoundedTracksCorners()->Recreate( net_code, &itemsList );
#endif

    SaveCopyInUndoList( itemsList, UR_DELETED );

    if( net_code > 0 )
        TestNetConnection( DC, net_code );
}
