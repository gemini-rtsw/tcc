static char rcsid[]="$Id:";
/* *INDENT-OFF* */
/*
*   FILENAME
*   tccTelSim.c
*
*   FUNCTION NAME(S)
*   tccTelSimSlew - Implements the TelescopeSim object slew method.
*   tccTelSimGetXY - Implements the TelescopeSim object getxy method.
*
*   D L Terrett 21 February 2003
*
*   Copyright CCLRC
*/
/*
 * $Log:
 */
/* *INDENT-ON* */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <tcl.h>
#include <itcl.h>
#include <slalib.h>
#include <timeLib.h>
#include <astLib.h>
#include "tccConstants.h"

/* 0 deg C in deg K. */
#define FREEZING 273.2

/* Declarations of Itcl method functions. */
static int tccTelSimSlew( ClientData cdata, Tcl_Interp *interp, int objc, 
      Tcl_Obj *CONST objv[]);
static int tccTelSimGetXY( ClientData cdata, Tcl_Interp *interp, int objc, 
      Tcl_Obj *CONST objv[]);

/* Declarations of internal procedures. */
static int initParams( Tcl_Interp *interp, double utc, double aoprms[15], 
      struct TELP *telp, double *xpmr, double *ypmr, double *elong, 
      double *lat );
static int getTarget(  Tcl_Interp *interp, double elong, double lat,
      double tt, double aoprms[15], struct TELP *telp, double *a, double *b, 
      FRAMETYPE *system, struct EPOCH *equinox);
static int getPo( Tcl_Interp *interp, double *pox, double *poy);
static int getRotator( Tcl_Interp *interp, double *iaa, double *ipa,
      FRAMETYPE *system, struct EPOCH *equinox);
static int getLimits( Tcl_Interp *interp, double *azlolim, double *azhilim,
      double *ellim, double *rotlolim, double* rothilim, double *zlim);
static void limitTimes( int visible, double ra, double dec, double ha,
      FRAMETYPE rotsys, double iaa, double ipa, double az1, double az2, 
      double rma1, double rmas2, double aoprms[15], double azlolim, 
      double azhilim, double ellim, double rotlolim, double rothilim, 
      double zlim, double *azttl1, double *azttl2, double *elttl, 
      double *rotttl1, double *rotttl, double *zttl );
static int storeOutputs( Tcl_Interp *interp, double utc, int visible, 
      double az1, double az2, double el, double rma1, double rma2, double ha,
      double zd, double am, double moonSep, double azttl1, double azttl2, 
      double elttl, double rotttl1, double rotttl2, double zttl);
static int storeContext( Tcl_Interp *interp, double elong, double lat,
      struct WCS_CTX *ctx);
static int getContext( Tcl_Interp *interp, double *elong, double *lat,
      struct WCS_CTX *ctx);
static int setTarget( Tcl_Interp *interp, double a, double b, 
      FRAMETYPE system, struct EPOCH equinox);

/* Loadable extension initialisation routine. */
int Tcctelsim_Init( Tcl_Interp *interp )
{

/* Register methods with Itcl. */
   if ( Itcl_RegisterObjC( interp, "tccTelSimSlew", 
         (Tcl_ObjCmdProc*)tccTelSimSlew, (ClientData)NULL,
         (Tcl_CmdDeleteProc*)NULL ) == TCL_ERROR ) return TCL_ERROR;
   if ( Itcl_RegisterObjC( interp, "tccTelSimGetXY", 
         (Tcl_ObjCmdProc*)tccTelSimGetXY, (ClientData)NULL,
         (Tcl_CmdDeleteProc*)NULL ) == TCL_ERROR ) return TCL_ERROR;
   return TCL_OK;
}

