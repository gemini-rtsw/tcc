static char rcsid[]="$Id:";
/* *INDENT-OFF* */
/*
*   FILENAME
*   slaext.c
*
*   FUNCTION NAME(S)
*   Slaext_Init - Initialisation function for TCL loadable image
*
*   D L Terrett 5 December 2000
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

static int DafinCmd( ClientData clientdata, Tcl_Interp *interp, int objc,
    Tcl_Obj *CONST objv[]);
static int Dr2afCmd( ClientData clientdata, Tcl_Interp *interp, int objc,
    Tcl_Obj *CONST objv[]);
static int Dr2tfCmd( ClientData clientdata, Tcl_Interp *interp, int objc,
    Tcl_Obj *CONST objv[]);

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

    if ( Tcl_InitStubs( interp, "8.0", 0 ) == NULL ) return TCL_ERROR;

    Tcl_CreateObjCommand( interp, "slaDafin", DafinCmd,
        (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateObjCommand( interp, "slaDr2af", Dr2afCmd,
        (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateObjCommand( interp, "slaDr2tf", Dr2tfCmd,
        (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);

    return TCL_OK;
}

static int DafinCmd( ClientData clientdata, Tcl_Interp *interp, int objc,
    Tcl_Obj *CONST objv[])
{
/*
 * TCL front end for slaDafin.
 */
   int status;
   int nstrt = 1;
   double angle;
   Tcl_Obj *errobj;

/* Check number of arguments. */
   if ( objc != 2 ) {
      Tcl_WrongNumArgs( interp, 1, objv, "angle");
      return TCL_ERROR;
   }

/* Decode angle. */
   slaDafin( Tcl_GetStringFromObj( objv[1], NULL), &nstrt, &angle, &status);

/* Report failure. */
   if ( status != 0 ) {
      errobj = Tcl_NewStringObj(  "\"", -1);
      Tcl_AppendStringsToObj( errobj, Tcl_GetStringFromObj(objv[1], NULL),
         "\" is not a valid angle", (char *) NULL);
      Tcl_SetObjResult( interp, errobj );
      return TCL_ERROR;
   }

/* Return decoded angle. */
   Tcl_SetObjResult( interp, Tcl_NewDoubleObj( angle));
   return TCL_OK;
}

static int Dr2afCmd( ClientData clientdata, Tcl_Interp *interp, int objc,
    Tcl_Obj *CONST objv[])
{
/*
 * TCL front end for slaDr2af.
 */
   char sign;
   int dmsf[4];
   int ndp;
   double angle;
   Tcl_Obj *result, *reslist[5];

/* Check number of arguments. */
   if ( objc != 3 ) {
      Tcl_WrongNumArgs( interp, 1, objv, "precision angle");
      return TCL_ERROR;
   }

/* Decode input arguments. */
   if ( Tcl_GetIntFromObj( interp, objv[1], &ndp) != TCL_OK) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, objv[2], &angle) != TCL_OK) 
         return TCL_ERROR;

/* Decompose angle into dms. */
   slaDr2af( ndp, angle, &sign, dmsf);

/* Build result list. */
   reslist[0] = Tcl_NewStringObj( &sign, 1);
   reslist[1] = Tcl_NewIntObj( dmsf[0] );
   reslist[2] = Tcl_NewIntObj( dmsf[1] );
   reslist[3] = Tcl_NewIntObj( dmsf[2] );
   reslist[4] = Tcl_NewIntObj( dmsf[3] );
   result = Tcl_NewListObj( 5, reslist );
   Tcl_SetObjResult( interp, result);
   return TCL_OK;
}

static int Dr2tfCmd( ClientData clientdata, Tcl_Interp *interp, int objc,
    Tcl_Obj *CONST objv[])
{
/*
 * TCL front end for slaDr2tf.
 */
   char sign;
   int hmsf[4];
   int ndp;
   double angle;
   Tcl_Obj *result, *reslist[5];

/* Check number of arguments. */
   if ( objc != 3 ) {
      Tcl_WrongNumArgs( interp, 1, objv, "precision angle");
      return TCL_ERROR;
   }

/* Decode input arguments. */
   if ( Tcl_GetIntFromObj( interp, objv[1], &ndp) != TCL_OK) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, objv[2], &angle) != TCL_OK) 
         return TCL_ERROR;

/* Decompose angle into hms. */
   slaDr2tf( ndp, angle, &sign, hmsf);

/* Build result list. */
   reslist[0] = Tcl_NewStringObj( &sign, 1);
   reslist[1] = Tcl_NewIntObj( hmsf[0] );
   reslist[2] = Tcl_NewIntObj( hmsf[1] );
   reslist[3] = Tcl_NewIntObj( hmsf[2] );
   reslist[4] = Tcl_NewIntObj( hmsf[3] );
   result = Tcl_NewListObj( 5, reslist );
   Tcl_SetObjResult( interp, result);
   return TCL_OK;
}
