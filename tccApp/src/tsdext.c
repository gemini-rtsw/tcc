static char rcsid[]="$Id:";
/* *INDENT-OFF* */
/*
*   FILENAME
*   tsdext.c
*
*   FUNCTION NAME(S)
*   Tsdext_Init - Initialisation function for TCL loadable image
*
*   D L Terrett 6 December 2000
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

void tsdDomeView();
void tsdWrapView();
void tsdElView();

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
    if ( Tcl_InitStubs( interp, "8.0", 0 ) == NULL ) return TCL_ERROR;
    if ( Tk_InitStubs( interp, "8.0", 0 ) == NULL ) return TCL_ERROR;

    tsdDomeView();
    tsdWrapView();
    tsdElView();

    Tcl_CreateObjCommand( interp, "ast", Tccext_AstCmd,
        (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);

    return TCL_OK;
}
