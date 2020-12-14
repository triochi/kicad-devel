/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2016 Jean-Pierre Charras, jp.charras at wanadoo.fr
 * Copyright (C) 2015 SoftPLC Corporation, Dick Hollenbeck <dick@softplc.com>
 * Copyright (C) 2015 Wayne Stambaugh <stambaughw@verizon.net>
 * Copyright (C) 2017 KiCad Developers, see AUTHORS.txt for contributors.
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
 * @file edit.cpp
 * @brief Edit PCB implementation.
 */

#include <fctsys.h>
#include <pgm_base.h>
#include <kiface_i.h>
#include <class_drawpanel.h>
#include <confirm.h>
#include <eda_doc.h>
#include <gestfich.h>
#include <kicad_device_context.h>
#include <wxPcbStruct.h>

#include <pcbnew_id.h>
#include <pcbnew.h>
#include <module_editor_frame.h>

#include <class_board.h>
#include <class_module.h>
#include <class_track.h>
#include <class_zone.h>
#include <class_pcb_text.h>
#include <modview_frame.h>
#include <class_pcb_layer_box_selector.h>
#include <dialog_drc.h>
#include <dialog_global_edit_tracks_and_vias.h>
#include <invoke_pcb_dialog.h>
#include <array_creator.h>

#include <dialog_move_exact.h>

#include <tool/tool_manager.h>
#include <tools/pcb_actions.h>

#ifdef PCBNEW_WITH_TRACKITEMS
#include "trackitems/viastitching.h"
#include "trackitems/trackitems.h"
#endif


