#ifndef PCBNEW_ID_H_
#define PCBNEW_ID_H_

#include <id.h>

/**
 * Command IDs for the printed circuit board editor.
 *
 * Please add IDs that are unique to the printed circuit board editor (Pcbnew)
 * here and not in the global id.h file.  This will prevent the entire project
 * from being rebuilt when adding new commands to the Pcbnew.
 */

#define MAX_ITEMS_IN_PICKER     15  ///< max no. items in the popup menu for item selection

enum pcbnew_ids
{
    ID_MAIN_MENUBAR = ID_END_LIST,
    ID_MICROWAVE_V_TOOLBAR,
    ID_OPEN_MODULE_EDITOR,
    ID_OPEN_MODULE_VIEWER,
    ID_READ_NETLIST,
    ID_SET_RELATIVE_OFFSET,
    ID_COPY_BOARD_AS,
    ID_IMPORT_NON_KICAD_BOARD,

    // Right vertical tool bar command IDs.
    ID_PCB_HIGHLIGHT_BUTT,
    ID_PCB_SHOW_1_RATSNEST_BUTT,
    ID_PCB_MODULE_BUTT,
    ID_TRACK_BUTT,
    ID_PCB_ZONES_BUTT,
    ID_PCB_KEEPOUT_AREA_BUTT,
    ID_PCB_ADD_LINE_BUTT,
    ID_PCB_CIRCLE_BUTT,
    ID_PCB_ARC_BUTT,
    ID_PCB_ADD_TEXT_BUTT,
    ID_PCB_DIMENSION_BUTT,
    ID_PCB_MIRE_BUTT,
    ID_PCB_DELETE_ITEM_BUTT,
    ID_PCB_PLACE_OFFSET_COORD_BUTT,
    ID_PCB_PLACE_GRID_COORD_BUTT,
    ID_PCB_MEASUREMENT_TOOL,

    ID_DIFF_PAIR_BUTT,
    ID_TUNE_SINGLE_TRACK_LEN_BUTT,
    ID_TUNE_DIFF_PAIR_LEN_BUTT,
    ID_TUNE_DIFF_PAIR_SKEW_BUTT,
    ID_MENU_REMOVE_MEANDERS,
    ID_MENU_MITER_TRACES,
    ID_MENU_ADD_TEARDROPS,
    ID_MENU_DIFF_PAIR_DIMENSIONS,
    ID_MENU_INTERACTIVE_ROUTER_SETTINGS,

    ID_PCB_MASK_CLEARANCE,
    ID_PCB_LAYERS_SETUP,
    ID_MENU_PCB_FLIP_VIEW,

    ID_POPUP_PCB_START_RANGE,
    ID_POPUP_PCB_MOVE_MODULE_REQUEST,
    ID_POPUP_PCB_DRAG_MODULE_REQUEST,
    ID_POPUP_PCB_EDIT_MODULE_PRMS,
    ID_POPUP_PCB_EDIT_MODULE_WITH_MODEDIT,
    ID_POPUP_PCB_CHANGE_SIDE_MODULE,
    ID_POPUP_PCB_DELETE_MODULE,
    ID_POPUP_PCB_ROTATE_MODULE_CLOCKWISE,
    ID_POPUP_PCB_ROTATE_MODULE_COUNTERCLOCKWISE,
    ID_POPUP_PCB_EXCHANGE_FOOTPRINTS,

    ID_POPUP_PCB_EDIT_PAD,
    ID_POPUP_PCB_EDIT_MORE_PAD,
    ID_POPUP_PCB_EDIT_PADS_ON_MODULE,
    ID_POPUP_PCB_DELETE_PAD,
    ID_POPUP_PCB_ADD_PAD,
    ID_POPUP_PCB_NEWSIZE_PAD,
    ID_POPUP_PCB_ROTATE_PAD,
    ID_POPUP_PCB_MOVE_PAD_REQUEST,
    ID_POPUP_PCB_DRAG_PAD_REQUEST,
    ID_POPUP_PCB_DUPLICATE_ITEM,
    ID_POPUP_PCB_DUPLICATE_ITEM_AND_INCREMENT,
    ID_POPUP_PCB_MOVE_EXACT,
    ID_POPUP_PCB_CREATE_ARRAY,

