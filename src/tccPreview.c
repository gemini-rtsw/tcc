static char rcsid[]="$Id:";
/* *INDENT-OFF* */
/*
*   FILENAME
*   tccPreview.c
*
*   FUNCTION NAME(S)
*   tccPreviewCmd - Implements the tccPreview tcl command.
*
*   D L Terrett 16 February 2000
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
#include "tccConstants.h"
#include "tccUtil.h"
#include "tccDecode.h"

static int begin( Tcl_Interp* );
static int end( Tcl_Interp*, char* );
static int getFpxy( Tcl_Interp*, int );
static int getMount( Tcl_Interp*);
static int getLimits( Tcl_Interp*);
static int getSource( Tcl_Interp*);
static int getTarget( Tcl_Interp*, int);
static int getRotator( Tcl_Interp*);
static int setPo( Tcl_Interp*, int, int, char*[]); 
static int setTarget( Tcl_Interp*, int, int, char*[]);
static int setWavel( Tcl_Interp*, int, int, char*[]); 
static int setLimits( Tcl_Interp*, int, char*[]); 
static int setIpa( Tcl_Interp*, int, char*[]); 
static int setIaa( Tcl_Interp*, int, char*[]); 
static int setPlanet( Tcl_Interp*, int, char*[]); 
static int setOrbit( Tcl_Interp*, int, char*[]); 
static int update( Tcl_Interp*, char* );
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

int Tccext_PreviewCmd( ClientData clientdata, Tcl_Interp *interp, int argc,
    char *argv[])
{
    int result;

    if ( argc < 2 ) {
        Tcl_AppendResult( interp,
            "wrong # args: should be \"tccPreview option ?arg ...?\"",
             (char *) NULL);
        return TCL_ERROR;
    }

    if ( strcmp( argv[1], "begin" ) == 0 ) {
        if ( argc != 2 ) {
            Tcl_AppendResult( interp,
                "wrong # args: should be \"tccPreview begin\"",
                (char *) NULL);
            return TCL_ERROR;
        }
        result = begin( interp );
    } else if ( strcmp( argv[1], "end" ) == 0 ) {
        if ( argc != 3 ) {
            Tcl_AppendResult( interp,
                "wrong # args: should be \"tccPreview end time\"",
                (char *) NULL);
            return TCL_ERROR;
        }
        result = end( interp, argv[2] );
    } else if ( strcmp( argv[1], "get" ) == 0 ) {
        if ( argc != 3 ) {
            Tcl_AppendResult( interp,
                "wrong # args: should be \"tccPreview get item\"",
                (char *) NULL);
            return TCL_ERROR;
        }
        if ( strcmp( argv[2], "mount" ) == 0 ) {
            result = getMount( interp );
        } else if ( strcmp( argv[2], "source" ) == 0 ) {
            result = getSource( interp );
        } else if ( strcmp( argv[2], "source_target" ) == 0 ) {
            result = getTarget( interp, 0 );
        } else if ( strcmp( argv[2], "pwfs1_target" ) == 0 ) {
            result = getTarget( interp, 1 );
        } else if ( strcmp( argv[2], "pwfs2_target" ) == 0 ) {
            result = getTarget( interp, 2 );
        } else if ( strcmp( argv[2], "oiwfs_target" ) == 0 ) {
            result = getTarget( interp, 3 );
        } else if ( strcmp( argv[2], "rotator" ) == 0 ) {
            result = getRotator( interp );
        } else if ( strcmp( argv[2], "pwfs1" ) == 0 ) {
            result = getFpxy( interp, 1 );
        } else if ( strcmp( argv[2], "pwfs2" ) == 0 ) {
            result = getFpxy( interp, 2 );
        } else if ( strcmp( argv[2], "oiwfs" ) == 0 ) {
            result = getFpxy( interp, 3 );
        } else if ( strcmp( argv[2], "limits" ) == 0 ) {
            result = getLimits( interp );
        } else {
            Tcl_AppendResult( interp, "unknown option \"", argv[2],
        "\" must be mount source rotator, pwfs1, pwfs2, oiwfs or limits", 
                (char *) NULL);
        }
    } else if ( strcmp( argv[1], "set" ) == 0 ) {
        if ( strcmp( argv[2], "sourceA" ) == 0 ) {
            result = setTarget( interp, 0, argc, argv );
        } else if ( strcmp( argv[2], "pwfs1" ) == 0 ) {
            result = setTarget( interp, 1, argc, argv );
        } else if ( strcmp( argv[2], "pwfs2" ) == 0 ) {
            result = setTarget( interp, 2, argc, argv );
        } else if ( strcmp( argv[2], "oiwfs" ) == 0 ) {
            result = setTarget( interp, 3, argc, argv );
        } else if ( strcmp( argv[2], "poriginA" ) == 0 ) {
            result = setPo( interp, 0, argc, argv );
        } else if ( strcmp( argv[2], "poriginB" ) == 0 ) {
            result = setPo( interp, 1, argc, argv );
        } else if ( strcmp( argv[2], "wavelsourceA" ) == 0 ) {
            result = setWavel( interp, 0, argc, argv);
        } else if ( strcmp( argv[2], "wavelpwfs1" ) == 0 ) {
            result = setWavel( interp, 1, argc, argv);
        } else if ( strcmp( argv[2], "wavelpwfs2" ) == 0 ) {
            result = setWavel( interp, 2, argc, argv);
        } else if ( strcmp( argv[2], "waveloiwfs" ) == 0 ) {
            result = setWavel( interp, 3, argc, argv);
        } else if ( strcmp( argv[2], "limits" ) == 0 ) {
            result = setLimits( interp, argc, argv);
        } else if ( strcmp( argv[2], "ipa" ) == 0 ) {
            result = setIpa( interp, argc, argv);
        } else if ( strcmp( argv[2], "iaa" ) == 0 ) {
            result = setIaa( interp, argc, argv);
        } else if ( strcmp( argv[2], "planet" ) == 0 ) {
            result = setPlanet( interp, argc, argv);
        } else if ( strcmp( argv[2], "orbit" ) == 0 ) {
            result = setOrbit( interp, argc, argv);
        } else {
            Tcl_AppendResult( interp, "unknown option \"", argv[2],
                "\" must be sourceA, pwfs1, pwfs2, oiwfs, poriginA, poriginB,",
                " wavelsourceA wavelpwfs1, wavelpwfs2, oiwfswavel, planet", 
                " ipa or iaa", (char *) NULL);
        }
    } else if ( strcmp( argv[1], "update" ) == 0 ) {
        if ( argc != 3 ) {
            Tcl_AppendResult( interp,
                "wrong # args: should be \"tccPreview update arrayname\"",
                (char *) NULL);
            return TCL_ERROR;
        }
        result = update( interp, argv[2] );
    } else {
        Tcl_AppendResult( interp, "unknown option \"", argv[1], 
            "\" must be begin, end, sourceA, pwfs1, pwfs2, oiwfs or update", 
            (char *) NULL);
        return TCL_ERROR;
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

static int end( Tcl_Interp *interp, char *arg )
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

/* Check that there is at least a source A target. */
    if ( !TargetSet[0] ) {
        Tcl_AppendResult( interp,
            "Configuration must contain a science target", (char*) NULL);
        return TCL_ERROR;
    }