// Handles the selection of command events.
void PCB_EDIT_FRAME::Process_Special_Functions( wxCommandEvent& event )
{
    int         id = event.GetId();

    INSTALL_UNBUFFERED_DC( dc, m_canvas );
    MODULE* module;
    DISPLAY_OPTIONS* displ_opts = (DISPLAY_OPTIONS*)GetDisplayOptions();

    m_canvas->CrossHairOff( &dc );

    switch( id )   // Some (not all ) edit commands must be finished or aborted
    {
    case wxID_CUT:
    case wxID_COPY:
    case ID_PCB_USER_GRID_SETUP:
    case ID_TOOLBARH_PCB_SELECT_LAYER:
    case ID_AUX_TOOLBAR_PCB_SELECT_LAYER_PAIR:
    case ID_POPUP_PCB_ROTATE_TEXTEPCB:
    case ID_POPUP_PCB_FLIP_TEXTEPCB:
    case ID_POPUP_PCB_COPY_TEXTEPCB:
    case ID_POPUP_PCB_EDIT_TEXTEPCB:
    case ID_POPUP_PCB_EDIT_MIRE:
    case ID_POPUP_PCB_ROTATE_TEXTMODULE:
    case ID_POPUP_PCB_ROTATE_MODULE_CLOCKWISE:
    case ID_POPUP_PCB_ROTATE_MODULE_COUNTERCLOCKWISE:
    case ID_POPUP_PCB_CHANGE_SIDE_MODULE:
    case ID_POPUP_PCB_EDIT_MODULE_PRMS:
    case ID_POPUP_PCB_EDIT_MODULE_WITH_MODEDIT:
    case ID_POPUP_PCB_EDIT_TEXTMODULE:
    case ID_POPUP_PCB_STOP_CURRENT_DRAWING:
    case ID_POPUP_PCB_BEGIN_TRACK:
    case ID_POPUP_PCB_END_TRACK:
    case ID_POPUP_PCB_PLACE_THROUGH_VIA:
    case ID_POPUP_PCB_SELECT_CU_LAYER_AND_PLACE_THROUGH_VIA:
    case ID_POPUP_PCB_PLACE_BLIND_BURIED_VIA:
    case ID_POPUP_PCB_SELECT_CU_LAYER_AND_PLACE_BLIND_BURIED_VIA:
    case ID_POPUP_PCB_PLACE_MICROVIA:
    case ID_POPUP_PCB_SWITCH_TRACK_POSTURE:
    case ID_POPUP_PCB_APPLY_PAD_SETTINGS:
    case ID_POPUP_PCB_COPY_PAD_SETTINGS:
    case ID_POPUP_PCB_GLOBAL_IMPORT_PAD_SETTINGS:
    case ID_POPUP_PCB_STOP_CURRENT_EDGE_ZONE:
    case ID_POPUP_PCB_DELETE_ZONE_LAST_CREATED_CORNER:
    case ID_POPUP_PCB_FILL_ALL_ZONES:
    case ID_POPUP_PCB_REMOVE_FILLED_AREAS_IN_ALL_ZONES:
    case ID_POPUP_PCB_REMOVE_FILLED_AREAS_IN_CURRENT_ZONE:
    case ID_POPUP_PCB_PLACE_ZONE_CORNER:
    case ID_POPUP_PCB_PLACE_ZONE_OUTLINES:
    case ID_POPUP_PCB_EDIT_ZONE_PARAMS:
    case ID_POPUP_PCB_DELETE_ZONE:
    case ID_POPUP_PCB_DELETE_ZONE_CORNER:
    case ID_POPUP_PCB_MOVE_ZONE_CORNER:
    case ID_POPUP_PCB_DRAG_ZONE_OUTLINE_SEGMENT:
    case ID_POPUP_PCB_PLACE_DRAGGED_ZONE_OUTLINE_SEGMENT:
    case ID_POPUP_PCB_MOVE_ZONE_OUTLINES:
    case ID_POPUP_PCB_ADD_ZONE_CORNER:
    case ID_POPUP_PCB_DELETE_TRACKSEG:
    case ID_POPUP_PCB_DELETE_TRACK:
    case ID_POPUP_PCB_DELETE_TRACKNET:
    case ID_POPUP_PCB_FILL_ZONE:
    case ID_POPUP_PCB_SELECT_LAYER:
    case ID_POPUP_PCB_SELECT_CU_LAYER:
    case ID_POPUP_PCB_SELECT_LAYER_PAIR:
    case ID_POPUP_PCB_SELECT_NO_CU_LAYER:
    case ID_POPUP_PCB_MOVE_TRACK_NODE:
    case ID_POPUP_PCB_MOVE_TEXTEPCB_REQUEST:
    case ID_POPUP_PCB_DRAG_TRACK_SEGMENT_KEEP_SLOPE:
    case ID_POPUP_PCB_DRAG_TRACK_SEGMENT:
    case ID_POPUP_PCB_MOVE_TRACK_SEGMENT:
    case ID_POPUP_PCB_PLACE_MOVED_TRACK_NODE:
    case ID_POPUP_PCB_BREAK_TRACK:
    case ID_POPUP_PCB_EDIT_NET:
    case ID_POPUP_PCB_EDIT_TRACK:
    case ID_POPUP_PCB_EDIT_TRACKSEG:
    case ID_POPUP_PCB_LOCK_ON_TRACKSEG:
    case ID_POPUP_PCB_LOCK_OFF_TRACKSEG:
    case ID_POPUP_PCB_LOCK_ON_TRACK:
    case ID_POPUP_PCB_LOCK_OFF_TRACK:
    case ID_POPUP_PCB_LOCK_ON_NET:
    case ID_POPUP_PCB_LOCK_OFF_NET:
    case ID_POPUP_DELETE_BLOCK:
    case ID_POPUP_PLACE_BLOCK:
    case ID_POPUP_ZOOM_BLOCK:
    case ID_POPUP_FLIP_BLOCK:
    case ID_POPUP_ROTATE_BLOCK:
    case ID_POPUP_COPY_BLOCK:
    case ID_POPUP_PCB_EDIT_DRAWING:
    case ID_POPUP_PCB_GETINFO_MARKER:
    case ID_POPUP_PCB_MOVE_TEXT_DIMENSION_REQUEST:
    case ID_POPUP_PCB_DRAG_MODULE_REQUEST:
    case ID_POPUP_PCB_MOVE_MODULE_REQUEST:
    case ID_POPUP_PCB_MOVE_TEXTMODULE_REQUEST:
    case ID_POPUP_PCB_MOVE_MIRE_REQUEST:
    case ID_POPUP_PCB_DELETE_MARKER:
#ifdef PCBNEW_WITH_TRACKITEMS
    case ID_POPUP_PCB_PLACE_ZONE_THROUGH_VIA:
    case ID_POPUP_PCB_PLACE_ZONE_BLIND_BURIED_VIA:
    case ID_POPUP_PCB_SEL_LAYERS_AND_PLACE_ZONE_BLIND_BURIED_VIA:
    case ID_POPUP_PCB_SEL_LAYER_AND_PLACE_ZONE_THROUGH_VIA:
    case ID_POPUP_PCB_CREATE_VIA_ARRAY:
    //Teardrops
    case ID_POPUP_PCB_TEARDROP_COPYCURRENT:
    case ID_POPUP_PCB_TEARDROP_LOCK_TOGGLE:
    case ID_POPUP_PCB_TEARDROPS_LOCK_ALL_SAME:
    case ID_POPUP_PCB_TEARDROPS_UNLOCK_ALL_SAME:
    case ID_POPUP_PCB_TEARDROPS_LOCK_MODULES_ALL:
    case ID_POPUP_PCB_TEARDROPS_LOCK_VIAS_ALL:
    case ID_POPUP_PCB_TJUNCTIONS_LOCK_ALL:
    case ID_POPUP_PCB_JUNCTIONS_LOCK_ALL:
    case ID_POPUP_PCB_TEARDROPS_UNLOCK_MODULES_ALL:
    case ID_POPUP_PCB_TEARDROPS_UNLOCK_VIAS_ALL:
    case ID_POPUP_PCB_TJUNCTIONS_UNLOCK_ALL:
    case ID_POPUP_PCB_JUNCTIONS_UNLOCK_ALL:
    case ID_POPUP_PCB_TEARDROPS_LOCK_MODULE:
    case ID_POPUP_PCB_TEARDROPS_UNLOCK_MODULE:
    case ID_POPUP_PCB_TEARDROPS_LOCK:
    case ID_POPUP_PCB_TEARDROPS_UNLOCK:
    case ID_POPUP_PCB_TEARDROP_EDIT_LENGTH:
    case ID_POPUP_PCB_TEARDROP_EDIT_WIDTH:
    case ID_POPUP_PCB_TEARDROP_SET_DEFAULT_PARAMS:
    case ID_POPUP_PCB_TEARDROP_SELECT_TEARDROP:
    case ID_POPUP_PCB_TEARDROP_SELECT_FILLET:
    case ID_POPUP_PCB_TEARDROP_SELECT_SUBLAND:
    case ID_POPUP_PCB_TEARDROP_SELECT_ZERO:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 1:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 2:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 3:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 4:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 5:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 6:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 7:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 8:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 9:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 10:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 11:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 12:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 13:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 14:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 15:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 16:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 17:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 18:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_LAST:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 1:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 2:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 3:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 4:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 5:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 6:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 7:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 8:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 9:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 10:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 11:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 12:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 13:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 14:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 15:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 16:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 17:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 18:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_LAST:
    case ID_POPUP_PCB_TEARDROP_NUM_SEGMENTS_FIRST:
    case ID_POPUP_PCB_TEARDROP_NUM_SEGMENTS_FIRST + 1:
    case ID_POPUP_PCB_TEARDROP_NUM_SEGMENTS_FIRST + 2:
    case ID_POPUP_PCB_TEARDROP_NUM_SEGMENTS_FIRST + 3:
    case ID_POPUP_PCB_TEARDROP_NUM_SEGMENTS_FIRST + 4:
    case ID_POPUP_PCB_TEARDROP_NUM_SEGMENTS_FIRST + 5:
    case ID_POPUP_PCB_TEARDROP_NUM_SEGMENTS_FIRST + 6:
    case ID_POPUP_PCB_TEARDROP_NUM_SEGMENTS_FIRST + 7:
    case ID_POPUP_PCB_TEARDROP_NUM_SEGMENTS_FIRST + 8:
    case ID_POPUP_PCB_TEARDROP_NUM_SEGMENTS_LAST:
    case ID_POPUP_PCB_TEARDROP_PLACE:
    case ID_POPUP_PCB_TEARDROPS_PLACE:
    case ID_POPUP_PCB_TEARDROP_CHANGE:
    case ID_POPUP_PCB_TEARDROPS_CHANGE:
    case ID_POPUP_PCB_TEARDROP_DELETE:
    case ID_POPUP_PCB_TEARDROPS_DELETE:
    case ID_POPUP_PCB_TEARDROPS_PLACE_MODULE:
    case ID_POPUP_PCB_TEARDROPS_CHANGE_MODULE:
    case ID_POPUP_PCB_TEARDROPS_DELETE_MODULE:
    case ID_POPUP_PCB_TEARDROPS_PLACE_VIAS_ALL:
    case ID_POPUP_PCB_TEARDROPS_CHANGE_VIAS_ALL:
    case ID_POPUP_PCB_TEARDROPS_DELETE_VIAS_ALL:
    case ID_POPUP_PCB_TEARDROPS_PLACE_MODULES_ALL:
    case ID_POPUP_PCB_TEARDROPS_CHANGE_MODULES_ALL:
    case ID_POPUP_PCB_TEARDROPS_DELETE_MODULES_ALL:
    case ID_POPUP_PCB_TEARDROPS_PLACE_NET:
    case ID_POPUP_PCB_TEARDROPS_CHANGE_NET:
    case ID_POPUP_PCB_TEARDROPS_DELETE_NET:
    case ID_POPUP_PCB_TJUNCTIONS_PLACE_ALL:
    case ID_POPUP_PCB_TJUNCTIONS_CHANGE_ALL:
    case ID_POPUP_PCB_TJUNCTIONS_DELETE_ALL:
    case ID_POPUP_PCB_TJUNCTIONS_MARK_NOT_T:
    case ID_POPUP_PCB_JUNCTIONS_PLACE_ALL:
    case ID_POPUP_PCB_JUNCTIONS_CHANGE_ALL:
    case ID_POPUP_PCB_JUNCTIONS_DELETE_ALL:
    case ID_POPUP_PCB_TJUNCTIONS_PLACE_NET:
    case ID_POPUP_PCB_TJUNCTIONS_CHANGE_NET:
    case ID_POPUP_PCB_TJUNCTIONS_DELETE_NET:
    case ID_POPUP_PCB_JUNCTIONS_PLACE_NET:
    case ID_POPUP_PCB_JUNCTIONS_CHANGE_NET:
    case ID_POPUP_PCB_JUNCTIONS_DELETE_NET:
    case ID_POPUP_PCB_TEARDROPS_MARK_WARNINGS_MODULES:
    case ID_POPUP_PCB_TEARDROPS_MARK_WARNINGS_TEARDROPS_VIAS:
    case ID_POPUP_PCB_TEARDROPS_MARK_WARNINGS_TJUNCTIONS:
    case ID_POPUP_PCB_TEARDROPS_MARK_WARNINGS_JUNCTIONS:
    case ID_POPUP_PCB_TEARDROPS_MARK_DIFF_MODULES:
    case ID_POPUP_PCB_TEARDROPS_MARK_DIFF_TEARDROPS_VIAS:
    case ID_POPUP_PCB_TEARDROPS_MARK_DIFF_TJUNCTIONS:
    case ID_POPUP_PCB_TEARDROPS_MARK_DIFF_JUNCTIONS:
    case ID_POPUP_PCB_TEARDROPS_MARK_CURR_MODULES:
    case ID_POPUP_PCB_TEARDROPS_MARK_CURR_TEARDROPS_VIAS:
    case ID_POPUP_PCB_TEARDROPS_MARK_CURR_TJUNCTIONS:
    case ID_POPUP_PCB_TEARDROPS_MARK_CURR_JUNCTIONS:
    case ID_POPUP_PCB_VIA_PICK_SIZE:
    case ID_POPUP_PCB_TRACK_PICK_WIDTH:
    case ID_POPUP_PCB_TRACKS_MARK_SHARP_ANGLES:
        //rounded corners
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNER_PLACE:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNER_PLACE_NET:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNER_DELETE:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNER_DELETE_NET:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNER_CHANGE:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNER_COPYCURRENT:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_SET_FIRST:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_SET_FIRST + 1:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_SET_FIRST + 2:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_SET_FIRST + 3:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_SET_FIRST + 4:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_SET_FIRST + 5:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_SET_FIRST + 6:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_SET_FIRST + 7:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_SET_FIRST + 8:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_SET_FIRST + 9:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_SET_LAST:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_RATIO_FIRST:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_RATIO_FIRST + 1:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_RATIO_FIRST + 2:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_RATIO_FIRST + 3:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_RATIO_FIRST + 4:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_RATIO_FIRST + 5:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_RATIO_FIRST + 6:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_RATIO_FIRST + 7:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_RATIO_FIRST + 8:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_RATIO_FIRST + 9:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_RATIO_LAST:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SET_DEFAULT_PARAMS:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_EDIT_LENGTH_SET:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_EDIT_LENGTH_RATIO:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNER_CONVERT_SEGMENTED:
#endif
        break;

    case ID_POPUP_CANCEL_CURRENT_COMMAND:
        if( m_canvas->IsMouseCaptured() )
        {
            m_canvas->EndMouseCapture();
        }

        // Should not be executed, just in case
        if( GetScreen()->m_BlockLocate.GetCommand() != BLOCK_IDLE )
        {
            GetScreen()->m_BlockLocate.SetCommand( BLOCK_IDLE );
            GetScreen()->m_BlockLocate.SetState( STATE_NO_BLOCK );
            GetScreen()->m_BlockLocate.ClearItemsList();
        }

        if( GetToolId() == ID_NO_TOOL_SELECTED )
            SetNoToolSelected();
        else
            SetCursor( (wxStockCursor) m_canvas->GetDefaultCursor() );

        break;

    default:        // Finish (abort) the command
        if( m_canvas->IsMouseCaptured() )
            m_canvas->CallEndMouseCapture( &dc );

        if( GetToolId() != id )
        {
            if( m_lastDrawToolId != GetToolId() )
                m_lastDrawToolId = GetToolId();

            SetNoToolSelected();
        }
        break;
    }

    switch( id )   // Execute command
    {
    case 0:
        break;

    case ID_OPEN_MODULE_EDITOR:
        {
            FOOTPRINT_EDIT_FRAME* editor = (FOOTPRINT_EDIT_FRAME*) Kiway().Player( FRAME_PCB_MODULE_EDITOR, false );

            if( !editor )
            {
                editor = (FOOTPRINT_EDIT_FRAME*) Kiway().Player( FRAME_PCB_MODULE_EDITOR, true );
                editor->Zoom_Automatique( false );
            }
            else
            {
                // Needed on Windows, other platforms do not use it,
                // but it creates no issue
                if( editor->IsIconized() )
                     editor->Iconize( false );

                editor->Raise();

                // Raising the window does not set the focus on Linux.  This should work on
                // any platform.
                if( wxWindow::FindFocus() != editor )
                    editor->SetFocus();
            }

            editor->PushPreferences( m_canvas );
        }
        break;

    case ID_OPEN_MODULE_VIEWER:
        {
            FOOTPRINT_VIEWER_FRAME* viewer = (FOOTPRINT_VIEWER_FRAME*) Kiway().Player( FRAME_PCB_MODULE_VIEWER, false );

            if( !viewer )
            {
                viewer = (FOOTPRINT_VIEWER_FRAME*) Kiway().Player( FRAME_PCB_MODULE_VIEWER, true );
                viewer->Zoom_Automatique( false );
            }
            else
            {
                // Needed on Windows, other platforms do not use it,
                // but it creates no issue
                if( viewer->IsIconized() )
                     viewer->Iconize( false );

                viewer->Raise();

                // Raising the window does not set the focus on Linux.  This should work on
                // any platform.
                if( wxWindow::FindFocus() != viewer )
                    viewer->SetFocus();
            }

            viewer->PushPreferences( m_canvas );
        }
        break;

    case ID_PCB_GLOBAL_DELETE:
        InstallPcbGlobalDeleteFrame( wxDefaultPosition );
        break;

    case ID_POPUP_PLACE_BLOCK:
        GetScreen()->m_BlockLocate.SetCommand( BLOCK_MOVE );
        m_canvas->SetAutoPanRequest( false );
        HandleBlockPlace( &dc );
        break;

    case ID_POPUP_COPY_BLOCK:
        GetScreen()->m_BlockLocate.SetCommand( BLOCK_COPY );
        GetScreen()->m_BlockLocate.SetMessageBlock( this );
        m_canvas->SetAutoPanRequest( false );
        HandleBlockPlace( &dc );
        break;

    case ID_POPUP_ZOOM_BLOCK:
        GetScreen()->m_BlockLocate.SetCommand( BLOCK_ZOOM );
        GetScreen()->m_BlockLocate.SetMessageBlock( this );
        HandleBlockEnd( &dc );
        break;

    case ID_POPUP_DELETE_BLOCK:
        GetScreen()->m_BlockLocate.SetCommand( BLOCK_DELETE );
        GetScreen()->m_BlockLocate.SetMessageBlock( this );
        HandleBlockEnd( &dc );
        break;

    case ID_POPUP_ROTATE_BLOCK:
        GetScreen()->m_BlockLocate.SetCommand( BLOCK_ROTATE );
        GetScreen()->m_BlockLocate.SetMessageBlock( this );
        HandleBlockEnd( &dc );
        break;

    case ID_POPUP_FLIP_BLOCK:
        GetScreen()->m_BlockLocate.SetCommand( BLOCK_FLIP );
        GetScreen()->m_BlockLocate.SetMessageBlock( this );
        HandleBlockEnd( &dc );
        break;

    case ID_DRC_CONTROL:
        // Shows the DRC dialog in non modal mode, to allows board edition
        // with the DRC dialog opened and showing errors.
        m_drc->ShowDRCDialog();
        break;

    case ID_GET_NETLIST:
        InstallNetlistFrame( &dc );
        break;

    case ID_FIND_ITEMS:
        InstallFindFrame();
        break;

    case ID_POPUP_CLOSE_CURRENT_TOOL:
        SetNoToolSelected();
        break;

    case ID_POPUP_CANCEL_CURRENT_COMMAND:
        break;

    case ID_POPUP_PCB_END_LINE:
        m_canvas->MoveCursorToCrossHair();

        //  EndSegment(&dc);
        break;

    case ID_POPUP_PCB_EDIT_TRACK:
        if( GetCurItem() == NULL )
            break;
        Edit_Track_Width( &dc, (TRACK*) GetCurItem() );
        m_canvas->MoveCursorToCrossHair();
        OnModify();
        break;

    case ID_POPUP_PCB_EDIT_TRACKSEG:
        if( GetCurItem() == NULL )
            break;
        Edit_TrackSegm_Width( &dc, (TRACK*) GetCurItem() );
        m_canvas->MoveCursorToCrossHair();
        OnModify();
        break;

    case ID_PCB_EDIT_ALL_VIAS_AND_TRACK_SIZE:
        {
        DIALOG_GLOBAL_EDIT_TRACKS_AND_VIAS dlg( this, GetBoard()->GetHighLightNetCode() );
        dlg.ShowModal();
        }
        break;

    case ID_POPUP_PCB_EDIT_ALL_VIAS_AND_TRACK_SIZE:
        if( GetCurItem() == NULL )
            break;
        {
        int type = GetCurItem()->Type();

        if( type == PCB_TRACE_T || type == PCB_VIA_T )
        {
            BOARD_CONNECTED_ITEM*item = (BOARD_CONNECTED_ITEM*) GetCurItem();
            DIALOG_GLOBAL_EDIT_TRACKS_AND_VIAS dlg( this, item->GetNetCode() );
            dlg.ShowModal();
        }

        }
        m_canvas->MoveCursorToCrossHair();
        break;

    case ID_POPUP_PCB_BEGIN_TRACK:
        m_canvas->MoveCursorToCrossHair();
        OnHotkeyBeginRoute( &dc );
        break;

    case ID_POPUP_PCB_END_TRACK:
        m_canvas->MoveCursorToCrossHair();
        End_Route( (TRACK*) GetCurItem(), &dc );
        break;

    case ID_POPUP_PCB_PLACE_MOVED_TRACK_NODE:
        m_canvas->MoveCursorToCrossHair();

        if( GetCurItem()->IsDragging() )
        {
            PlaceDraggedOrMovedTrackSegment( (TRACK*) GetCurItem(), &dc );
        }

        break;

    case ID_POPUP_PCB_SWITCH_TRACK_POSTURE:
        /* change the position of initial segment when creating new tracks
         * switch from _/  to -\ .
         * If a track is in progress, it will be redrawn
        */
        if( m_canvas->IsMouseCaptured() )
            m_canvas->CallMouseCapture( &dc, wxDefaultPosition, false );

#ifdef PCBNEW_WITH_TRACKITEMS
        if(TrackNodeItem::ROUNDED_TRACKS_CORNER* cor = GetBoard()->TrackItems()->RoundedTracksCorners()->GetEditCorner())
            cor->Draw( m_canvas, &dc, GR_XOR );
#endif

        g_Alternate_Track_Posture = !g_Alternate_Track_Posture;

        if( m_canvas->IsMouseCaptured() )
            m_canvas->CallMouseCapture( &dc, wxDefaultPosition, false );

#ifdef PCBNEW_WITH_TRACKITEMS
        if(TrackNodeItem::ROUNDED_TRACKS_CORNER* cor = GetBoard()->TrackItems()->RoundedTracksCorners()->GetEditCorner())
            cor->Draw( m_canvas, &dc, GR_XOR );
#endif

        break;

    case ID_POPUP_PCB_PLACE_MICROVIA:
        if( !IsMicroViaAcceptable() )
            break;
        // fall through
    case ID_POPUP_PCB_PLACE_BLIND_BURIED_VIA:
    case ID_POPUP_PCB_PLACE_THROUGH_VIA:
    case ID_POPUP_PCB_SELECT_CU_LAYER_AND_PLACE_THROUGH_VIA:
    case ID_POPUP_PCB_SELECT_CU_LAYER_AND_PLACE_BLIND_BURIED_VIA:
        m_canvas->MoveCursorToCrossHair();

        if( GetCurItem()->IsDragging() )
        {
            PlaceDraggedOrMovedTrackSegment( (TRACK*) GetCurItem(), &dc );
        }
        else
        {
            BOARD_DESIGN_SETTINGS &settings = GetDesignSettings();
            VIATYPE_T v_type = settings.m_CurrentViaType;
            switch( id )
            {
            case ID_POPUP_PCB_PLACE_BLIND_BURIED_VIA:
            case ID_POPUP_PCB_SELECT_CU_LAYER_AND_PLACE_BLIND_BURIED_VIA:
                settings.m_CurrentViaType = VIA_BLIND_BURIED;
                break;

            case ID_POPUP_PCB_PLACE_MICROVIA:
                settings.m_CurrentViaType = VIA_MICROVIA;
                break;

            default:
                settings.m_CurrentViaType = VIA_THROUGH;
                break;
            }

            // place via and switch layer.
            if( id == ID_POPUP_PCB_SELECT_CU_LAYER_AND_PLACE_THROUGH_VIA ||
                id == ID_POPUP_PCB_SELECT_CU_LAYER_AND_PLACE_BLIND_BURIED_VIA )
            {
                m_canvas->SetIgnoreMouseEvents( true );

                wxPoint dlgPosition;

                wxGetMousePosition( &dlgPosition.x, &dlgPosition.y );

                PCB_LAYER_ID layer = SelectLayer( GetActiveLayer(), LSET::AllNonCuMask(), dlgPosition );

                m_canvas->SetIgnoreMouseEvents( false );
                m_canvas->MoveCursorToCrossHair();

                if( GetActiveLayer() != layer )
                {
                    GetScreen()->m_Route_Layer_TOP    = GetActiveLayer();
                    GetScreen()->m_Route_Layer_BOTTOM = layer;
                    Other_Layer_Route( (TRACK*) GetCurItem(), &dc );
                }
            }

            else
                Other_Layer_Route( (TRACK*) GetCurItem(), &dc );

            settings.m_CurrentViaType = v_type;

            if( displ_opts->m_ContrastModeDisplay )
                m_canvas->Refresh();
        }
        break;

    case ID_POPUP_PCB_DELETE_TRACKSEG:
        if( GetCurItem() == NULL )
            break;

        m_canvas->MoveCursorToCrossHair();
        SetCurItem( Delete_Segment( &dc, (TRACK*) GetCurItem() ) );
        OnModify();
        break;

    case ID_POPUP_PCB_DELETE_TRACK:
        if( GetCurItem() == NULL )
            break;
        m_canvas->MoveCursorToCrossHair();
        Delete_Track( &dc, (TRACK*) GetCurItem() );
        SetCurItem( NULL );
        OnModify();
        break;

    case ID_POPUP_PCB_DELETE_TRACKNET:
        m_canvas->MoveCursorToCrossHair();
        Delete_net( &dc, (TRACK*) GetCurItem() );
        SetCurItem( NULL );
        OnModify();
        break;

    case ID_POPUP_PCB_LOCK_ON_TRACKSEG:
        Attribut_Segment( (TRACK*) GetCurItem(), &dc, true );
        break;

    case ID_POPUP_PCB_LOCK_OFF_TRACKSEG:
        Attribut_Segment( (TRACK*) GetCurItem(), &dc, false );
        break;

    case ID_POPUP_PCB_LOCK_ON_TRACK:
        Attribut_Track( (TRACK*) GetCurItem(), &dc, true );
        break;

    case ID_POPUP_PCB_LOCK_OFF_TRACK:
        Attribut_Track( (TRACK*) GetCurItem(), &dc, false );
        break;

    case ID_POPUP_PCB_LOCK_ON_NET:
        Attribut_net( &dc, ( (TRACK*) GetCurItem() )->GetNetCode(), true );
        break;

    case ID_POPUP_PCB_LOCK_OFF_NET:
        Attribut_net( &dc, ( (TRACK*) GetCurItem() )->GetNetCode(), false );
        break;

    case ID_POPUP_PCB_SETFLAGS_TRACK_MNU:
        break;

    case ID_POPUP_PCB_DELETE_ZONE:
        m_canvas->MoveCursorToCrossHair();

        if( GetCurItem() == NULL )
            break;

        {
            SEGZONE* zsegm   = (SEGZONE*) GetCurItem();
            int      netcode = zsegm->GetNetCode();
            Delete_OldZone_Fill( zsegm );
            SetCurItem( NULL );
            TestNetConnection( NULL, netcode );
            OnModify();
            SetMsgPanel( GetBoard() );
        }
        break;

    case ID_POPUP_PCB_EDIT_ZONE_PARAMS:
        Edit_Zone_Params( &dc, (ZONE_CONTAINER*) GetCurItem() );
        SetCurItem( NULL ); // Outlines can have changed
        break;

    case ID_POPUP_PCB_ZONE_DUPLICATE:
        {
            ZONE_CONTAINER* zone = (ZONE_CONTAINER*) GetCurItem();
            duplicateZone( &dc, zone );
        }
        break;

    case ID_POPUP_PCB_ZONE_ADD_SIMILAR_ZONE:
        m_canvas->MoveCursorToCrossHair();
        m_canvas->SetAutoPanRequest( true );
        Add_Similar_Zone( &dc, (ZONE_CONTAINER*) GetCurItem() );
        break;

    case ID_POPUP_PCB_ZONE_ADD_CUTOUT_ZONE:
        m_canvas->MoveCursorToCrossHair();
        m_canvas->SetAutoPanRequest( true );
        Add_Zone_Cutout( &dc, (ZONE_CONTAINER*) GetCurItem() );
        break;

    case ID_POPUP_PCB_DELETE_ZONE_CONTAINER:
        // Force the main contour selection, to remove the entire zone:
        ((ZONE_CONTAINER*) GetCurItem())->SetSelectedCorner( 0 );
        // Fall through
    case ID_POPUP_PCB_DELETE_ZONE_CUTOUT:
        m_canvas->MoveCursorToCrossHair();
        {
            int netcode = ( (ZONE_CONTAINER*) GetCurItem() )->GetNetCode();
            Delete_Zone_Contour( &dc, (ZONE_CONTAINER*) GetCurItem() );
            SetCurItem( NULL );
            TestNetConnection( NULL, netcode );
            SetMsgPanel( GetBoard() );
        }
        break;

    case ID_POPUP_PCB_DELETE_ZONE_CORNER:
        Remove_Zone_Corner( &dc, (ZONE_CONTAINER*) GetCurItem() );
        SetCurItem( NULL );
        break;

    case ID_POPUP_PCB_MOVE_ZONE_CORNER:
        {
            m_canvas->MoveCursorToCrossHair();
            ZONE_CONTAINER* zone_cont = (ZONE_CONTAINER*) GetCurItem();
            m_canvas->SetAutoPanRequest( true );
            Start_Move_Zone_Corner( &dc, zone_cont, zone_cont->GetSelectedCorner(), false );
        }
        break;

    case ID_POPUP_PCB_DRAG_ZONE_OUTLINE_SEGMENT:
        {
            m_canvas->MoveCursorToCrossHair();
            ZONE_CONTAINER* zone_cont = (ZONE_CONTAINER*) GetCurItem();
            m_canvas->SetAutoPanRequest( true );
            Start_Move_Zone_Drag_Outline_Edge( &dc, zone_cont, zone_cont->GetSelectedCorner() );
        }
        break;

    case ID_POPUP_PCB_MOVE_ZONE_OUTLINES:
        {
            m_canvas->MoveCursorToCrossHair();
            ZONE_CONTAINER* zone_cont = (ZONE_CONTAINER*) GetCurItem();
            m_canvas->SetAutoPanRequest( true );
            Start_Move_Zone_Outlines( &dc, zone_cont );
        }
        break;

    case ID_POPUP_PCB_ADD_ZONE_CORNER:
        {
            m_canvas->MoveCursorToCrossHair();
            ZONE_CONTAINER* zone_cont = (ZONE_CONTAINER*) GetCurItem();
            wxPoint         pos = GetCrossHairPosition();

            /* add corner between zone_cont->m_CornerSelection
             * and zone_cont->m_CornerSelection+1
             * and start move the new corner
             */
            zone_cont->Draw( m_canvas, &dc, GR_XOR );
            zone_cont->Outline()->InsertVertex( zone_cont->GetSelectedCorner(), pos );
            zone_cont->SetSelectedCorner( zone_cont->GetSelectedCorner() + 1 );
            zone_cont->Draw( m_canvas, &dc, GR_XOR );
            m_canvas->SetAutoPanRequest( true );
            Start_Move_Zone_Corner( &dc, zone_cont, zone_cont->GetSelectedCorner(), true );
        }
        break;

    case ID_POPUP_PCB_PLACE_ZONE_OUTLINES:
    case ID_POPUP_PCB_PLACE_ZONE_CORNER:
    case ID_POPUP_PCB_PLACE_DRAGGED_ZONE_OUTLINE_SEGMENT:
        {
            m_canvas->MoveCursorToCrossHair();
            ZONE_CONTAINER* zone_cont = (ZONE_CONTAINER*) GetCurItem();
            End_Move_Zone_Corner_Or_Outlines( &dc, zone_cont );
            m_canvas->SetAutoPanRequest( false );
        }
        break;

    case ID_POPUP_PCB_FILL_ALL_ZONES:
        m_canvas->MoveCursorToCrossHair();
        Fill_All_Zones( this );
        m_canvas->Refresh();
        SetMsgPanel( GetBoard() );
        break;

    case ID_POPUP_PCB_REMOVE_FILLED_AREAS_IN_CURRENT_ZONE:
        if( ( GetCurItem() )->Type() == PCB_ZONE_AREA_T )
        {
            ZONE_CONTAINER* zone_container = (ZONE_CONTAINER*) GetCurItem();
            zone_container->UnFill();
            TestNetConnection( NULL, zone_container->GetNetCode() );
            OnModify();
            SetMsgPanel( GetBoard() );
            m_canvas->Refresh();
        }
        SetCurItem( NULL );
        break;

    case ID_POPUP_PCB_REMOVE_FILLED_AREAS_IN_ALL_ZONES: // Remove all zones :
        GetBoard()->m_Zone.DeleteAll();                 // remove zone segments used to fill zones.

        for( int ii = 0; ii < GetBoard()->GetAreaCount(); ii++ )
        {
            // Remove filled areas in zone
            ZONE_CONTAINER* zone_container = GetBoard()->GetArea( ii );
            zone_container->UnFill();
        }

        SetCurItem( NULL );        // CurItem might be deleted by this command, clear the pointer
        TestConnections();
        TestForActiveLinksInRatsnest( 0 );   // Recalculate the active ratsnest, i.e. the unconnected links
        OnModify();
        SetMsgPanel( GetBoard() );
        m_canvas->Refresh();
        break;

    case ID_POPUP_PCB_FILL_ZONE:
        m_canvas->MoveCursorToCrossHair();
        Fill_Zone( (ZONE_CONTAINER*) GetCurItem() );
        TestNetConnection( NULL, ( (ZONE_CONTAINER*) GetCurItem() )->GetNetCode() );
        SetMsgPanel( GetBoard() );
        m_canvas->Refresh();
        break;

    case ID_POPUP_PCB_MOVE_TEXTEPCB_REQUEST:
        StartMoveTextePcb( (TEXTE_PCB*) GetCurItem(), &dc );
        m_canvas->SetAutoPanRequest( true );
        break;

    case ID_POPUP_PCB_DRAG_MODULE_REQUEST:
    case ID_POPUP_PCB_MOVE_MODULE_REQUEST:
        if( GetCurItem() == NULL )
            break;

        // If the current Item is a pad, text module ...: Get its parent
        if( GetCurItem()->Type() != PCB_MODULE_T )
            SetCurItem( GetCurItem()->GetParent() );

        if( !GetCurItem() || GetCurItem()->Type() != PCB_MODULE_T )
            break;

        module = (MODULE*) GetCurItem();

        if( module->IsLocked() )
        {
            wxString msg;
            msg.Printf( _( "Footprint %s found, but it is locked" ),
                        module->GetReference().GetData() );
            DisplayInfoMessage( this, msg );
            break;
        }

        SendMessageToEESCHEMA( module );
        SetCrossHairPosition( module->GetPosition() );
        m_canvas->MoveCursorToCrossHair();
        StartMoveModule( module, &dc, id == ID_POPUP_PCB_DRAG_MODULE_REQUEST );
        break;

    case ID_POPUP_PCB_GET_AND_MOVE_MODULE_REQUEST:      // get module by name and move it
        SetCurItem( GetFootprintFromBoardByReference() );
        module = (MODULE*) GetCurItem();

        if( module == NULL )
            break;

        if( module->IsLocked() )
        {
            wxString msg = wxString::Format(
                _( "Footprint %s found, but it is locked" ),
                module->GetReference().GetData() );
            DisplayInfoMessage( this, msg );
            break;
        }

        SendMessageToEESCHEMA( module );
        m_canvas->MoveCursorToCrossHair();
        StartMoveModule( module, &dc, false );
        break;

    case ID_POPUP_PCB_DELETE_MODULE:
        m_canvas->MoveCursorToCrossHair();

        // If the current Item is a pad, text module ...: Get its parent
        if( GetCurItem()->Type() != PCB_MODULE_T )
            SetCurItem( GetCurItem()->GetParent() );

        if( !GetCurItem() || GetCurItem()->Type() != PCB_MODULE_T )
            break;

        module = (MODULE*) GetCurItem();

        if( module->IsLocked() )
        {
            wxString msg;
            msg.Printf( _( "Footprint %s found, but it is locked" ),
                        module->GetReference().GetData() );
            DisplayInfoMessage( this, msg );
            break;
        }

        if( Delete_Module( (MODULE*) GetCurItem(), &dc ) )
        {
            SetCurItem( NULL );
        }

        break;

    case ID_POPUP_PCB_ROTATE_MODULE_COUNTERCLOCKWISE:
        m_canvas->MoveCursorToCrossHair();

        // If the current Item is a pad, text module ...: Get its parent
        if( GetCurItem()->Type() != PCB_MODULE_T )
            SetCurItem( GetCurItem()->GetParent() );

        if( !GetCurItem() || GetCurItem()->Type() != PCB_MODULE_T )
            break;

        module = (MODULE*) GetCurItem();

        if( module->IsLocked() )
        {
            wxString msg;
            msg.Printf( _( "Footprint %s found, but it is locked" ),
                        module->GetReference().GetData() );
            DisplayInfoMessage( this, msg );
            break;
        }

        // This is a simple rotation, no other editing in progress
        if( !GetCurItem()->IsMoving() )
#ifdef PCBNEW_WITH_TRACKITEMS
        {
            PICKED_ITEMS_LIST pickedItems;
            ITEM_PICKER       picker( NULL, UR_CHANGED );
            //Delete teardrops
            GetBoard()->TrackItems()->Teardrops()->Remove( static_cast<MODULE*>(GetCurItem()), &pickedItems, true );
            picker.SetItem( GetCurItem() );
            pickedItems.PushItem( picker );
            SaveCopyInUndoList( pickedItems, UR_CHANGED,
                                static_cast<MODULE*>(GetCurItem())->GetPosition() );
        }
#else
            SaveCopyInUndoList( GetCurItem(), UR_CHANGED, ((MODULE*)GetCurItem())->GetPosition() );
#endif

        Rotate_Module( &dc, (MODULE*) GetCurItem(), m_rotationAngle, true );
        break;

    case ID_POPUP_PCB_ROTATE_MODULE_CLOCKWISE:
        m_canvas->MoveCursorToCrossHair();

        // If the current Item is a pad, text module ...: Get its parent
        if( GetCurItem()->Type() != PCB_MODULE_T )
            SetCurItem( GetCurItem()->GetParent() );

        if( !GetCurItem() || GetCurItem()->Type() != PCB_MODULE_T )
            break;

        module = (MODULE*) GetCurItem();

        if( module->IsLocked() )
        {
            wxString msg;
            msg.Printf( _( "Footprint %s found, but it is locked" ),
                        module->GetReference().GetData() );
            DisplayInfoMessage( this, msg );
            break;
        }

        // This is a simple rotation, no other editing in progress
        if( !GetCurItem()->IsMoving() )
#ifdef PCBNEW_WITH_TRACKITEMS
        {
            PICKED_ITEMS_LIST pickedItems;
            ITEM_PICKER       picker( NULL, UR_CHANGED );
            //Delete teardrops
            GetBoard()->TrackItems()->Teardrops()->Remove( static_cast<MODULE*>(GetCurItem()), &pickedItems, true );
            picker.SetItem( GetCurItem() );
            pickedItems.PushItem( picker );
            SaveCopyInUndoList( pickedItems, UR_CHANGED,
                                static_cast<MODULE*>(GetCurItem())->GetPosition() );
        }
#else
            SaveCopyInUndoList( GetCurItem(), UR_CHANGED, ((MODULE*)GetCurItem())->GetPosition() );
#endif

        Rotate_Module( &dc, (MODULE*) GetCurItem(), -m_rotationAngle, true );
        break;

    case ID_POPUP_PCB_CHANGE_SIDE_MODULE:
        m_canvas->MoveCursorToCrossHair();

        // If the current Item is a pad, text module ...: Get its parent
        if( GetCurItem()->Type() != PCB_MODULE_T )
            SetCurItem( GetCurItem()->GetParent() );

        if( !GetCurItem() || GetCurItem()->Type() != PCB_MODULE_T )
            break;

        module = (MODULE*) GetCurItem();

        if( module->IsLocked() )
        {
            wxString msg;
            msg.Printf( _( "Footprint %s found, but it is locked" ),
                        module->GetReference().GetData() );
            DisplayInfoMessage( this, msg );
            break;
        }

        // This is a simple flip, no other editing in progress
        if( !GetCurItem()->IsMoving() )
#ifdef PCBNEW_WITH_TRACKITEMS
        {
            PICKED_ITEMS_LIST pickedItems;
            ITEM_PICKER       picker( NULL, UR_FLIPPED );
            //Delete teardrops
            GetBoard()->TrackItems()->Teardrops()->Remove( static_cast<MODULE*>(GetCurItem()), &pickedItems, true );
            picker.SetItem( GetCurItem() );
            pickedItems.PushItem( picker );
            SaveCopyInUndoList( pickedItems, UR_FLIPPED,
                                static_cast<MODULE*>(GetCurItem())->GetPosition() );
        }
#else
            SaveCopyInUndoList( GetCurItem(), UR_FLIPPED, ((MODULE*)GetCurItem())->GetPosition() );
#endif

        Change_Side_Module( (MODULE*) GetCurItem(), &dc );
        break;

    case ID_POPUP_PCB_EXCHANGE_FOOTPRINTS:
        if( !GetCurItem() || GetCurItem()->Type() != PCB_MODULE_T )
            break;

        InstallExchangeModuleFrame( (MODULE*) GetCurItem() );
        // Warning: the current item can be deleted by exchange module
        SetCurItem( NULL );
        m_canvas->MoveCursorToCrossHair();
        break;

    case ID_POPUP_PCB_EDIT_MODULE_PRMS:
        // If the current Item is a pad, text module ...: Get its parent
        if( GetCurItem()->Type() != PCB_MODULE_T )
            SetCurItem( GetCurItem()->GetParent() );

        if( !GetCurItem() || GetCurItem()->Type() != PCB_MODULE_T )
            break;

        InstallModuleOptionsFrame( (MODULE*) GetCurItem(), &dc );
        m_canvas->MoveCursorToCrossHair();
        break;

    case ID_POPUP_PCB_EDIT_MODULE_WITH_MODEDIT:

        // If we don't have a current item, there's nothing we can do here
        if( !GetCurItem() )
            break;

        // If the current Item is a pad, text module ...: Get its parent
        if( GetCurItem()->Type() != PCB_MODULE_T )
            SetCurItem( GetCurItem()->GetParent() );

        if( !GetCurItem() || GetCurItem()->Type() != PCB_MODULE_T )
            break;

        if( GetCurItem()->GetTimeStamp() == 0 )    // Module Editor needs a non null timestamp
        {
            GetCurItem()->SetTimeStamp( GetNewTimeStamp() );
            OnModify();
        }

        {
            FOOTPRINT_EDIT_FRAME* editor = (FOOTPRINT_EDIT_FRAME*) Kiway().Player( FRAME_PCB_MODULE_EDITOR, true );

            editor->Load_Module_From_BOARD( (MODULE*)GetCurItem() );
            SetCurItem( NULL );     // the current module could be deleted by
        }
        m_canvas->MoveCursorToCrossHair();
        break;

    case ID_POPUP_PCB_DRAG_PAD_REQUEST:
        module = (MODULE*) GetCurItem()->GetParent();

        if( !module || module->Type() != PCB_MODULE_T )
            break;

        if( module->IsLocked() )
        {
            wxString msg;
            msg.Printf( _( "The parent (%s) of the pad is locked" ),
                        module->GetReference().GetData() );
            DisplayInfoMessage( this, msg );
            break;
        }

        m_canvas->MoveCursorToCrossHair();
        StartMovePad( (D_PAD*) GetCurItem(), &dc, true );
        break;

    case ID_POPUP_PCB_MOVE_PAD_REQUEST:
        module = (MODULE*) GetCurItem()->GetParent();

        if( !module || module->Type() != PCB_MODULE_T )
            break;

        if( module->IsLocked() )
        {
            wxString msg;
            msg.Printf( _( "The parent (%s) of the pad is locked" ),
                        module->GetReference().GetData() );
            DisplayInfoMessage( this, msg );
            break;
        }

        m_canvas->MoveCursorToCrossHair();
        StartMovePad( (D_PAD*) GetCurItem(), &dc, false );
        break;

    case ID_POPUP_PCB_EDIT_PAD:
        InstallPadOptionsFrame( (D_PAD*) GetCurItem() );
#ifdef PCBNEW_WITH_TRACKITEMS
        GetBoard()->TrackItems()->Teardrops()->Update( static_cast<D_PAD*>(GetCurItem()) );
#endif
        m_canvas->MoveCursorToCrossHair();
        break;

    case ID_POPUP_PCB_APPLY_PAD_SETTINGS:
        m_canvas->MoveCursorToCrossHair();
        SaveCopyInUndoList( GetCurItem()->GetParent(), UR_CHANGED );
        Import_Pad_Settings( (D_PAD*) GetCurItem(), true );
        break;

    case ID_POPUP_PCB_GLOBAL_IMPORT_PAD_SETTINGS:
        m_canvas->MoveCursorToCrossHair();
        DlgGlobalChange_PadSettings( (D_PAD*) GetCurItem(), true );
        break;

    case ID_POPUP_PCB_COPY_PAD_SETTINGS:
        m_canvas->MoveCursorToCrossHair();
        Export_Pad_Settings( (D_PAD*) GetCurItem() );
        break;

    case ID_POPUP_PCB_DELETE_PAD:
#ifdef PCBNEW_WITH_TRACKITEMS
    {
        PICKED_ITEMS_LIST pickedItems;
        ITEM_PICKER       picker( NULL, UR_CHANGED );
        D_PAD* pad_delete = static_cast<D_PAD*>(GetCurItem());
        D_PAD pad_data = *pad_delete;
        GetBoard()->TrackItems()->Teardrops()->Remove( pad_delete, &pickedItems, true );
        picker.SetItem( GetCurItem()->GetParent() );
        pickedItems.PushItem( picker );
        SaveCopyInUndoList( pickedItems, UR_CHANGED );
#else
        SaveCopyInUndoList( GetCurItem()->GetParent(), UR_CHANGED );
#endif
        DeletePad( (D_PAD*) GetCurItem() );

#ifdef PCBNEW_WITH_TRACKITEMS
        D_PAD* still_pad = GetBoard()->GetPadFast( pad_data.GetPosition(), pad_data.GetLayerSet() );
        if( still_pad == pad_delete )
        {
            GetBoard()->PadDelete( pad_delete ); //Delete pad to undo deletion.
            wxCommandEvent e = wxID_UNDO;
            RestoreCopyFromUndoList( e );
        }
    }
#endif
        SetCurItem( NULL );
        m_canvas->MoveCursorToCrossHair();
        break;

    case ID_POPUP_PCB_EDIT_TEXTMODULE:
        InstallTextModOptionsFrame( static_cast<TEXTE_MODULE*>( GetCurItem() ), &dc );
        m_canvas->MoveCursorToCrossHair();
        break;

    case ID_POPUP_PCB_RESET_TEXT_SIZE:
        ResetTextSize( GetCurItem(), &dc );
        break;

    case ID_POPUP_PCB_MOVE_TEXTMODULE_REQUEST:
        m_canvas->MoveCursorToCrossHair();
        StartMoveTexteModule( static_cast<TEXTE_MODULE*>( GetCurItem() ), &dc );
        break;

    case ID_POPUP_PCB_ROTATE_TEXTMODULE:
        RotateTextModule( static_cast<TEXTE_MODULE*>( GetCurItem() ), &dc );
        m_canvas->MoveCursorToCrossHair();
        break;

    case ID_POPUP_PCB_DELETE_TEXTMODULE:
        DeleteTextModule( static_cast<TEXTE_MODULE*>( GetCurItem() ) );
        SetCurItem( NULL );
        m_canvas->MoveCursorToCrossHair();
        break;

    case ID_POPUP_PCB_SELECT_LAYER:
        {
            PCB_LAYER_ID itmp = SelectLayer( GetActiveLayer() );

            if( itmp >= 0 )
            {
                // if user changed colors and we are in high contrast mode, then redraw
                // because the PAD_ATTRIB_SMD pads may change color.
                if( displ_opts->m_ContrastModeDisplay && GetActiveLayer() != itmp )
                {
                    m_canvas->Refresh();
                }
                SetActiveLayer( itmp );
            }

            m_canvas->MoveCursorToCrossHair();
        }
        break;

    case ID_AUX_TOOLBAR_PCB_SELECT_LAYER_PAIR:
        SelectCopperLayerPair();
        break;

    case ID_POPUP_PCB_SELECT_NO_CU_LAYER:
        {
            PCB_LAYER_ID itmp = SelectLayer( GetActiveLayer(), LSET::AllCuMask() );

            if( itmp >= 0 )
                SetActiveLayer( itmp );

            m_canvas->MoveCursorToCrossHair();
        }
        break;

    case ID_POPUP_PCB_SELECT_CU_LAYER:
        {
            PCB_LAYER_ID itmp = SelectLayer( GetActiveLayer(), LSET::AllNonCuMask() );

            if( itmp >= 0 )
                SetActiveLayer( itmp );
        }
        break;

    case ID_POPUP_PCB_SELECT_LAYER_PAIR:
        SelectCopperLayerPair();
        m_canvas->MoveCursorToCrossHair();
        break;

    case ID_TOOLBARH_PCB_SELECT_LAYER:
        SetActiveLayer( ToLAYER_ID( m_SelLayerBox->GetLayerSelection() ) );

        if( displ_opts->m_ContrastModeDisplay )
            m_canvas->Refresh( true );
        break;

    case ID_POPUP_PCB_EDIT_TEXTEPCB:
        InstallTextPCBOptionsFrame( (TEXTE_PCB*) GetCurItem(), &dc );
        m_canvas->MoveCursorToCrossHair();
        break;

    case ID_POPUP_PCB_ROTATE_TEXTEPCB:
        Rotate_Texte_Pcb( (TEXTE_PCB*) GetCurItem(), &dc );
        m_canvas->MoveCursorToCrossHair();
        break;

    case ID_POPUP_PCB_COPY_TEXTEPCB:
        CreateTextePcb( &dc, (TEXTE_PCB*) GetCurItem() );
        m_canvas->MoveCursorToCrossHair();
        m_canvas->SetAutoPanRequest( true );
        break;

    case ID_POPUP_PCB_FLIP_TEXTEPCB:
        FlipTextePcb( (TEXTE_PCB*) GetCurItem(), &dc );
        m_canvas->MoveCursorToCrossHair();
        break;

    case ID_POPUP_PCB_DELETE_TEXTEPCB:
        Delete_Texte_Pcb( (TEXTE_PCB*) GetCurItem(), &dc );
        m_canvas->MoveCursorToCrossHair();
        break;

    case ID_POPUP_PCB_MOVE_MIRE_REQUEST:
        BeginMoveTarget( (PCB_TARGET*) GetCurItem(), &dc );
        m_canvas->MoveCursorToCrossHair();
        break;

    case ID_POPUP_PCB_EDIT_MIRE:
        ShowTargetOptionsDialog( (PCB_TARGET*) GetCurItem(), &dc );
        m_canvas->MoveCursorToCrossHair();
        break;

    case ID_POPUP_PCB_DELETE_MIRE:
        m_canvas->MoveCursorToCrossHair();
        DeleteTarget( (PCB_TARGET*) GetCurItem(), &dc );
        SetCurItem( NULL );
        break;

    case ID_POPUP_PCB_DELETE_DIMENSION:
        m_canvas->MoveCursorToCrossHair();
        DeleteDimension( (DIMENSION*) GetCurItem(), &dc );
        SetCurItem( NULL );
        break;

    case ID_POPUP_PCB_EDIT_DIMENSION:
        ShowDimensionPropertyDialog( (DIMENSION*) GetCurItem(), &dc );
        m_canvas->MoveCursorToCrossHair();
        break;

    case ID_POPUP_PCB_MOVE_TEXT_DIMENSION_REQUEST:
        BeginMoveDimensionText( (DIMENSION*) GetCurItem(), &dc );
        break;

    case ID_POPUP_PCB_DELETE_DRAWING:
        Delete_Segment_Edge( (DRAWSEGMENT*) GetCurItem(), &dc );
        m_canvas->MoveCursorToCrossHair();
        break;

    case ID_POPUP_PCB_DELETE_MARKER:
        RemoveStruct( GetCurItem(), &dc );
        m_canvas->MoveCursorToCrossHair();
        break;

    case ID_POPUP_PCB_GETINFO_MARKER:
        if( GetCurItem() && GetCurItem()->Type() == PCB_MARKER_T )
            ( (MARKER_PCB*) GetCurItem() )->DisplayMarkerInfo( this );

        m_canvas->MoveCursorToCrossHair();
        break;

    case ID_POPUP_PCB_DELETE_DRAWING_LAYER:
        if( GetCurItem()->GetFlags() != 0 )
            break;

        Delete_Drawings_All_Layer( GetCurItem()->GetLayer() );
        SetCurItem( NULL );
        m_canvas->MoveCursorToCrossHair();
        m_canvas->Refresh();
        break;

    case ID_POPUP_PCB_EDIT_DRAWING:
#ifndef USE_WX_OVERLAY
        InstallGraphicItemPropertiesDialog( (DRAWSEGMENT*) GetCurItem(), &dc );
#else
        // #1277772 - Draw into dialog converted in refresh request
        InstallGraphicItemPropertiesDialog( (DRAWSEGMENT*) GetCurItem(), NULL );
        m_canvas->Refresh();
#endif
        m_canvas->MoveCursorToCrossHair();
        break;

    case ID_POPUP_PCB_MOVE_DRAWING_REQUEST:
        m_canvas->MoveCursorToCrossHair();
        Start_Move_DrawItem( (DRAWSEGMENT*) GetCurItem(), &dc );
        break;

    case ID_POPUP_PCB_STOP_CURRENT_DRAWING:
        m_canvas->MoveCursorToCrossHair();

        if( GetCurItem() && (GetCurItem()->IsNew()) )
        {
            End_Edge( (DRAWSEGMENT*) GetCurItem(), &dc );
            SetCurItem( NULL );
        }

        break;

    case ID_POPUP_PCB_STOP_CURRENT_EDGE_ZONE:
        m_canvas->MoveCursorToCrossHair();

        if( GetCurItem() && (GetCurItem()->IsNew()) )
        {
            if( End_Zone( &dc ) )
                SetCurItem( NULL );
        }

        m_canvas->SetAutoPanRequest( false );
        break;

    case ID_POPUP_PCB_DELETE_ZONE_LAST_CREATED_CORNER:
        m_canvas->MoveCursorToCrossHair();

        if( GetCurItem() && (GetCurItem()->IsNew()) )
        {
            if( Delete_LastCreatedCorner( &dc ) == 0 )  // No more segment in outline,
                SetCurItem( NULL );
        }

        break;


    case ID_POPUP_PCB_MOVE_TRACK_SEGMENT:
        m_canvas->MoveCursorToCrossHair();
        StartMoveOneNodeOrSegment( (TRACK*) GetScreen()->GetCurItem(), &dc, id );
        break;

    case ID_POPUP_PCB_DRAG_TRACK_SEGMENT:
    case ID_POPUP_PCB_MOVE_TRACK_NODE:
        m_canvas->MoveCursorToCrossHair();
        StartMoveOneNodeOrSegment( (TRACK*) GetScreen()->GetCurItem(), &dc, id );
        break;

    case ID_POPUP_PCB_DRAG_TRACK_SEGMENT_KEEP_SLOPE:
        m_canvas->MoveCursorToCrossHair();
        Start_DragTrackSegmentAndKeepSlope( (TRACK*) GetScreen()->GetCurItem(), &dc );
        break;

    case ID_POPUP_PCB_BREAK_TRACK:
        m_canvas->MoveCursorToCrossHair();
        {
            TRACK*  track = (TRACK*) GetScreen()->GetCurItem();
            wxPoint pos   = GetCrossHairPosition();

            track->Draw( m_canvas, &dc, GR_XOR );
            PICKED_ITEMS_LIST itemsListPicker;

            TRACK*  newtrack = GetBoard()->CreateLockPoint( pos, track, &itemsListPicker );

#ifdef PCBNEW_WITH_TRACKITEMS
            if(GetBoard()->TrackItems()->RoundedTracksCorners()->IsOn())
            {
                GetBoard()->TrackItems()->RoundedTracksCorners()->Add( newtrack, pos, &itemsListPicker );//New node
                GetBoard()->TrackItems()->RoundedTracksCorners()->Add( track, pos, &itemsListPicker );//or previous one
            }
            OnModify();
#endif

            SaveCopyInUndoList( itemsListPicker, UR_UNSPECIFIED );
            track->Draw( m_canvas, &dc, GR_XOR );
            newtrack->Draw( m_canvas, &dc, GR_XOR );

            // compute the new ratsnest, because connectivity could change
            TestNetConnection( &dc, track->GetNetCode() );
        }
        break;

    case ID_POPUP_PCB_MOVE_EXACT:
        moveExact();
        break;

    case ID_POPUP_PCB_DUPLICATE_ITEM:
    case ID_POPUP_PCB_DUPLICATE_ITEM_AND_INCREMENT:
        duplicateItems( id == ID_POPUP_PCB_DUPLICATE_ITEM_AND_INCREMENT );
        break;

    case ID_POPUP_PCB_CREATE_ARRAY:
        createArray();
        break;

    case ID_MENU_PCB_CLEAN:
        Clean_Pcb();
        break;

    case ID_MENU_PCB_SWAP_LAYERS:
        Swap_Layers( event );
        break;

    case ID_PCB_USER_GRID_SETUP:
        InvokeDialogGrid();
        break;

    case ID_POPUP_PCB_DISPLAY_FOOTPRINT_DOC:
        {
            wxConfigBase* cfg = Pgm().CommonSettings();
            cfg->Read( wxT( "module_doc_file" ), g_DocModulesFileName );
            GetAssociatedDocument( this, g_DocModulesFileName, &Kiface().KifaceSearch() );
        }
        break;

    case ID_MENU_ARCHIVE_MODULES_IN_LIBRARY:
        ArchiveModulesOnBoard( false );
        break;

    case ID_MENU_CREATE_LIBRARY_AND_ARCHIVE_MODULES:
        ArchiveModulesOnBoard( true );
        break;

    case ID_GEN_IMPORT_DXF_FILE:
        InvokeDXFDialogBoardImport( this );
        m_canvas->Refresh();
        break;

#ifdef PCBNEW_WITH_TRACKITEMS
    case ID_POPUP_PCB_GOTO_NEXT_MARKER:
        if( !GetBoard()->m_markers.empty() )
        {
            unsigned int index = 0;
            if( GetCurItem() && GetCurItem()->Type() == PCB_MARKER_T )
            {
                for( MARKER_PCB* curma : GetBoard()->m_markers )
                {
                    index++;
                    if( static_cast<MARKER_PCB*>(GetCurItem()) == curma )
                        break;
                }

            }

            if( index >= GetBoard()->m_markers.size() )
                index = 0;

            MARKER_PCB* mark = GetBoard()->GetMARKER( index );
            if( mark )
            {
                CursorGoto( mark->GetPosition() );
                SetCurItem( mark );
            }
        }

        break;

    //Via stitching.
    case ID_POPUP_PCB_PLACE_ZONE_THROUGH_VIA:
    case ID_POPUP_PCB_PLACE_ZONE_BLIND_BURIED_VIA:
    case ID_POPUP_PCB_SEL_LAYERS_AND_PLACE_ZONE_BLIND_BURIED_VIA:
    case ID_POPUP_PCB_SEL_LAYER_AND_PLACE_ZONE_THROUGH_VIA:
        GetBoard()->ViaStitching()->AddThermalVia( this, id );
        break;

    case ID_POPUP_PCB_CREATE_VIA_ARRAY:
        if((GetCurItem()->Type() == PCB_VIA_T) && (GetToolId() == ID_PCB_DRAW_VIA_BUTT))
        {
            const VIA* via = static_cast<const VIA*>(GetCurItem());
            if(dynamic_cast<const VIA*>(via)->GetThermalCode())
            {
                GetBoard()->ViaStitching()->AddViaArrayPrepare( this, via );
                createArray();
                GetBoard()->ViaStitching()->AddViaArrayFinnish();
            }
        }
        break;

    //Teardrops
    case ID_POPUP_PCB_TEARDROP_COPYCURRENT:
        if( GetCurItem() )
        {
            TRACK* cur_track = static_cast<TRACK*>(GetCurItem());
            GetBoard()->TrackItems()->Teardrops()->CopyCurrentParams( cur_track,
                TrackNodeItem::TrackSegNearestEndpoint(cur_track, GetCrossHairPosition() ) );
        }
        break;

    case ID_POPUP_PCB_TEARDROP_LOCK_TOGGLE:
        if( GetCurItem() )
        {
            GetBoard()->TrackItems()->Teardrops()->LockToggle( static_cast<TRACK*>(GetCurItem()),
                                                               GetCrossHairPosition() );
            OnModify();
        }
        break;

    case ID_POPUP_PCB_TEARDROPS_LOCK_ALL_SAME:
        if( GetCurItem() )
        {
            GetBoard()->TrackItems()->Teardrops()->Lock( static_cast<TRACK*>(GetCurItem()),
                                                         GetCrossHairPosition(),
                                                         &GetBoard()->m_Track);
            OnModify();
        }
        break;

    case ID_POPUP_PCB_TEARDROPS_UNLOCK_ALL_SAME:
        if( GetCurItem() )
        {
            GetBoard()->TrackItems()->Teardrops()->Unlock( static_cast<TRACK*>(GetCurItem()),
                                                           GetCrossHairPosition(),
                                                           &GetBoard()->m_Track);
            OnModify();
        }
        break;

    case ID_POPUP_PCB_TEARDROPS_LOCK_NET:
        if( GetCurItem() )
        {
            if( GetCurItem()->Type() == PCB_TRACE_T )
            {
                GetBoard()->TrackItems()->Teardrops()->Lock( static_cast<TRACK*>(GetCurItem())->GetNetCode(),
                                                             static_cast<TRACK*>(GetCurItem()) );
                OnModify();
            }
        }
        break;

    case ID_POPUP_PCB_TEARDROPS_UNLOCK_NET:
        if( GetCurItem() )
        {
            if( GetCurItem()->Type() == PCB_TRACE_T )
            {
                GetBoard()->TrackItems()->Teardrops()->Unlock( static_cast<TRACK*>(GetCurItem())->GetNetCode(),
                                                               static_cast<TRACK*>(GetCurItem()) );
                OnModify();
            }
        }
        break;

    case ID_POPUP_PCB_TEARDROPS_LOCK_MODULES_ALL:
        GetBoard()->TrackItems()->Teardrops()->Lock(&GetBoard()->m_Modules);
        OnModify();
        break;

    case ID_POPUP_PCB_TEARDROPS_UNLOCK_MODULES_ALL:
        GetBoard()->TrackItems()->Teardrops()->Unlock(&GetBoard()->m_Modules);
        OnModify();
        break;

    case ID_POPUP_PCB_TEARDROPS_LOCK_VIAS_ALL:
        GetBoard()->TrackItems()->Teardrops()->Lock( &GetBoard()->m_Track, TEARDROPS::ONLY_TEARDROPS_T );
        OnModify();
        break;

    case ID_POPUP_PCB_TEARDROPS_UNLOCK_VIAS_ALL:
        GetBoard()->TrackItems()->Teardrops()->Unlock( &GetBoard()->m_Track, TEARDROPS::ONLY_TEARDROPS_T );
        OnModify();
        break;

    case  ID_POPUP_PCB_TJUNCTIONS_LOCK_ALL:
        GetBoard()->TrackItems()->Teardrops()->Lock(&GetBoard()->m_Track, TEARDROPS::ONLY_TJUNCTIONS_T);
        OnModify();
        break;

    case  ID_POPUP_PCB_TJUNCTIONS_UNLOCK_ALL:
        GetBoard()->TrackItems()->Teardrops()->Unlock(&GetBoard()->m_Track, TEARDROPS::ONLY_TJUNCTIONS_T);
        OnModify();
        break;

    case ID_POPUP_PCB_JUNCTIONS_LOCK_ALL:
        GetBoard()->TrackItems()->Teardrops()->Lock(&GetBoard()->m_Track, TEARDROPS::ONLY_JUNCTIONS_T);
        OnModify();
        break;

    case ID_POPUP_PCB_JUNCTIONS_UNLOCK_ALL:
        GetBoard()->TrackItems()->Teardrops()->Unlock(&GetBoard()->m_Track, TEARDROPS::ONLY_JUNCTIONS_T);
        OnModify();
        break;

    case ID_POPUP_PCB_TEARDROPS_LOCK_MODULE:
        if( GetCurItem() )
        {
            if( GetCurItem()->Type() != PCB_MODULE_T )
                SetCurItem( GetCurItem()->GetParent() );
            if( GetCurItem() )
                if( GetCurItem()->Type() == PCB_MODULE_T )
                {
                    GetBoard()->TrackItems()->Teardrops()->Lock( static_cast<MODULE*>(GetCurItem()) );
                    OnModify();
                }
        }
        break;

    case ID_POPUP_PCB_TEARDROPS_UNLOCK_MODULE:
        if( GetCurItem() )
        {
            if( GetCurItem()->Type() != PCB_MODULE_T )
                SetCurItem( GetCurItem()->GetParent() );
            if( GetCurItem() )
                if( GetCurItem()->Type() == PCB_MODULE_T )
                {
                    GetBoard()->TrackItems()->Teardrops()->Unlock( static_cast<MODULE*>(GetCurItem()) );
                    OnModify();
                }
        }
        break;

    case ID_POPUP_PCB_TEARDROPS_LOCK:
        if( GetCurItem() )
        {
            m_canvas->MoveCursorToCrossHair();
            GetBoard()->TrackItems()->Teardrops()->Lock( static_cast<BOARD_CONNECTED_ITEM*>(GetCurItem()) );
            OnModify();
        }
        break;

    case ID_POPUP_PCB_TEARDROPS_UNLOCK:
        if( GetCurItem() )
        {
            m_canvas->MoveCursorToCrossHair();
            GetBoard()->TrackItems()->Teardrops()->Unlock( static_cast<BOARD_CONNECTED_ITEM*>(GetCurItem()) );
            OnModify();
        }
        break;

    case ID_POPUP_PCB_TEARDROP_EDIT_LENGTH:
        GetBoard()->TrackItems()->Teardrops()->ToggleEdit( TEARDROPS::EDIT_SIZE_LENGTH_T );
        break;

    case ID_POPUP_PCB_TEARDROP_EDIT_WIDTH:
        GetBoard()->TrackItems()->Teardrops()->ToggleEdit( TEARDROPS::EDIT_SIZE_WIDTH_T );
        break;

    case ID_POPUP_PCB_TEARDROP_SET_DEFAULT_PARAMS:
        GetBoard()->TrackItems()->Teardrops()->LoadDefaultParams(
            GetBoard()->TrackItems()->Teardrops()->GetCurrentShape());
        break;

    case ID_POPUP_PCB_TEARDROP_SELECT_TEARDROP:
        GetBoard()->TrackItems()->Teardrops()->SelectShape(TrackNodeItem::TEARDROP::TEARDROP_T);
        break;

    case ID_POPUP_PCB_TEARDROP_SELECT_FILLET:
        GetBoard()->TrackItems()->Teardrops()->SelectShape(TrackNodeItem::TEARDROP::FILLET_T);
        break;

    case ID_POPUP_PCB_TEARDROP_SELECT_SUBLAND:
        GetBoard()->TrackItems()->Teardrops()->SelectShape(TrackNodeItem::TEARDROP::SUBLAND_T);
        break;

    case ID_POPUP_PCB_TEARDROP_SELECT_ZERO:
        GetBoard()->TrackItems()->Teardrops()->SelectShape(TrackNodeItem::TEARDROP::ZERO_T);
        break;

    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 1:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 2:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 3:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 4:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 5:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 6:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 7:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 8:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 9:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 10:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 11:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 12:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 13:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 14:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 15:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 16:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 17:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 18:
    case ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_LAST:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 1:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 2:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 3:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 4:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 5:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 6:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 7:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 8:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 9:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 10:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 11:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 12:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 13:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 14:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 15:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 16:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 17:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 18:
    case ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_LAST:
    case ID_POPUP_PCB_TEARDROP_NUM_SEGMENTS_FIRST:
    case ID_POPUP_PCB_TEARDROP_NUM_SEGMENTS_FIRST + 1:
    case ID_POPUP_PCB_TEARDROP_NUM_SEGMENTS_FIRST + 2:
    case ID_POPUP_PCB_TEARDROP_NUM_SEGMENTS_FIRST + 3:
    case ID_POPUP_PCB_TEARDROP_NUM_SEGMENTS_FIRST + 4:
    case ID_POPUP_PCB_TEARDROP_NUM_SEGMENTS_FIRST + 5:
    case ID_POPUP_PCB_TEARDROP_NUM_SEGMENTS_FIRST + 6:
    case ID_POPUP_PCB_TEARDROP_NUM_SEGMENTS_FIRST + 7:
    case ID_POPUP_PCB_TEARDROP_NUM_SEGMENTS_FIRST + 8:
    case ID_POPUP_PCB_TEARDROP_NUM_SEGMENTS_LAST:
        GetBoard()->TrackItems()->Teardrops()->MenuToDo_ChangeSize( id );
        break;

    case ID_POPUP_PCB_TEARDROP_PLACE:
        if( GetCurItem() )
        {
            m_canvas->MoveCursorToCrossHair();
            GetBoard()->TrackItems()->Teardrops()->Add( static_cast<TRACK*>(GetCurItem()), GetCrossHairPosition() );
            OnModify();
        }
        break;

    case ID_POPUP_PCB_TEARDROPS_PLACE:
        if( GetCurItem() )
        {
            m_canvas->MoveCursorToCrossHair();
            GetBoard()->TrackItems()->Teardrops()->Add( static_cast<BOARD_CONNECTED_ITEM*>(GetCurItem()) );
            OnModify();
        }
        break;

    case ID_POPUP_PCB_TEARDROP_CHANGE:
        if( GetCurItem() )
        {
            m_canvas->MoveCursorToCrossHair();
            TRACK* cur_track = static_cast<TRACK*>(GetCurItem());
            GetBoard()->TrackItems()->Teardrops()->Change( GetBoard()->TrackItems()->Teardrops()->Get( cur_track,
                TrackNodeItem::TrackSegNearestEndpoint(cur_track, GetCrossHairPosition()) ) );
            OnModify();
        }
        break;

    case ID_POPUP_PCB_TEARDROPS_CHANGE:
        if( GetCurItem() )
        {
            m_canvas->MoveCursorToCrossHair();
            GetBoard()->TrackItems()->Teardrops()->Change( static_cast<BOARD_CONNECTED_ITEM*>(GetCurItem()) );
            OnModify();
        }
        break;

    case ID_POPUP_PCB_TEARDROP_DELETE:
        if( GetCurItem() )
        {
            m_canvas->MoveCursorToCrossHair();
            TRACK* cur_track = static_cast<TRACK*>(GetCurItem());
            GetBoard()->TrackItems()->Teardrops()->Remove( GetBoard()->TrackItems()->Teardrops()->Get( cur_track,
                TrackNodeItem::TrackSegNearestEndpoint(cur_track, GetCrossHairPosition()) ),  true, false );
            OnModify();
        }
        break;

    case ID_POPUP_PCB_TEARDROPS_DELETE:
        if( GetCurItem() )
        {
            m_canvas->MoveCursorToCrossHair();
            GetBoard()->TrackItems()->Teardrops()->Remove( static_cast<BOARD_CONNECTED_ITEM*>(GetCurItem()), true, false );
            OnModify();
        }
        break;

    case ID_POPUP_PCB_TEARDROPS_PLACE_NET:
        if( GetCurItem() )
        {
            m_canvas->MoveCursorToCrossHair();
            GetBoard()->TrackItems()->Teardrops()->Add( static_cast<TRACK*>(GetCurItem())->GetNetCode(),
                                          TEARDROPS::ONLY_TEARDROPS_T );
            OnModify();
        }
        break;

    case ID_POPUP_PCB_TEARDROPS_CHANGE_NET:
        if( GetCurItem() )
        {
            m_canvas->MoveCursorToCrossHair();
            GetBoard()->TrackItems()->Teardrops()->Change( static_cast<TRACK*>(GetCurItem())->GetNetCode(),
                                             TEARDROPS::ONLY_TEARDROPS_T );
            OnModify();
        }
        break;

    case ID_POPUP_PCB_TEARDROPS_DELETE_NET:
        if( GetCurItem() )
        {
            m_canvas->MoveCursorToCrossHair();
            GetBoard()->TrackItems()->Teardrops()->Remove( static_cast<TRACK*>(GetCurItem())->GetNetCode(),
                                             TEARDROPS::ONLY_TEARDROPS_T,  true, false );
            OnModify();
        }
        break;

    case ID_POPUP_PCB_TEARDROPS_PLACE_MODULE:
        if( GetCurItem() )
        {
            if( GetCurItem()->Type() != PCB_MODULE_T )
                SetCurItem( GetCurItem()->GetParent() );
            if( GetCurItem() )
                if( GetCurItem()->Type() == PCB_MODULE_T )
                {
                    GetBoard()->TrackItems()->Teardrops()->Add( static_cast<MODULE*>(GetCurItem()) );
                    OnModify();
                }
        }
        break;

    case ID_POPUP_PCB_TEARDROPS_CHANGE_MODULE:
        if( GetCurItem() )
        {
            if( GetCurItem()->Type() != PCB_MODULE_T )
                SetCurItem( GetCurItem()->GetParent() );
            if( GetCurItem() )
                if( GetCurItem()->Type() == PCB_MODULE_T )
                {
                    GetBoard()->TrackItems()->Teardrops()->Change( static_cast<MODULE*>(GetCurItem()) );
                    OnModify();
                }
        }
        break;

    case ID_POPUP_PCB_TEARDROPS_DELETE_MODULE:
        if( GetCurItem() )
        {
            if( GetCurItem()->Type() != PCB_MODULE_T )
                SetCurItem( GetCurItem()->GetParent() );
            if( GetCurItem() )
                if( GetCurItem()->Type() == PCB_MODULE_T )
                {
                    GetBoard()->TrackItems()->Teardrops()->Remove( static_cast<MODULE*>(GetCurItem()), true, false );
                    OnModify();
                }
        }
        break;

    case ID_POPUP_PCB_TJUNCTIONS_PLACE_NET:
        if( GetCurItem() )
        {
            m_canvas->MoveCursorToCrossHair();
            GetBoard()->TrackItems()->Teardrops()->Add( static_cast<TRACK*>(GetCurItem())->GetNetCode(),
                                          TEARDROPS::ONLY_TJUNCTIONS_T);
            OnModify();
        }
        break;

    case ID_POPUP_PCB_TJUNCTIONS_CHANGE_NET:
        if( GetCurItem() )
        {
            m_canvas->MoveCursorToCrossHair();
            GetBoard()->TrackItems()->Teardrops()->Change( static_cast<TRACK*>(GetCurItem())->GetNetCode(),
                                             TEARDROPS::ONLY_TJUNCTIONS_T );
            OnModify();
        }
        break;

    case ID_POPUP_PCB_TJUNCTIONS_DELETE_NET:
        if( GetCurItem() )
        {
            m_canvas->MoveCursorToCrossHair();
            GetBoard()->TrackItems()->Teardrops()->Remove( static_cast<TRACK*>(GetCurItem())->GetNetCode(),
                                             TEARDROPS::ONLY_TJUNCTIONS_T, true, false );
            OnModify();
        }
        break;

    case ID_POPUP_PCB_JUNCTIONS_PLACE_NET:
        if( GetCurItem() )
        {
            m_canvas->MoveCursorToCrossHair();
            GetBoard()->TrackItems()->Teardrops()->Add( static_cast<TRACK*>(GetCurItem())->GetNetCode(),
                                          TEARDROPS::ONLY_JUNCTIONS_T );
            OnModify();
        }
        break;

    case ID_POPUP_PCB_JUNCTIONS_CHANGE_NET:
        if( GetCurItem() )
        {
            m_canvas->MoveCursorToCrossHair();
            GetBoard()->TrackItems()->Teardrops()->Change( static_cast<TRACK*>(GetCurItem())->GetNetCode(),
                                             TEARDROPS::ONLY_JUNCTIONS_T
                                           );
            OnModify();
        }
        break;

    case ID_POPUP_PCB_JUNCTIONS_DELETE_NET:
        if( GetCurItem() )
        {
            m_canvas->MoveCursorToCrossHair();
            GetBoard()->TrackItems()->Teardrops()->Remove( static_cast<TRACK*>(GetCurItem())->GetNetCode(),
                                             TEARDROPS::ONLY_JUNCTIONS_T, true, false );
            OnModify();
        }
        break;

    case ID_POPUP_PCB_TEARDROPS_PLACE_MODULES_ALL:
        GetBoard()->TrackItems()->Teardrops()->Add(&GetBoard()->m_Modules);
        OnModify();
        break;

    case ID_POPUP_PCB_TEARDROPS_CHANGE_MODULES_ALL:
        GetBoard()->TrackItems()->Teardrops()->Change(&GetBoard()->m_Modules);
        OnModify();
        break;

    case ID_POPUP_PCB_TEARDROPS_DELETE_MODULES_ALL:
        GetBoard()->TrackItems()->Teardrops()->Remove(&GetBoard()->m_Modules);
        OnModify();
        break;

    case ID_POPUP_PCB_TEARDROPS_PLACE_VIAS_ALL:
        GetBoard()->TrackItems()->Teardrops()->Add( &GetBoard()->m_Track, TEARDROPS::ONLY_TEARDROPS_T );
        OnModify();
        break;

    case ID_POPUP_PCB_TEARDROPS_CHANGE_VIAS_ALL:
        GetBoard()->TrackItems()->Teardrops()->Change( &GetBoard()->m_Track, TEARDROPS::ONLY_TEARDROPS_T );
        OnModify();
        break;

    case ID_POPUP_PCB_TEARDROPS_DELETE_VIAS_ALL:
        GetBoard()->TrackItems()->Teardrops()->Remove( &GetBoard()->m_Track, TEARDROPS::ONLY_TEARDROPS_T );
        OnModify();
        break;

    case  ID_POPUP_PCB_TJUNCTIONS_PLACE_ALL:
        GetBoard()->TrackItems()->Teardrops()->Add(&GetBoard()->m_Track, TEARDROPS::ONLY_TJUNCTIONS_T);
        OnModify();
        break;

    case  ID_POPUP_PCB_TJUNCTIONS_CHANGE_ALL:
        GetBoard()->TrackItems()->Teardrops()->Change(&GetBoard()->m_Track, TEARDROPS::ONLY_TJUNCTIONS_T);
        OnModify();
        break;

    case  ID_POPUP_PCB_TJUNCTIONS_DELETE_ALL:
        GetBoard()->TrackItems()->Teardrops()->Remove(&GetBoard()->m_Track, TEARDROPS::ONLY_TJUNCTIONS_T);
        OnModify();
        break;

    case ID_POPUP_PCB_JUNCTIONS_PLACE_ALL:
        GetBoard()->TrackItems()->Teardrops()->Add(&GetBoard()->m_Track, TEARDROPS::ONLY_JUNCTIONS_T);
        OnModify();
        break;

    case ID_POPUP_PCB_JUNCTIONS_CHANGE_ALL:
        GetBoard()->TrackItems()->Teardrops()->Change(&GetBoard()->m_Track, TEARDROPS::ONLY_JUNCTIONS_T);
        OnModify();
        break;

    case ID_POPUP_PCB_JUNCTIONS_DELETE_ALL:
        GetBoard()->TrackItems()->Teardrops()->Remove(&GetBoard()->m_Track, TEARDROPS::ONLY_JUNCTIONS_T);
        OnModify();
        break;

    case ID_POPUP_PCB_TEARDROPS_MARK_WARNINGS_MODULES:
        GetBoard()->TrackItems()->Teardrops()->MarkWarnings(&GetBoard()->m_Modules, m_drc);
        break;

    case ID_POPUP_PCB_TEARDROPS_MARK_WARNINGS_TEARDROPS_VIAS:
    case ID_POPUP_PCB_TEARDROPS_MARK_WARNINGS_TJUNCTIONS:
    case ID_POPUP_PCB_TEARDROPS_MARK_WARNINGS_JUNCTIONS:
        GetBoard()->TrackItems()->Teardrops()->MarkWarnings(&GetBoard()->m_Track, TEARDROPS::TEARDROPS_TYPE_TODO(TEARDROPS::ONLY_TEARDROPS_T + id - ID_POPUP_PCB_TEARDROPS_MARK_WARNINGS_TEARDROPS_VIAS), m_drc);
        break;

    case ID_POPUP_PCB_TEARDROPS_MARK_DIFF_MODULES:
        GetBoard()->TrackItems()->Teardrops()->MarkDifferent(&GetBoard()->m_Track, TEARDROPS::ONLY_PAD_TEARDROPS_T, m_drc);
        break;

    case ID_POPUP_PCB_TEARDROPS_MARK_DIFF_TEARDROPS_VIAS:
    case ID_POPUP_PCB_TEARDROPS_MARK_DIFF_TJUNCTIONS:
    case ID_POPUP_PCB_TEARDROPS_MARK_DIFF_JUNCTIONS:
        GetBoard()->TrackItems()->Teardrops()->MarkDifferent(&GetBoard()->m_Track, TEARDROPS::TEARDROPS_TYPE_TODO(TEARDROPS::ONLY_TEARDROPS_T + id - ID_POPUP_PCB_TEARDROPS_MARK_DIFF_TEARDROPS_VIAS), m_drc);
        break;

    case ID_POPUP_PCB_TEARDROPS_MARK_CURR_MODULES:
        GetBoard()->TrackItems()->Teardrops()->MarkCurrent(&GetBoard()->m_Track, TEARDROPS::ONLY_PAD_TEARDROPS_T, m_drc);
        break;

    case ID_POPUP_PCB_TEARDROPS_MARK_CURR_TEARDROPS_VIAS:
    case ID_POPUP_PCB_TEARDROPS_MARK_CURR_TJUNCTIONS:
    case ID_POPUP_PCB_TEARDROPS_MARK_CURR_JUNCTIONS:
        GetBoard()->TrackItems()->Teardrops()->MarkCurrent(&GetBoard()->m_Track, TEARDROPS::TEARDROPS_TYPE_TODO(TEARDROPS::ONLY_TEARDROPS_T + id - ID_POPUP_PCB_TEARDROPS_MARK_CURR_TEARDROPS_VIAS), m_drc);
        break;

    case ID_POPUP_PCB_VIA_PICK_SIZE:
        if(GetCurItem()->Type() == PCB_VIA_T)
        {
            GetBoard()->TrackItems()->PickViaSize(static_cast<VIA*>(GetCurItem()));
            updateViaSizeSelectBox();
        }
        break;

    case ID_POPUP_PCB_TRACK_PICK_WIDTH:
        if(GetCurItem()->Type() == PCB_TRACE_T)
        {
            GetBoard()->TrackItems()->PickTrackSize(static_cast<TRACK*>(GetCurItem()));
            updateTraceWidthSelectBox();
        }
        break;

    case ID_POPUP_PCB_TRACKS_MARK_SHARP_ANGLES:
        GetBoard()->TrackItems()->MarkSharpAngles(&GetBoard()->m_Track, m_drc);
        break;

    case ID_POPUP_PCB_TRACKS_CONNECT_CENTER_IN_ITEM:
        GetBoard()->TrackItems()->CentralizeTrackConnection(&GetBoard()->m_Track);
        OnModify();
        break;

//------------------------------------------------------------------------------------
//Rounded corner
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNER_PLACE:
        if( GetCurItem() && GetCurItem()->Type() == PCB_TRACE_T )
        {
            m_canvas->MoveCursorToCrossHair();
            GetBoard()->TrackItems()->RoundedTracksCorners()->Add( static_cast<TRACK*>(GetCurItem()), GetCrossHairPosition() );
            if( static_cast<TRACK*>(GetCurItem())->GetNetCode() > 0 )
                TestNetConnection( nullptr, static_cast<TRACK*>(GetCurItem())->GetNetCode() );
            OnModify();
        }
        break;

    case ID_POPUP_PCB_ROUNDEDTRACKSCORNER_PLACE_NET:
        if( GetCurItem() && GetCurItem()->Type() == PCB_TRACE_T )
        {
            m_canvas->MoveCursorToCrossHair();
            GetBoard()->TrackItems()->RoundedTracksCorners()->Add( static_cast<TRACK*>(GetCurItem())->GetNetCode() );
            if( static_cast<TRACK*>(GetCurItem())->GetNetCode() > 0 )
                TestNetConnection( nullptr, static_cast<TRACK*>(GetCurItem())->GetNetCode() );
            OnModify();
        }
        break;

    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_PLACE_ALL:
        GetBoard()->TrackItems()->RoundedTracksCorners()->Add(&GetBoard()->m_Track);
        OnModify();
        break;

    case ID_POPUP_PCB_ROUNDEDTRACKSCORNER_DELETE:
        if( GetCurItem() )
        {
            m_canvas->MoveCursorToCrossHair();
            TRACK* cur_track = static_cast<TRACK*>(GetCurItem());
            GetBoard()->TrackItems()->RoundedTracksCorners()->Remove( GetBoard()->TrackItems()->RoundedTracksCorners()->Get( cur_track, TrackNodeItem::TrackSegNearestEndpoint(cur_track, GetCrossHairPosition())), true, false );
            OnModify();
        }
        break;

    case ID_POPUP_PCB_ROUNDEDTRACKSCORNER_DELETE_NET:
        if( GetCurItem() )
        {
            m_canvas->MoveCursorToCrossHair();
            GetBoard()->TrackItems()->RoundedTracksCorners()->Remove( static_cast<TRACK*>(GetCurItem())->GetNetCode(), true, false );
            OnModify();
        }
        break;

    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_DELETE_ALL:
        GetBoard()->TrackItems()->RoundedTracksCorners()->Remove(&GetBoard()->m_Track);
        OnModify();
        break;

    case ID_POPUP_PCB_ROUNDEDTRACKSCORNER_CHANGE:
        if( GetCurItem() )
        {
            m_canvas->MoveCursorToCrossHair();
            TRACK* cur_track = static_cast<TRACK*>(GetCurItem());
            GetBoard()->TrackItems()->RoundedTracksCorners()->Change( GetBoard()->TrackItems()->RoundedTracksCorners()->Get( cur_track,
                TrackNodeItem::TrackSegNearestEndpoint(cur_track, GetCrossHairPosition()) ), true, false );
            OnModify();
        }
        break;

    case ID_POPUP_PCB_ROUNDEDTRACKSCORNER_CHANGE_ALL:
        break;

    case ID_POPUP_PCB_ROUNDEDTRACKSCORNER_COPYCURRENT:
        if( GetCurItem() )
        {
            TRACK* cur_track = static_cast<TRACK*>(GetCurItem());
            GetBoard()->TrackItems()->RoundedTracksCorners()->CopyCurrentParams( cur_track,
                TrackNodeItem::TrackSegNearestEndpoint(cur_track, GetCrossHairPosition()) );
        }
        break;

    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_SET_FIRST:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_SET_FIRST + 1:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_SET_FIRST + 2:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_SET_FIRST + 3:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_SET_FIRST + 4:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_SET_FIRST + 5:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_SET_FIRST + 6:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_SET_FIRST + 7:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_SET_FIRST + 8:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_SET_FIRST + 9:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_SET_LAST:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_RATIO_FIRST:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_RATIO_FIRST + 1:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_RATIO_FIRST + 2:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_RATIO_FIRST + 3:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_RATIO_FIRST + 4:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_RATIO_FIRST + 5:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_RATIO_FIRST + 6:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_RATIO_FIRST + 7:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_RATIO_FIRST + 8:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_RATIO_FIRST + 9:
    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_RATIO_LAST:
        GetBoard()->TrackItems()->RoundedTracksCorners()->MenuToDo_ChangeSize( id );
        break;

    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SET_DEFAULT_PARAMS:
        GetBoard()->TrackItems()->RoundedTracksCorners()->LoadDefaultParams();
        break;

    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_EDIT_LENGTH_SET:
        GetBoard()->TrackItems()->RoundedTracksCorners()->ToggleEdit( ROUNDED_TRACKS_CORNERS::EDIT_LENGTH_SET_T );
        break;

    case ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_EDIT_LENGTH_RATIO:
        GetBoard()->TrackItems()->RoundedTracksCorners()->ToggleEdit( ROUNDED_TRACKS_CORNERS::EDIT_LENGTH_RATIO_T );
        break;

    case ID_POPUP_PCB_ROUNDEDTRACKSCORNER_CONVERT_SEGMENTED:
        if( GetCurItem() && GetCurItem()->Type() == PCB_TRACE_T )
        {
            m_canvas->MoveCursorToCrossHair();
            GetBoard()->TrackItems()->RoundedTracksCorners()->ConvertSegmentedCorners( static_cast<TRACK*>(GetCurItem()),
                                                                                       true );
            GetBoard()->TrackItems()->RoundedTracksCorners()->Update( static_cast<TRACK*>(GetCurItem())->GetNetCode(),
                                                                      static_cast<TRACK*>(GetCurItem()) );
            if( static_cast<TRACK*>(GetCurItem())->GetNetCode() > 0 )
                TestNetConnection( nullptr, static_cast<TRACK*>(GetCurItem())->GetNetCode() );
            OnModify();
        }
        break;

    case ID_POPUP_PCB_ROUNDEDTRACKSCORNER_CONVERT_SEGMENTED_NET:
        if( GetCurItem() && GetCurItem()->Type() == PCB_TRACE_T )
        {
            m_canvas->MoveCursorToCrossHair();
            GetBoard()->TrackItems()->RoundedTracksCorners()->ConvertSegmentedCorners( static_cast<TRACK*>(GetCurItem())->GetNetCode(), true );
            GetBoard()->TrackItems()->RoundedTracksCorners()->Update( static_cast<TRACK*>(GetCurItem())->GetNetCode(),
                                                                      static_cast<TRACK*>(GetCurItem()) );
            if( static_cast<TRACK*>(GetCurItem())->GetNetCode() > 0 )
                TestNetConnection( nullptr, static_cast<TRACK*>(GetCurItem())->GetNetCode() );
            OnModify();
        }
        break;
#endif

    default:
        wxString msg;
        msg.Printf( wxT( "PCB_EDIT_FRAME::Process_Special_Functions() unknown event id %d" ), id );
        DisplayError( this, msg );
        break;
    }

    m_canvas->CrossHairOn( &dc );
    m_canvas->SetIgnoreMouseEvents( false );
}