    ID_POPUP_PCB_MOVE_TEXTMODULE_REQUEST,
    ID_POPUP_PCB_ROTATE_TEXTMODULE,
    ID_POPUP_PCB_EDIT_TEXTMODULE,
    ID_POPUP_PCB_DELETE_TEXTMODULE,
    ID_POPUP_PCB_RESET_TEXT_SIZE,

    ID_POPUP_PCB_MOVE_TEXTEPCB_REQUEST,
    ID_POPUP_PCB_ROTATE_TEXTEPCB,
    ID_POPUP_PCB_COPY_TEXTEPCB,
    ID_POPUP_PCB_FLIP_TEXTEPCB,
    ID_POPUP_PCB_EDIT_TEXTEPCB,
    ID_POPUP_PCB_DELETE_TEXTEPCB,

    ID_POPUP_PCB_MOVE_DRAWING_REQUEST,
    ID_POPUP_PCB_EDIT_DRAWING,
    ID_POPUP_PCB_DELETE_DRAWING,
    ID_POPUP_PCB_DELETE_DRAWING_LAYER,
    ID_POPUP_PCB_END_LINE,

    ID_POPUP_PCB_BEGIN_TRACK,
    ID_POPUP_PCB_EDIT_TRACK,
    ID_POPUP_PCB_DELETE_TRACK,
    ID_POPUP_PCB_DELETE_TRACKNET,
    ID_POPUP_PCB_DELETE_TRACK_MNU,

#ifdef PCBNEW_WITH_TRACKITEMS
    //TrackNodeItems
    ID_POPUP_PCB_TRACKITEMS_COMMON_MNU,
    ID_POPUP_PCB_VIA_PICK_SIZE,
    ID_POPUP_PCB_TRACK_PICK_WIDTH,
    ID_POPUP_PCB_TRACKS_MARK_SHARP_ANGLES,
    ID_POPUP_PCB_TRACKS_CONNECT_CENTER_IN_ITEM,
    //Teardrops
    ID_POPUP_PCB_TEARDROPS_COMMON_MNU,
    ID_POPUP_PCB_TEARDROP_EDIT_LENGTH,
    ID_POPUP_PCB_TEARDROP_EDIT_WIDTH,
    ID_POPUP_PCB_TEARDROP_COPYCURRENT,
    ID_POPUP_PCB_TEARDROP_LOCK_TOGGLE,
    ID_POPUP_PCB_TEARDROPS_LOCK_ALL_SAME,
    ID_POPUP_PCB_TEARDROPS_UNLOCK_ALL_SAME,
    ID_POPUP_PCB_TEARDROPS_LOCK_MODULE,
    ID_POPUP_PCB_TEARDROPS_UNLOCK_MODULE,
    ID_POPUP_PCB_TEARDROPS_LOCK, //via or pad
    ID_POPUP_PCB_TEARDROPS_UNLOCK, //via or pad
    ID_POPUP_PCB_TEARDROPS_LOCK_NET, //net, junctioms & T- too.
    ID_POPUP_PCB_TEARDROPS_UNLOCK_NET, //net, junctioms & T- too.
    ID_POPUP_PCB_TEARDROPS_LOCK_MODULES_ALL, //0
    ID_POPUP_PCB_TEARDROPS_LOCK_VIAS_ALL, //1
    ID_POPUP_PCB_TJUNCTIONS_LOCK_ALL, //2
    ID_POPUP_PCB_JUNCTIONS_LOCK_ALL, //3
    ID_POPUP_PCB_TEARDROPS_UNLOCK_MODULES_ALL, //0
    ID_POPUP_PCB_TEARDROPS_UNLOCK_VIAS_ALL, //1
    ID_POPUP_PCB_TJUNCTIONS_UNLOCK_ALL, //2
    ID_POPUP_PCB_JUNCTIONS_UNLOCK_ALL, //3
    ID_POPUP_PCB_TEARDROP_SELECT_TEARDROP,
    ID_POPUP_PCB_TEARDROP_SELECT_FILLET,
    ID_POPUP_PCB_TEARDROP_SELECT_SUBLAND,
    ID_POPUP_PCB_TEARDROP_SELECT_ZERO,
    ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST,
    ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_LAST = ID_POPUP_PCB_TEARDROP_SIZE_LENGTH_FIRST + 9,
    ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST,
    ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_LAST = ID_POPUP_PCB_TEARDROP_SIZE_WIDTH_FIRST + 9,
    ID_POPUP_PCB_TEARDROP_NUM_SEGMENTS_FIRST,
    ID_POPUP_PCB_TEARDROP_NUM_SEGMENTS_LAST = ID_POPUP_PCB_TEARDROP_NUM_SEGMENTS_FIRST + 9,
    ID_POPUP_PCB_TEARDROP_SET_DEFAULT_PARAMS,
    ID_POPUP_PCB_TEARDROP_PLACE,    //Place teardrop to via or pad, or place tjunction / junction
    ID_POPUP_PCB_TEARDROP_CHANGE,   //Change teardrop from via, pad, or tjunction / junction
    ID_POPUP_PCB_TEARDROP_DELETE,   //Delete teardrop from via or pad, or place tjunction / junction
    ID_POPUP_PCB_TEARDROPS_PLACE,   //Place all connected segments of via or pad
    ID_POPUP_PCB_TEARDROPS_CHANGE,  //Change from all connected segments of via or pad
    ID_POPUP_PCB_TEARDROPS_DELETE,  //Delete from all connected segments of via or pad
    ID_POPUP_PCB_TEARDROPS_PLACE_MODULE,
    ID_POPUP_PCB_TEARDROPS_CHANGE_MODULE,
    ID_POPUP_PCB_TEARDROPS_DELETE_MODULE,
    ID_POPUP_PCB_TEARDROPS_PLACE_NET,
    ID_POPUP_PCB_TEARDROPS_CHANGE_NET,
    ID_POPUP_PCB_TEARDROPS_DELETE_NET,
    ID_POPUP_PCB_TJUNCTIONS_PLACE_NET,
    ID_POPUP_PCB_TJUNCTIONS_CHANGE_NET,
    ID_POPUP_PCB_TJUNCTIONS_DELETE_NET,
    ID_POPUP_PCB_JUNCTIONS_PLACE_NET,
    ID_POPUP_PCB_JUNCTIONS_CHANGE_NET,
    ID_POPUP_PCB_JUNCTIONS_DELETE_NET,
    ID_POPUP_PCB_TEARDROPS_PLACE_MODULES_ALL, //0
    ID_POPUP_PCB_TEARDROPS_PLACE_VIAS_ALL, //1
    ID_POPUP_PCB_TJUNCTIONS_PLACE_ALL, //2
    ID_POPUP_PCB_JUNCTIONS_PLACE_ALL, //3
    ID_POPUP_PCB_TEARDROPS_CHANGE_MODULES_ALL, //0
    ID_POPUP_PCB_TEARDROPS_CHANGE_VIAS_ALL, //1
    ID_POPUP_PCB_TJUNCTIONS_CHANGE_ALL, //2
    ID_POPUP_PCB_JUNCTIONS_CHANGE_ALL, //3
    ID_POPUP_PCB_TEARDROPS_DELETE_MODULES_ALL, //0
    ID_POPUP_PCB_TEARDROPS_DELETE_VIAS_ALL, //1 
    ID_POPUP_PCB_TJUNCTIONS_DELETE_ALL, //2
    ID_POPUP_PCB_JUNCTIONS_DELETE_ALL, //3
    ID_POPUP_PCB_TEARDROPS_MARK_WARNINGS_MODULES, //0
    ID_POPUP_PCB_TEARDROPS_MARK_WARNINGS_TEARDROPS_VIAS, //1
    ID_POPUP_PCB_TEARDROPS_MARK_WARNINGS_TJUNCTIONS, //2
    ID_POPUP_PCB_TEARDROPS_MARK_WARNINGS_JUNCTIONS, //3
    ID_POPUP_PCB_TEARDROPS_MARK_DIFF_MODULES, //0
    ID_POPUP_PCB_TEARDROPS_MARK_DIFF_TEARDROPS_VIAS, //1
    ID_POPUP_PCB_TEARDROPS_MARK_DIFF_TJUNCTIONS, //2
    ID_POPUP_PCB_TEARDROPS_MARK_DIFF_JUNCTIONS, //3
    ID_POPUP_PCB_TEARDROPS_MARK_CURR_MODULES, //0
    ID_POPUP_PCB_TEARDROPS_MARK_CURR_TEARDROPS_VIAS, //1
    ID_POPUP_PCB_TEARDROPS_MARK_CURR_TJUNCTIONS, //2
    ID_POPUP_PCB_TEARDROPS_MARK_CURR_JUNCTIONS, //3
    ID_POPUP_PCB_TJUNCTIONS_MARK_NOT_T,