static int tccTelSimSlew( ClientData cdata, Tcl_Interp *interp, int objc, 
      Tcl_Obj *CONST objv[])
{
   double aoprms[15];
   struct TELP telp;
   double xpmr, ypmr;
   double elong, lat;
   double date, offset, rawt, utc, tt, tai;
   double a, b;
   FRAMETYPE system;
   struct EPOCH equinox, epoch;
   struct PMPXRV pm;
   double iaa, ipa;
   FRAMETYPE rotsys;
   struct EPOCH roteqx;
   int i;
   double m2xy[3][2];
   struct WCS_CTX ctx;
   double ra, dec, ha, zd, am;
   double moonRA, moonDec, moonDiam, moonSep;
   double az1, az2, el, rma1, rma2;
   double azlolim, azhilim, ellim, rotlolim, rothilim, zlim;
   double azttl1, azttl2, elttl, rotttl1, rotttl2, zttl;
   int visible;

/* Get slew time and time offset from argument list. A time of zero or less
   means "now". */
   if ( objc != 3 ) {
      Tcl_WrongNumArgs( interp, 1, objv, "date offset");
      return TCL_ERROR;
   }
   if ( Tcl_GetDoubleFromObj( interp, objv[1], &date ) != TCL_OK )
         return TCL_ERROR;
   if ( date <= 0.0 ) {
      timeNow( &rawt );
   } else {
      rawt = timeTai2raw( date );
   }
   if ( Tcl_GetDoubleFromObj( interp, objv[2], &offset ) != TCL_OK )
         return TCL_ERROR;
   rawt += offset;

/* Get time of slew in various timescales. */
   timeThenD( rawt, UTC, &utc );
   timeThenD( rawt, TT, &tt );
   timeThenD( rawt, TAI, &tai );

/* Fill out the apparent to observed array and telescope structure. */
   if ( initParams( interp, utc, aoprms, &telp, &xpmr, &ypmr, &elong, &lat )
         != TCL_OK ) return TCL_ERROR;

/* Get the target position at epoch of slew. */
   if ( getTarget( interp, tt, elong, lat, aoprms, &telp, &a, &b, 
         &system, &equinox) != TCL_OK ) return TCL_ERROR;

/* Get pointing origin. */
   if ( getPo( interp, &telp.pox, &telp.poy ) != TCL_OK ) return TCL_ERROR;

/* Get rotator orientation. */
   if ( getRotator( interp, &iaa, &ipa, &rotsys, &roteqx ) != TCL_OK ) 
         return TCL_ERROR;

/* Get mechanism limits. */
   if ( getLimits( interp, &azlolim, &azhilim, &ellim, &rotlolim, &rothilim,
      &zlim) != TCL_OK ) return TCL_ERROR;

/* Simulate telescope context. */
   for ( i = 0; i < 3; i++ ) {
      m2xy[i][0] = 0.0;
      m2xy[i][1] = 0.0;
   }
   if ( astSimctx_r( tai, elong, aoprms[0], aoprms[4], xpmr, ypmr, aoprms[5],
         aoprms[6], aoprms[7], aoprms[9], aoprms[8], telp, m2xy, a, b,
         system, equinox, iaa, ipa, rotsys, roteqx, &ctx) != 0 ) {
      Tcl_SetResult( interp, "unreachable position", TCL_VOLATILE);
      return TCL_ERROR;
   }

/* Compute Apparent RA/Dec of telescope. */
   if ( astCoco( a, b, pm, system, equinox, epoch,
         APPT, roteqx, tt, aoprms, ctx.tel, &ra, &dec) != 0 ) {
      Tcl_SetResult( interp, "unreachable position", TCL_VOLATILE);
      return TCL_ERROR;
   }

/* Convert to HA/Dec. */
    ha = slaDrange(aoprms[13] - ra);

/* Compute zenith distance and airmass. */
    zd = slaZd( ha, dec, aoprms[0]);
    am = slaAirmas( zd);

/* Compute distance from Moon. */
    slaRdplan( tt, 3, elong, lat, &moonRA, &moonDec, &moonDiam);
    moonSep = slaDsep( ra, dec, moonRA, moonDec);

/* Compute Azimuths, elevation, and rotator positions. */
    az1 = az2 = slaDranrm(ctx.ab0[0]);
    if ( az1 > PI ) az1 -= PI2;
    el = slaDrange(ctx.ab0[1]);
    rma1 = rma2 = slaDranrm(ctx.tel.rma);
    if ( rma1 > D90 ) rma1 -= PI2;
    if ( rma2 > PI + D90 ) rma2 -= PI2;

/* Compute times to limits and visiblity. */
    visible = el > ellim;
    limitTimes( visible, ra, dec, ha, rotsys, iaa, ipa, az1, az2, rma1, rma2,
       aoprms, azlolim, azhilim, ellim, rotlolim, rothilim, zlim, &azttl1, 
       &azttl2, &elttl, &rotttl1, &rotttl2, &zttl );

/* Store telescope context. */
    if ( storeContext( interp, lat, elong, &ctx ) != TCL_OK ) return TCL_ERROR;

/* Store source position. */
   if ( setTarget( interp, a, b, system, equinox) != TCL_OK ) return TCL_ERROR;

/* Set output variables. */
   if ( storeOutputs( interp, rawt, visible, az1, az2, el, rma1, rma2, ha, zd,
        am, moonSep, azttl1, azttl2, elttl, rotttl1, rotttl2, zttl) != TCL_OK )
        return TCL_ERROR;

   return TCL_OK;
}