void PCB_EDIT_FRAME::RemoveStruct( BOARD_ITEM* Item, wxDC* DC )
{
    if( Item == NULL )
        return;

    switch( Item->Type() )
    {
    case PCB_MODULE_T:
        Delete_Module( (MODULE*) Item, DC );
        break;

    case PCB_DIMENSION_T:
        DeleteDimension( (DIMENSION*) Item, DC );
        break;

    case PCB_TARGET_T:
        DeleteTarget( (PCB_TARGET*) Item, DC );
        break;

    case PCB_LINE_T:
        Delete_Segment_Edge( (DRAWSEGMENT*) Item, DC );
        break;

    case PCB_TEXT_T:
        Delete_Texte_Pcb( (TEXTE_PCB*) Item, DC );
        break;

    case PCB_TRACE_T:
        Delete_Track( DC, (TRACK*) Item );
        break;

    case PCB_VIA_T:
        Delete_Segment( DC, (TRACK*) Item );
        break;

    case PCB_ZONE_T:
        Delete_OldZone_Fill( (SEGZONE*) Item );
        break;

    case PCB_ZONE_AREA_T:
        {
            SetCurItem( NULL );
            int netcode = ( (ZONE_CONTAINER*) Item )->GetNetCode();
            Delete_Zone_Contour( DC, (ZONE_CONTAINER*) Item );
            TestNetConnection( NULL, netcode );
            SetMsgPanel( GetBoard() );
        }
        break;

    case PCB_MARKER_T:
        if( Item == GetCurItem() )
            SetCurItem( NULL );

        ( (MARKER_PCB*) Item )->Draw( m_canvas, DC, GR_XOR );

        // delete the marker, and free memory.  Don't use undo stack.
        GetBoard()->Delete( Item );
        break;

    case PCB_PAD_T:
    case PCB_MODULE_TEXT_T:
    case PCB_MODULE_EDGE_T:
        break;

    case TYPE_NOT_INIT:
    case PCB_T:
    default:
        {
            wxString msg = wxString::Format(
                wxT( "Remove: item type %d unknown." ), Item->Type() );
            DisplayError( this, msg );
        }
        break;
    }
}