    //Via Stitching
    ID_POPUP_PCB_PLACE_ZONE_THROUGH_VIA,
    ID_POPUP_PCB_PLACE_ZONE_BLIND_BURIED_VIA,
    ID_POPUP_PCB_SEL_LAYERS_AND_PLACE_ZONE_BLIND_BURIED_VIA,
    ID_POPUP_PCB_SEL_LAYER_AND_PLACE_ZONE_THROUGH_VIA,
    ID_POPUP_PCB_CREATE_VIA_ARRAY,

    //Rounded Corners
    ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_COMMON_MNU,
    ID_POPUP_PCB_ROUNDEDTRACKSCORNER_PLACE,
    ID_POPUP_PCB_ROUNDEDTRACKSCORNER_PLACE_NET,
    ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_PLACE_ALL,
    ID_POPUP_PCB_ROUNDEDTRACKSCORNER_DELETE,
    ID_POPUP_PCB_ROUNDEDTRACKSCORNER_DELETE_NET,
    ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_DELETE_ALL,
    ID_POPUP_PCB_ROUNDEDTRACKSCORNER_CHANGE,
    ID_POPUP_PCB_ROUNDEDTRACKSCORNER_CHANGE_ALL,
    ID_POPUP_PCB_ROUNDEDTRACKSCORNER_COPYCURRENT,
    ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_SET_FIRST,
    ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_SET_LAST = ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_SET_FIRST + 10,
    ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_RATIO_FIRST,
    ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_RATIO_LAST = ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SIZE_LENGTH_RATIO_FIRST + 10,
    ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_SET_DEFAULT_PARAMS,
    ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_EDIT_LENGTH_SET,
    ID_POPUP_PCB_ROUNDEDTRACKSCORNERS_EDIT_LENGTH_RATIO,
    ID_POPUP_PCB_ROUNDEDTRACKSCORNER_CONVERT_SEGMENTED,
    