/* Compute time for preview. */
    if ( sscanf( arg, "%lf", &delta) != 1 ) {
        Tcl_AppendResult( interp, "invalid delta time \"", arg, "\"", 
           (char *) NULL);
        return TCL_ERROR;
    }
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
             Tcl_AppendResult( interp, "unreachable position", (char *) NULL);
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
          slaPlante( tt, Elong, Elat, Jtype, T0, Orbel[0], Orbel[1], Orbel[2],
             Orbel[3], Orbel[4], Orbel[5], Orbel[6], &a, &b, &r, &jstat );
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
        Tcl_AppendResult( interp, "unreachable position", (char *) NULL);
        return TCL_ERROR;
    }

/* Compute WFS positions */
    for ( i = 0; i < 3; i++ ) {
        if ( TargetSet[i + 1] ) {
            if ( astCoco( Theta1[i + 1], Theta2[i + 1], Pm[i + 1], 
                System[i + 1], Equinox[i + 1], Epoch[i + 1], System[i + 1],
                Equinox[i + 1], tt, aoprms, ctx.tel, &a, &b ) ) {
                Tcl_AppendResult( interp, "uncomputable WFS target",
                    (char *) NULL);
                return TCL_ERROR;
            }
            if ( astCtx2tr( ctx, System[i + 1], Equinox[i + 1], Wavel[i + 1], 0,
                &wcs, &rawt ) ) {
                Tcl_AppendResult( interp, 
                    "WCS transformation generation failed", (char *) NULL);
                return TCL_ERROR;
            }
            if ( astInvtr( wcs, &iwcs ) ) {
                Tcl_AppendResult( interp, "inverting WCS transformation failed",
                    (char *) NULL);
                return TCL_ERROR;
            }
            if ( astS2xyq( a, b, iwcs, &Wfsx[i], &Wfsy[i] ) ) {
                Tcl_AppendResult( interp, "uncomputable WFS x/y",
                    (char *) NULL);
                return TCL_ERROR;
            }

/* Correct position for optical distortions. */
            tccXy2Probe( &Wfsx[i], &Wfsy[i]);
        }
    }
    

