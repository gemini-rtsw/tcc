static char rcsid[]="$Id:";
/* *INDENT-OFF* */
/*
*   FILENAME
*   tccPreview.c
*
*   FUNCTION NAME(S)
*   tccPreviewCmd - Implements the tccPreview tcl command.
*
*   D L Terrett 22 February 2001
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
#include <slalib.h>
#include <timeLib.h>
#include <astLib.h>
#include "tccPreview.h"
#include "tccConstants.h"
#include "tccUtil.h"
#include "tccDecode.h"

static int begin( Tcl_Interp* );
static int end( Tcl_Interp*, Tcl_Obj* );
static int getFpxy( Tcl_Interp*, int );
static int getMount( Tcl_Interp*);
static int getLimits( Tcl_Interp*);
static int getSource( Tcl_Interp*);
static int getTarget( Tcl_Interp*, int);
static int getRotator( Tcl_Interp*);
static int setPo( Tcl_Interp*, int, int, Tcl_Obj *CONST[]); 
static int setTarget( Tcl_Interp*, int, int, Tcl_Obj *CONST[]);
static int setWavel( Tcl_Interp*, int, int, Tcl_Obj *CONST[]); 
static int setLimits( Tcl_Interp*, int, Tcl_Obj *CONST[]); 
static int setIpa( Tcl_Interp*, int, Tcl_Obj *CONST[]); 
static int setIaa( Tcl_Interp*, int, Tcl_Obj *CONST[]); 
static int setPlanet( Tcl_Interp*, int, Tcl_Obj *CONST[]); 
static int setOrbit( Tcl_Interp*, int, Tcl_Obj *CONST[]); 
static int update( Tcl_Interp*, char*);
static int limitTimes( double, double );

/* Target types */
#define NONE 0
#define COORD 1
#define PLANET 2
#define ORBIT 3

/* Static storage for configuration data */
double Aoprms[15];   /* Apparent to observed parameters */
struct TELP Telp;    /* Telescope structure */
double Elong;        /* True geodetic longitude */
double Elat;         /* True geodetic latitude */
double Xpmr, Ypmr;   /* Polar motion */
int TargetSet[4];    /* Targets for source A, PWFS1, PWFS2 & OIWFS */
double Theta1[4];
double Theta2[4];
FRAMETYPE System[4];
struct EPOCH Equinox[4];
struct EPOCH Epoch[4];
struct PMPXRV Pm[4];
double Wavel[4];
FRAMETYPE Rotsys;    /* Rotator frame */
struct EPOCH Roteqx; /* Rotator equinox */
double Iaa;          /* Instrument alignment angle */
double Ipa;          /* Sky position angle */
double Pox[2];       /* Pointing origins */
double Poy[2];
int Planet;          /* Planet */
int Jtype;           /* Orbital element type */
double T0;           /* reference epoch */
double Orbel[7];     /* Orbital elements */

/* Static storage for results */
int Visible;                     /* Source visible flag */
double Az1, Az2, El, Zd, HA;
double Rma1, Rma2;
double HA, ZD, Airmass;
double Wfsx[3];             /* Wave front sensor position */
double Wfsy[4];
double Azttl1, Azttl2, Elttl;
double Rotttl1, Rotttl2, Zttl;

/* Limits for times to limits calculations. */
double Ellim = 15.0 * D2R;
double Azhilim = 360.0 * D2R;
double Azlolim = -180.0 * D2R;
double Rothilim = 270.0 * D2R;
double Rotlolim = -270.0 * D2R;
double Zlim = 0.5 * D2R;

/* Reference wavelenth (microns) */
#define WAVELR 0.55

#define FREEZING 273.2

/* *INDENT-OFF* */
/*+
 *   Function name:
 *   Tccext_PreviewCmd
 *
 *   Purpose:
 *   Implements the tccPreview tcl command
 *
 *   Description:
 *
 *   The tccPreview command supports the following options:
 *
 *      begin
 *
 *         Starts a new configuration.
 *
 *      end
 *
 *         Returns the time (UTC), and visibility of source.
 *
 *      get limits
 *
 *         Returns the times in minutes to the following limits; elevation,
 *         azimuth wraps, rotator wraps and zenith avoidance region.
 *
 *      get mount
 *
 *         Returns the two azimuths (one per wrap state) and elevation of 
 *         the telescope.
 *
 *      get rotator
 *
 *         Returns the two rotator positions (one per rotator wrap state).
 *
 *      get pwfs1|pwfs2|oiwfs
 *
 *         Returns the X/Y of the guide probe (or an empty string if no
 *         target has been set).
 *
 *      get source
 *
 *         Returns the hour angle zenith distance and airmass of the source.
 *
 *      get source_target
 *      get pwfs1_target
 *      get pwfs2_target
 *      get oiwfs_target
 *
 *         Returns the RA, Dec and Equinox of the source
 *
 *      set sourceA|pwfs1|pwfs2|pwfs3 <system> <theta1> <theta2> ?<equinox> 
 *         <epoch> <proper motion> dec> <parallax> <radial vel>?
 *
 *         Sets the a target position.
 *
 *      set orbit <jtype> <epoch> <orbinc> <anode> <perih> <aorq> 
 *                <e> ?<aorl> <dm>?
 *
 *         Sets the Source A target position to the named planet.
 *
 *      set planet <planet>
 *
 *         Sets the Source A target position to the named planet.
 *
 *      set poriginA|poriginB <x> <y>
 *
 *         Sets a pointing origin.
 *
 *      set wavelsourceA|wavelpwfs1|wavelpwfs2|waveloiwfs <wavelength>
 *
 *         Sets the effective wavelength of a virtual telescope.
 *
 *      set limits <az low limit> <az high limit> <elevation limit>
 *         <rotator low limit> <rotator high limit>
 *
 *         Sets the mount and rotator limits (in degrees).
 *
 *      set ipa <ipa> <frame> <equinox>
 *
 *         Sets the instrument position angle.
 *
 *      set iia <iia>
 *
 *         Sets the instrument alignment angle.
 *
 *      update <array name>
 *
 *         Updates the "quasi-fixed" parameters that are required by astlib
 *         from the elements of the named tcl array. The array must have the
 *         following elements defined:
 *
 *            tlongm - Mean longitude (degrees)
 *            tlatm - Mean latitude (degrees)
 *            hm - Height above sea-level (metres)
 *            xpm - X polar motion (arcsec)
 *            ypm - Y polar motion (arcsec)
 *            tdc - Temperature (deg C)
 *            pmb - Pressure (millibars)
 *            rh - Relative humidity
 *            tlr - Tropospheric lapse rate
 *            an - Azimuth tilt North (arcsec)
 *            aw - Azimuth tilt West (arcsec)
 *            pnpae - Non-perpedicularity (arcsec)
 *            ca - Collimation error in azimuth (arcsec)
 *            ce - Collimation error in elevation (arcsec)
 *            fl - Focal length (mm)
 *            delut - UT1-UTC (SI seconds)
 *-
 */