void PCB_EDIT_FRAME::SwitchLayer( wxDC* DC, PCB_LAYER_ID layer )
{
    PCB_LAYER_ID curLayer = GetActiveLayer();
    DISPLAY_OPTIONS* displ_opts = (DISPLAY_OPTIONS*)GetDisplayOptions();

    // Check if the specified layer matches the present layer
    if( layer == curLayer )
        return;

    // Copper layers cannot be selected unconditionally; how many
    // of those layers are currently enabled needs to be checked.
    if( IsCopperLayer( layer ) )
    {
        // If only one copper layer is enabled, the only such layer
        // that can be selected to is the "Back" layer (so the
        // selection of any other copper layer is disregarded).
        if( GetBoard()->GetCopperLayerCount() < 2 )
        {
            if( layer != B_Cu )
                return;
        }
        // If more than one copper layer is enabled, the "Copper"
        // and "Component" layers can be selected, but the total
        // number of copper layers determines which internal
        // layers are also capable of being selected.
        else
        {
            if( layer != B_Cu  &&  layer != F_Cu  && layer >= GetBoard()->GetCopperLayerCount() - 1 )
                return;
        }

        EDA_ITEM* current = GetScreen()->GetCurItem();

        // See if we are drawing a segment; if so, add a via?
        if( GetToolId() == ID_TRACK_BUTT && current )
        {
            if( current->Type() == PCB_TRACE_T && current->IsNew() )
            {
                // Want to set the routing layers so that it switches properly -
                // see the implementation of Other_Layer_Route - the working
                // layer is used to 'start' the via and set the layer masks appropriately.
                GetScreen()->m_Route_Layer_TOP    = curLayer;
                GetScreen()->m_Route_Layer_BOTTOM = layer;

                SetActiveLayer( curLayer );

                if( Other_Layer_Route( (TRACK*) GetScreen()->GetCurItem(), DC ) )
                {
                    if( displ_opts->m_ContrastModeDisplay )
                        m_canvas->Refresh();
                }

                // if the via was allowed by DRC, then the layer swap has already
                // been done by Other_Layer_Route(). if via not allowed, then
                // return now so assignment to setActiveLayer() below doesn't happen.
                return;
            }
        }
    }

    // Is yet more checking required? E.g. when the layer to be selected
    // is a non-copper layer, or when switching between a copper layer
    // and a non-copper layer, or vice-versa?
    // ...

    SetActiveLayer( layer );

    if( displ_opts->m_ContrastModeDisplay )
        m_canvas->Refresh();
}