/* Compute Apparent RA/Dec of telescope. */
    if ( astCoco( Theta1[0], Theta2[0], Pm[0], System[0], Equinox[0], Epoch[0],
        APPT, Roteqx, tt, aoprms, ctx.tel, &ra, &dec) != 0 ) {
        Tcl_AppendResult( interp, "unreachable position", (char *) NULL);
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
    sprintf( interp->result, "%02d:%02d:%02d %d", hmsf[0], hmsf[1], hmsf[2],
       Visible);
    return TCL_OK;
}

static int setPo( Tcl_Interp *interp, int po, int argc, char *argv[])
{
    if ( argc != 5 ) {
        Tcl_AppendResult( interp,
            "wrong # args: should be \"tccPreview set item arg1 arg2\"",
            (char *) NULL);
        return TCL_ERROR;
    }
    if ( sscanf( argv[3], "%lf", &Pox[po]) != 1 ) {
        Tcl_AppendResult( interp, "invalid pointing origin \"", argv[3], 
            "\"", (char *) NULL);
        return TCL_ERROR;
    }
    if ( sscanf( argv[4], "%lf", &Poy[po]) != 1 ) {
        Tcl_AppendResult( interp, "invalid pointing origin \"", argv[4], 
            "\"", (char *) NULL);
        return TCL_ERROR;
    }
    return TCL_OK;
}

static int setIaa( Tcl_Interp *interp, int argc, char *argv[])
{
    if ( argc != 4 ) {
        Tcl_AppendResult( interp,
      "wrong # args: should be \"tccPreview set ipa angle\"",
            (char *) NULL);
       return TCL_ERROR;
    }
    if ( sscanf( argv[3], "%lf", &Iaa) != 1 ) {
        Tcl_AppendResult( interp, "invalid alignment angle \"", argv[3], 
            "\"", (char *) NULL);
        return TCL_ERROR;
    }
    Iaa *= D2R;
    return TCL_OK;
}

static int setIpa( Tcl_Interp *interp, int argc, char *argv[])
{
    if ( argc != 6 ) {
        Tcl_AppendResult( interp,
      "wrong # args: should be \"tccPreview set ipa angle frame equinox\"",
            (char *) NULL);
       return TCL_ERROR;
    }
    if ( sscanf( argv[3], "%lf", &Ipa) != 1 ) {
        Tcl_AppendResult( interp, "invalid position angle \"", argv[3], 
            "\"", (char *) NULL);
        return TCL_ERROR;
    }
    Ipa *= D2R;
    if ( tccDcFrame( interp, argv[4], &Rotsys) != TCL_OK )
        return TCL_ERROR;
    if ( tccDcEpoch( interp, argv[5], &Roteqx.type, 
            &Roteqx.year ) != TCL_OK ) return TCL_ERROR;
    return TCL_OK;
}

static int setWavel( Tcl_Interp *interp, int target, int argc, char *argv[])
{
    if ( argc != 4 ) {
        Tcl_AppendResult( interp,
            "wrong # args: should be \"tccPreview set wavelength value\"",
            (char *) NULL);
        return TCL_ERROR;
    }
    if ( sscanf( argv[3], "%lf", &Wavel[target]) != 1 ) {
        Tcl_AppendResult( interp, "invalid wavelength \"", argv[3], 
            "\"", (char *) NULL);
        return TCL_ERROR;
    }
    return TCL_OK;
}

static int setLimits( Tcl_Interp *interp, int argc, char *argv[])
{
    if ( argc != 9 ) {
        Tcl_AppendResult( interp,
       "wrong # args: should be \"tccPreview set limits az az el rot rot zd\"",
            (char *) NULL);
        return TCL_ERROR;
    }
    if ( sscanf( argv[3], "%lf", &Azlolim) != 1 ) {
        Tcl_AppendResult( interp, "invalid azimuth limit \"", argv[3], 
            "\"", (char *) NULL);
        return TCL_ERROR;
    }
    if ( sscanf( argv[4], "%lf", &Azhilim) != 1 ) {
        Tcl_AppendResult( interp, "invalid azimuth limit \"", argv[4], 
            "\"", (char *) NULL);
        return TCL_ERROR;
    }
    if ( sscanf( argv[5], "%lf", &Ellim) != 1 ) {
        Tcl_AppendResult( interp, "invalid elevation limit \"", argv[5], 
            "\"", (char *) NULL);
        return TCL_ERROR;
    }
    if ( sscanf( argv[6], "%lf", &Rotlolim) != 1 ) {
        Tcl_AppendResult( interp, "invalid rotator limit \"", argv[6], 
            "\"", (char *) NULL);
        return TCL_ERROR;
    }
    if ( sscanf( argv[7], "%lf", &Rothilim) != 1 ) {
        Tcl_AppendResult( interp, "invalid rotator limit \"", argv[7], 
            "\"", (char *) NULL);
        return TCL_ERROR;
    }
    if ( sscanf( argv[8], "%lf", &Zlim) != 1 ) {
        Tcl_AppendResult( interp, "invalid zenith limit \"", argv[8], 
            "\"", (char *) NULL);
        return TCL_ERROR;
    }
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
    if ( TargetSet[target] ) {
        sprintf( interp->result, "%.3f %.3f", Wfsx[target - 1], 
            Wfsy[target - 1]);
    }
    return TCL_OK;
}

