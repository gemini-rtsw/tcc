static char rcsid[]="$Id:";
/* *INDENT-OFF* */
/*
*   FILENAME
*   tccTime.c
*
*   FUNCTION NAME(S)
*   timeCmd - Implements the tccTime tcl command.
*
*   D L Terrett 13 December 2000
*
*   Copyright CCLRC
*/
/*
 * $Log:
 */
/* *INDENT-ON* */

#include <stdio.h>
#include <string.h>
#include <tcl.h>
#include <slalib.h>
#include <timeLib.h>
#include "tccConstants.h"

static int TccTimeCmd( ClientData clientdata, Tcl_Interp *interp, int objc,
    Tcl_Obj *CONST objv[]);
static int tccTimeInit( Tcl_Interp *interp);
static int tccTimeUTC( Tcl_Interp *interp);
static int tccTimeLAST( Tcl_Interp *interp);
static int tccTimeLocalTime( Tcl_Interp *interp, double offset);
static int tccTimeMJD( Tcl_Interp *interp);

static double Rawt;

/* *INDENT-OFF* */
/*+
 *   Function name:
 *   Tcctime_Init
 *
 *   Purpose:
 *   Initialisation function for TCL loadable image.
 *
 *   Description:
 *   Creates the timeInit command.
 *-
 */
/* *INDENT-ON* */