/* *INDENT-ON* */

int Tccext_PreviewCmd( ClientData clientdata, Tcl_Interp *interp, int objc,
    Tcl_Obj *CONST objv[])
{
    char *options[] = { "begin", "end", "get", "set", "update", NULL};
    char *getopts[] = { "mount", "source", "source_target", "pwfs1_target",
            "pwfs2_target", "oiwfs_target", "rotator", "pwfs1", "pwfs2",
            "oiwfs", "limits", NULL};
    char *setopts[] = { "sourceA", "pwfs1", "pwfs2", "oiwfs", "poriginA",
            "poriginB", "wavelsourceA", "wavelpwfs1", "wavelpwfs2",
            "waveloiwfs", "limits", "ipa", "iaa", "planet", "orbit", NULL};
    int ind;
    int result;

    if ( objc < 2 ) {
        Tcl_WrongNumArgs( interp, 1, objv, "option ?arg ...?");
        return TCL_ERROR;
    }
    if ( Tcl_GetIndexFromObj( interp, objv[1], options, "option", 0, &ind)
        != TCL_OK ) return TCL_ERROR;

    switch (ind) {
        case 0:
            if ( objc != 2 ) {
                Tcl_WrongNumArgs( interp, 2, objv, "");
                return TCL_ERROR;
            }
            result = begin( interp );
            break;
        case 1:
            if ( objc != 3 ) {
                Tcl_WrongNumArgs( interp, 2, objv, "time");
                return TCL_ERROR;
            }
            result = end( interp, objv[2] );
            break;
        case 2:
            if ( objc != 3 ) {
                Tcl_WrongNumArgs( interp, 2, objv, "item");
                return TCL_ERROR;
            }
            if ( Tcl_GetIndexFromObj( interp, objv[2], getopts, "item", 0, &ind)
                != TCL_OK ) return TCL_ERROR;
            switch (ind) {
                case 0:
                    result = getMount( interp );
                    break;
                case 1:
                    result = getSource( interp );
                    break;
                case 2:
                    result = getTarget( interp, 0 );
                    break;
                case 3:
                    result = getTarget( interp, 1 );
                    break;
                case 4:
                    result = getTarget( interp, 2 );
                    break;
                case 5:
                    result = getTarget( interp, 3 );
                    break;
                case 6:
                    result = getRotator( interp );
                    break;
                case 7:
                    result = getFpxy( interp, 1 );
                    break;
                case 8:
                    result = getFpxy( interp, 2 );
                    break;
                case 9:
                    result = getFpxy( interp, 3 );
                    break;
                case 10:
                    result = getLimits( interp );
                    break;
            }
            break;
        case 3:
            if ( objc < 3 ) {
                Tcl_WrongNumArgs( interp, 2, objv, "item");
                return TCL_ERROR;
            }
            if ( Tcl_GetIndexFromObj( interp, objv[2], setopts, "item", 0, &ind)
                != TCL_OK ) return TCL_ERROR;
            switch (ind) {
                case 0:
                    result = setTarget( interp, 0, objc, objv );
                    break;
                case 1:
                    result = setTarget( interp, 1, objc, objv );
                    break;
                case 2:
                    result = setTarget( interp, 2, objc, objv );
                    break;
                case 3:
                    result = setTarget( interp, 3, objc, objv );
                    break;
                case 4:
                    result = setPo( interp, 0, objc, objv );
                    break;
                case 5:
                    result = setPo( interp, 1, objc, objv );
                    break;
                case 6:
                    result = setWavel( interp, 0, objc, objv);
                    break;
                case 7:
                    result = setWavel( interp, 1, objc, objv);
                    break;
                case 8:
                    result = setWavel( interp, 2, objc, objv);
                    break;
                case 9:
                    result = setWavel( interp, 3, objc, objv);
                    break;
                case 10:
                    result = setLimits( interp, objc, objv);
                    break;
                case 11:
                    result = setIpa( interp, objc, objv);
                    break;
                case 12:
                    result = setIaa( interp, objc, objv);
                    break;
                case 13:
                    result = setPlanet( interp, objc, objv);
                    break;
                case 14:
                    result = setOrbit( interp, objc, objv);
                    break;
            }
            break;
        case 4:
            if ( objc != 3 ) {
                Tcl_WrongNumArgs( interp, 2, objv, "arrayname");
                return TCL_ERROR;
            }
            result = update( interp, Tcl_GetStringFromObj(objv[2], NULL) );
            break;
    }

    return result;
}