    ID_POPUP_PCB_GOTO_NEXT_MARKER,
#endif
    
    ID_POPUP_PCB_MOVE_ZONE_CORNER,
    ID_POPUP_PCB_ADD_ZONE_CORNER,
    ID_POPUP_PCB_DELETE_ZONE_CORNER,
    ID_POPUP_PCB_PLACE_ZONE_CORNER,
    ID_POPUP_PCB_DELETE_ZONE_LAST_CREATED_CORNER,
    ID_POPUP_PCB_EDIT_ZONE_PARAMS,
    ID_POPUP_PCB_DELETE_ZONE,
    ID_POPUP_PCB_STOP_CURRENT_EDGE_ZONE,
    ID_POPUP_PCB_FILL_ALL_ZONES,
    ID_POPUP_PCB_FILL_ZONE,
    ID_POPUP_PCB_DELETE_ZONE_CONTAINER,
    ID_POPUP_PCB_ZONE_DUPLICATE,
    ID_POPUP_PCB_ZONE_ADD_SIMILAR_ZONE,
    ID_POPUP_PCB_ZONE_ADD_CUTOUT_ZONE,
    ID_POPUP_PCB_DELETE_ZONE_CUTOUT,
    ID_POPUP_PCB_MOVE_ZONE_OUTLINES,
    ID_POPUP_PCB_PLACE_ZONE_OUTLINES,
    ID_POPUP_PCB_DRAG_ZONE_OUTLINE_SEGMENT,
    ID_POPUP_PCB_PLACE_DRAGGED_ZONE_OUTLINE_SEGMENT,
    ID_POPUP_PCB_REMOVE_FILLED_AREAS_IN_ALL_ZONES,
    ID_POPUP_PCB_REMOVE_FILLED_AREAS_IN_CURRENT_ZONE,

    ID_POPUP_PCB_DELETE_MARKER,

    ID_POPUP_PCB_DELETE_DIMENSION,
    ID_POPUP_PCB_MOVE_TEXT_DIMENSION_REQUEST,

    ID_POPUP_PCB_MOVE_MIRE_REQUEST,
    ID_POPUP_PCB_DELETE_MIRE,
    ID_POPUP_PCB_EDIT_MIRE,

