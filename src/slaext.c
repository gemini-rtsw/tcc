static char rcsid[]="$Id:";
/* *INDENT-OFF* */
/*
*   FILENAME
*   slaext.c
*
*   FUNCTION NAME(S)
*   Slaext_Init - Initialisation function for TCL loadable image
*
*   D L Terrett 2 December 1998
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
#include <slalib.h>

static int DafinCmd( ClientData clientdata, Tcl_Interp *interp, int argc,
    char *argv[]);
static int Dr2afCmd( ClientData clientdata, Tcl_Interp *interp, int argc,
    char *argv[]);
static int Dr2tfCmd( ClientData clientdata, Tcl_Interp *interp, int argc,
    char *argv[]);

/* *INDENT-OFF* */
/*+
 *   Function name:
 *   Slaext_Init
 *
 *   Purpose:
 *   Initialisation function for TCL loadable image.
 *
 *   Description:
 *-
 */
/* *INDENT-ON* */

int Slaext_Init( Tcl_Interp *interp)
{

    Tcl_CreateCommand( interp, "slaDafin", DafinCmd,
        (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand( interp, "slaDr2af", Dr2afCmd,
        (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand( interp, "slaDr2tf", Dr2tfCmd,
        (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);

    return TCL_OK;
}

static int DafinCmd( ClientData clientdata, Tcl_Interp *interp, int argc,
    char *argv[])
{
/*
 * TCL front end for slaDafin.
 */
   int status;
   int nstrt = 1;
   double angle;

   if ( argc < 2 ) {
      Tcl_AppendResult( interp,
         "wrong # args: should be \"slaDafin angle\"",
          (char *) NULL);
      return TCL_ERROR;
   }

   slaDafin( argv[1], &nstrt, &angle, &status);

   if ( status != 0 ) {
      Tcl_AppendResult( interp, "\"", argv[1], "\" is not a valid angle",
         (char *) NULL);
      return TCL_ERROR;
   }
   Tcl_PrintDouble( interp, angle, interp->result);
   return TCL_OK;
}

static int Dr2afCmd( ClientData clientdata, Tcl_Interp *interp, int argc,
    char *argv[])
{
/*
 * TCL front end for slaDr2af.
 */
   char sign;
   int dmsf[4];
   long ndp;
   double angle;

   if ( argc < 3 ) {
      Tcl_AppendResult( interp,
         "wrong # args: should be \"slaDr2af precision angle\"",
          (char *) NULL);
      return TCL_ERROR;
   }

   if ( Tcl_ExprLong( interp, argv[1], &ndp) != TCL_OK) return TCL_ERROR;
   if ( Tcl_ExprDouble( interp, argv[2], &angle) != TCL_OK) return TCL_ERROR;
   slaDr2af( (int) ndp, angle, &sign, dmsf);

   sprintf( interp->result, "%c %d %d %d %d", sign, dmsf[0], dmsf[1], dmsf[2],
      dmsf[3]);
   return TCL_OK;
}

static int Dr2tfCmd( ClientData clientdata, Tcl_Interp *interp, int argc,
    char *argv[])
{
/*
 * TCL front end for slaDr2tf.
 */
   char sign;
   int hmsf[4];
   long ndp;
   double angle;

   if ( argc < 3 ) {
      Tcl_AppendResult( interp,
         "wrong # args: should be \"slaDr2tf precision angle\"",
          (char *) NULL);
      return TCL_ERROR;
   }

   if ( Tcl_ExprLong( interp, argv[1], &ndp) != TCL_OK) return TCL_ERROR;
   if ( Tcl_ExprDouble( interp, argv[2], &angle) != TCL_OK) return TCL_ERROR;
   slaDr2tf( (int) ndp, angle, &sign, hmsf);

   sprintf( interp->result, "%c %d %d %d %d", sign, hmsf[0], hmsf[1], hmsf[2],
      hmsf[3]);
   return TCL_OK;
}
