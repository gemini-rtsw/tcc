static char rcsid[]="$Id:";
/* *INDENT-OFF* */
/*
*   FILENAME
*   tccAst.c
*
*   FUNCTION NAME(S)
*   astCmd - Implements the ast tcl command.
*
*   D L Terrett 6 June 1999
*
*   Copyright CCLRC
*/
/*
 * $Log:
 */
/* *INDENT-ON* */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tcl.h>
#include <slalib.h>
#include <astLib.h>
#include <timeLib.h>
#include "tccConstants.h"
#include "tccUtil.h"

/* Default wavelength */
#define WAVEL 0.5

static int update( Tcl_Interp * );
static int target( Tcl_Interp *, int, char *[] );
static int instrument( Tcl_Interp *, int, char *[] );
static int format( Tcl_Interp *, int, char *[] );

/* *INDENT-OFF* */
/*+
 *   Function name:
 *   Tccext_AstCmd
 *
 *   Purpose:
 *   Implements the ast tcl command
 *
 *   Description:
 *-
 */
/* *INDENT-ON* */

int Tccext_AstCmd( ClientData clientdata, Tcl_Interp *interp, int argc,
    char *argv[])
{
    if ( argc < 2 ) {
        Tcl_AppendResult( interp,
           "wrong # args: should be \"ast option ?arg ...?\"",
            (char *) NULL);
        return TCL_ERROR;
    }

    else if ( strcmp( argv[1], "update" ) == 0 ) {
        if ( argc != 2 ) {
            Tcl_AppendResult( interp,
                "wrong # args: should be \"ast update\"", (char *) NULL);
            return TCL_ERROR;
        }
        return update( interp );
    }

    else if ( strcmp( argv[1], "target" ) == 0 ) {
        if ( argc != 7 ) {
            Tcl_AppendResult( interp,
 "wrong # args: should be \"ast target origin chop wavelength frame equinox\"",
               (char *) NULL);
            return TCL_ERROR;
        }
        return target( interp, argc, argv );
    } 

    else if ( strcmp( argv[1], "format" ) == 0 ) {
        if ( argc != 6 ) {
            Tcl_AppendResult( interp,
 "wrong # args: should be \"ast format theta1 theta2 frame equinox\"",
               (char *) NULL);
            return TCL_ERROR;
        }
        return format( interp, argc, argv );
    } 

    else if ( strcmp( argv[1], "instrument" ) == 0 ) {
        if ( argc != 3 ) {
            Tcl_AppendResult( interp,
                  "wrong # args: should be \"ast instrument origin\"",
               (char *) NULL);
            return TCL_ERROR;
        }
        return instrument( interp, argc, argv );
    } 

    else {
        Tcl_AppendResult( interp, "unknown option \"", argv[1],
            "\" must be update, format or target", (char *) NULL);
        return TCL_ERROR;
    }
}

static int update( Tcl_Interp *interp )
{
    double ctxa[AST_CTXA_SIZE];
    int i, nel;
    char **listPtr;

/* Get the ast context from the tcs SAD. */
    if ( Tcl_Eval( interp, "sa tcssad get astCtx value" ) != TCL_OK )
        return TCL_ERROR;

/* Decode the result. */
    if ( Tcl_SplitList( interp, interp->result, &nel, &listPtr) != TCL_OK )
        return TCL_ERROR;
    if ( nel != AST_CTXA_SIZE ) {
        Tcl_Free( (char *) listPtr );
        strcpy( interp->result, "ast context array has wrong size!" );
        return TCL_ERROR;
    }
    for ( i = 0; i < AST_CTXA_SIZE; i++ ) {
        if ( Tcl_GetDouble( interp, listPtr[i], &ctxa[i] ) != TCL_OK ) {
            Tcl_Free( (char *) listPtr );
            return TCL_ERROR;
        }
    }

/* Free the list allocated by Tcl_SplitList. */
    Tcl_Free( (char *) listPtr );

/* Set the astlib context. */
    astSetctx( ctxa );

/* Return the Gemini raw time. */
    Tcl_ResetResult( interp );
    sprintf( interp->result, "%f", ctxa[0] );

    return TCL_OK;
}

