/*
*   FILENAME
*   autorepeat.h
*
*   DESCRIPTION
*   Routine definitions for the autorepeat commands.
*
*/
/* *INDENT-OFF* */
/*
 * $Log:
 */
/* *INDENT-ON* */

#ifndef AUTOREPEAT_H
#define AUTOREPEAT_H

int autoRepeatOnCmd( ClientData clientdata, Tcl_Interp *interp, int argc,
    char *argv[]);

int autoRepeatOffCmd( ClientData clientdata, Tcl_Interp *interp, int argc,
    char *argv[]);

#endif