    ID_POPUP_PCB_STOP_CURRENT_DRAWING,

    ID_POPUP_PCB_EDIT_DIMENSION,
    ID_POPUP_PCB_END_TRACK,
    ID_POPUP_PCB_PLACE_THROUGH_VIA,
    ID_POPUP_PCB_SELECT_CU_LAYER_AND_PLACE_THROUGH_VIA,
    ID_POPUP_PCB_PLACE_BLIND_BURIED_VIA,
    ID_POPUP_PCB_SELECT_CU_LAYER_AND_PLACE_BLIND_BURIED_VIA,
    ID_POPUP_PCB_PLACE_MICROVIA,
    ID_POPUP_PCB_SWITCH_TRACK_POSTURE,

    ID_POPUP_PCB_APPLY_PAD_SETTINGS,
    ID_POPUP_PCB_COPY_PAD_SETTINGS,

    ID_POPUP_PCB_EDIT_TRACKSEG,
    ID_POPUP_PCB_EDIT_TRACK_MNU,
    ID_POPUP_PCB_EDIT_NET,
    ID_POPUP_PCB_SELECT_LAYER,
    ID_POPUP_PCB_SELECT_CU_LAYER,
    ID_POPUP_PCB_SELECT_NO_CU_LAYER,
    ID_POPUP_PCB_SELECT_LAYER_PAIR,
    ID_POPUP_PCB_DELETE_EDGE,
    ID_POPUP_PCB_MOVE_EDGE,
    ID_POPUP_PCB_PLACE_EDGE,
    ID_POPUP_PCB_END_EDGE,
    ID_POPUP_PCB_LOCK_ON_TRACKSEG,
    ID_POPUP_PCB_LOCK_OFF_TRACKSEG,
    ID_POPUP_PCB_LOCK_ON_TRACK,
    ID_POPUP_PCB_LOCK_OFF_TRACK,
    ID_POPUP_PCB_LOCK_ON_NET,
    ID_POPUP_PCB_LOCK_OFF_NET,
    ID_POPUP_PCB_SETFLAGS_TRACK_MNU,

    ID_POPUP_PCB_GLOBAL_IMPORT_PAD_SETTINGS,
    ID_POPUP_PCB_EDIT_ALL_VIAS_AND_TRACK_SIZE,
    ID_POPUP_PCB_EDIT_ALL_VIAS_SIZE,
    ID_POPUP_PCB_EDIT_ALL_TRACK_SIZE,
    ID_POPUP_PCB_DISPLAY_FOOTPRINT_DOC,
    ID_POPUP_PCB_MOVE_TRACK_NODE,
    ID_POPUP_PCB_BREAK_TRACK,
    ID_POPUP_PCB_PLACE_MOVED_TRACK_NODE,
    ID_POPUP_PCB_DRAG_TRACK_SEGMENT_KEEP_SLOPE,
    ID_POPUP_PCB_DRAG_TRACK_SEGMENT,
    ID_POPUP_PCB_MOVE_TRACK_SEGMENT,
    ID_POPUP_PCB_GET_AND_MOVE_MODULE_REQUEST,
    ID_POPUP_PCB_GETINFO_MARKER,
    ID_POPUP_PCB_END_RANGE,

