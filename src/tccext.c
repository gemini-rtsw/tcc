static char rcsid[]="$Id:";
/* *INDENT-OFF* */
/*
*   FILENAME
*   tccext.c
*
*   FUNCTION NAME(S)
*   Tccext_Init - Initialisation function for TCL loadable image
*
*   D L Terrett 18 August 2003
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
#include "tccAst.h"
#include "autorepeat.h"
#include "plosc.h"

/* *INDENT-OFF* */
/*+
 *   Function name:
 *   Tccext_Init
 *
 *   Purpose:
 *   Initialisation function for TCL loadable image.
 *
 *   Description:
 *   Creates the ast and autorepeat On/off commands.
 *-
 */
/* *INDENT-ON* */

int Tccext_Init( Tcl_Interp *interp)
{
    if ( Tcl_InitStubs( interp, "8.0", 0 ) == NULL ) return TCL_ERROR;
    if ( Tk_InitStubs( interp, "8.0", 0 ) == NULL ) return TCL_ERROR;

    Tcl_CreateObjCommand( interp, "ast", Tccext_AstCmd,
        (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);

    Tcl_CreateObjCommand( interp, "autoRepeatOn", autoRepeatOnCmd,
        (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);

    Tcl_CreateObjCommand( interp, "autoRepeatOff", autoRepeatOffCmd,
        (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);

    Tcl_CreateObjCommand( interp, "plosc", ploscCmd,
        (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);

    return TCL_OK;
}