void PCB_EDIT_FRAME::OnSelectTool( wxCommandEvent& aEvent )
{
    int id = aEvent.GetId();
    int lastToolID = GetToolId();

    INSTALL_UNBUFFERED_DC( dc, m_canvas );
    DISPLAY_OPTIONS* displ_opts = (DISPLAY_OPTIONS*)GetDisplayOptions();

    // Stop the current command and deselect the current tool.
    m_canvas->EndMouseCapture( ID_NO_TOOL_SELECTED, m_canvas->GetDefaultCursor() );

    switch( id )
    {
    case ID_NO_TOOL_SELECTED:
        SetNoToolSelected();
        break;

    case ID_ZOOM_SELECTION:
        // This tool is located on the main toolbar: switch it on or off on click on it
        if( lastToolID != ID_ZOOM_SELECTION )
            SetToolID( ID_ZOOM_SELECTION, wxCURSOR_MAGNIFIER, _( "Zoom to selection" ) );
        else
            SetNoToolSelected();
        break;

    case ID_TRACK_BUTT:
        if( g_Drc_On )
            SetToolID( id, wxCURSOR_PENCIL, _( "Add tracks" ) );
        else
            SetToolID( id, wxCURSOR_QUESTION_ARROW, _( "Add tracks" ) );

        if( (GetBoard()->m_Status_Pcb & LISTE_RATSNEST_ITEM_OK) == 0 )
        {
            Compile_Ratsnest( &dc, true );
        }

        break;

#ifdef PCBNEW_WITH_TRACKITEMS
    case ID_PCB_DRAW_VIA_BUTT:
        SetToolID( id, wxCURSOR_PENCIL, _( "Via Stitching" ) );
        GetBoard()->ViaStitching()->StartDrawingVia( m_canvas, &dc );
        break;
#endif

    case ID_PCB_MODULE_BUTT:
        SetToolID( id, wxCURSOR_PENCIL, _( "Add footprint" ) );
        break;

    case ID_PCB_ZONES_BUTT:
        SetToolID( id, wxCURSOR_PENCIL, _( "Add zones" ) );

        if( displ_opts->m_DisplayZonesMode != 0 )
            DisplayInfoMessage( this, _( "Warning: zone display is OFF!!!" ) );

        if( !GetBoard()->IsHighLightNetON() && (GetBoard()->GetHighLightNetCode() > 0 ) )
            HighLight( &dc );

        break;

    case ID_PCB_KEEPOUT_AREA_BUTT:
        SetToolID( id, wxCURSOR_PENCIL, _( "Add keepout" ) );
        break;

    case ID_PCB_MIRE_BUTT:
        SetToolID( id, wxCURSOR_PENCIL, _( "Add layer alignment target" ) );
        break;

    case ID_PCB_PLACE_OFFSET_COORD_BUTT:
        SetToolID( id, wxCURSOR_PENCIL, _( "Adjust zero" ) );
        break;

    case ID_PCB_PLACE_GRID_COORD_BUTT:
        SetToolID( id, wxCURSOR_PENCIL, _( "Adjust grid origin" ) );
        break;

    case ID_PCB_ADD_LINE_BUTT:
        SetToolID( id, wxCURSOR_PENCIL, _( "Add graphic line" ) );
        break;

    case ID_PCB_ARC_BUTT:
        SetToolID( id, wxCURSOR_PENCIL, _( "Add graphic arc" ) );
        break;

    case ID_PCB_CIRCLE_BUTT:
        SetToolID( id, wxCURSOR_PENCIL, _( "Add graphic circle" ) );
        break;

    case ID_PCB_ADD_TEXT_BUTT:
        SetToolID( id, wxCURSOR_PENCIL, _( "Add text" ) );
        break;

    case ID_COMPONENT_BUTT:
        SetToolID( id, wxCURSOR_HAND, _( "Add footprint" ) );
        break;

    case ID_PCB_DIMENSION_BUTT:
        SetToolID( id, wxCURSOR_PENCIL, _( "Add dimension" ) );
        break;

    case ID_PCB_DELETE_ITEM_BUTT:
        SetToolID( id, wxCURSOR_BULLSEYE, _( "Delete item" ) );
        break;

    case ID_PCB_HIGHLIGHT_BUTT:
        SetToolID( id, wxCURSOR_HAND, _( "Highlight net" ) );
        break;

    case ID_PCB_SHOW_1_RATSNEST_BUTT:
        SetToolID( id, wxCURSOR_HAND, _( "Select rats nest" ) );

        if( ( GetBoard()->m_Status_Pcb & LISTE_RATSNEST_ITEM_OK ) == 0 )
            Compile_Ratsnest( &dc, true );

        break;

    // collect GAL-only tools here
    case ID_PCB_MEASUREMENT_TOOL:
        SetToolID( id, wxCURSOR_DEFAULT, _( "Unsupported tool in this canvas" ) );
        break;
    }
}