    // Tracks and vias sizes general options
    ID_AUX_TOOLBAR_PCB_VIA_SIZE,
    ID_AUX_TOOLBAR_PCB_TRACK_WIDTH,
    ID_AUX_TOOLBAR_PCB_SELECT_AUTO_WIDTH,
    ID_POPUP_PCB_SELECT_WIDTH_START_RANGE,
    ID_POPUP_PCB_SELECT_WIDTH,
    ID_POPUP_PCB_SELECT_CUSTOM_WIDTH,
    ID_POPUP_PCB_SELECT_AUTO_WIDTH,
    ID_POPUP_PCB_SELECT_USE_NETCLASS_VALUES,
    ID_POPUP_PCB_SELECT_WIDTH1,
    ID_POPUP_PCB_SELECT_WIDTH2,
    ID_POPUP_PCB_SELECT_WIDTH3,
    ID_POPUP_PCB_SELECT_WIDTH4,
    ID_POPUP_PCB_SELECT_WIDTH5,
    ID_POPUP_PCB_SELECT_WIDTH6,
    ID_POPUP_PCB_SELECT_WIDTH7,
    ID_POPUP_PCB_SELECT_WIDTH8,
    ID_POPUP_PCB_SELECT_WIDTH9,
    ID_POPUP_PCB_SELECT_WIDTH10,
    ID_POPUP_PCB_SELECT_WIDTH11,
    ID_POPUP_PCB_SELECT_WIDTH12,
    ID_POPUP_PCB_SELECT_WIDTH13,
    ID_POPUP_PCB_SELECT_WIDTH14,
    ID_POPUP_PCB_SELECT_WIDTH15,
    ID_POPUP_PCB_SELECT_WIDTH16,
    ID_POPUP_PCB_SELECT_VIASIZE,
    ID_POPUP_PCB_SELECT_VIASIZE1,
    ID_POPUP_PCB_SELECT_VIASIZE2,
    ID_POPUP_PCB_SELECT_VIASIZE3,
    ID_POPUP_PCB_SELECT_VIASIZE4,
    ID_POPUP_PCB_SELECT_VIASIZE5,
    ID_POPUP_PCB_SELECT_VIASIZE6,
    ID_POPUP_PCB_SELECT_VIASIZE7,
    ID_POPUP_PCB_SELECT_VIASIZE8,
    ID_POPUP_PCB_SELECT_VIASIZE9,
    ID_POPUP_PCB_SELECT_VIASIZE10,
    ID_POPUP_PCB_SELECT_VIASIZE11,
    ID_POPUP_PCB_SELECT_VIASIZE12,
    ID_POPUP_PCB_SELECT_VIASIZE13,
    ID_POPUP_PCB_SELECT_VIASIZE14,
    ID_POPUP_PCB_SELECT_VIASIZE15,
    ID_POPUP_PCB_SELECT_VIASIZE16,
    ID_POPUP_PCB_SELECT_WIDTH_END_RANGE,

    // reserve a block of MAX_ITEMS_IN_PICKER ids for the item selection popup
    ID_POPUP_PCB_ITEM_SELECTION_START,
    ID_POPUP_PCB_ITEM_SELECTION_END = MAX_ITEMS_IN_PICKER + ID_POPUP_PCB_ITEM_SELECTION_START,
    ID_POPUP_PCB_AUTOPLACE_START_RANGE,
    ID_POPUP_PCB_AUTOPLACE_FIXE_MODULE,
    ID_POPUP_PCB_AUTOPLACE_FREE_MODULE,
    ID_POPUP_PCB_AUTOPLACE_FREE_ALL_MODULES,
    ID_POPUP_PCB_AUTOPLACE_FIXE_ALL_MODULES,
    ID_POPUP_PCB_AUTOPLACE_CURRENT_MODULE,
    ID_POPUP_PCB_SPREAD_ALL_MODULES,
    ID_POPUP_PCB_SPREAD_NEW_MODULES,
    ID_POPUP_PCB_AUTOPLACE_COMMANDS,
    ID_POPUP_PCB_AUTOPLACE_ALL_MODULES,
    ID_POPUP_PCB_AUTOPLACE_NEW_MODULES,
    ID_POPUP_PCB_AUTOPLACE_NEXT_MODULE,

    ID_POPUP_PCB_AUTOROUTE_COMMANDS,
    ID_POPUP_PCB_AUTOROUTE_ALL_MODULES,
    ID_POPUP_PCB_AUTOROUTE_MODULE,
    ID_POPUP_PCB_AUTOROUTE_PAD,
    ID_POPUP_PCB_AUTOROUTE_NET,

    ID_POPUP_PCB_AUTOROUTE_RESET_UNROUTED,
    ID_POPUP_PCB_AUTOROUTE_SELECT_LAYERS,
    ID_POPUP_PCB_AUTOPLACE_END_RANGE,

    ID_MENU_READ_BOARD_BACKUP_FILE,
    ID_MENU_RECOVER_BOARD_AUTOSAVE,
    ID_MENU_ARCHIVE_MODULES,
    ID_MENU_ARCHIVE_MODULES_IN_LIBRARY,
    ID_MENU_CREATE_LIBRARY_AND_ARCHIVE_MODULES,
    ID_MENU_MICELLANOUS,
    ID_MENU_LIST_NETS,
    ID_PCB_EDIT_ALL_VIAS_AND_TRACK_SIZE,
    ID_MENU_PCB_CLEAN,
    ID_MENU_PCB_SWAP_LAYERS,
    ID_MENU_PCB_RESET_TEXTMODULE_FIELDS_SIZES,

