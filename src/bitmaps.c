static char rcsid[]="$Id:";
/* *INDENT-OFF* */
/*
*   FILENAME
*   bitmaps.c
*
*   FUNCTION NAME(S)
*   Bitmaps_Init - Initialisation function for TCL loadable image
*
*   D L Terrett 21 April 2001
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

/* *INDENT-OFF* */
/*+
 *   Function name:
 *   Bitmaps_Init
 *
 *   Purpose:
 *   Initialisation function for TCL loadable image.
 *
 *   Description:
 *   Creates some bitmaps.
 *-
 */
/* *INDENT-ON* */

int Bitmaps_Init( Tcl_Interp *interp)
{

#include "edit.xbm"
#include "delete.xbm"
#include "rotate.xbm"
#include "arotate.xbm"
#include "hflip.xbm"
#include "vflip.xbm"
#include "reset.xbm"

    if ( Tcl_InitStubs( interp, "8.0", 0 ) == NULL ) return TCL_ERROR;

    if ( Tk_DefineBitmap( interp, Tk_GetUid("edit"), (char*)edit_bits, 
        edit_width, edit_height) != TCL_OK ) {
        return TCL_ERROR;
    }

    if ( Tk_DefineBitmap( interp, Tk_GetUid("delete"), (char*)delete_bits, 
        delete_width, delete_height) != TCL_OK ) {
        return TCL_ERROR;
    }

    if ( Tk_DefineBitmap( interp, Tk_GetUid("rotate"), (char*)rotate_bits, 
        rotate_width, rotate_height) != TCL_OK ) {
        return TCL_ERROR;
    }

    if ( Tk_DefineBitmap( interp, Tk_GetUid("arotate"), (char*)arotate_bits, 
        arotate_width, arotate_height) != TCL_OK ) {
        return TCL_ERROR;
    }

    if ( Tk_DefineBitmap( interp, Tk_GetUid("hflip"), (char*)hflip_bits, 
        hflip_width, hflip_height) != TCL_OK ) {
        return TCL_ERROR;
    }

    if ( Tk_DefineBitmap( interp, Tk_GetUid("vflip"), (char*)vflip_bits, 
        vflip_width, vflip_height) != TCL_OK ) {
        return TCL_ERROR;
    }

    if ( Tk_DefineBitmap( interp, Tk_GetUid("reset"), (char*)reset_bits, 
        reset_width, reset_height) != TCL_OK ) {
        return TCL_ERROR;
    }

    return TCL_OK;
}
