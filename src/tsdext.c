static char rcsid[]="$Id:";
/* *INDENT-OFF* */
/*
*   FILENAME
*   tsdext.c
*
*   FUNCTION NAME(S)
*   Tsdext_Init - Initialisation function for TCL loadable image
*
*   D L Terrett 1 September 1998
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

/* *INDENT-OFF* */
/*+
 *   Function name:
 *   Tsdext_Init
 *
 *   Purpose:
 *   Initialisation function for TCL loadable image.
 *
 *   Description:
 *   Creates the DomeView, Wrapview and ElView canvas item types and the ast 
 *   command.
 *-
 */
/* *INDENT-ON* */

int Tsdext_Init( Tcl_Interp *interp)
{
    tsdDomeView();
    tsdWrapView();
    tsdElView();

    Tcl_CreateCommand( interp, "ast", Tccext_AstCmd,
        (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);

    return TCL_OK;
}