    ID_GEN_EXPORT_FILE_IDF3,
    ID_GEN_EXPORT_FILE_VRML,
    ID_GEN_EXPORT_FILE_STEP,
    ID_GEN_EXPORT_SPECCTRA,
    ID_GEN_EXPORT_FILE_GENCADFORMAT,
    ID_GEN_EXPORT_FILE_MODULE_REPORT,
    ID_GEN_IMPORT_SPECCTRA_SESSION,
    ID_GEN_IMPORT_SPECCTRA_DESIGN,
    ID_GEN_IMPORT_DXF_FILE,

    ID_TOOLBARH_PCB_MODE_MODULE,
    ID_TOOLBARH_PCB_MODE_TRACKS,
    ID_TOOLBARH_PCB_FREEROUTE_ACCESS,
    ID_TOOLBARH_PCB_SCRIPTING_CONSOLE,

    ID_TOOLBARH_PCB_ACTION_PLUGIN,
    ID_TOOLBARH_PCB_ACTION_PLUGIN_REFRESH,

    ID_AUX_TOOLBAR_PCB_SELECT_LAYER_PAIR,

    ID_PCB_GEN_POS_MODULES_FILE,
    ID_PCB_GEN_DRILL_FILE,
    ID_PCB_GEN_D356_FILE,

    ID_PCB_PAD_SETUP,

    ID_PCB_DRAWINGS_WIDTHS_SETUP,

    ID_PCB_GEN_CMP_FILE,
    ID_MENU_PCB_SHOW_3D_FRAME,
    ID_MENU_CANVAS_LEGACY,
    ID_MENU_CANVAS_OPENGL,
    ID_MENU_CANVAS_CAIRO,
    ID_PCB_USER_GRID_SETUP,
    ID_PCB_GEN_BOM_FILE_FROM_BOARD,
    ID_PCB_LIB_WIZARD,
    ID_PCB_3DSHAPELIB_WIZARD,
    ID_PCB_LIB_TABLE_EDIT,
    ID_MENU_PCB_SHOW_DESIGN_RULES_DIALOG,
    ID_MENU_PCB_SHOW_HIDE_LAYERS_MANAGER,
    ID_MENU_PCB_SHOW_HIDE_MUWAVE_TOOLBAR,

    ID_TB_OPTIONS_SHOW_MANAGE_LAYERS_VERTICAL_TOOLBAR,
    ID_TB_OPTIONS_SHOW_ZONES,
    ID_TB_OPTIONS_SHOW_ZONES_DISABLE,
    ID_TB_OPTIONS_SHOW_ZONES_OUTLINES_ONLY,
    ID_TB_OPTIONS_DRC_OFF,
    ID_TB_OPTIONS_SHOW_RATSNEST,
    ID_TB_OPTIONS_AUTO_DEL_TRACK,
    ID_TB_OPTIONS_SHOW_VIAS_SKETCH,
    ID_TB_OPTIONS_SHOW_TRACKS_SKETCH,
    ID_TB_OPTIONS_SHOW_HIGH_CONTRAST_MODE,
    ID_TB_OPTIONS_SHOW_EXTRA_VERTICAL_TOOLBAR_MICROWAVE,

    ID_PCB_MUWAVE_START_CMD,
    ID_PCB_MUWAVE_TOOL_DISPLAY_TOOLS,
    ID_PCB_MUWAVE_TOOL_SELF_CMD,
    ID_PCB_MUWAVE_TOOL_GAP_CMD,
    ID_PCB_MUWAVE_TOOL_STUB_CMD,
    ID_PCB_MUWAVE_TOOL_STUB_ARC_CMD,
    ID_PCB_MUWAVE_TOOL_FUNCTION_SHAPE_CMD,
    ID_PCB_MUWAVE_END_CMD,

    ID_DRC_CONTROL,
    ID_PCB_GLOBAL_DELETE,
    ID_POPUP_PCB_DELETE_TRACKSEG,
    ID_TOOLBARH_PCB_SELECT_LAYER,
    ID_PCB_DISPLAY_OPTIONS_SETUP,