static int begin( Tcl_Interp *interp )
{
    int i;

    for ( i = 0; i < 4; i++ ) {
        TargetSet[i] = NONE;
        Pm[i].pm = 0;
        Pm[i].px = 0;
        Pm[i].rv = 0;
        Wavel[i] = WAVELR;
    }
    for ( i = 0; i < 2; i++ ) {
        Pox[i] = 0.0;
        Poy[i] = 0.0;
    }

    Rotsys = FK5;
    Roteqx.year = 2000.0;
    Roteqx.type = 'J';
    Iaa = 0.0;
    Ipa = 0.0;

    return TCL_OK;
}

static int end( Tcl_Interp *interp, Tcl_Obj *obj )
{
    double delta, rawt, tt, utc, tai;
    double a, b, ra, dec;
    double aoprms[15];
    int i;
    double m2xy[3][2];
    struct WCS_CTX ctx;
    struct WCS wcs, iwcs;
    int hmsf[4];
    double diam, r;
    int jstat;
    char result[12];

/* Check that there is at least a source A target. */
    if ( !TargetSet[0] ) {
        Tcl_SetResult( interp, "Configuration must contain a science target",
            TCL_VOLATILE);
        return TCL_ERROR;
    }

/* Compute time for preview. */
    if ( Tcl_GetDoubleFromObj( interp, obj, &delta ) != TCL_OK ) 
        return TCL_ERROR;
    timeNow( &rawt );
    rawt += delta;
    timeThenD( rawt, UTC, &utc );
    timeThenD( rawt, TT, &tt );
    timeThenD( rawt, TAI, &tai );

/* Update sidereal time and refraction constants in observed to apparent
   array. */
    for ( i = 0; i < 15; i++ ) aoprms[i] = Aoprms[i];
    slaAoppat( utc, aoprms );
    slaAtmdsp( aoprms[5], aoprms[6], aoprms[7], WAVELR, aoprms[10], aoprms[11],
        Wavel[0], &aoprms[10], &aoprms[11]);
    aoprms[8] = Wavel[0];

    switch ( TargetSet[0] ) {

/* Convert source position to current epoch */
       case COORD:
          if ( astCoco( Theta1[0], Theta2[0], Pm[0], System[0], Equinox[0], 
             Epoch[0], System[0], Equinox[0], tt, aoprms, Telp, &a, &b) != 0 ) {
             Tcl_SetResult( interp, "unreachable position", TCL_VOLATILE);
             return TCL_ERROR;
          }
          break;

/* Get planet position */
       case PLANET:
          slaRdplan( tt, Planet, Elong, Elat, &a, &b, &diam );
          System[0] = APPT;
          Theta1[0] = a;
          Theta2[0] = b;
          break;

/* Get orbit position */
       case ORBIT:
          slaPlante( tt, Elong, Elat, Jtype, T0, Orbel[0] * D2R, 
             Orbel[1] * D2R, Orbel[2] * D2R, Orbel[3], Orbel[4], 
             Orbel[5] * D2R, Orbel[6] * D2R, &a, &b, &r, &jstat );
          System[0] = APPT;
          Theta1[0] = a;
          Theta2[0] = b;
          break;
    }

/* Set pointing origin in telescope structure. */
    Telp.pox = Pox[0];
    Telp.poy = Poy[0];

/* Simulate telescope context. */
    for ( i = 0; i < 3; i++ ) {
        m2xy[i][0] = 0.0;
        m2xy[i][1] = 0.0;
    }
    if ( astSimctx_r( tai, Elong, aoprms[0], aoprms[4], Xpmr, Ypmr, aoprms[5], 
        aoprms[6], aoprms[7], aoprms[9], aoprms[8], Telp, m2xy, a, b, 
        System[0], Equinox[0], Iaa, Ipa, Rotsys, Roteqx, &ctx) ) {
        Tcl_SetResult( interp, "unreachable position", TCL_VOLATILE);
        return TCL_ERROR;
    }

/* Compute WFS positions */
    for ( i = 0; i < 3; i++ ) {
        if ( TargetSet[i + 1] ) {
            if ( astCoco( Theta1[i + 1], Theta2[i + 1], Pm[i + 1], 
                System[i + 1], Equinox[i + 1], Epoch[i + 1], System[i + 1],
                Equinox[i + 1], tt, aoprms, ctx.tel, &a, &b ) ) {
                Tcl_SetResult( interp, "uncomputable WFS target", TCL_VOLATILE);
                return TCL_ERROR;
            }
            if ( astCtx2tr( ctx, System[i + 1], Equinox[i + 1], Wavel[i + 1], 0,
                &wcs, &rawt ) ) {
                Tcl_SetResult( interp, 
                    "WCS transformation generation failed", TCL_VOLATILE);
                return TCL_ERROR;
            }
            if ( astInvtr( wcs, &iwcs ) ) {
                Tcl_SetResult( interp, "inverting WCS transformation failed",
                    TCL_VOLATILE);
                return TCL_ERROR;
            }
            if ( astS2xyq( a, b, iwcs, &Wfsx[i], &Wfsy[i] ) ) {
                Tcl_SetResult( interp, "uncomputable WFS x/y", TCL_VOLATILE);
                return TCL_ERROR;
            }

/* Correct position for optical distortions. */
            tccXy2Probe( &Wfsx[i], &Wfsy[i]);
        }
    }
    

/* Compute Apparent RA/Dec of telescope. */
    if ( astCoco( Theta1[0], Theta2[0], Pm[0], System[0], Equinox[0], Epoch[0],
        APPT, Roteqx, tt, aoprms, ctx.tel, &ra, &dec) != 0 ) {
        Tcl_SetResult( interp, "unreachable position", TCL_VOLATILE);
        return TCL_ERROR;
    }

/* Convert to HA/Dec. */
    HA = slaDrange(aoprms[13] - ra);

/* Compute zenith distance and airmass. */
    ZD = slaZd( HA, dec, aoprms[0]);
    Airmass = slaAirmas( ZD);

/* Store mount and rotator data. */
    Az1 = Az2 = slaDranrm(ctx.ab0[0]);
    if ( Az1 > PI ) Az1 -= PI2;
    El = slaDrange(ctx.ab0[1]);
    Rma1 = Rma2 = slaDranrm(ctx.tel.rma);
    if ( Rma1 > D90 ) Rma1 -= PI2;
    if ( Rma2 > PI + D90 ) Rma2 -= PI2;
    Visible = El > Ellim;

/* Compute times to limits */
    limitTimes( ra, dec );

/* Return UTC of preview and visible flag. */
    timeThenT( rawt, UTC, 0, hmsf);
    sprintf( result, "%02d:%02d:%02d %d", hmsf[0], hmsf[1], hmsf[2], Visible);
    Tcl_SetResult( interp, result, TCL_VOLATILE);
    return TCL_OK;
}