int Tcctime_Init( Tcl_Interp *interp)
{

    Tcl_CreateObjCommand( interp, "tccTime", TccTimeCmd,
        (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);

    return TCL_OK;
}
/* *INDENT-OFF* */
/*+
 *   Function name:
 *   TccTimeCmd
 *
 *   Purpose:
 *   Implements the tcctime tcl command
 *
 *-
 */
/* *INDENT-ON* */

int TccTimeCmd( ClientData clientdata, Tcl_Interp *interp, int objc,
    Tcl_Obj *CONST objv[])
{
    int ind;
    const char *options[] = {
            "localtime", "utc", "last", "init", "mjd", "set", NULL
    };
    double offset;

    if ( objc == 1 ) {
        Tcl_WrongNumArgs( interp, 1, objv, "option ?args ...?" );
        return TCL_ERROR;
    }
    if ( Tcl_GetIndexFromObj( interp, objv[1], options, "option", 0, &ind) 
        != TCL_OK ) return TCL_ERROR;

    switch ( ind ) {
        case 0:
            if ( objc != 3 ) {
                Tcl_WrongNumArgs( interp, 2, objv, 
                      "offset" );
                return TCL_ERROR;
            }
            if ( Tcl_GetDoubleFromObj( interp, objv[2], &offset) != TCL_OK ) 
                   return TCL_ERROR;
            return tccTimeLocalTime( interp, offset );
        case 1:
            if ( objc != 2 ) {
                Tcl_WrongNumArgs( interp, 1, objv, "utc" );
                return TCL_ERROR;
            }
            return tccTimeUTC( interp );
        case 2:
            if ( objc != 2 ) {
                Tcl_WrongNumArgs( interp, 1, objv, "last" );
                return TCL_ERROR;
            }
            return tccTimeLAST( interp );
        case 3:
            if ( objc != 2 ) {
                Tcl_WrongNumArgs( interp, 1, objv, "init" );
                return TCL_ERROR;
            }
            return tccTimeInit( interp );
        case 4:
            if ( objc != 2 ) {
                Tcl_WrongNumArgs( interp, 1, objv, "mjd" );
                return TCL_ERROR;
            }
            return tccTimeMJD( interp );
        case 5:
            if ( objc != 3 ) {
                Tcl_WrongNumArgs( interp, 2, objv, "time" );
                return TCL_ERROR;
            }
            if (Tcl_GetDoubleFromObj( interp, objv[2], &Rawt) != TCL_OK )
                return TCL_ERROR;
            return TCL_OK;
    }
    return TCL_OK;
}

int tccTimeInit( Tcl_Interp *interp)
{
    double latm, longm, hm, djmls, delat, delut, xpm, ypm;
    int status;
    char cmd[80];
    int y, m, d;
    double elat, elong, daz, date;

    if ( Tcl_Eval( interp, "sa tcssad get tlongm value" ) != TCL_OK )
        return TCL_ERROR;
    if ( Tcl_GetDouble( interp, interp->result, &longm) != TCL_OK )
        return TCL_ERROR;
    if ( Tcl_Eval( interp, "sa tcssad get tlatm value" ) != TCL_OK )
        return TCL_ERROR;
    if ( Tcl_GetDouble( interp, interp->result, &latm) != TCL_OK )
        return TCL_ERROR;
    if ( Tcl_Eval( interp, "sa tcssad get height value" ) != TCL_OK )
        return TCL_ERROR;
    if ( Tcl_GetDouble( interp, interp->result, &hm) != TCL_OK )
        return TCL_ERROR;
    if ( Tcl_Eval( interp, "sa tcssad get djmls value" ) != TCL_OK )
        return TCL_ERROR;
    if ( Tcl_GetDouble( interp, interp->result, &djmls) != TCL_OK )
        return TCL_ERROR;
    if ( Tcl_Eval( interp, "sa tcssad get delat value" ) != TCL_OK )
        return TCL_ERROR;
    if ( Tcl_GetDouble( interp, interp->result, &delat) != TCL_OK )
        return TCL_ERROR;
    if ( Tcl_Eval( interp, "sa tcssad get delut value" ) != TCL_OK )
        return TCL_ERROR;
    if ( Tcl_GetDouble( interp, interp->result, &delut) != TCL_OK )
        return TCL_ERROR;
    if ( Tcl_Eval( interp, "sa tcssad get xpm value" ) != TCL_OK )
        return TCL_ERROR;
    if ( Tcl_GetDouble( interp, interp->result, &xpm) != TCL_OK )
        return TCL_ERROR;
    if ( Tcl_Eval( interp, "sa tcssad get ypm value" ) != TCL_OK )
        return TCL_ERROR;
    if ( Tcl_GetDouble( interp, interp->result, &ypm) != TCL_OK )
        return TCL_ERROR;

/* Get an approximate TAI (i.e. UTC as supplied by the TCL clock command). */
    strcpy( cmd, "clock format [clock seconds] -format \"%Y %m %d\" -gmt 1");
    if ( Tcl_Eval( interp, cmd ) == TCL_ERROR ) return TCL_ERROR;
    sscanf(Tcl_GetStringResult(interp) , "%d %d %d", &y, &m, &d);
    slaCldj( y, m, d, &date, &status);

/* Correct lat/long for polar motion. */
     slaPolmo( longm * D2R, latm * D2R, xpm * S2R, ypm * S2R, &elong, &elat,
         &daz);

/* Initialise the time system. */
     timeOffline( date, elong, elat, hm, djmls, delat, delut);

/* Store raw time. */
     Rawt = timeTai2raw( date );

     return TCL_OK;
}

int tccTimeUTC( Tcl_Interp *interp)
{
    int ymdhmsf[7];
    char* month[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
                 "Sep", "Oct", "Nov", "Dec"};
    char result[40];

    timeThenC( Rawt, UTC, 0, ymdhmsf );
    sprintf( result, "%2.2d:%2.2d:%2.2d  %2.2d %s %4.4d", 
        ymdhmsf[3], ymdhmsf[4], ymdhmsf[5], ymdhmsf[2], month[ymdhmsf[1]-1], 
        ymdhmsf[0]);
    Tcl_SetResult( interp, result, TCL_VOLATILE);
    return TCL_OK;
}

int tccTimeLAST( Tcl_Interp *interp)
{
    int hmsf[4];
    char result[40];

    timeThenT( Rawt, LAST, 0, hmsf );
    sprintf( result, "%2.2d:%2.2d:%2.2d", hmsf[0], hmsf[1], hmsf[2] );
    Tcl_SetResult( interp, result, TCL_VOLATILE);
    return TCL_OK;
}

int tccTimeLocalTime( Tcl_Interp *interp, double offset)
{
    int lymdhmsf[7];
    char* month[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
                 "Sep", "Oct", "Nov", "Dec"};
    char result[40];

    timeThenC( Rawt + offset * 3600.0, UTC, 0, lymdhmsf );
    sprintf( result, "%2.2d:%2.2d:%2.2d  %2.2d %s %4.4d", lymdhmsf[3], 
        lymdhmsf[4], lymdhmsf[5], lymdhmsf[2], month[lymdhmsf[1]-1], 
        lymdhmsf[0]);
    Tcl_SetResult( interp, result, TCL_VOLATILE);
    return TCL_OK;
}

int tccTimeMJD( Tcl_Interp *interp)
{
    double mjd;
    char result[10];

    timeThenD( Rawt, UTC, &mjd );
    mjd += 2400000.5;
    sprintf( result, "%9.1f", mjd);
    Tcl_SetResult( interp, result, TCL_VOLATILE);
    return TCL_OK;
}
