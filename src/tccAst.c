static char rcsid[]="$Id:";
/* *INDENT-OFF* */
/*
*   FILENAME
*   tccAst.c
*
*   FUNCTION NAME(S)
*   astCmd - Implements the ast tcl command.
*
*   D L Terrett 21 April 2001
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
#include "tccAst.h"
#include "tccDecode.h"

static int update( Tcl_Interp * );
static int target( Tcl_Interp *, int, Tcl_Obj *CONST [] );
static int instrument( Tcl_Interp *, int, Tcl_Obj *CONST [] );
static int format( Tcl_Interp *, int, Tcl_Obj *CONST [] );

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

int Tccext_AstCmd( ClientData clientdata, Tcl_Interp *interp, int objc,
    Tcl_Obj *CONST objv[])
{
    int ind;
    const char* options[] = {"update", "target", "format", "instrument", NULL};

    if ( objc < 2 ) {
        Tcl_WrongNumArgs( interp, 1, objv, "option ?arg ...?");
        return TCL_ERROR;
    }

    if ( Tcl_GetIndexFromObj( interp, objv[1], options, "option", 0, &ind)
        != TCL_OK ) return TCL_ERROR;

    switch ( ind ) {
        case 0:
            if ( objc != 2 ) {
                Tcl_WrongNumArgs( interp, 2, objv, "");
                return TCL_ERROR;
            }
            return update( interp );
        case 1:
            if ( objc != 7 ) {
                Tcl_WrongNumArgs( interp, 2, objv, 
                        "origin chop wavelength frame equinox");
                return TCL_ERROR;
            }
            return target( interp, objc, objv );
        case 2:
            if ( objc != 6 ) {
                Tcl_WrongNumArgs( interp, 2, objv, 
                        "theta1 theta2 frame equinox");
                return TCL_ERROR;
            }
            return format( interp, objc, objv );
        case 3:
            if ( objc != 3 ) {
                Tcl_WrongNumArgs( interp, 2, objv, "origin");
                return TCL_ERROR;
            }
            return instrument( interp, objc, objv );
    }

/* Never reached. */
    return 0;
}