    // Module editor right vertical tool bar commands.
    ID_MODEDIT_PAD_TOOL,
    ID_MODEDIT_LINE_TOOL,
    ID_MODEDIT_CIRCLE_TOOL,
    ID_MODEDIT_ARC_TOOL,
    ID_MODEDIT_TEXT_TOOL,
    ID_MODEDIT_ANCHOR_TOOL,
    ID_MODEDIT_DELETE_TOOL,
    ID_MODEDIT_PLACE_GRID_COORD,
    ID_MODEDIT_MEASUREMENT_TOOL,

    // ID used in module editor:
    ID_POPUP_MODEDIT_GLOBAL_EDIT_EDGE,
    ID_POPUP_MODEDIT_EDIT_WIDTH_ALL_EDGE,
    ID_POPUP_MODEDIT_EDIT_LAYER_ALL_EDGE,
    ID_POPUP_MODEDIT_ENTER_EDGE_WIDTH,
    ID_POPUP_MODEDIT_EDIT_EDGE,
    ID_MODEDIT_CHECK,
    ID_MODEDIT_SELECT_CURRENT_LIB,
    ID_MODEDIT_SAVE_LIBMODULE,
    ID_MODEDIT_SAVE_LIBRARY_AS,
    ID_MODEDIT_DELETE_PART,
    ID_MODEDIT_NEW_MODULE,
    ID_MODEDIT_NEW_MODULE_FROM_WIZARD,
    ID_MODEDIT_SHEET_SET,
    ID_MODEDIT_LOAD_MODULE,
    ID_MODEDIT_PAD_SETTINGS,
    ID_MODEDIT_LOAD_MODULE_FROM_BOARD,
    ID_MODEDIT_INSERT_MODULE_IN_BOARD,
    ID_MODEDIT_UPDATE_MODULE_IN_BOARD,
    ID_MODEDIT_EDIT_MODULE_PROPERTIES,
    ID_MODEDIT_TRANSFORM_MODULE,
    ID_MODEDIT_MODULE_ROTATE,
    ID_MODEDIT_MODULE_MIRROR,
    ID_MODEDIT_MODULE_MOVE_EXACT,
    ID_MODEDIT_IMPORT_PART,
    ID_MODEDIT_EXPORT_PART,
    ID_MODEDIT_CREATE_NEW_LIB_AND_SAVE_CURRENT_PART,
    ID_POPUP_MODEDIT_EDIT_BODY_ITEM,

    ID_MODVIEW_LIBWINDOW,
    ID_MODVIEW_FOOTPRINT_WINDOW,
    ID_MODVIEW_LIB_LIST,
    ID_MODVIEW_FOOTPRINT_LIST,
    ID_MODVIEW_SELECT_LIB,
    ID_MODVIEW_SELECT_PART,
    ID_MODVIEW_PREVIOUS,
    ID_MODVIEW_NEXT,
    ID_MODVIEW_SHOW_3D_VIEW,
    ID_MODVIEW_FOOTPRINT_EXPORT_TO_BOARD,
    ID_FOOTPRINT_WIZARD_WINDOW,
    ID_FOOTPRINT_WIZARD_PAGES,
    ID_FOOTPRINT_WIZARD_PARAMETERS,
    ID_FOOTPRINT_WIZARD_NEXT,
    ID_FOOTPRINT_WIZARD_PREVIOUS,
    ID_FOOTPRINT_WIZARD_DONE,
    ID_FOOTPRINT_WIZARD_SHOW_3D_VIEW,
    ID_FOOTPRINT_WIZARD_PAGE_LIST,
    ID_FOOTPRINT_WIZARD_PARAMETER_LIST,
    ID_FOOTPRINT_WIZARD_PAGES_WINDOW,
    ID_FOOTPRINT_WIZARD_PARAMETERS_WINDOW,
    ID_FOOTPRINT_WIZARD_SELECT_WIZARD,
    ID_FOOTPRINT_WIZARD_RESET_TO_DEFAULT,
    ID_FOOTPRINT_WIZARD_EXPORT_TO_BOARD,

    ID_UPDATE_PCB_FROM_SCH,
    ID_PCBNEW_END_LIST
};

#endif  // PCBNEW_ID_H_