static int initParams( Tcl_Interp *interp, double utc, double aoprms[15], 
      struct TELP *telp, double *xpmr, double *ypmr, double *elong, 
      double *lat )
{
   Tcl_Obj *valobj;
   double elongm, latm, hm, tdc, pmb, rh, tlr, delut, wavel;
   int y, m, d, status, i;

/* Copy data from Itcl method variables. */
   if ((valobj = Tcl_GetVar2Ex( interp, "elongm", NULL, 
      TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, &elongm ) != TCL_OK )
      return TCL_ERROR;

   if ((valobj = Tcl_GetVar2Ex( interp, "latm", NULL, 
      TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, &latm ) != TCL_OK )
      return TCL_ERROR;

   if ((valobj = Tcl_GetVar2Ex( interp, "hm", NULL, 
      TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, &hm ) != TCL_OK )
      return TCL_ERROR;

   if ((valobj = Tcl_GetVar2Ex( interp, "xpm", NULL, 
      TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, xpmr ) != TCL_OK )
      return TCL_ERROR;
   *xpmr *= AS2R;

   if ((valobj = Tcl_GetVar2Ex( interp, "ypm", NULL, 
      TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, ypmr ) != TCL_OK )
      return TCL_ERROR;
   *ypmr *= AS2R;

   if ((valobj = Tcl_GetVar2Ex( interp, "temperature", NULL, 
      TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, &tdc ) != TCL_OK )
      return TCL_ERROR;

   if ((valobj = Tcl_GetVar2Ex( interp, "pressure", NULL, 
      TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, &pmb ) != TCL_OK )
      return TCL_ERROR;

   if ((valobj = Tcl_GetVar2Ex( interp, "humidity", NULL, 
      TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, &rh ) != TCL_OK )
      return TCL_ERROR;

   if ((valobj = Tcl_GetVar2Ex( interp, "tlr", NULL, 
      TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, &tlr ) != TCL_OK )
      return TCL_ERROR;

   if ((valobj = Tcl_GetVar2Ex( interp, "focalLength", NULL, 
      TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, &telp->fl ) != TCL_OK )
      return TCL_ERROR;

/* rma is just the starting point for the field orientation calculation
   so can be set to any valid angle. */
   telp->rma = 0.0;

   if ((valobj = Tcl_GetVar2Ex( interp, "an", NULL, 
      TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, &telp->an ) != TCL_OK )
      return TCL_ERROR;
   telp->an *= AS2R;

   if ((valobj = Tcl_GetVar2Ex( interp, "aw", NULL, 
       TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, &telp->aw ) != TCL_OK )
       return TCL_ERROR;
   telp->aw *= AS2R;

   if ((valobj = Tcl_GetVar2Ex( interp, "pnpae", NULL, 
      TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, &telp->pnpae ) != TCL_OK )
      return TCL_ERROR;
   telp->pnpae *= AS2R;

   if ((valobj = Tcl_GetVar2Ex( interp, "ca", NULL, 
       TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, &telp->ca ) != TCL_OK )
       return TCL_ERROR;
   telp->ca *= AS2R;

   if ((valobj = Tcl_GetVar2Ex( interp, "ce", NULL, 
       TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, &telp->ce ) != TCL_OK )
       return TCL_ERROR;
   telp->ce *= AS2R;

   if ((valobj = Tcl_GetVar2Ex( interp, "delut", NULL, 
       TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, &delut ) != TCL_OK )
       return TCL_ERROR;

   if ((valobj = Tcl_GetVar2Ex( interp, "wavelength", NULL, 
       TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, &wavel ) != TCL_OK )
       return TCL_ERROR;

/* Set Apparent to observed parameters. */
   slaAoppa( utc, delut, elongm * D2R, latm * D2R, hm, *xpmr, *ypmr,
       tdc + FREEZING, pmb, rh/100.0, wavel, tlr, aoprms);
   slaPolmo( elongm * D2R, latm * D2R, *xpmr, *ypmr, elong,
       lat, &aoprms[14]);

   return TCL_OK;
}

static int getTarget(  Tcl_Interp *interp, double tt, double elong, double lat, 
   double aoprms[15], struct TELP *telp, double *a, double *b, 
   FRAMETYPE *system, struct EPOCH *equinox)
{
   char *targetTypes[] = { "hmsdegTarget", "degdegTarget", "conicTarget",
         NULL };
   Tcl_Obj *obj, *obj1, *obj2, **objv;
   int ind, nel, len, i;
   double theta1, theta2;
   struct EPOCH epoch; 
   struct PMPXRV pm;
   int jtype, jstat;
   double t0, orbel[6], r;

/* Get the target type. */
   if ((obj = Tcl_GetVar2Ex( interp, "targetType", NULL, 
       TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetIndexFromObj( interp, obj, targetTypes, "target types", 
       0, &ind) != TCL_OK ) return TCL_ERROR;

   switch (ind) {

/* Decode hmsdeg target. */
      case 0:

/* Coordinate system. */
         if ((obj = Tcl_GetVar2Ex( interp, "system", NULL, TCL_NAMESPACE_ONLY
               | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
         if ( tccDcFrame( interp, Tcl_GetStringFromObj( obj, NULL), system)
               != TCL_OK ) return TCL_ERROR;

/* RA & Dec. */
         if ((obj1 = Tcl_GetVar2Ex( interp, "theta1", NULL, TCL_NAMESPACE_ONLY
               | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
         if ((obj2 = Tcl_GetVar2Ex( interp, "theta2", NULL, TCL_NAMESPACE_ONLY
               | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
         if ( tccDcRadec( interp, *system,
               Tcl_GetStringFromObj( obj1, NULL),
               Tcl_GetStringFromObj( obj2, NULL), &theta1, &theta2) != TCL_OK )
            return TCL_ERROR;

/* Equinox */
         if ((obj = Tcl_GetVar2Ex( interp, "equinox", NULL, TCL_NAMESPACE_ONLY
               | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
         if ( tccDcEpoch( interp, Tcl_GetStringFromObj( obj, NULL),
               &equinox->type, &equinox->year ) != TCL_OK )
               return TCL_ERROR;

/* Epoch */
         if ((obj = Tcl_GetVar2Ex( interp, "epoch", NULL, TCL_NAMESPACE_ONLY
               | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
         if ( tccDcEpoch( interp, Tcl_GetStringFromObj( obj, NULL),
               &epoch.type, &epoch.year ) != TCL_OK ) return TCL_ERROR;

/* Proper motions (empty pm1 means no proper motions) */
         if ((obj = Tcl_GetVar2Ex( interp, "pm1", NULL, TCL_NAMESPACE_ONLY
               | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
         if ( strlen( Tcl_GetStringFromObj( obj, NULL) ) ) {
            if ( Tcl_GetDoubleFromObj( interp, obj, &pm.pmRA )
                  != TCL_OK ) return TCL_ERROR;
            if ((obj = Tcl_GetVar2Ex( interp, "pm1", NULL, TCL_NAMESPACE_ONLY
                     | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
            if ( Tcl_GetDoubleFromObj( interp, obj, &pm.pmDec )
                != TCL_OK ) return TCL_ERROR;
            pm.pm = 1;
            pm.pmRA *= AS2R;
            pm.pmDec *= AS2R;
         } else {
            pm.pm = 0;
         }

/* Parallax */
         if ((obj = Tcl_GetVar2Ex( interp, "parallax", NULL, TCL_NAMESPACE_ONLY
              | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
         if ( Tcl_GetDoubleFromObj( interp, obj, &pm.px ) !=
               TCL_OK ) return TCL_ERROR;

/* Radial velocity */
         if ((obj = Tcl_GetVar2Ex( interp, "rv", NULL, TCL_NAMESPACE_ONLY
              | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
         if ( Tcl_GetDoubleFromObj( interp, obj, &pm.rv ) !=
               TCL_OK ) return TCL_ERROR;

/* To current epoch */
         if ( astCoco( theta1, theta2, pm, *system, *equinox, epoch, *system, 
               *equinox, tt, aoprms, *telp, a, b) > 0 ) {
            Tcl_SetResult( interp, "unreachable position", TCL_VOLATILE);
            return TCL_ERROR;
         }
         break;

/* Decode degdeg target. */
      case 1:
         *system = AZEL_TOPO;
         if ((obj1 = Tcl_GetVar2Ex( interp, "theta1", NULL, TCL_NAMESPACE_ONLY
               | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
         if ((obj2 = Tcl_GetVar2Ex( interp, "theta2", NULL, TCL_NAMESPACE_ONLY
               | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
         if ( tccDcRadec( interp, *system,
               Tcl_GetStringFromObj( obj1, NULL),
               Tcl_GetStringFromObj( obj2, NULL), &theta1, &theta2) != TCL_OK )
            return TCL_ERROR;
         break;
      case 2:

/* Decode orbital elements. */
         if ((obj = Tcl_GetVar2Ex( interp, "orbtype", NULL, TCL_NAMESPACE_ONLY
              | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
         if ( Tcl_GetIntFromObj( interp, obj, &jtype ) !=
               TCL_OK ) return TCL_ERROR;

/* Check the value of jtype and the number of elements. */
         switch (jtype) {
            case 1:
               nel = 8;
               break;
            case 2:
               nel = 7;
               break;
            case 3:
               nel = 6;
               break;
            default:
               Tcl_SetResult( interp, "invalid orbital element set", 
                     TCL_VOLATILE );
               return TCL_ERROR;
         }
         if ( Tcl_ListObjGetElements( interp, obj, &len, &objv ) != TCL_OK ) 
               return TCL_ERROR;
         if ( nel != len ) {
            Tcl_SetResult( interp, "wrong number of orbital elements", 
                  TCL_VOLATILE );
            return TCL_ERROR;
         }
         if ( tccDcT0( interp, Tcl_GetStringFromObj( objv[0], NULL) , &t0 ) !=
               TCL_OK) return TCL_ERROR;
         for ( i = 1; i < nel; i++ ) {
            if ( Tcl_GetDoubleFromObj( interp, objv[i], &orbel[i-1] ) != 
                  TCL_OK ) return TCL_ERROR;
         }

/* Convert JD to MJD if necessary. */
         if ( t0 > 2400000.5 ) t0 -= 2400000.5;

/* Predict topocentric (i.e. no refraction) apparent place. */
         slaPlante( tt, elong, lat, jtype, t0, orbel[0] * D2R,
            orbel[1] * D2R, orbel[2] * D2R, orbel[3], orbel[4],
            orbel[5] * D2R, orbel[6] * D2R, &theta1, &theta2, &r, &jstat );

/* to geocentric apparent */
         slaOap( "r", theta1, theta2, tt, 0.0, elong, lat, 0.0, 0.0, 0.0,
               0.0, 0.0, 0.0, 0.0, 0.0, &theta1, &theta2);

/* To J2000. */
         slaAmp( theta1, theta2, tt, 2000.0, a, b );
         *system = FK5;
         equinox->year = 2000.0;
         equinox->type = 'J';
         break;
   }

   return TCL_OK;
}

static int getPo( Tcl_Interp *interp, double *pox, double *poy)
{
   Tcl_Obj *valobj;

/* Get pointing origin from Itcl instance variables. */
   if ((valobj = Tcl_GetVar2Ex( interp, "pox", NULL, 
       TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, pox ) != TCL_OK )
       return TCL_ERROR;
   if ((valobj = Tcl_GetVar2Ex( interp, "poy", NULL, 
       TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, poy ) != TCL_OK )
       return TCL_ERROR;

   return TCL_OK;
}

static int getRotator( Tcl_Interp *interp, double *iaa, double *ipa,
       FRAMETYPE *system, struct EPOCH *equinox)
{
   Tcl_Obj *valobj;

/* Get rotator angles. */
   if ((valobj = Tcl_GetVar2Ex( interp, "iaa", NULL, TCL_NAMESPACE_ONLY
        | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, iaa ) != TCL_OK )
         return TCL_ERROR;
   *iaa *= D2R;
   if ((valobj = Tcl_GetVar2Ex( interp, "ipa", NULL, TCL_NAMESPACE_ONLY
        | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, ipa ) != TCL_OK ) 
         return TCL_ERROR;
   *ipa *= D2R;

/* Get frame and equinox. */
   if ((valobj = Tcl_GetVar2Ex( interp, "rotSystem", NULL, TCL_NAMESPACE_ONLY
         | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( tccDcFrame( interp, Tcl_GetStringFromObj( valobj, NULL), system)
         != TCL_OK ) return TCL_ERROR;

   if ( *system == FK5 || *system == FK4 ) {
      if ((valobj = Tcl_GetVar2Ex( interp, "rotEquinox", NULL, 
            TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
      if ( tccDcEpoch( interp, Tcl_GetStringFromObj( valobj, NULL),
            &equinox->type, &equinox->year ) != TCL_OK ) return TCL_ERROR;
   }
   return TCL_OK;
}

static int getLimits( Tcl_Interp *interp, double *azlolim, double *azhilim,
      double *ellim, double *rotlolim, double* rothilim, double *zlim)
{
   Tcl_Obj *valobj;

   if ((valobj = Tcl_GetVar2Ex( interp, "azlolim", NULL, TCL_NAMESPACE_ONLY
        | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, azlolim ) != TCL_OK )
         return TCL_ERROR;
   *azlolim *= D2R;

   if ((valobj = Tcl_GetVar2Ex( interp, "azhilim", NULL, TCL_NAMESPACE_ONLY
        | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, azhilim ) != TCL_OK )
         return TCL_ERROR;
   *azhilim *= D2R;

   if ((valobj = Tcl_GetVar2Ex( interp, "ellim", NULL, TCL_NAMESPACE_ONLY
        | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, ellim ) != TCL_OK )
         return TCL_ERROR;
   *ellim *= D2R;

   if ((valobj = Tcl_GetVar2Ex( interp, "rotlolim", NULL, TCL_NAMESPACE_ONLY
        | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, rotlolim ) != TCL_OK )
         return TCL_ERROR;
   *rotlolim *= D2R;

   if ((valobj = Tcl_GetVar2Ex( interp, "rothilim", NULL, TCL_NAMESPACE_ONLY
        | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, rothilim ) != TCL_OK )
         return TCL_ERROR;
   *rothilim *= D2R;

   if ((valobj = Tcl_GetVar2Ex( interp, "zlim", NULL, TCL_NAMESPACE_ONLY
        | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, zlim ) != TCL_OK )
         return TCL_ERROR;
   *zlim *= D2R;

   return TCL_OK;
}

static void limitTimes( int visible, double ra, double dec, double ha,
      FRAMETYPE rotsys, double iaa, double ipa, double az1, double az2, 
      double rma1, double rma2, double aoprms[15], double azlolim, 
      double azhilim, double ellim, double rotlolim, double rothilim, 
      double zlim, double *azttl1, double *azttl2, double *elttl, 
      double *rotttl1, double *rotttl2, double *zttl)
{
   double tlat, clat, slat, halim, hatemp;
   double az[2], rma[2], cdec, sdec, cha, sha;
   double azhalo[2], azhahi[2], azttl[2];
   double rothalo[2], rothahi[2], rotttl[2];
   double x, y, a, r;
   double sqsz, cqsz;
   int wrap, i, j;
   int azlocount, azhicount, azhidone[2][2], azlodone[2][2], azvalid[2];
   int rotlocount, rothicount, rothidone[2][2], rotlodone[2][2], rotvalid[2];

/* Latitude and ST from apparent to observed array. */
   tlat = aoprms[0];
   slat = aoprms[1];
   clat = aoprms[2];

/* Functions of declination. */
   cdec = cos( dec );
   sdec = sin( dec );

/* Elevation limit. This is either the time to set or the time to rise 
   depending on whether the source is visible or not. */
   if ( tccLimEl( cdec, sdec, ellim, clat, slat, &halim) == 0 ) {
      *elttl = visible ? slaDranrm( halim - ha) : slaDranrm( - halim - ha);
      *elttl *= ST2MIN;
   } else {
      *elttl = -1.0;
   }

/* Azimuth and rotator wrap limits. */

/* Hour angles at which azimuth limits are reached. */
   azlocount = tccLimAz( cdec, sdec, azlolim, clat, slat, azhalo);
   azhicount = tccLimAz( cdec, sdec, azhilim, clat, slat, azhahi);

/* Hour angles at which rotator limits (adjusted for the desired
   rotator angle) are reached. */
   rotlocount = tccLimPa( cdec, sdec, PI - rotlolim + ipa - iaa,
         clat, slat, rothalo);
   rothicount = tccLimPa( cdec, sdec, PI - rothilim + ipa - iaa, 
         clat, slat, rothahi);

/* Adjust limit hour angles to be greater than current. */
   for ( i = 0; i < azlocount; i++ ) {
      if ( azhalo[i] < ha ) azhalo[i] += PI2;
   }
   for ( i = 0; i < azhicount; i++ ) {
      if ( azhahi[i] < ha ) azhahi[i] += PI2;
   }
   for ( i = 0; i < rotlocount; i++ ) {
      if ( rothalo[i] < ha ) rothalo[i] += PI2;
   }
   for ( i = 0; i < rothicount; i++ ) {
      if ( rothahi[i] < ha ) rothahi[i] += PI2;
   }

/* Step the azimuth forward in 6 hour increments up to the hour
   angle of the limit. */
   az[0] = az1;
   az[1] = az2;
   rma[0] = rma1;
   rma[1] = rma2;
   a = slaDrange( az1 );
   r = slaDrange( rma1 );
   hatemp = floor(ha/D90) * D90;
   for ( wrap = 0; wrap < 2; wrap++ ) {
      for ( i = 0; i < 2; i++ ) {
         azlodone[wrap][i] = azhidone[wrap][i] = 0;
         rotlodone[wrap][i] = rothidone[wrap][i] = 0;
      }
   }
   azvalid[0] = azvalid[1] = 0;
   rotvalid[0] = rotvalid[1] = 0;
   for ( i = 0; i < 6; i++)  {
      hatemp += D90;
 
   /* Azimuth at this hour angle. */
      cha = cos( hatemp );
      sha = sin( hatemp );
      x = - cha * cdec * slat + sdec * clat;
      y = - sha * cdec;
      a = ( x == 0.0 && y == 0.0) ? a : atan2( y, x);
      for ( wrap = 0; wrap < 2; wrap++ ) {
         az[wrap] += slaDrange( a - az[wrap]);

      /* Is the hour angle greater than any of the limits? */
         for ( j = 0; j < azlocount; j++ ) {
            if ( !azlodone[wrap][j] && azhalo[j] <= hatemp ) {
               azlodone[wrap][j] = 1;

            /* Is the azimuth close to the limit? */
               if ( fabs( az[wrap] - azlolim ) <= PI ) {
                  if ( !azvalid[wrap] || (azhalo[j] - ha) < azttl[wrap] ) {
                     azttl[wrap] = azhalo[j] - ha;
                     azvalid[wrap] = 1;
                  }
               }
            }
         }

         /* Repeat for the high limit. */
         for ( j = 0; j < azhicount; j++ ) {
            if ( !azhidone[wrap][j] && azhahi[j] <= hatemp ) {
               azhidone[wrap][j] = 1;

         /* Is the azimuth close to the limit? */
               if ( fabs( az[wrap] - azhilim ) <= PI ) {
                  if ( !azvalid[wrap] || (azhahi[j] - ha) < azttl[wrap] ) {
                     azttl[wrap] = azhahi[j] - ha;
                     azvalid[wrap] = 1;
                  }
                }
             }
          }
       }

    /* Rotator angle at this hour angle. */
       sqsz = clat * sha;
       cqsz = slat * cdec - clat * sdec * cha;
       r = ( sqsz != 0.0 || cqsz != 0.0 ) ? atan2 ( -sqsz, cqsz ) : r;
       r += PI + (ipa - iaa);
       for ( wrap = 0; wrap < 2; wrap++ ) {
          rma[wrap] += slaDrange( r - rma[wrap]);

          for ( j = 0; j < rotlocount; j++ ) {
             if ( !rotlodone[wrap][j] && rothalo[j] <= hatemp ) {
                rotlodone[wrap][j] = 1;

          /* Is the rma close to the limit? */
                if ( fabs( rma[wrap] - rotlolim ) <= PI ) {
                   if ( !rotvalid[wrap] || (rothalo[j] - ha) < 
                         rotttl[wrap] ) {
                      rotttl[wrap] = rothalo[j] - ha;
                      rotvalid[wrap] = 1;
                   }
                }
             }
          }

        /* Repeat for the high limit. */
          for ( j = 0; j < rothicount; j++ ) {
            if ( !rothidone[wrap][j] && rothahi[j] <= hatemp ) {
               rothidone[wrap][j] = 1;

           /* Is the azimuth close to the limit? */
               if ( fabs( rma[wrap] - rothilim ) <= PI ) {
                  if ( !rotvalid[wrap] || (rothahi[j] - ha) < 
                          rotttl[wrap] ) {
                      rotttl[wrap] = rothahi[j] - ha;
                      rotvalid[wrap] = 1;
                  }
               }
            }
         }
      }
   }

/* Copy azimuth limit to outputs. */
   if (azvalid[0]) {
      *azttl1 = azttl[0] * ST2MIN;
   } else {
      *azttl1 = -1.0;
   }
   if (azvalid[1]) {
      *azttl2 = azttl[1] * ST2MIN;
   } else {
      *azttl2 = -1.0;
   }

/* Copy rotator limit to outputs. */
   if (rotsys != AZEL_MNT) {
      if (rotvalid[0]) {
         *rotttl1 = rotttl[0] * ST2MIN;
      } else {
         *rotttl1 = -1.0;
      }
      if (rotvalid[1]) {
         *rotttl2 = rotttl[1] * ST2MIN;
      } else {
         *rotttl2 = -1.0;
      }
   }

/* Zenith at risk area limit */
   if ( zlim < fabs( dec - tlat )  || ha > zlim ) {
      *zttl = -1.0;
   } else {
      *zttl = ( - zlim - ha ) * ST2MIN;
   }
}

static int storeContext( Tcl_Interp *interp, double elong, double lat,
      struct WCS_CTX *ctx)
{
   int i, j;
   Tcl_Obj *valobj, *obj[15];

/* Store context in Itcl instance variables. */

/* True Longitude and latitude. */
   if ( Tcl_SetVar2Ex( interp, "elong", NULL, Tcl_NewDoubleObj( elong ),
         TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG) == NULL) return TCL_ERROR;
   if ( Tcl_SetVar2Ex( interp, "lat", NULL, Tcl_NewDoubleObj( lat ),
         TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG) == NULL) return TCL_ERROR;

/* Pre-flexure Az/El. */
   for ( i = 0; i < 2; i++ ) {
      obj[i] = Tcl_NewDoubleObj( ctx->ab0[i] );
   }
   if ((valobj = Tcl_GetVar2Ex( interp, "ab0", NULL, 
       TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_ListObjReplace( interp, valobj, 0, 2, 2, obj) 
         != TCL_OK) return TCL_ERROR;

/* Telescope parameters. */
   obj[0] = Tcl_NewDoubleObj( ctx->tel.fl );
   obj[1] = Tcl_NewDoubleObj( ctx->tel.rma );
   obj[2] = Tcl_NewDoubleObj( ctx->tel.an );
   obj[3] = Tcl_NewDoubleObj( ctx->tel.aw );
   obj[4] = Tcl_NewDoubleObj( ctx->tel.pnpae );
   obj[5] = Tcl_NewDoubleObj( ctx->tel.ca );
   obj[6] = Tcl_NewDoubleObj( ctx->tel.ce );
   obj[7] = Tcl_NewDoubleObj( ctx->tel.pox );
   obj[8] = Tcl_NewDoubleObj( ctx->tel.poy );
   if ((valobj = Tcl_GetVar2Ex( interp, "telp", NULL, 
       TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_ListObjReplace( interp, valobj, 0, 8, 8, obj) 
         != TCL_OK) return TCL_ERROR;

/* Apparent to observed parameters. */
   for ( i = 0; i < 15; i++ ) {
      obj[i] = Tcl_NewDoubleObj( ctx->aoprms[i] );
   }
   if ((valobj = Tcl_GetVar2Ex( interp, "aoprms", NULL, 
       TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_ListObjReplace( interp, valobj, 0, 15, 15, obj) 
         != TCL_OK) return TCL_ERROR;

/* M2 tip/tilt. */
   for ( i = 0; i < 3; i++ ) {
      for ( j = 0; j < 2; j++ ) {
         obj[i * 2 + j] = Tcl_NewDoubleObj( ctx->m2xy[i][j] );
      }
   }
   if ((valobj = Tcl_GetVar2Ex( interp, "m2xy", NULL, 
       TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_ListObjReplace( interp, valobj, 0, 6, 6, obj) 
         != TCL_OK) return TCL_ERROR;

/* Gemini raw time. */
   if ( Tcl_SetVar2Ex( interp, "grt", NULL, Tcl_NewDoubleObj( ctx->time ),
         TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG) == NULL) return TCL_ERROR;

   return TCL_OK;
}

static int storeOutputs( Tcl_Interp *interp, double rawt, int visible, 
      double az1, double az2, double el, double rma1, double rma2, 
      double ha, double zd, double am, double moonSep, double azttl1, 
      double azttl2, double elttl, double rotttl1, double rotttl2, double zttl)
{
   int hmsf[4], dmsf[4];
   char result[9], sign;

   timeThenT( rawt, UTC, 0, hmsf);
   sprintf( result, "%02d:%02d:%02d", hmsf[0], hmsf[1], hmsf[2]);
   if ( Tcl_SetVar2Ex( interp, "utc", NULL, Tcl_NewStringObj( result, -1 ),
         TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG) == NULL) return TCL_ERROR;

   if ( Tcl_SetVar2Ex( interp, "visible", NULL, Tcl_NewIntObj( visible ),
         TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG) == NULL) return TCL_ERROR;

   slaDr2af( 0, az1, &sign, dmsf);
   sprintf( result, "%c%02d:%02d:%02d", sign, dmsf[0], dmsf[1], dmsf[2]);
   if ( Tcl_SetVar2Ex( interp, "az1", NULL, Tcl_NewStringObj( result, -1 ),
         TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG) == NULL) return TCL_ERROR;

   slaDr2af( 0, az2, &sign, dmsf);
   sprintf( result, "%c%02d:%02d:%02d", sign, dmsf[0], dmsf[1], dmsf[2]);
   if ( Tcl_SetVar2Ex( interp, "az2", NULL, Tcl_NewStringObj( result, -1 ),
         TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG) == NULL) return TCL_ERROR;

   slaDr2af( 0, el, &sign, dmsf);
   sprintf( result, "%c%02d:%02d:%02d", sign, dmsf[0], dmsf[1], dmsf[2]);
   if ( Tcl_SetVar2Ex( interp, "el", NULL, Tcl_NewStringObj( result, -1 ),
         TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG) == NULL) return TCL_ERROR;

   if ( Tcl_SetVar2Ex( interp, "rma1", NULL, Tcl_NewDoubleObj( rma1/D2R ),
         TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG) == NULL) return TCL_ERROR;
   if ( Tcl_SetVar2Ex( interp, "rma2", NULL, Tcl_NewDoubleObj( rma2/D2R ),
         TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG) == NULL) return TCL_ERROR;

   slaDr2tf( 0, ha, &sign, hmsf);
   sprintf( result, "%c%02d:%02d:%02d", sign, hmsf[0], hmsf[1], hmsf[2]);
   if ( Tcl_SetVar2Ex( interp, "ha", NULL, Tcl_NewStringObj( result, -1 ),
         TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG) == NULL) return TCL_ERROR;

   if ( Tcl_SetVar2Ex( interp, "zd", NULL, Tcl_NewDoubleObj( zd/D2R ),
         TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG) == NULL) return TCL_ERROR;
   if ( Tcl_SetVar2Ex( interp, "am", NULL, Tcl_NewDoubleObj( am ),
         TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG) == NULL) return TCL_ERROR;
   if ( Tcl_SetVar2Ex( interp, "moonSep", NULL, Tcl_NewDoubleObj( moonSep/D2R ),
         TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG) == NULL) return TCL_ERROR;
   if ( azttl1 >= 0.0 ) {
      if ( Tcl_SetVar2Ex( interp, "azttl1", NULL, Tcl_NewDoubleObj( azttl1 ),
            TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG) == NULL) return TCL_ERROR;
   } else {
      if ( Tcl_SetVar2Ex( interp, "azttl1", NULL, Tcl_NewStringObj( "", 0 ),
            TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG) == NULL) return TCL_ERROR;
   }
   if ( azttl2 >= 0.0 ) {
      if ( Tcl_SetVar2Ex( interp, "azttl2", NULL, Tcl_NewDoubleObj( azttl2 ),
            TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG) == NULL) return TCL_ERROR;
   } else {
      if ( Tcl_SetVar2Ex( interp, "azttl2", NULL, Tcl_NewStringObj( "", 0 ),
            TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG) == NULL) return TCL_ERROR;
   }
   if ( elttl >= 0.0 ) {
      if ( Tcl_SetVar2Ex( interp, "elttl", NULL, Tcl_NewDoubleObj( elttl ),
            TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG) == NULL) return TCL_ERROR;
   } else {
      if ( Tcl_SetVar2Ex( interp, "elttl", NULL, Tcl_NewStringObj( "", 0 ),
            TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG) == NULL) return TCL_ERROR;
   }
   if ( rotttl1 >= 0.0 ) {
      if ( Tcl_SetVar2Ex( interp, "rotttl1", NULL, Tcl_NewDoubleObj( rotttl1 ),
            TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG) == NULL) return TCL_ERROR;
   } else {
      if ( Tcl_SetVar2Ex( interp, "rotttl1", NULL, Tcl_NewStringObj( "", 0 ),
            TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG) == NULL) return TCL_ERROR;
   }
   if ( rotttl2 >= 0.0 ) {
      if ( Tcl_SetVar2Ex( interp, "rotttl2", NULL, Tcl_NewDoubleObj( rotttl2 ),
            TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG) == NULL) return TCL_ERROR;
   } else {
      if ( Tcl_SetVar2Ex( interp, "rotttl2", NULL, Tcl_NewStringObj( "", 0 ),
            TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG) == NULL) return TCL_ERROR;
   }

   return TCL_OK;
}

static int tccTelSimGetXY( ClientData cdata, Tcl_Interp *interp, 
   int objc, Tcl_Obj *CONST objv[])
{
   Tcl_Obj *valobj, *listobj;
   double elong, lat, tt;
   struct WCS_CTX ctx;
   double a, b, wavel, x, y;
   FRAMETYPE system;
   struct EPOCH equinox;
   struct WCS wcs, iwcs;

   if ( objc != 1 ) {
      Tcl_WrongNumArgs( interp, 1, objv, "");
      return TCL_ERROR;
   }

/* Get context. */
   if ( getContext( interp, &elong, &lat, &ctx ) != TCL_OK ) return TCL_ERROR;

/* Time as TT. */
   timeThenD( ctx.time, TT, &tt );

/* Get target position. */
   if ( getTarget( interp, tt, elong, lat, ctx.aoprms, &ctx.tel, &a, &b, 
         &system, &equinox) != TCL_OK ) return TCL_ERROR;

/* Get wavelength. */
   if ((valobj = Tcl_GetVar2Ex( interp, "wavelength", NULL, 
       TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, &wavel ) != TCL_OK )
       return TCL_ERROR;

/* Generate WCS transformation. */
   if ( astCtx2tr( ctx, system, equinox, wavel, 0, &wcs, &ctx.time ) ) {
      Tcl_SetResult( interp,
           "WCS transformation generation failed", TCL_VOLATILE);
      return TCL_ERROR;
   }
   if ( astInvtr( wcs, &iwcs ) ) {
       Tcl_SetResult( interp, "inverting WCS transformation failed",
           TCL_VOLATILE);
       return TCL_ERROR;
   }
   if ( astS2xyq( a, b, iwcs, &x, &y ) ) {
       Tcl_SetResult( interp, "uncomputable WFS x/y", TCL_VOLATILE);
       return TCL_ERROR;
   }

/* Correct position for optical distortions. */
   tccXy2Probe( &x, &y);

/* Construct result list. */
   listobj = Tcl_NewListObj( 0, NULL);
   Tcl_ListObjAppendList( interp, listobj, Tcl_NewDoubleObj( x ));
   Tcl_ListObjAppendList( interp, listobj, Tcl_NewDoubleObj( y ));
   Tcl_SetObjResult( interp, listobj );

   return TCL_OK;
}

static int getContext( Tcl_Interp *interp, double *elong, double *lat,
      struct WCS_CTX *ctx)
{
   Tcl_Obj *valobj, *listobj, **objv;
   int objc, i, j;

/* Longitude and latitude */
   if ((valobj = Tcl_GetVar2Ex( interp, "elong", NULL, 
       TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, elong ) != TCL_OK )
       return TCL_ERROR;
   if ((valobj = Tcl_GetVar2Ex( interp, "lat", NULL, 
       TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, lat ) != TCL_OK )
       return TCL_ERROR;

/* Pre-flexure Az/El. */
   if ((listobj = Tcl_GetVar2Ex( interp, "ab0", NULL, 
       TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_ListObjGetElements( interp, listobj, &objc, &objv ) != TCL_OK )
         return TCL_ERROR;
   if ( objc != 2 ) {
      Tcl_SetResult( interp, "ab0 has wrong number of elements", TCL_VOLATILE);
      return TCL_ERROR;
   }
   for ( i = 0; i < 2; i++ ) {
      if ( Tcl_GetDoubleFromObj( interp, *objv++, &ctx->ab0[i] ) != TCL_OK )
          return TCL_ERROR;
   }

/* Telescope parameters. */
   if ((listobj = Tcl_GetVar2Ex( interp, "telp", NULL, 
       TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_ListObjGetElements( interp, listobj, &objc, &objv ) != TCL_OK )
         return TCL_ERROR;
   if ( objc != 9 ) {
      Tcl_SetResult( interp, "telp has wrong number of elements", TCL_VOLATILE);
      return TCL_ERROR;
   }
   if ( Tcl_GetDoubleFromObj( interp, *objv++, &ctx->tel.fl ) != TCL_OK )
        return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, *objv++, &ctx->tel.rma ) != TCL_OK )
        return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, *objv++, &ctx->tel.an ) != TCL_OK )
        return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, *objv++, &ctx->tel.aw ) != TCL_OK )
        return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, *objv++, &ctx->tel.pnpae ) != TCL_OK )
        return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, *objv++, &ctx->tel.ca ) != TCL_OK )
        return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, *objv++, &ctx->tel.ce ) != TCL_OK )
        return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, *objv++, &ctx->tel.pox ) != TCL_OK )
        return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, *objv++, &ctx->tel.poy ) != TCL_OK )
        return TCL_ERROR;

/* Apparent to observed parameters */
   if ((listobj = Tcl_GetVar2Ex( interp, "aoprms", NULL, 
       TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_ListObjGetElements( interp, listobj, &objc, &objv ) != TCL_OK )
         return TCL_ERROR;
   if ( objc != 15 ) {
      Tcl_SetResult( interp, "aoprms has wrong number of elements",
            TCL_VOLATILE); return TCL_ERROR;
   }
   for ( i = 0; i < 15; i++ ) {
      if ( Tcl_GetDoubleFromObj( interp, *objv++, &ctx->aoprms[i] ) != TCL_OK )
          return TCL_ERROR;
   }

/* M2 tip/tilt. */
   if ((listobj = Tcl_GetVar2Ex( interp, "m2xy", NULL, 
       TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_ListObjGetElements( interp, listobj, &objc, &objv ) != TCL_OK )
         return TCL_ERROR;
   if ( objc != 6 ) {
      Tcl_SetResult( interp, "m2xy has wrong number of elements", TCL_VOLATILE);
      return TCL_ERROR;
   }
   for ( i = 0; i < 3; i++ ) {
      for ( j = 0; j < 2; j++ ) {
         if ( Tcl_GetDoubleFromObj( interp, *objv++, &ctx->m2xy[i][j] ) 
               != TCL_OK )
            return TCL_ERROR;
      }
   }

/* Gemini raw time. */
   if ((valobj = Tcl_GetVar2Ex( interp, "grt", NULL, 
       TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG)) == NULL) return TCL_ERROR;
   if ( Tcl_GetDoubleFromObj( interp, valobj, &ctx->time ) != TCL_OK )
       return TCL_ERROR;

   return TCL_OK;
}

static int setTarget( Tcl_Interp *interp, double a, double b, 
      FRAMETYPE system, struct EPOCH equinox)
{
   int hmsf[4], dmsf[4];
   char sign1, sign2;
   char result[60];

   slaDr2tf( 3, a, &sign1, hmsf);
   slaDr2af( 2, b, &sign2, dmsf);
   switch ( system ) {
      case FK4:
         sprintf( result,
               "%02d:%02d:%02d.%03d %c%02d:%02d:%02d.%02d FK4 %c%.2f",
               hmsf[0], hmsf[1], hmsf[2], hmsf[3], sign2, dmsf[0], dmsf[1],
               dmsf[2], dmsf[3], equinox.type, equinox.year);
         break;
      case FK5:
         sprintf( result,
               "%02d:%02d:%02d.%03d %c%02d:%02d:%02d.%02d FK5 %c%.2f",
               hmsf[0], hmsf[1], hmsf[2], hmsf[3], sign2, dmsf[0], dmsf[1],
               dmsf[2], dmsf[3], equinox.type, equinox.year);
         break;
      case APPT:
         sprintf( result,
               "%02d:%02d:%02d.%03d %c%02d:%02d:%02d.%02d Apparent",
               hmsf[0], hmsf[1], hmsf[2], hmsf[3], sign2, dmsf[0], dmsf[1],
               dmsf[2], dmsf[3]);
         break;
      case AZEL_TOPO:
         sprintf( result,
               "%.3f %.3f Topocentric Az/El", a/D2R, b/D2R);
         break;
   }
   
   if ( Tcl_SetVar2Ex( interp, "target", NULL, Tcl_NewStringObj( result, -1 ),
         TCL_NAMESPACE_ONLY | TCL_LEAVE_ERR_MSG) == NULL) return TCL_ERROR;
   return TCL_OK;
}