static int setPo( Tcl_Interp *interp, int po, int objc, Tcl_Obj *CONST objv[])
{
    if ( objc != 5 ) {
        Tcl_WrongNumArgs( interp, 3, objv, "x y");
        return TCL_ERROR;
    }
    if ( Tcl_GetDoubleFromObj( interp, objv[3],  &Pox[po] ) != TCL_OK )
        return TCL_ERROR;
    if ( Tcl_GetDoubleFromObj( interp, objv[4],  &Poy[po] ) != TCL_OK ) 
        return TCL_ERROR;
    return TCL_OK;
}

static int setIaa( Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
    if ( objc != 4 ) {
        Tcl_WrongNumArgs( interp, 3, objv, "angle");
        return TCL_ERROR;
    }
    if ( Tcl_GetDoubleFromObj( interp, objv[3],  &Iaa ) != TCL_OK ) 
        return TCL_ERROR;
    Iaa *= D2R;
    return TCL_OK;
}

static int setIpa( Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
    if ( objc != 6 ) {
        Tcl_WrongNumArgs( interp, 3, objv, "angle frame equinox");
        return TCL_ERROR;
    }
    if ( Tcl_GetDoubleFromObj( interp, objv[3],  &Ipa ) != TCL_OK ) 
        return TCL_ERROR;
    Ipa *= D2R;
    if ( tccDcFrame( interp, Tcl_GetStringFromObj( objv[4], NULL), &Rotsys) 
        != TCL_OK ) return TCL_ERROR;
    if ( tccDcEpoch( interp, Tcl_GetStringFromObj( objv[5], NULL), 
        &Roteqx.type, &Roteqx.year ) != TCL_OK ) return TCL_ERROR;
    return TCL_OK;
}

static int setWavel( Tcl_Interp *interp, int target, int objc, 
    Tcl_Obj *CONST objv[])
{
    if ( objc != 4 ) {
        Tcl_WrongNumArgs( interp, 3, objv, "value");
        return TCL_ERROR;
    }
    if ( Tcl_GetDoubleFromObj( interp, objv[3],  &Wavel[target] ) != TCL_OK ) 
        return TCL_ERROR;
    return TCL_OK;
}

static int setLimits( Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
    if ( objc != 9 ) {
        Tcl_WrongNumArgs( interp, 3, objv, "az az el rot rot zd");
        return TCL_ERROR;
    }
    if ( Tcl_GetDoubleFromObj( interp, objv[3],  &Azlolim ) != TCL_OK ) 
        return TCL_ERROR;
    if ( Tcl_GetDoubleFromObj( interp, objv[4],  &Azhilim ) != TCL_OK ) 
        return TCL_ERROR;
    if ( Tcl_GetDoubleFromObj( interp, objv[5],  &Ellim ) != TCL_OK ) 
        return TCL_ERROR;
    if ( Tcl_GetDoubleFromObj( interp, objv[6],  &Rotlolim ) != TCL_OK ) 
        return TCL_ERROR;
    if ( Tcl_GetDoubleFromObj( interp, objv[7],  &Rothilim ) != TCL_OK ) 
        return TCL_ERROR;
    if ( Tcl_GetDoubleFromObj( interp, objv[8],  &Zlim ) != TCL_OK ) 
        return TCL_ERROR;
    Azlolim *= D2R;
    Azhilim *= D2R;
    Ellim *= D2R;
    Rotlolim *= D2R;
    Rothilim *= D2R;
    Zlim *= D2R;
    return TCL_OK;
}

static int getFpxy( Tcl_Interp *interp, int target)
{
    char result[30];

    if ( TargetSet[target] ) {
        sprintf( result, "%.3f %.3f", Wfsx[target - 1], Wfsy[target - 1]);
        Tcl_SetResult( interp, result, TCL_VOLATILE);
    }
    return TCL_OK;
}

