static char rcsid[]="$Id:";
/* *INDENT-OFF* */
/*
*   FILENAME
*   tccext.c
*
*   FUNCTION NAME(S)
*   Tccext_Init - Initialisation function for TCL loadable image
*
*   D L Terrett 17 February 2000
*
*   Copyright CCLRC
*
*/
/*
 * $Log:
 */
/* *INDENT-ON* */

#include <tcl.h>
#include <tk.h>
#include "tccPreview.h"
#include "tccAst.h"
#include "autorepeat.h"

/* *INDENT-OFF* */
/*+
 *   Function name:
 *   Tccext_Init
 *
 *   Purpose:
 *   Initialisation function for TCL loadable image.
 *
 *   Description:
 *   Creates the tccPreview, ast and autorepeat On/off commands
 *   and some bitmaps.
 *-
 */
/* *INDENT-ON* */

int Tccext_Init( Tcl_Interp *interp)
{

#include "edit.xbm"
#include "delete.xbm"
#include "up.xbm"
#include "down.xbm"
#include "left.xbm"
#include "right.xbm"

    if ( Tcl_InitStubs( interp, "8.0", 0 ) == NULL ) return TCL_ERROR;

    Tcl_CreateCommand( interp, "tccPreview", Tccext_PreviewCmd,
        (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);

    Tcl_CreateCommand( interp, "ast", Tccext_AstCmd,
        (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);

    Tcl_CreateCommand( interp, "autoRepeatOn", autoRepeatOnCmd,
        (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);

    Tcl_CreateCommand( interp, "autoRepeatOff", autoRepeatOffCmd,
        (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);

    if ( Tk_DefineBitmap( interp, Tk_GetUid("edit"), (char*)edit_bits, 
        edit_width, edit_height) != TCL_OK ) {
        return TCL_ERROR;
    }

    if ( Tk_DefineBitmap( interp, Tk_GetUid("delete"), (char*)delete_bits, 
        delete_width, delete_height) != TCL_OK ) {
        return TCL_ERROR;
    }

#if 0
    if ( Tk_DefineBitmap( interp, Tk_GetUid("up"), (char*)up_bits, 
        up_width, up_height) != TCL_OK ) {
        return TCL_ERROR;
    }

    if ( Tk_DefineBitmap( interp, Tk_GetUid("down"), (char*)down_bits, 
        down_width, down_height) != TCL_OK ) {
        return TCL_ERROR;
    }

    if ( Tk_DefineBitmap( interp, Tk_GetUid("left"), (char*)left_bits, 
        left_width, left_height) != TCL_OK ) {
        return TCL_ERROR;
    }

    if ( Tk_DefineBitmap( interp, Tk_GetUid("right"), (char*)right_bits, 
        right_width, right_height) != TCL_OK ) {
        return TCL_ERROR;
    }
#endif

    return TCL_OK;
}
