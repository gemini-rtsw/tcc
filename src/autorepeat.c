static char rcsid[] = "$Id: autorepeat.c,v 1.1.1.1 1999-05-27 04:53:40 dlt Exp $";
/* *INDENT-OFF* */
/*
*   FILENAME
*   autorepeat.c
*
*   FUNCTION NAME(S)
*   autoRepeatOnCmd  - Implements the autoRepeatOn TK command.
*   autoRepeatOffCmd - Implements the autoRepeatOff TK command.
*
*   D L Terrett 25 November 1998
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

int autoRepeatOnCmd( ClientData clientdata, Tcl_Interp *interp, int argc,
    char *argv[])
{
   Tk_Window tkwin;

   if ( argc != 1 ) {
        Tcl_AppendResult( interp,
           "wrong # args: should be \"autoRepeatOn\"", (char *) NULL);
        return TCL_ERROR;
    }
    tkwin = Tk_MainWindow(interp);
    if ( ! tkwin ) return TCL_ERROR;

    XAutoRepeatOn(Tk_Display(tkwin));
    return TCL_OK;
}

int autoRepeatOffCmd( ClientData clientdata, Tcl_Interp *interp, int argc,
    char *argv[])
{
   Tk_Window tkwin;

   if ( argc != 1 ) {
        Tcl_AppendResult( interp,
           "wrong # args: should be \"autoRepeatOn\"", (char *) NULL);
        return TCL_ERROR;
    }
    tkwin = Tk_MainWindow(interp);
    if ( ! tkwin ) return TCL_ERROR;

    XAutoRepeatOff(Tk_Display(tkwin));
    return TCL_OK;
}