static int getMount( Tcl_Interp *interp)
{
    int dmsf1[4], dmsf2[4], dmsf3[4];
    char sign1, sign2, sign3;
    char result[40];

    slaDr2af( 0, Az1, &sign1, dmsf1);
    slaDr2af( 0, Az2, &sign2, dmsf2);
    slaDr2af( 0, El, &sign3, dmsf3);
    sprintf( result, "%c%02d:%02d:%02d %c%02d:%02d:%02d %c%02d:%02d:%02d", 
        sign1, dmsf1[0], dmsf1[1], dmsf1[2], 
        sign2, dmsf2[0], dmsf2[1], dmsf2[2],
        sign3, dmsf3[0], dmsf3[1], dmsf3[2]);
    Tcl_SetResult( interp, result, TCL_VOLATILE);
    return TCL_OK;
}

static int getSource( Tcl_Interp *interp)
{
    int hmsf0[4];
    char sign0;
    char result[30];

    slaDr2tf( 0, HA, &sign0, hmsf0);
    sprintf( result, "%c%02d:%02d:%02d %.1f %.2f", sign0, hmsf0[0], 
        hmsf0[1], hmsf0[2], ZD/D2R, Airmass);
    Tcl_SetResult( interp, result, TCL_VOLATILE);
    return TCL_OK;
}

static int getTarget( Tcl_Interp *interp, int i)
{
    int hmsf[4], dmsf[4];
    char sign1, sign2;
    char result[60];

    if ( TargetSet[i] ) {
       slaDr2tf( 3, Theta1[i], &sign1, hmsf);
       slaDr2af( 2, Theta2[i], &sign2, dmsf);
       switch ( System[i] ) {
          case FK4:
             sprintf( result, 
                   "%02d:%02d:%02d.%03d %c%02d:%02d:%02d.%02d FK4 %c%.2f",
                   hmsf[0], hmsf[1], hmsf[2], hmsf[3], sign2, dmsf[0], dmsf[1], 
                   dmsf[2], dmsf[3], Equinox[i].type, Equinox[i].year);
             break;
          case FK5:
             sprintf( result, 
                   "%02d:%02d:%02d.%03d %c%02d:%02d:%02d.%02d FK5 %c%.2f",
                   hmsf[0], hmsf[1], hmsf[2], hmsf[3], sign2, dmsf[0], dmsf[1], 
                   dmsf[2], dmsf[3], Equinox[i].type, Equinox[i].year);
             break;
          case APPT:
             sprintf( result, 
                   "%02d:%02d:%02d.%03d %c%02d:%02d:%02d.%02d Apparent",
                   hmsf[0], hmsf[1], hmsf[2], hmsf[3], sign2, dmsf[0], dmsf[1], 
                   dmsf[2], dmsf[3]);
             break;
          case AZEL_TOPO:
             sprintf( result, 
                   "%.3f %.3f Topocentric Az/El", Theta1[i]/D2R, Theta2[i]/D2R);
             break;
        }
    }
    Tcl_SetResult( interp, result, TCL_VOLATILE);
    return TCL_OK;
}

static int getRotator( Tcl_Interp *interp)
{
    char result[20];
    sprintf( result, "%6.2f %6.2f", Rma1 / D2R, Rma2 / D2R);
    Tcl_SetResult( interp, result, TCL_VOLATILE);
    return TCL_OK;
}

static int getLimits( Tcl_Interp *interp)
{
    char val[5];
    if ( Azttl1 >= 0.0 ) {
        sprintf( val, "%4d", (int) floor(Azttl1));
        Tcl_AppendElement( interp, val);
    } else {
        Tcl_AppendElement( interp, "");
    }
    if ( Azttl2 >= 0.0 ) {
        sprintf( val, " %4d", (int) floor(Azttl2));
        Tcl_AppendElement( interp, val);
    } else {
        Tcl_AppendElement( interp, "");
    }
    if ( Elttl >= 0.0 ) {
        sprintf( val, " %4d", (int) floor(Elttl));
        Tcl_AppendElement( interp, val);
    } else {
        Tcl_AppendElement( interp, "");
    }
    if ( Rotttl1 >= 0.0 ) {
        sprintf( val, " %4d", (int) floor(Rotttl1));
        Tcl_AppendElement( interp, val);
    } else {
        Tcl_AppendElement( interp, "");
    }
    if ( Rotttl2 >= 0.0 ) {
        sprintf( val, " %4d", (int) floor(Rotttl2));
        Tcl_AppendElement( interp, val);
    } else {
        Tcl_AppendElement( interp, "");
    }
    if ( Zttl >= 0.0 ) {
        sprintf( val, " %4d", (int) floor(Zttl));
        Tcl_AppendElement( interp, val);
    } else {
        Tcl_AppendElement( interp, "");
    }
    return TCL_OK;
}

