static char rcsid[]="$Id:";
/* *INDENT-OFF* */
/*
*   FILENAME
*   tccTime.c
*
*   FUNCTION NAME(S)
*   timeCmd - Implements the tccTime tcl command.
*
*   D L Terrett 7 October 1998
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
#include "tccTime.h"

static int tccTimeInit( Tcl_Interp *interp);
static int tccTimeUTC( Tcl_Interp *interp);
static int tccTimeLAST( Tcl_Interp *interp);
static int tccTimeLocalTime( Tcl_Interp *interp);
static int tccTimeMJD( Tcl_Interp *interp);

static double Rawt;
static int LocalTimeOffset;


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

    Tcl_CreateCommand( interp, "tccTime", TccTimeCmd,
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

int TccTimeCmd( ClientData clientdata, Tcl_Interp *interp, int argc,
    char *argv[])
{
    if ( argc < 2 ) {
        Tcl_AppendResult( interp,
           "wrong # args: should be \"tccTime option ?args ...?\"", 
           (char *) NULL);
        return TCL_ERROR;
    }
    else if ( strcmp( argv[1], "init" ) == 0 ) {
        if ( argc != 3 ) {
            Tcl_AppendResult( interp,
               "wrong # args: should be \"tccTime init offset\"",
               (char *) NULL);
            return TCL_ERROR;
        }
        sscanf( argv[2], "%d", &LocalTimeOffset );
        return tccTimeInit( interp );
    }
    else if ( strcmp( argv[1], "utc" ) == 0 ) {
        return tccTimeUTC( interp );
    }
    else if ( strcmp( argv[1], "last" ) == 0 ) {
        return tccTimeLAST( interp );
    }
    else if ( strcmp( argv[1], "localtime" ) == 0 ) {
        return tccTimeLocalTime( interp );
    }
    else if ( strcmp( argv[1], "mjd" ) == 0 ) {
        return tccTimeMJD( interp );
    }
    else if ( strcmp( argv[1], "set" ) == 0 ) {
        if ( argc != 3 ) {
            Tcl_AppendResult( interp,
               "wrong # args: should be \"tccTime set time\"", (char *) NULL);
            return TCL_ERROR;
        }
        sscanf( argv[2], "%lf", &Rawt );
        return TCL_OK;
    }
    else {
        Tcl_AppendResult( interp, "unknown option \"", argv[1],
            "\" must be init, utc, last, localtime or mjd", (char *) NULL);
        return TCL_ERROR;
    }
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
    sscanf(interp->result, "%d %d %d", &y, &m, &d);
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

    timeThenC( Rawt, UTC, 0, ymdhmsf );
    sprintf( interp->result, 
    "%2.2d:%2.2d:%2.2d  %2.2d %s %4.4d", 
        ymdhmsf[3], ymdhmsf[4], ymdhmsf[5], ymdhmsf[2], month[ymdhmsf[1]-1], 
        ymdhmsf[0]);
    return TCL_OK;
}

int tccTimeLAST( Tcl_Interp *interp)
{
    int hmsf[4];

    timeThenT( Rawt, LAST, 0, hmsf );
    sprintf( interp->result, 
    "%2.2d:%2.2d:%2.2d", hmsf[0], hmsf[1], hmsf[2] );
    return TCL_OK;
}

int tccTimeLocalTime( Tcl_Interp *interp)
{
    int lymdhmsf[7];
    char* month[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
                 "Sep", "Oct", "Nov", "Dec"};

    timeThenC( Rawt + (double)LocalTimeOffset * 3600.0, UTC, 0, lymdhmsf );
    sprintf( interp->result, 
    "%2.2d:%2.2d:%2.2d  %2.2d %s %4.4d", lymdhmsf[3], lymdhmsf[4], 
        lymdhmsf[5], lymdhmsf[2], month[lymdhmsf[1]-1], lymdhmsf[0]);
    return TCL_OK;
}

int tccTimeMJD( Tcl_Interp *interp)
{
    double mjd;

    timeThenD( Rawt, UTC, &mjd );
    mjd += 2400000.5;
    sprintf( interp->result, "%9.1f", mjd);
    return TCL_OK;
}