static int getMount( Tcl_Interp *interp)
{
    int dmsf1[4], dmsf2[4], dmsf3[4];
    char sign1, sign2, sign3;

    slaDr2af( 0, Az1, &sign1, dmsf1);
    slaDr2af( 0, Az2, &sign2, dmsf2);
    slaDr2af( 0, El, &sign3, dmsf3);
    sprintf( interp->result, 
        "%c%02d:%02d:%02d %c%02d:%02d:%02d %c%02d:%02d:%02d", 
        sign1, dmsf1[0], dmsf1[1], dmsf1[2], 
        sign2, dmsf2[0], dmsf2[1], dmsf2[2],
        sign3, dmsf3[0], dmsf3[1], dmsf3[2]);
    return TCL_OK;
}

static int getSource( Tcl_Interp *interp)
{
    int hmsf0[4];
    char sign0;

    slaDr2tf( 0, HA, &sign0, hmsf0);
    sprintf( interp->result, "%c%02d:%02d:%02d %.1f %.2f", sign0, hmsf0[0], 
        hmsf0[1], hmsf0[2], ZD/D2R, Airmass);
    return TCL_OK;
}

static int getTarget( Tcl_Interp *interp, int i)
{
    int hmsf[4], dmsf[4];
    char sign1, sign2;

    if ( TargetSet[i] ) {
       slaDr2tf( 3, Theta1[i], &sign1, hmsf);
       slaDr2af( 2, Theta2[i], &sign2, dmsf);
       sprintf( interp->result, 
           "%02d:%02d:%02d.%03d %c%02d:%02d:%02d.%02d %c%f",
           hmsf[0], hmsf[1], hmsf[2], hmsf[3], sign2, dmsf[0], dmsf[1], 
           dmsf[2], dmsf[3], Equinox[i].type, Equinox[i].year);
    }
    return TCL_OK;
}

static int getRotator( Tcl_Interp *interp)
{
    sprintf( interp->result, "%6.2f %6.2f", Rma1 / D2R, Rma2 / D2R);
    return TCL_OK;
}

static int getLimits( Tcl_Interp *interp)
{
    char val[5];
    if ( Azttl1 >= 0.0 ) {
        sprintf( val, "%4d", (int) floor(Azttl1));
        Tcl_AppendResult( interp, val, (char*) NULL);
    } else {
        Tcl_AppendResult( interp, "{}", (char*) NULL);
    }
    if ( Azttl2 >= 0.0 ) {
        sprintf( val, " %4d", (int) floor(Azttl2));
        Tcl_AppendResult( interp, val, (char*) NULL);
    } else {
        Tcl_AppendResult( interp, " {}", (char*) NULL);
    }
    if ( Elttl >= 0.0 ) {
        sprintf( val, " %4d", (int) floor(Elttl));
        Tcl_AppendResult( interp, val, (char*) NULL);
    } else {
        Tcl_AppendResult( interp, " {}", (char*) NULL);
    }
    if ( Rotttl1 >= 0.0 ) {
        sprintf( val, " %4d", (int) floor(Rotttl1));
        Tcl_AppendResult( interp, val, (char*) NULL);
    } else {
        Tcl_AppendResult( interp, " {}", (char*) NULL);
    }
    if ( Rotttl2 >= 0.0 ) {
        sprintf( val, " %4d", (int) floor(Rotttl2));
        Tcl_AppendResult( interp, val, (char*) NULL);
    } else {
        Tcl_AppendResult( interp, " {}", (char*) NULL);
    }
    if ( Zttl >= 0.0 ) {
        sprintf( val, " %4d", (int) floor(Zttl));
        Tcl_AppendResult( interp, val, (char*) NULL);
    } else {
        Tcl_AppendResult( interp, " {}", (char*) NULL);
    }
    return TCL_OK;
}