static int setTarget( Tcl_Interp *interp, int target, int objc,
    Tcl_Obj *CONST objv[])
{
    int nel;
    Tcl_Obj **objptr;
    if ( objc < 6 || objc > 11) {
        Tcl_WrongNumArgs( interp, 3, objv, 
            "frame theta1 theta2 ?equinox epoch pm parallax rv?");
        return TCL_ERROR;
    }
    if ( tccDcFrame( interp, Tcl_GetStringFromObj( objv[3], NULL), 
        &System[target]) != TCL_OK ) return TCL_ERROR;
    if ( tccDcRadec( interp, System[target], 
        Tcl_GetStringFromObj( objv[4], NULL), 
        Tcl_GetStringFromObj( objv[5], NULL), &Theta1[target], &Theta2[target])
        != TCL_OK ) return TCL_ERROR;
    if ( objc > 6 ) {
        if ( tccDcEpoch( interp, Tcl_GetStringFromObj( objv[6], NULL), 
            &Equinox[target].type, &Equinox[target].year ) != TCL_OK ) 
            return TCL_ERROR;
    } else {
        Equinox[target].type = 'J';
        Equinox[target].year = 2000.0;
    }
    if ( objc > 7 ) {
        if ( tccDcEpoch( interp, Tcl_GetStringFromObj( objv[7], NULL), 
            &Epoch[target].type, &Epoch[target].year ) != TCL_OK ) 
            return TCL_ERROR;
    } else {
        Epoch[target].type = 'J';
        Epoch[target].year = 2000.0;
    }
    if ( objc > 8 ) {
        if ( Tcl_ListObjGetElements( interp, objv[8], &nel, &objptr) != 
            TCL_OK) return TCL_ERROR;
        if ( nel == 0 ) {
            Pm[target].pm = 0;
        } else {
            if ( nel != 2 ) {
                Tcl_SetResult( interp, 
                    "proper motion has the wrong number of elements", 
                    TCL_VOLATILE);
                return TCL_ERROR;
            }
            if ( Tcl_GetDoubleFromObj( interp, *objptr++, &Pm[target].pmRA )
                != TCL_OK ) return TCL_ERROR;
            if ( Tcl_GetDoubleFromObj( interp, *objptr++, &Pm[target].pmDec )
                != TCL_OK ) return TCL_ERROR;
            Pm[target].pm = 1;
        }
    }
    Pm[target].px = 0.0;
    Pm[target].rv = 0.0;
    if ( objc > 9 ) {
        if ( Tcl_GetDoubleFromObj( interp, objv[9],  &Pm[target].px ) != 
            TCL_OK ) return TCL_ERROR;
    }
    if ( objc > 10 ) {
        if ( Tcl_GetDoubleFromObj( interp, objv[10],  &Pm[target].rv ) != 
            TCL_OK ) return TCL_ERROR;
    }
    TargetSet[target] = COORD;
    return TCL_OK;
}

static int setPlanet( Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
    if ( objc != 4 ) {
        Tcl_WrongNumArgs( interp, 3, objv, "planet");
        return TCL_ERROR;
    }
    if ( tccDcPlanet( interp, Tcl_GetStringFromObj( objv[3], NULL), &Planet ) ) 
        return TCL_ERROR;
    TargetSet[0] = PLANET;
    return TCL_OK;
}

static int setOrbit( Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
    int i, nel;

    if ( objc < 4 ) {
        Tcl_WrongNumArgs( interp, 3, objv, "type elements...");
        return TCL_ERROR;
    }
    if ( Tcl_GetIntFromObj( interp, objv[3], &Jtype) != TCL_OK) 
       return TCL_ERROR;
    if ( Jtype == 1 ) {
      if ( objc != 12 ) {
         Tcl_WrongNumArgs( interp, 4, objv, 
    "epoch inc longa longper meandist perdist e meanlong meananom dailymot");
         return TCL_ERROR;
      }
      nel = 7;
    } else if ( Jtype == 2 ) {
      if ( objc != 11 ) {
         Tcl_WrongNumArgs( interp, 4, objv, 
              "epoch inc longa argofper meandist perdist e meanlong meananom");
         return TCL_ERROR;
      }
      nel = 6;
    } else if ( Jtype == 3 ) {
      if ( objc != 10 ) {
         Tcl_WrongNumArgs( interp, 4, objv, 
              "epoch inc longa argofper meandist perdist e meanlong");
         return TCL_ERROR;
      }
      nel = 5;
    } else {
       Tcl_SetResult( interp, "invalid orbital element set", TCL_VOLATILE );
       return TCL_ERROR;
    }
    if ( tccDcT0( interp, Tcl_GetStringFromObj( objv[4], NULL) , &T0 ) != 
        TCL_OK) return TCL_ERROR;
    for ( i = 0; i < nel; i++ ) {
        if ( Tcl_GetDoubleFromObj( interp, objv[i+5], &Orbel[i] ) != TCL_OK )
            return TCL_ERROR;
    }
    TargetSet[0] = ORBIT;
    return TCL_OK;
}

