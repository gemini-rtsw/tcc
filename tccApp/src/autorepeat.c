static char rcsid[] = "$Id: autorepeat.c,v 1.2 2000-12-13 12:38:09 dlt Exp $";
/* *INDENT-OFF* */
/*
*   FILENAME
*   autorepeat.c
*
*   FUNCTION NAME(S)
*   autoRepeatOnCmd  - Implements the autoRepeatOn TK command.
*   autoRepeatOffCmd - Implements the autoRepeatOff TK command.
*
*   D L Terrett 6 December 2000
*
*   Copyright CCLRC
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
 *   autoRepeatOnCmd
 *
 *   Purpose:
 *   Implements the autoRepeatOn command
 *
 *   Description:
 *-
 */
/* *INDENT-ON* */

int autoRepeatOnCmd( ClientData clientdata, Tcl_Interp *interp, int objc,
    Tcl_Obj *CONST objv[])
{
   Tk_Window tkwin;

   if ( objc != 1 ) {
        Tcl_WrongNumArgs( interp, 1, objv, "");
        return TCL_ERROR;
    }
    tkwin = Tk_MainWindow(interp);
    if ( ! tkwin ) return TCL_ERROR;

    XAutoRepeatOn(Tk_Display(tkwin));
    return TCL_OK;
}

int autoRepeatOffCmd( ClientData clientdata, Tcl_Interp *interp, int objc,
    Tcl_Obj *CONST objv[])
{
   Tk_Window tkwin;

   if ( objc != 1 ) {
        Tcl_WrongNumArgs( interp, 1, objv, "");
        return TCL_ERROR;
    }
    tkwin = Tk_MainWindow(interp);
    if ( ! tkwin ) return TCL_ERROR;

    XAutoRepeatOff(Tk_Display(tkwin));
    return TCL_OK;
}
