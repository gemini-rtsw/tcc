static char rcsid[]="$Id:";
/* *INDENT-OFF* */
/*
*   FILENAME
*   slaext.c
*
*   FUNCTION NAME(S)
*   Slaext_Init - Initialisation function for TCL loadable image
*
*   D L Terrett 5 June 2002
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
#include <slamac.h>
#include <timeLib.h>

#include "tccDecode.h"

static int DafinCmd( ClientData clientdata, Tcl_Interp *interp, int objc,
    Tcl_Obj *CONST objv[]);
static int Dr2afCmd( ClientData clientdata, Tcl_Interp *interp, int objc,
    Tcl_Obj *CONST objv[]);
static int Dr2tfCmd( ClientData clientdata, Tcl_Interp *interp, int objc,
    Tcl_Obj *CONST objv[]);
static int Dtp2sCmd( ClientData clientdata, Tcl_Interp *interp, int objc,
    Tcl_Obj *CONST objv[]);
static int Pertel( ClientData clientdata, Tcl_Interp *interp, int objc,
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
    Tcl_CreateObjCommand( interp, "slaDtp2s", Dtp2sCmd,
        (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateObjCommand( interp, "slaPertel", Pertel,
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

static int Dtp2sCmd( ClientData clientdata, Tcl_Interp *interp, int objc,
    Tcl_Obj *CONST objv[])
{
/*
 * TCL front end for slaDtp2s.
 */
   double xi, eta, raz, decz, ra, dec;
   Tcl_Obj *result, *reslist[2];

/* Check number of arguments. */
   if ( objc != 5 ) {
      Tcl_WrongNumArgs( interp, 1, objv, "xi eta ra dec");
      return TCL_ERROR;
   }

/* Decode input arguments. */
   if ( Tcl_GetDoubleFromObj( interp, objv[1], &xi) != TCL_OK) 
         return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, objv[2], &eta) != TCL_OK) 
         return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, objv[3], &raz) != TCL_OK) 
         return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, objv[4], &decz) != TCL_OK) 
         return TCL_ERROR;

/* Convert tangent plane coordinates to spherical. */
   slaDtp2s( xi, eta, raz, decz, &ra, &dec );

/* Build result list. */
   reslist[0] = Tcl_NewDoubleObj( ra );
   reslist[1] = Tcl_NewDoubleObj( dec );
   result = Tcl_NewListObj( 2, reslist );
   Tcl_SetObjResult( interp, result);
   return TCL_OK;
}

static int Pertel( ClientData clientdata, Tcl_Interp *interp, int objc,
    Tcl_Obj *CONST objv[])
{
/*
 * TCL front end for slaPertel.
 */
   int jform, jstat;
   double date0, rawt, date1;
   double epoch0, orbi0, anode0, perih0, aorq0, e0, am0;
   double epoch1, orbi1, anode1, perih1, aorq1, e1, am1;
   Tcl_Obj *result, *reslist[7];

/* Check number of arguments. */
   if ( objc < 9 ) {
      Tcl_WrongNumArgs( interp, 1, objv, 
          "jform date0 date1 epoch orbi anode perih aorq e ?am?");
      return TCL_ERROR;
   }

/* Decode input arguments. */
   if ( Tcl_GetIntFromObj( interp, objv[1], &jform) != TCL_OK) 
         return TCL_ERROR;
   if ( tccDcT0( interp, Tcl_GetString( objv[2] ), &date0) != TCL_OK)
         return TCL_ERROR;
   if ( tccDcT0( interp, Tcl_GetString( objv[3] ), &epoch0) != TCL_OK)
         return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, objv[4], &orbi0) != TCL_OK) 
         return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, objv[5], &anode0) != TCL_OK) 
         return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, objv[6], &perih0) != TCL_OK) 
         return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, objv[7], &aorq0) != TCL_OK) 
         return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, objv[8], &e0) != TCL_OK) 
         return TCL_ERROR;
   if ( jform == 2 ) {
      if ( objc < 10 ) {
          Tcl_SetResult( interp, "am missing", TCL_VOLATILE);
          return TCL_ERROR;
      }
      if ( Tcl_GetDoubleFromObj( interp, objv[9], &am0) != TCL_OK) 
          return TCL_ERROR;
   } else {
      am0 = 0.0;
   }

/* Make epochs MJD. */
   if ( date0 > 2400000.5 ) date0 -= 2400000.5;
   if ( epoch0 > 2400000.5 ) epoch0 -= 2400000.5;

/* Get current date. */
    timeNow( &rawt );
    timeThenD( rawt, TT, &date1 );

/* Check that dates are not too far apart to avoid accidental lengthy
   integrations. */
   if ( fabs( date0 - date1 ) > 40000.0 ) {
      Tcl_SetResult( interp, "Dates are too far apart", TCL_VOLATILE);
      return TCL_ERROR;
   }

/* Update elements */
   jstat = 0;
   slaPertel( jform, date0, date1, epoch0, orbi0 * DD2R, anode0 * DD2R, 
         perih0 * DD2R, aorq0, e0, am0 * DD2R, &epoch1, &orbi1, &anode1, 
         &perih1, &aorq1, &e1, &am1, &jstat );

/* Report errors. */
   if ( jstat < 0 ) {
      switch ( jstat ) {
         case -1:
            Tcl_SetResult( interp, "illegal JFORM", TCL_VOLATILE);
            return TCL_ERROR;
         case -2:
            Tcl_SetResult( interp, "illegal E", TCL_VOLATILE);
            return TCL_ERROR;
         case -3:
            Tcl_SetResult( interp, "illegal AORQ", TCL_VOLATILE);
            return TCL_ERROR;
         case -4:
            Tcl_SetResult( interp, "Internal error in slaPertel", 
                  TCL_VOLATILE);
            return TCL_ERROR;
         case -5:
            Tcl_SetResult( interp, "Numerical error in slaPertel", 
                  TCL_VOLATILE);
            return TCL_ERROR;
         default:
            Tcl_SetResult( interp, "Unknown error in slaPertel", TCL_VOLATILE);
            return TCL_ERROR;
      }
   }

/* Build result list. */
   reslist[0] = Tcl_NewDoubleObj( epoch1 );
   reslist[1] = Tcl_NewDoubleObj( orbi1 / DD2R );
   reslist[2] = Tcl_NewDoubleObj( anode1 / DD2R );
   reslist[3] = Tcl_NewDoubleObj( perih1 / DD2R );
   reslist[4] = Tcl_NewDoubleObj( aorq1 );
   reslist[5] = Tcl_NewDoubleObj( e1 );
   if ( jform == 2 ) {
      reslist[6] = Tcl_NewDoubleObj( am1 / DD2R );
   }
   result = Tcl_NewListObj( jform == 2 ? 7 : 6, reslist );
   Tcl_SetObjResult( interp, result);
   return TCL_OK;
}