static int setTarget( Tcl_Interp *interp, int target, int argc,
    char *argv[])
{
    if ( argc < 6 ) {
        Tcl_AppendResult( interp,
  "wrong # args: should be \"tccPreview set item arg1 arg2 arg3 ?arg ...?\"",
            (char *) NULL);
        return TCL_ERROR;
    }
    if ( tccDcFrame( interp, argv[3], &System[target]) != TCL_OK )
        return TCL_ERROR;
    if ( tccDcRadec( interp, System[target], argv[4], argv[5], 
        &Theta1[target], &Theta2[target]) != TCL_OK ) return TCL_ERROR;
    if ( argc > 6 ) {
        if ( tccDcEpoch( interp, argv[6], &Equinox[target].type, 
            &Equinox[target].year ) != TCL_OK ) return TCL_ERROR;
    } else {
        Equinox[target].type = 'J';
        Equinox[target].year = 2000.0;
    }
    if ( argc > 7 ) {
        if ( tccDcEpoch( interp, argv[7], &Epoch[target].type, 
            &Epoch[target].year ) != TCL_OK ) return TCL_ERROR;
    } else {
        Epoch[target].type = 'J';
        Epoch[target].year = 2000.0;
    }
    if ( argc > 8 ) {
        if ( *argv[8] == '\0' ) {
            Pm[target].pm = 0;
        } else {
            if ( sscanf( argv[8], "%lf %fl", &Pm[target].pmRA, \
                &Pm[target].pmDec ) != 2 ) {
                Tcl_AppendResult( interp, "invalid proper motion \"", argv[8], 
                    "\"", (char *) NULL);
                return TCL_ERROR;
            }
            Pm[target].pm = 1;
        }
    }
    Pm[target].px = 0.0;
    Pm[target].rv = 0.0;
    if ( argc > 9 ) {
        if ( sscanf( argv[9], "%lf", &Pm[target].px) != 1 ) {
            Tcl_AppendResult( interp, "invalid parallax \"", argv[9], "\"", 
                (char *) NULL);
            return TCL_ERROR;
        }
    }
    if ( argc > 10 ) {
        if ( sscanf( argv[10], "%lf", &Pm[target].rv) != 1 ) {
            Tcl_AppendResult( interp, "invalid radial velocity \"", argv[10],
                "\"", (char *) NULL);
            return TCL_ERROR;
        }
    }
    TargetSet[target] = COORD;
    return TCL_OK;
}

static int setPlanet( Tcl_Interp *interp, int argc, char *argv[])
{
    if ( argc != 4 ) {
        Tcl_AppendResult( interp,
      "wrong # args: should be \"tccPreview set planet planet\"",
            (char *) NULL);
       return TCL_ERROR;
    }
    if ( tccDcPlanet( interp, argv[3], &Planet ) ) return TCL_ERROR;
    TargetSet[0] = PLANET;
    return TCL_OK;
}

static int setOrbit( Tcl_Interp *interp, int argc, char *argv[])
{
    int i, nel;

    if ( argc < 4 ) {
        Tcl_AppendResult( interp,
      "wrong # args: should be \"tccPreview set orbit jtype elements\"",
            (char *) NULL);
       return TCL_ERROR;
    }
    if ( sscanf( argv[3], "%d", &Jtype) != 1 ) {
        Tcl_AppendResult( interp, "invalid orbit element set", (char *) NULL);
        return TCL_ERROR;
    }
    if ( Jtype == 1 ) {
      if ( argc != 12 ) {
         Tcl_AppendResult(interp, 
            "wrong number of orbital elements: 7 expected", (char *) NULL);
         return TCL_ERROR;
      }
      nel = 7;
    } else if ( Jtype == 2 ) {
      if ( argc != 11 ) {
         Tcl_AppendResult(interp, 
            "wrong number of orbital elements: 6 expected", (char *) NULL);
         return TCL_ERROR;
      }
      nel = 6;
    } else if ( Jtype == 3 ) {
      if ( argc != 10 ) {
         Tcl_AppendResult(interp, 
            "wrong number of oribital elements: 5 expected", (char *) NULL);
         return TCL_ERROR;
      }
      nel = 5;
    } else {
       Tcl_AppendResult( interp, "invalid orbital element set \"", argv[3], 
            "\"", (char *) NULL);
       return TCL_ERROR;
    }
    if ( tccDcT0( interp, argv[4], &T0 ) != TCL_OK) return TCL_ERROR;
    for ( i = 0; i < nel; i++ ) {
        if ( sscanf( argv[i+5], "%lf", &Orbel[i]) != 1 ) {
            Tcl_AppendResult( interp, "invalid orbital element \"", argv[i],
                "\"", (char *) NULL);
            return TCL_ERROR;
        }
    }
    TargetSet[0] = ORBIT;
    return TCL_OK;
}