static int update( Tcl_Interp *interp, char *arrayname )
{
    Tcl_Obj *val;
    double tlongm, tlatm, hm, tdc, pmb, rh, tlr, delut;
    double date;
    int y, m, d, status;
    char cmd[80];

/* Copy data from tcl array. */
    if ((val = Tcl_GetVar2Ex( interp, arrayname, "tlongm", TCL_LEAVE_ERR_MSG))
        == NULL) return TCL_ERROR;
    if ( Tcl_GetDoubleFromObj( interp, val, &tlongm ) != TCL_OK ) 
        return TCL_ERROR;

    if ((val = Tcl_GetVar2Ex( interp, arrayname, "tlatm", TCL_LEAVE_ERR_MSG))
        == NULL) return TCL_ERROR;
    if ( Tcl_GetDoubleFromObj( interp, val, &tlatm ) != TCL_OK ) 
        return TCL_ERROR;

    if ((val = Tcl_GetVar2Ex( interp, arrayname, "hm", TCL_LEAVE_ERR_MSG))
        == NULL) return TCL_ERROR;
    if ( Tcl_GetDoubleFromObj( interp, val, &hm ) != TCL_OK ) 
        return TCL_ERROR;

    if ((val = Tcl_GetVar2Ex( interp, arrayname, "xpm", TCL_LEAVE_ERR_MSG))
        == NULL) return TCL_ERROR;
    if ( Tcl_GetDoubleFromObj( interp, val, &Xpmr ) != TCL_OK ) 
        return TCL_ERROR;
    Xpmr *= AS2R;

    if ((val = Tcl_GetVar2Ex( interp, arrayname, "ypm", TCL_LEAVE_ERR_MSG))
        == NULL) return TCL_ERROR;
    if ( Tcl_GetDoubleFromObj( interp, val, &Ypmr ) != TCL_OK ) 
        return TCL_ERROR;
    Ypmr *= AS2R;

    if ((val = Tcl_GetVar2Ex( interp, arrayname, "tdc", TCL_LEAVE_ERR_MSG))
        == NULL) return TCL_ERROR;
    if ( Tcl_GetDoubleFromObj( interp, val, &tdc ) != TCL_OK ) 
        return TCL_ERROR;

    if ((val = Tcl_GetVar2Ex( interp, arrayname, "pmb", TCL_LEAVE_ERR_MSG))
        == NULL) return TCL_ERROR;
    if ( Tcl_GetDoubleFromObj( interp, val, &pmb ) != TCL_OK ) 
        return TCL_ERROR;

    if ((val = Tcl_GetVar2Ex( interp, arrayname, "rh", TCL_LEAVE_ERR_MSG))
        == NULL) return TCL_ERROR;
    if ( Tcl_GetDoubleFromObj( interp, val, &rh ) != TCL_OK ) 
        return TCL_ERROR;

    if ((val = Tcl_GetVar2Ex( interp, arrayname, "tlr", TCL_LEAVE_ERR_MSG))
        == NULL) return TCL_ERROR;
    if ( Tcl_GetDoubleFromObj( interp, val, &tlr ) != TCL_OK ) 
        return TCL_ERROR;

    if ((val = Tcl_GetVar2Ex( interp, arrayname, "fl", TCL_LEAVE_ERR_MSG))
        == NULL) return TCL_ERROR;
    if ( Tcl_GetDoubleFromObj( interp, val, &Telp.fl ) != TCL_OK ) 
        return TCL_ERROR;

    if ((val = Tcl_GetVar2Ex( interp, arrayname, "an", TCL_LEAVE_ERR_MSG))
        == NULL) return TCL_ERROR;
    if ( Tcl_GetDoubleFromObj( interp, val, &Telp.an ) != TCL_OK ) 
        return TCL_ERROR;
    Telp.an *= AS2R;

    if ((val = Tcl_GetVar2Ex( interp, arrayname, "aw", TCL_LEAVE_ERR_MSG))
        == NULL) return TCL_ERROR;
    if ( Tcl_GetDoubleFromObj( interp, val, &Telp.aw ) != TCL_OK ) 
        return TCL_ERROR;
    Telp.aw *= AS2R;

    if ((val = Tcl_GetVar2Ex( interp, arrayname, "pnpae", TCL_LEAVE_ERR_MSG))
        == NULL) return TCL_ERROR;
    if ( Tcl_GetDoubleFromObj( interp, val, &Telp.pnpae ) != TCL_OK ) 
        return TCL_ERROR;
    Telp.pnpae *= AS2R;

    if ((val = Tcl_GetVar2Ex( interp, arrayname, "ca", TCL_LEAVE_ERR_MSG))
        == NULL) return TCL_ERROR;
    if ( Tcl_GetDoubleFromObj( interp, val, &Telp.ca ) != TCL_OK ) 
        return TCL_ERROR;
    Telp.ca *= AS2R;

    if ((val = Tcl_GetVar2Ex( interp, arrayname, "ce", TCL_LEAVE_ERR_MSG))
        == NULL) return TCL_ERROR;
    if ( Tcl_GetDoubleFromObj( interp, val, &Telp.ce ) != TCL_OK ) 
        return TCL_ERROR;
    Telp.ce *= AS2R;

    if ((val = Tcl_GetVar2Ex( interp, arrayname, "delut", TCL_LEAVE_ERR_MSG))
        == NULL) return TCL_ERROR;
    if ( Tcl_GetDoubleFromObj( interp, val, &delut ) != TCL_OK ) 
        return TCL_ERROR;

/* Set Apparent to observed parameters. */
    timeNowD( UTC, &date);
    slaAoppa( date, delut, tlongm * D2R, tlatm * D2R, hm, Xpmr, Ypmr,
        tdc + FREEZING, pmb, rh/100.0, WAVELR, tlr, Aoprms);
    slaPolmo( tlongm * D2R, tlatm * D2R, Xpmr, Ypmr, &Elong,
        &Elat, &Aoprms[14]);

     return TCL_OK;
}