void PCB_EDIT_FRAME::moveExact()
{
    MOVE_PARAMETERS params;

    DIALOG_MOVE_EXACT dialog( this, params );
    int ret = dialog.ShowModal();

    if( ret == wxID_OK )
    {
        if( BOARD_ITEM* item = GetScreen()->GetCurItem() )
        {
            // When a pad is modified, the full footprint is saved
            BOARD_ITEM* itemToSave = item;

            if( item->Type() == PCB_PAD_T )
                itemToSave = item->GetParent();

            // Could be moved or rotated
            SaveCopyInUndoList( itemToSave, UR_CHANGED );

            // begin with the default anchor
            wxPoint anchorPoint = item->GetPosition();

            if( item->Type() == PCB_MODULE_T )
            {
                // cast to module to allow access to the pads
                MODULE* mod = static_cast<MODULE*>( item );

                switch( params.anchor )
                {
                case ANCHOR_TOP_LEFT_PAD:
                    if( mod->GetTopLeftPad()->GetAttribute() == PAD_ATTRIB_SMD )
                    {
                        anchorPoint = mod->GetTopLeftPad()->GetBoundingBox().GetPosition();
                    }
                    else
                    {
                        anchorPoint = mod->GetTopLeftPad()->GetPosition();
                    }
                    break;
                case ANCHOR_CENTER_FOOTPRINT:
                    anchorPoint = mod->GetFootprintRect().GetCenter();
                    break;
                case ANCHOR_FROM_LIBRARY:
                    ; // nothing to do
                }
            }

            wxPoint origin;

            switch( params.origin )
            {
            case RELATIVE_TO_USER_ORIGIN:
                origin = GetScreen()->m_O_Curseur;
                break;

            case RELATIVE_TO_GRID_ORIGIN:
                origin = GetGridOrigin();
                break;

            case RELATIVE_TO_DRILL_PLACE_ORIGIN:
                origin = GetAuxOrigin();
                break;

            case RELATIVE_TO_SHEET_ORIGIN:
                origin = wxPoint( 0, 0 );
                break;

            case RELATIVE_TO_CURRENT_POSITION:
                // relative movement means that only the translation values should be used:
                // -> set origin and anchor to zero
                origin = wxPoint( 0, 0 );
                anchorPoint = wxPoint( 0, 0 );
                break;
            }

            wxPoint finalMoveVector = params.translation + origin - anchorPoint;

            item->Move( finalMoveVector );
            item->Rotate( item->GetPosition(), params.rotation );
            m_canvas->Refresh();
        }
    }

    m_canvas->MoveCursorToCrossHair();
}