static int update( Tcl_Interp *interp, char *arrayname )
{
    char *val;
    double tlongm, tlatm, hm, tdc, pmb, rh, tlr, delut;
    double date;
    int y, m, d, status;
    char cmd[80];

/* Copy data from tcl array. */
    val = Tcl_GetVar2( interp, arrayname, "tlongm", TCL_LEAVE_ERR_MSG);
    if ( !val ) return TCL_ERROR;
    if ( sscanf( val, "%lf", &tlongm) != 1 ) {
        Tcl_AppendResult( interp, "invalid longitude \"", val, "\"",
            (char *) NULL);
        return TCL_ERROR;
    }

    val = Tcl_GetVar2( interp, arrayname, "tlatm", TCL_LEAVE_ERR_MSG);
    if ( !val ) return TCL_ERROR;
    if ( sscanf( val, "%lf", &tlatm) != 1 ) {
        Tcl_AppendResult( interp, "invalid latitude \"", val, "\"",
            (char *) NULL);
        return TCL_ERROR;
    }

    val = Tcl_GetVar2( interp, arrayname, "hm", TCL_LEAVE_ERR_MSG);
    if ( !val ) return TCL_ERROR;
    if ( sscanf( val, "%lf", &hm) != 1 ) {
        Tcl_AppendResult( interp, "invalid height \"", val, "\"",
            (char *) NULL);
        return TCL_ERROR;
    }

    val = Tcl_GetVar2( interp, arrayname, "xpm", TCL_LEAVE_ERR_MSG);
    if ( !val ) return TCL_ERROR;
    if ( sscanf( val, "%lf", &Xpmr) != 1 ) {
        Tcl_AppendResult( interp, "invalid X polar motion \"", val, "\"",
            (char *) NULL);
        return TCL_ERROR;
    }
    Xpmr *= AS2R;

    val = Tcl_GetVar2( interp, arrayname, "ypm", TCL_LEAVE_ERR_MSG);
    if ( !val ) return TCL_ERROR;
    if ( sscanf( val, "%lf", &Ypmr) != 1 ) {
        Tcl_AppendResult( interp, "invalid Y polar motion \"", val, "\"",
            (char *) NULL);
        return TCL_ERROR;
    }
    Ypmr *= AS2R;

    val = Tcl_GetVar2( interp, arrayname, "tdc", TCL_LEAVE_ERR_MSG);
    if ( !val ) return TCL_ERROR;
    if ( sscanf( val, "%lf", &tdc) != 1 ) {
        Tcl_AppendResult( interp, "invalid temperature \"", val, "\"",
            (char *) NULL);
        return TCL_ERROR;
    }

    val = Tcl_GetVar2( interp, arrayname, "pmb", TCL_LEAVE_ERR_MSG);
    if ( !val ) return TCL_ERROR;
    if ( sscanf( val, "%lf", &pmb) != 1 ) {
        Tcl_AppendResult( interp, "invalid pressure \"", val, "\"",
            (char *) NULL);
        return TCL_ERROR;
    }

    val = Tcl_GetVar2( interp, arrayname, "rh", TCL_LEAVE_ERR_MSG);
    if ( !val ) return TCL_ERROR;
    if ( sscanf( val, "%lf", &rh) != 1 ) {
        Tcl_AppendResult( interp, "invalid relative humidiy \"", val, "\"",
            (char *) NULL);
        return TCL_ERROR;
    }

    val = Tcl_GetVar2( interp, arrayname, "tlr", TCL_LEAVE_ERR_MSG);
    if ( !val ) return TCL_ERROR;
    if ( sscanf( val, "%lf", &tlr) != 1 ) {
        Tcl_AppendResult( interp, "invalid tropospheric lapse rate \"", val, "\"",
            (char *) NULL);
        return TCL_ERROR;
    }

    val = Tcl_GetVar2( interp, arrayname, "fl", TCL_LEAVE_ERR_MSG);
    if ( !val ) return TCL_ERROR;
    if ( sscanf( val, "%lf", &Telp.fl) != 1 ) {
        Tcl_AppendResult( interp, "invalid focal length \"", val, "\"",
            (char *) NULL);
        return TCL_ERROR;
    }

    val = Tcl_GetVar2( interp, arrayname, "an", TCL_LEAVE_ERR_MSG);
    if ( !val ) return TCL_ERROR;
    if ( sscanf( val, "%lf", &Telp.an) != 1 ) {
        Tcl_AppendResult( interp, "invalid axis tilt north \"", val, "\"",
            (char *) NULL);
        return TCL_ERROR;
    }
    Telp.an *= AS2R;

    val = Tcl_GetVar2( interp, arrayname, "aw", TCL_LEAVE_ERR_MSG);
    if ( !val ) return TCL_ERROR;
    if ( sscanf( val, "%lf", &Telp.aw) != 1 ) {
        Tcl_AppendResult( interp, "invalid axis tilt west \"", val, "\"",
            (char *) NULL);
        return TCL_ERROR;
    }
    Telp.aw *= AS2R;

    val = Tcl_GetVar2( interp, arrayname, "pnpae", TCL_LEAVE_ERR_MSG);
    if ( !val ) return TCL_ERROR;
    if ( sscanf( val, "%lf", &Telp.pnpae) != 1 ) {
        Tcl_AppendResult( interp, "invalid non-perpendicularity \"", val, "\"",
            (char *) NULL);
        return TCL_ERROR;
    }
    Telp.pnpae *= AS2R;

    val = Tcl_GetVar2( interp, arrayname, "ca", TCL_LEAVE_ERR_MSG);
    if ( !val ) return TCL_ERROR;
    if ( sscanf( val, "%lf", &Telp.ca) != 1 ) {
        Tcl_AppendResult( interp, "invalid collimation error azimuth \"",
            val, "\"", (char *) NULL);
        return TCL_ERROR;
    }
    Telp.ca *= AS2R;

    val = Tcl_GetVar2( interp, arrayname, "ce", TCL_LEAVE_ERR_MSG);
    if ( !val ) return TCL_ERROR;
    if ( sscanf( val, "%lf", &Telp.ce) != 1 ) {
        Tcl_AppendResult( interp, "invalid collimation error elevation \"",
            val, "\"", (char *) NULL);
        return TCL_ERROR;
    }
    Telp.ce *= AS2R;

    val = Tcl_GetVar2( interp, arrayname, "delut", TCL_LEAVE_ERR_MSG);
    if ( !val ) return TCL_ERROR;
    if ( sscanf( val, "%lf", &delut) != 1 ) {
        Tcl_AppendResult( interp, "invalid UT1-UTC\"",
            val, "\"", (char *) NULL);
        return TCL_ERROR;
    }

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
    double azha1, azha2, azt1[2], azt2[2];
    double rotha1, rotha2, rott1[2], rott2[2];
    double x, y, a, r;
    double sqsz, cqsz;
    int i, j;
    int azlovalid[2], azhivalid[2], azhidone[2], azlodone[2];
    int rotlovalid[2], rothivalid[2], rothidone[2], rotlodone[2];

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
    azlovalid[0] = azlovalid[1] = (tccLimAz( cdec, sdec, Azlolim, clat,
		slat, &azha1) == 0);
    azhivalid[0] = azhivalid[1] = (tccLimAz( cdec, sdec, Azhilim, clat,
		slat, &azha2) == 0);

/* Hour angles at which rotator limits (adjusted for the desired
    rotator angle) are reached. */
    rotlovalid[0] = rotlovalid[1] = (tccLimPa( cdec, sdec,
        - PI - Rotlolim + Ipa - Iaa, clat, slat, &rotha1) == 0);
    rothivalid[0] = rothivalid[1] = (tccLimPa( cdec, sdec,
        - PI - Rothilim + Ipa - Iaa, clat, slat, &rotha2) == 0);

/* Adjust limit hour angles to be greater than current. */
    if ( azha1 < HA ) azha1 += PI2;
    if ( azha2 < HA ) azha2 += PI2;
    if ( rotha1 < HA ) rotha1 += PI2;
    if ( rotha2 < HA ) rotha2 += PI2;

/* Step the azimuth forward in 6 hour increments up to the hour
   angle of the limit. */
    az[0] = Az1;
    az[1] = Az2;
    rma[0] = Rma1;
    rma[1] = Rma2;
    a = slaDrange( Az1 );
    r = slaDrange( Rma1 );
    ha = floor(HA/D90) * D90;
    for ( i = 0; i < 2; i++ ) {
        azlodone[i] = azhidone[i] = 0;
        rotlodone[i] = rothidone[i] = 0;
    }
    for ( j = 0; j < 6; j++)  {
        ha += D90;
 
    /* Azimuth at this hour angle. */
        cha = cos( ha );
        sha = sin( ha );
        x = - cha * cdec * slat + sdec * clat;
        y = - sha * cdec;
        a = ( x == 0.0 && y == 0.0) ? a : atan2( y, x);
        for ( i = 0; i < 2; i++ ) {
            az[i] += slaDrange( a - az[i]);

        /* Is the hour angle greater than any of the limits? */
            if ( azlovalid[i] && !azlodone[i] && azha1 < ha ) {
                azlodone[i] = 1;

            /* Is the azimuth close to the limit? */
                if ( fabs( az[i] - Azlolim ) > PI ) azlovalid[i] = 0;
                else azt1[i] = azha1 - HA;
            }

        /* Repeat for the high limit. */
            if ( azhivalid[i] && !azhidone[i] && azha2 < ha ) {
                azhidone[i] = 1;
                if ( fabs( az[i] - Azhilim ) > PI ) azhivalid[i] = 0;
                else azt2[i] = azha2 - HA;
            }
        }

    /* Rotator angle at this hour angle. */
        sqsz = clat * sha;
        cqsz = slat * cdec - clat * sdec * cha;
        r = ( sqsz != 0.0 || cqsz != 0.0 ) ? atan2 ( -sqsz, cqsz ) : r;
        r += PI + (Ipa - Iaa);
        for ( i = 0; i < 2; i++ ) {
            rma[i] += slaDrange( r - rma[i]);

        /* Is the hour angle greater than any of the limits? */
            if ( rotlovalid[i] && !rotlodone[i] && rotha1 < ha ) {
                rotlodone[i] = 1;
    
            /* Is the rotator angle close to the limit? */
                if ( fabs( rma[i] - Rotlolim ) > PI ) rotlovalid[i] = 0;
                else rott1[i] = rotha1 - HA;
            }

        /* Repeat for the high limit. */
            if ( rothivalid[i] && !rothidone[i] && rotha2 < ha ) {
                rothidone[i] = 1;
                if ( fabs( rma[i] - Rothilim ) > PI ) rothivalid[i] = 0;
                else rott2[i] = rotha2 - HA;
            }
        }
    }
    for ( i = 0; i < 2; i++ ) {
        if (!azlodone[i]) azlovalid[i] = 0;
        if (!azhidone[i]) azhivalid[i] = 0;
        if (!rotlodone[i]) rotlovalid[i] = 0;
        if (!rothidone[i]) rothivalid[i] = 0;
    }

/* Copy nearest azimuth limit to outputs. */
    if (azlovalid[0] && azhivalid[0]) {
        Azttl1 = (azt1[0] < azt2[0] ? azt1[0] : azt2[0]) * ST2MIN;
    } else if (azlovalid[0]) {
        Azttl1 = azt1[0] * ST2MIN;
    } else if (azhivalid[0]) {
        Azttl1 = azt2[0] * ST2MIN;
    } else {
        Azttl1 = -1.0;
    }
    if (azlovalid[1] && azhivalid[1]) {
        Azttl2 = (azt1[1] < azt2[1] ? azt1[1] : azt2[1]) * ST2MIN;
    } else if (azlovalid[1]) {
        Azttl2 = azt1[1] * ST2MIN;
    } else if (azhivalid[1]) {
        Azttl2 = azt2[1] * ST2MIN;
    } else {
        Azttl2 = -1.0;
    }


/* Copy nearest rotator limit to outputs. */
    if (Rotsys != AZEL_MNT) {
        if (rotlovalid[0] && rothivalid[0]) {
            Rotttl1 = (rott1[0] < rott2[0] ? rott1[0] : rott2[0]) * ST2MIN;
        } else if (rotlovalid[0]) {
            Rotttl1 = rott1[0] * ST2MIN;
        } else if (rothivalid[0]) {
            Rotttl1 = rott2[0] * ST2MIN;
        } else {
            Rotttl1 = -1.0;
        }
        if (rotlovalid[1] && rothivalid[1]) {
            Rotttl2 = (rott1[1] < rott2[1] ? rott1[1] : rott2[1]) * ST2MIN;
        } else if (rotlovalid[1]) {
            Rotttl2 = rott1[1] * ST2MIN;
        } else if (rothivalid[1]) {
            Rotttl2 = rott2[1] * ST2MIN;
        } else {
            Rotttl2 = -1.0;
        }
    } else {
        Rotttl1 = -1.0;
        Rotttl2 = -1.0;
    }

/* Zenith at risk area limit */
   if ( Zlim < fabs( dec - tlat )  || HA > Zlim ) {
       Zttl = -1.0;
   } else {
     Zttl = ( - Zlim - HA ) * ST2MIN;
   }

   return TCL_OK;
}