static int update( Tcl_Interp *interp )
{
    double ctxa[AST_CTXA_SIZE];
    int i, nel;
    const char **listPtr;
    Tcl_Obj *result;

/* Get the ast context from the tcs SAD. */
    if ( Tcl_Eval( interp, "sa tcssad get astCtx value" ) != TCL_OK )
        return TCL_ERROR;

/* Decode the result. */
    if ( Tcl_SplitList( interp, interp->result, &nel, &listPtr) != TCL_OK )
        return TCL_ERROR;
    if ( nel != AST_CTXA_SIZE ) {
        Tcl_Free( (char *) listPtr );
        Tcl_SetResult( interp, "ast context array has wrong size", 
             TCL_VOLATILE);
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
    result = Tcl_NewDoubleObj( ctxa[0] );
    Tcl_SetObjResult(interp, result);

    return TCL_OK;
}

static int target( Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[] )
{
    struct PO po;
    const char* pos[] = { "M", "m", "A", "a", "B", "b", "C", "c"};
    int chop, ind;
    const char* chops[] = { "A", "a", "B", "b", "C", "c"};
    double x, y;
    double wavel;
    FRAMETYPE frame;
    struct EPOCH equinox;
    struct WCS_CTX ctx;
    double lst, a, b, ha, zd, am;
    int dmsf[4], hmsf[4], hahmsf[4];
    char sign, hasign, result[43];

/* Focal plane X/Y. */
    astGetpo ( &po );
    if ( Tcl_GetIndexFromObj( interp, objv[2], pos, "pointing origin", 
        TCL_EXACT, &ind) != TCL_OK ) return TCL_ERROR;
    switch ( ind ) {
        case 0:
        case 1: x = po.mx; y = po.my; break;
        case 2:
        case 3: x = po.ax; y = po.ay; break;
        case 4:
        case 5: x = po.bx; y = po.by; break;
        case 6:
        case 7: x = po.cx; y = po.cy; break;
    }

/* Chop state. */
    if ( Tcl_GetIndexFromObj( interp, objv[3], chops, "chop states", 
        TCL_EXACT, &ind) != TCL_OK ) return TCL_ERROR;
    switch ( ind ) {
        case 0:
        case 1: chop = 0; break;
        case 2:
        case 3: chop = 1; break;
        case 4:
        case 5: chop = 2; break;
    }

/* Wavelength */
    if ( Tcl_GetDoubleFromObj( interp, objv[4], &wavel ) != TCL_OK )
            return TCL_ERROR;

/* Frame */
    if ( tccDcFrame( interp, Tcl_GetStringFromObj(objv[5], NULL), &frame ) != 
        TCL_OK ) return TCL_ERROR;

/* Equinox */
    if ( frame == FK4 || frame == FK5 ) {
         if ( tccDcEpoch( interp, Tcl_GetStringFromObj(objv[6], NULL), 
             &equinox.type, &equinox.year ) != TCL_OK ) return TCL_ERROR;
    }

/* Convert X/Y to celestal position. */
    if ( astXy2s( x, y, frame, equinox, wavel, chop, &a, &b ) ) {
        Tcl_SetResult( interp, "{} {}", TCL_VOLATILE );
    } else {

/* Format the result */
        a = slaDranrm( a );
        b = slaDrange( b );
        if ( frame == AZEL_TOPO ) {

/* Get ZD and airmass */
            zd = D90 - b;
            am = slaAirmas( zd );
            sprintf( result, "%9.4f %9.4f {} %4.1f %4.2f", a / D2R, 
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
            sprintf( result, 
  "%2.2d:%2.2d:%2.2d.%.3d %c%2.2d:%2.2d:%2.2d.%2.2d %c%2.2d:%2.2d:%2.2d %4.1f %4.2f",
                hmsf[0], hmsf[1], hmsf[2], hmsf[3], sign, dmsf[0], dmsf[1],
                dmsf[2], dmsf[3], hasign, hahmsf[0], hahmsf[1], hahmsf[2], 
                zd / D2R, am);
        }
        Tcl_SetResult( interp, result, TCL_VOLATILE);
    }

    return TCL_OK;
}

static int format( Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[] )
{
    double theta1, theta2;
    FRAMETYPE frame;
    struct EPOCH equinox;
    int hmsf[4], dmsf[4];
    char sign;
    char result[60];

/* Decode input arguments... theta1 & 2 */
    if ( Tcl_GetDoubleFromObj( interp, objv[2], &theta1 ) != TCL_OK )
        return TCL_ERROR;
    if ( Tcl_GetDoubleFromObj( interp, objv[3], &theta2 ) != TCL_OK )
        return TCL_ERROR;

/* Frame */
    if ( tccDcFrame( interp, Tcl_GetStringFromObj(objv[4], NULL), &frame ) != 
        TCL_OK ) return TCL_ERROR;

/* Equinox */
    if ( frame == FK4 || frame == FK5 ) {
         if ( tccDcEpoch( interp, Tcl_GetStringFromObj(objv[5], NULL), 
             &equinox.type, &equinox.year ) != TCL_OK ) return TCL_ERROR;
    }

/* Format the result */
    if ( frame == AZEL_TOPO ) {
        sprintf( result, "%.3f %.3f {}", theta1, theta2);
    } else {
        slaDr2tf( 3, slaDranrm( theta1 * D2R ), &sign, hmsf);
        if ( hmsf[0] == 24 ) hmsf[0] = 0;
        slaDr2af( 2, slaDrange( theta2 * D2R ), &sign, dmsf);
        if ( frame == APPT ) {
            sprintf( result, 
          "%2.2d:%2.2d:%2.2d.%3.3d %c%2.2d:%2.2d:%2.2d.%2.2d {Apparent place}", 
                hmsf[0], hmsf[1], hmsf[2], hmsf[3], sign, dmsf[0], dmsf[1],
                dmsf[2], dmsf[3]);
        } else if ( frame == FK5 ) {
            sprintf( result, 
                "%2.2d:%2.2d:%2.2d.%3.3d %c%2.2d:%2.2d:%2.2d.%2.2d FK5/%c%.1f", 
                hmsf[0], hmsf[1], hmsf[2], hmsf[3], sign, dmsf[0], dmsf[1],
                dmsf[2], dmsf[3], equinox.type, equinox.year);
        } else {
            sprintf( result, 
                "%2.2d:%2.2d:%2.2d.%3.3d %c%2.2d:%2.2d:%2.2d.%2.2d FK4/%c%.1f", 
                hmsf[0], hmsf[1], hmsf[2], hmsf[3], sign, dmsf[0], dmsf[1],
                dmsf[2], dmsf[3], equinox.type, equinox.year);
        }
    }
    Tcl_SetResult( interp, result, TCL_VOLATILE);
    return TCL_OK;
}

static int instrument( Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[] )
{
    struct PO po;
    const char* pos[] = { "M", "m", "A", "a", "B", "b", "C", "c"};
    int ind;
    double x, y;
    char result[40];

/* Focal plane X/Y. */
    astGetpo ( &po );
    if ( Tcl_GetIndexFromObj( interp, objv[2], pos, "pointing origin", 
        TCL_EXACT, &ind) != TCL_OK ) return TCL_ERROR;
    switch ( ind ) {
        case 0:
        case 1: x = po.mx; y = po.my; break;
        case 2:
        case 3: x = po.ax; y = po.ay; break;
        case 4:
        case 5: x = po.bx; y = po.by; break;
        case 6:
        case 7: x = po.cx; y = po.cy; break;
    }

/* Format the result */
    sprintf( result, "%f %f", x, y);
    Tcl_SetResult( interp, result, TCL_VOLATILE);

    return TCL_OK;
}