int limitTimes( double ra, double dec)
{
    double tlat, clat, slat;
    double ha, az[2], rma[2], cdec, sdec, cha, sha;
    double azhalo[2], azhahi[2], azttl[2];
    double rothalo[2], rothahi[2], rotttl[2];
    double x, y, a, r;
    double sqsz, cqsz;
    int wrap, i, j;
    int azlocount, azhicount, azhidone[2][2], azlodone[2][2], azvalid[2];
    int rotlocount, rothicount, rothidone[2][2], rotlodone[2][2], rotvalid[2];

/* Latitude and ST from apparent to observed array. */
    tlat = Aoprms[0];
    slat = Aoprms[1];
    clat = Aoprms[2];

/* Functions of declination. */
    cdec = cos( dec );
    sdec = sin( dec );

/* Elevation limit. This is either the time to set or the time to rise 
   depending on whether the source is visible or not. */
    if ( tccLimEl( cdec, sdec, Ellim, clat, slat, &ha) == 0 ) {
       Elttl = Visible ? slaDranrm( ha - HA) : slaDranrm( - ha - HA);
       Elttl *= ST2MIN;
    } else {
       Elttl = -1.0;
    }

/* Azimuth and rotator wrap limits. */

/* Hour angles at which azimuth limits are reached. */
    azlocount = tccLimAz( cdec, sdec, Azlolim, clat, slat, azhalo);
    azhicount = tccLimAz( cdec, sdec, Azhilim, clat, slat, azhahi);

/* Hour angles at which rotator limits (adjusted for the desired
    rotator angle) are reached. */
    rotlocount = tccLimPa( cdec, sdec, PI - Rotlolim + Ipa - Iaa,
        clat, slat, rothalo);
    rothicount = tccLimPa( cdec, sdec, PI - Rothilim + Ipa - Iaa, 
        clat, slat, rothahi);

/* Adjust limit hour angles to be greater than current. */
    for ( i = 0; i < azlocount; i++ ) {
        if ( azhalo[i] < HA ) azhalo[i] += PI2;
    }
    for ( i = 0; i < azhicount; i++ ) {
        if ( azhahi[i] < HA ) azhahi[i] += PI2;
    }
    for ( i = 0; i < rotlocount; i++ ) {
        if ( rothalo[i] < HA ) rothalo[i] += PI2;
    }
    for ( i = 0; i < rothicount; i++ ) {
        if ( rothahi[i] < HA ) rothahi[i] += PI2;
    }

/* Step the azimuth forward in 6 hour increments up to the hour
   angle of the limit. */
    az[0] = Az1;
    az[1] = Az2;
    rma[0] = Rma1;
    rma[1] = Rma2;
    a = slaDrange( Az1 );
    r = slaDrange( Rma1 );
    ha = floor(HA/D90) * D90;
    for ( wrap = 0; wrap < 2; wrap++ ) {
        for ( i = 0; i < 2; i++ ) {
            azlodone[wrap][i] = azhidone[wrap][i] = 0;
            rotlodone[wrap][i] = rothidone[wrap][i] = 0;
        }
    }
    azvalid[0] = azvalid[1] = 0;
    rotvalid[0] = rotvalid[1] = 0;
    for ( i = 0; i < 6; i++)  {
        ha += D90;
 
    /* Azimuth at this hour angle. */
        cha = cos( ha );
        sha = sin( ha );
        x = - cha * cdec * slat + sdec * clat;
        y = - sha * cdec;
        a = ( x == 0.0 && y == 0.0) ? a : atan2( y, x);
        for ( wrap = 0; wrap < 2; wrap++ ) {
            az[wrap] += slaDrange( a - az[wrap]);

        /* Is the hour angle greater than any of the limits? */
            for ( j = 0; j < azlocount; j++ ) {
                if ( !azlodone[wrap][j] && azhalo[j] <= ha ) {
                    azlodone[wrap][j] = 1;

            /* Is the azimuth close to the limit? */
                    if ( fabs( az[wrap] - Azlolim ) <= PI ) {
                       if ( !azvalid[wrap] || (azhalo[j] - HA) < azttl[wrap] ) {
                           azttl[wrap] = azhalo[j] - HA;
                           azvalid[wrap] = 1;
                       }
                    }
                }
            }

        /* Repeat for the high limit. */
            for ( j = 0; j < azhicount; j++ ) {
                if ( !azhidone[wrap][j] && azhahi[j] <= ha ) {
                    azhidone[wrap][j] = 1;

            /* Is the azimuth close to the limit? */
                    if ( fabs( az[wrap] - Azhilim ) <= PI ) {
                       if ( !azvalid[wrap] || (azhahi[j] - HA) < azttl[wrap] ) {
                           azttl[wrap] = azhahi[j] - HA;
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
        r += PI + (Ipa - Iaa);
        for ( wrap = 0; wrap < 2; wrap++ ) {
            rma[wrap] += slaDrange( r - rma[wrap]);

            for ( j = 0; j < rotlocount; j++ ) {
                if ( !rotlodone[wrap][j] && rothalo[j] <= ha ) {
                    rotlodone[wrap][j] = 1;

            /* Is the rma close to the limit? */
                    if ( fabs( rma[wrap] - Rotlolim ) <= PI ) {
                       if ( !rotvalid[wrap] || (rothalo[j] - HA) < 
                               rotttl[wrap] ) {
                           rotttl[wrap] = rothalo[j] - HA;
                           rotvalid[wrap] = 1;
                       }
                    }
                }
            }

        /* Repeat for the high limit. */
            for ( j = 0; j < rothicount; j++ ) {
                if ( !rothidone[wrap][j] && rothahi[j] <= ha ) {
                    rothidone[wrap][j] = 1;

            /* Is the azimuth close to the limit? */
                    if ( fabs( rma[wrap] - Rothilim ) <= PI ) {
                       if ( !rotvalid[wrap] || (rothahi[j] - HA) < 
                               rotttl[wrap] ) {
                           rotttl[wrap] = rothahi[j] - HA;
                           rotvalid[wrap] = 1;
                       }
                    }
                }
            }
        }
    }

/* Copy azimuth limit to outputs. */
    if (azvalid[0]) {
        Azttl1 = azttl[0] * ST2MIN;
    } else {
        Azttl1 = -1.0;
    }
    if (azvalid[1]) {
        Azttl2 = azttl[1] * ST2MIN;
    } else {
        Azttl2 = -1.0;
    }

/* Copy rotator limit to outputs. */
    if (Rotsys != AZEL_MNT) {
        if (rotvalid[0]) {
            Rotttl1 = rotttl[0] * ST2MIN;
        } else {
            Rotttl1 = -1.0;
        }
        if (rotvalid[1]) {
            Rotttl2 = rotttl[1] * ST2MIN;
        } else {
            Rotttl2 = -1.0;
        }
    }

/* Zenith at risk area limit */
   if ( Zlim < fabs( dec - tlat )  || HA > Zlim ) {
       Zttl = -1.0;
   } else {
     Zttl = ( - Zlim - HA ) * ST2MIN;
   }

   return TCL_OK;
}