void PCB_EDIT_FRAME::duplicateItems( bool aIncrement )
{
    BOARD_ITEM* item = GetScreen()->GetCurItem();

    if( !item )
        return;

    // In the board editor, the pads or fp texts can be edited
    // but cannot be duplicated (only the fp editor can do that).
    // only the footprint can be duplicated
    if( item->Type() == PCB_PAD_T || item->Type() == PCB_MODULE_TEXT_T )
        item = static_cast<MODULE*>( item )->GetParent();

    PCB_BASE_EDIT_FRAME::duplicateItem( item, aIncrement );
}


void PCB_BASE_EDIT_FRAME::duplicateItem( BOARD_ITEM* aItem, bool aIncrement )
{
    if( !aItem )
        return;

    // The easiest way to handle a duplicate item command
    // is to simulate a block copy command, which gives us the undo management
    // for free
    if( GetScreen()->m_BlockLocate.GetState() == STATE_NO_BLOCK )
    {
        m_canvas->MoveCursorToCrossHair();

        INSTALL_UNBUFFERED_DC( dc, m_canvas );

        wxPoint crossHairPos = GetCrossHairPosition();

        const BLOCK_COMMAND_T blockType = aIncrement ? BLOCK_COPY_AND_INCREMENT : BLOCK_COPY;

        if( !HandleBlockBegin( &dc, blockType, crossHairPos ) )
            return;

        // Add the item to the block copy pick list:
        PICKED_ITEMS_LIST& itemsList = GetScreen()->m_BlockLocate.GetItems();
        ITEM_PICKER        picker( NULL, UR_UNSPECIFIED );

        picker.SetItem ( aItem );
        itemsList.PushItem( picker );

        // Set 2 coordinates updated by the mouse, because our simulation
        // does not use the mouse to call HandleBlockEnd()
        GetScreen()->m_BlockLocate.SetLastCursorPosition( crossHairPos );
        GetScreen()->m_BlockLocate.SetEnd( crossHairPos );
        HandleBlockEnd( &dc );
    }
}


class LEGACY_ARRAY_CREATOR: public ARRAY_CREATOR
{
public:

    LEGACY_ARRAY_CREATOR( PCB_BASE_EDIT_FRAME& editFrame ):
        ARRAY_CREATOR( editFrame ),
        m_item( m_parent.GetScreen()->GetCurItem() )
    {}

private:

    int getNumberOfItemsToArray() const override
    {
        // only handle single items
        return (m_item != NULL) ? 1 : 0;
    }

    BOARD_ITEM* getNthItemToArray( int n ) const override
    {
        wxASSERT_MSG( n == 0, "Legacy array tool can only handle a single item" );
        return m_item;
    }

    BOARD* getBoard() const override
    {
        return m_parent.GetBoard();
    }

    MODULE* getModule() const override
    {
        return dynamic_cast<MODULE*>( m_item->GetParent() );
    }

    wxPoint getRotationCentre() const override
    {
        return m_item->GetCenter();
    }

    void finalise() override
    {
        m_parent.GetCanvas()->Refresh();
    }

    BOARD_ITEM* m_item; // only have the one
};


void PCB_BASE_EDIT_FRAME::createArray()
{
    LEGACY_ARRAY_CREATOR array_creator( *this );

    array_creator.Invoke();
}