static int target( Tcl_Interp *interp, int argc, char *argv[] )
{
    struct PO po;
    int chop;
    double x, y;
    double wavel;
    FRAMETYPE frame;
    struct EPOCH equinox;
    struct WCS_CTX ctx;
    double lst, a, b, ha, zd, am;
    int dmsf[4], hmsf[4], hahmsf[4];
    char sign, hasign;

/* Focal plane X/Y. */
    astGetpo ( &po );
    switch ( *argv[2] ) {
        case 'M':
        case 'm': x = po.mx; y = po.my; break;
        case 'A':
        case 'a': x = po.ax; y = po.ay; break;
        case 'B':
        case 'b': x = po.bx; y = po.by; break;
        case 'C':
        case 'c': x = po.cx; y = po.cy; break;
        default:
            Tcl_AppendResult( interp, "\"", argv[2], 
               "\" is not a valid pointing origin", (char *) NULL );
            return TCL_ERROR;
    }

/* Chop state. */
    switch ( *argv[3] ) {
        case 'A':
        case 'a': chop = 0; break;
        case 'B':
        case 'b': chop = 1; break;
        case 'C':
        case 'c': chop = 2; break;
        default:
            Tcl_AppendResult( interp, "\"", argv[3], 
               "\" is not a valid chop state", (char *) NULL );
            return TCL_ERROR;
    }

/* Wavelength */
    if ( *argv[4] == NULL ) {
        wavel = WAVEL;
    } else {
        if ( Tcl_GetDouble( interp, argv[4], &wavel ) != TCL_OK )
            return TCL_ERROR;
        if ( wavel == 0.0 ) wavel = WAVEL;
    }

/* Frame */
    if ( tccDcFrame( interp, argv[5], &frame ) != TCL_OK )
        return TCL_ERROR;

/* Equinox */
    if ( frame == FK4 || frame == FK5 ) {
         if ( tccDcEpoch( interp, argv[6], &equinox.type, &equinox.year ) != 
             TCL_OK ) return TCL_ERROR;
    }

/* Convert X/Y to celestal position. */
    if ( astXy2s( x, y, frame, equinox, wavel, chop, &a, &b ) ) {
        Tcl_AppendResult( interp, "{} {}", (char *) NULL );
    } else {

/* Format the result */
        a = slaDranrm( a );
        b = slaDrange( b );
        if ( frame == AZEL_TOPO ) {

/* Get ZD and airmass */
            zd = D90 - b;
            am = slaAirmas( zd );
            sprintf( interp->result, "%.4f %.4f {} %.1f %.2f", a / D2R, 
               b / D2R, zd / D2R, am );
        } else {

/* Get the hour angle, ZD and airmass. */
            astGetctx( &ctx );
            timeThenR( ctx.time, LAST, &lst );
            ha = slaDrange( lst - a ); 
            slaDr2tf( 3, ha, &hasign, hahmsf );
            zd = slaZd( ha, b, ctx.aoprms[0] );
            am = slaAirmas( zd );

            slaDr2tf( 3, a, &sign, hmsf );
            if ( hmsf[0] == 24 ) hmsf[0] = 0;
            slaDr2af( 2, b, &sign, dmsf );
            sprintf( interp->result, 
  "%2.2d:%2.2d:%2.2d.%.3d %c%2.2d:%2.2d:%2.2d.%2.2d %c%2.2d:%2.2d %.1f %.2f",
                hmsf[0], hmsf[1], hmsf[2], hmsf[3], sign, dmsf[0], dmsf[1],
                dmsf[2], dmsf[3], hasign, hahmsf[0], hahmsf[1], zd / D2R, am);
        }
    }

    return TCL_OK;
}

static int format( Tcl_Interp *interp, int argc, char *argv[] )
{
    double theta1, theta2;
    FRAMETYPE frame;
    struct EPOCH equinox;
    int hmsf[4], dmsf[4];
    char sign;

/* Decode input arguments... theta1 & 2 */
    if ( Tcl_GetDouble( interp, argv[2], &theta1 ) != TCL_OK )
        return TCL_ERROR;
    if ( Tcl_GetDouble( interp, argv[3], &theta2 ) != TCL_OK )
        return TCL_ERROR;

/* Frame */
    if ( tccDcFrame( interp, argv[4], &frame ) != TCL_OK )
        return TCL_ERROR;

/* Equinox */
    if ( frame == FK4 || frame == FK5 ) {
         if ( tccDcEpoch( interp, argv[5], &equinox.type, &equinox.year ) != 
             TCL_OK ) return TCL_ERROR;
    }

/* Format the result */
    if ( frame == AZEL_TOPO ) {
        sprintf( interp->result, "%.3f %.3f {}", theta1, theta2);
    } else {
        slaDr2tf( 3, slaDranrm( theta1 * D2R ), &sign, hmsf);
        if ( hmsf[0] == 24 ) hmsf[0] = 0;
        slaDr2af( 2, slaDrange( theta2 * D2R ), &sign, dmsf);
        if ( frame == APPT ) {
            sprintf( interp->result, 
          "%2.2d:%2.2d:%2.2d.%3.3d %c%2.2d:%2.2d:%2.2d.%2.2d {Apparent place}", 
                hmsf[0], hmsf[1], hmsf[2], hmsf[3], sign, dmsf[0], dmsf[1],
                dmsf[2], dmsf[3]);
        } else if ( frame == FK5 ) {
            sprintf( interp->result, 
                "%2.2d:%2.2d:%2.2d.%3.3d %c%2.2d:%2.2d:%2.2d.%2.2d FK5/%c%.1f", 
                hmsf[0], hmsf[1], hmsf[2], hmsf[3], sign, dmsf[0], dmsf[1],
                dmsf[2], dmsf[3], equinox.type, equinox.year);
        } else {
            sprintf( interp->result, 
                "%2.2d:%2.2d:%2.2d.%3.3d %c%2.2d:%2.2d:%2.2d.%2.2d FK4/%c%.1f", 
                hmsf[0], hmsf[1], hmsf[2], hmsf[3], sign, dmsf[0], dmsf[1],
                dmsf[2], dmsf[3], equinox.type, equinox.year);
        }
    }
    return TCL_OK;
}

static int instrument( Tcl_Interp *interp, int argc, char *argv[] )
{
    struct PO po;
    double x, y;
    double wavel;

/* Focal plane X/Y. */
    astGetpo ( &po );
    switch ( *argv[2] ) {
        case 'M':
        case 'm': x = po.mx; y = po.my; break;
        case 'A':
        case 'a': x = po.ax; y = po.ay; break;
        case 'B':
        case 'b': x = po.bx; y = po.by; break;
        case 'C':
        case 'c': x = po.cx; y = po.cy; break;
        default:
            Tcl_AppendResult( interp, "\"", argv[2], 
               "\" is not a valid pointing origin", (char *) NULL );
            return TCL_ERROR;
    }

/* Format the result */
    sprintf( interp->result, "%4.2f %4.2f", x, y);

    return TCL_OK;
}
