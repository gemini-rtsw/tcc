static char rcsid[] = "$Id: tccDecode.c,v 1.6 2005-01-20 00:03:25 dlt Exp $";
/* *INDENT-OFF* */
/*
*   FILENAME
*   tccDecode.c
*
*   FUNCTION NAME(S)
*   tccDcEpoch    - decode a epoch
*   tccDcFrame    - decode a coordinate frame
*   tccDcLc       - convert a string to lowercase
*   tccDcLen      - find length of string excluding whitespace
*   tccDcRadec    - decode an RA/Dec or Az/El
*   tccDcT0       - decode a reference time
*   tccDcUc       - convert a string to uppercase
*   tccDcPlanet   - decode planet name
*   tccDcT0       - Decode a reference epoch
*/
/*
 * $Log: not supported by cvs2svn $
 * Revision 1.5  2002/06/06 13:09:59  dlt
 * Implement new orbit formats and Chapront & Francou planet ephemeris
 *
 * Revision 1.4  2001/02/05 19:33:27  dlt
 * Eliminate use of AppendResult
 *
 * Revision 1.3  2000/02/18 02:29:18  dlt
 * New routine for decoding planet names
 *
 * Revision 1.2  1999/11/12 00:19:56  dlt
 * Improve handling of errors getting epics process variables
 *
 * Revision 1.1.1.1  1999/05/27 04:53:39  dlt
 * Initial load of Gemini telescope consoles
 *
 */
/* *INDENT-ON* */

#include <string.h>
#include <math.h>
#include <ctype.h>
#include <tcl.h>
#include <slalib.h>
#include <astLib.h>
#include "tccConstants.h"
#include "tccDecode.h"

/* Length of string for uppercased coordinate type (excluding terminator). */
#define LSUC 40

/* *INDENT-OFF* */
/*
**  - - - - - - - -
**   t c c D c D t
**  - - - - - - - -
**
**  Decode two strings containing, respectively, differential tracking
**  rates in either right ascension and declination or azimuth and
**  elevation.
**
**  Given:
**    interp    Tcl_Interp*
**    frame     int      frame ID
**    string1   char*    string to decode, containing RA/Az rate
**    string2   char*    string to decode, containing Dec/El rate
**
**  Returned (arguments):
**    dr        double*  RA/Az rate (radians per day)
**    dd        double*  Dec/El rate (radians per day)
**
**  Returned (function value):
**              int       TCL status
**
**  Defined in astLib.h:
**    AZEL_MNT     FRAMETYPE      frame ID for mount az/el
**    AZEL_TOPO    FRAMETYPE        "   "   "  topocentric az/el
**    APPT         FRAMETYPE        "   "   "  geocentric apparent RA/dec
**    FK5          FRAMETYPE        "   "   "  IAU 1976 RA/dec
**    FK4          FRAMETYPE        "   "   "  pre IAU 1976 RA/dec
**    S2R          double         seconds to radians
**    AS2R         double         arcseconds to radians
**
**  Called:  tccDcLen, slaDfltin
**
**  Notes:
**
**    1  Incoming units:
**
**         RA         seconds of time per second
**         Dec        arcseconds per second
**         Az         arcseconds per second
**         El         arcseconds per second
**
**    2  A blank or empty string returns zero for that quantity.
**
**    3  If either rate is outside the allowed range (see below),
**       neither is changed and a function value of TCL_ERROR is returned.
**
**    4  If either rate has been decoded successfully but the string
*        has not been exhausted, the value is returned but with a
**       function value of TCL_ERROR.
**
**    5  See the code for the various messages that can result.
**
*/
/* *INDENT-ON* */



/* Maximum dRA/dt */
#define MAXDR 10000.0

/* Maximum dDec/dt */
#define MAXDD 10000.0

/* Maximum dAz/dt */
#define MAXDA 10000.0

/* Maximum dAz/dt */
#define MAXDE 10000.0

int tccDcDt(Tcl_Interp *interp, FRAMETYPE frame, const char *string1, 
        const char *string2, double *dr, double *dd)
{
    int n, j, m;
    double f, vm1, vm2, d1, d2;
    char mesrx[] = "RA rate out of range";
    char mesrb[] = "illegal RA rate";
    char mesdx[] = "Dec rate out of range";
    char mesdb[] = "illegal Dec rate";
    char mesax[] = "Az rate out of range";
    char mesab[] = "illegal Az rate";
    char mesex[] = "El rate out of range";
    char meseb[] = "illegal El rate";
    char *px1, *pb1, *px2, *pb2;

/* Accept the frame ID. */
    switch (frame) {
    case AZEL_MNT:
    case AZEL_TOPO:
        f = AS2R * 86400.0;
        px1 = mesax;
        px2 = mesex;
        pb1 = mesab;
        pb2 = meseb;
        vm1 = MAXDA;
        vm2 = MAXDE;
        break;
    case APPT:
    case FK5:
    case FK4:
        f = S2R * 86400.0;
        px1 = mesrx;
        px2 = mesdx;
        pb1 = mesrb;
        pb2 = mesdb;
        vm1 = MAXDR;
        vm2 = MAXDD;
        break;
    default:
        Tcl_SetResult( interp, "illegal frame ID", TCL_VOLATILE);
        return TCL_ERROR;
    }

/* Look for blank input. */
    if (!tccDcLen(string1) && !tccDcLen(string2)) {
        *dr = 0.0;
        *dd = 0.0;
        return TCL_OK;
    }
/* Decode the RA/Az rate. */
    m = 1;
    d1 = 0.0;
    slaDfltin((char *)string1, &m, &d1, &j);
    if (j < 2) {
        if (fabs(d1) > vm1) {
            Tcl_SetResult( interp, px1, TCL_VOLATILE);
            return TCL_ERROR;
        }
    } else {
        Tcl_SetResult( interp, pb1, TCL_VOLATILE);
        return TCL_ERROR;
    }

/* Decode the Dec/El rate. */
    n = 1;
    d2 = 0.0;
    slaDfltin((char*)string2, &n, &d2, &j);
    if (j < 2) {
        if (fabs(d2) > vm2) {
            Tcl_SetResult( interp, px2, TCL_VOLATILE);
            return TCL_ERROR;
        }
    } else {
        Tcl_SetResult( interp, pb2, TCL_VOLATILE);
        return TCL_ERROR;
    }

/* OK: return the values and verify end of string reached. */
    *dr = d1 * f;
    *dd = d2 * AS2R * 86400.0;
    if ((int) string1[m - 1] == '\0' &&
        (int) string2[n - 1] == '\0') {
        return TCL_OK;
    } else {
        Tcl_SetResult( interp, "leftover text", TCL_VOLATILE);
        return TCL_ERROR;
    }
}

/* *INDENT-OFF* */
/*
**  - - - - - - - - - - -
**   t c c D c E p o c h
**  - - - - - - - - - - -
**
**  Decode a string containing an epoch (e.g. "J2000").
**
**  Given:
**    interp    Tcl_Interp*
**    string    char*    string to decode
**
**  Returned (arguments):
**    type      int*     'B' = Besselian
**                       'J' = Julian
**    epoch     double*  epoch
**
**  Returned (function value):
**              int       TCL status
**
**  Called:  tccDcLen, slaDbjin, slaKbj
**
**  Notes:
**
**    1  If a blank or empty string is supplied, epoch J2000
**       is returned.
**
**    2  If the value is outside the allowed range (see below),
**       the type and epoch are not changed and a function value
**       of TCL_ERROR is returned.
**
**    3  If the epoch has been decoded successfully but the
**       string has not been exhausted, the type and epoch are
**       returned but with a function value of TCL_ERROR.
**
**    4  See the code for the various messages that can result.
**
*/
/* *INDENT-ON* */



/* Allowed range */
#define MINEP 1850.0
#define MAXEP 2100.0

int tccDcEpoch(Tcl_Interp *interp, const char *string, char *type, 
        double *epoch)
{
    int n, j1, j2, j;
    char k;
    double e;

/* Look for blank input. */
    if (!tccDcLen(string)) {
        *type = 'J';
        *epoch = 2000.0;
        return TCL_OK;
    }
/* Decode the epoch. */
    n = 1;
    slaDbjin((char*)string, &n, &e, &j1, &j2);
    if (!j1) {
        if (e >= MINEP && e <= MAXEP) {
            slaKbj(j2, e, &k, &j);
            if (!j) {

                /* OK: verify end of string reached. */
                if ((int) string[n - 1] == '\0') {
                    *type = (int) k;
                    *epoch = e;
                    return TCL_OK;
                } else {
                    Tcl_SetResult( interp, "leftover text", TCL_VOLATILE);
                    return TCL_ERROR;
                }
            } else {
                Tcl_SetResult( interp, "prefix not B or J", TCL_VOLATILE);
                return TCL_ERROR;
            }
        } else {
            Tcl_SetResult( interp, "epoch out of range", TCL_VOLATILE);
            return TCL_ERROR;
        }
    }
    Tcl_SetResult( interp, "illegal epoch", TCL_VOLATILE);
    return TCL_ERROR;
}
/* *INDENT-OFF* */
/*
**  - - - - - - - - - - -
**   t c c D c F r a m e
**  - - - - - - - - - - -
**
**  Decode a string containing a frame (coordinate system) name.
**
**  Given:
**    interp    Tcl_Interp*
**    string    char*    string to decode
**
**  Returned (arguments):
**    frame     FRAMETYPE*     frame ID
**
**  Returned (function value):
**              int       TCL status
**
**  Defined in tccConstants.h:
**    AZEL_MNT     FRAMETYPE    frame ID for mount az/el
**    AZEL_TOPO    FRAMETYPE      "   "   "  topocentric az/el
**    APPT         FRAMETYPE      "   "   "  geocentric apparent RA/dec
**    FK5          FRAMETYPE      "   "   "  IAU 1976 RA/dec
**    FK4          FRAMETYPE      "   "   "  pre IAU 1976 RA/dec
**
**  Called:  tccDcUc, tccDcLen
**
**  Notes:
**
**    1  If a blank or empty string is supplied, a frame ID
**       of FK5 is returned.
**
**    2  If the string does not contain a recognized frame name,
**       the frame ID is not changed and a function value of TCL_ERROR
**       is returned.
**
**    3  Each frame has several aliases:  see the code.
**
**    4  See the code for the various messages that can result.
**
*/
/* *INDENT-ON* */



int tccDcFrame(Tcl_Interp *interp, const char *string, FRAMETYPE * frame)
{
    int n, ls;
    char suc[LSUC];

/* Fold the frame name to uppercase, stripping trailing blanks. */
    n = LSUC - 1;
    if ((ls = tccDcLen(string)) > n)
        ls = n;
    tccDcUc(string, ls + 1, suc);

/* Identify frame, return ID, message and status. */
    if (!ls) {
        *frame = FK5;
        return TCL_OK;
    } else if (!strcmp(suc, "FK4") ||
               !strcmp(suc, "B1950") ||
               !strcmp(suc, "1950") ||
               !strcmp(suc, "1950.0")) {
        *frame = FK4;
        return TCL_OK;
    } else if (!strcmp(suc, "FK5") ||
               !strcmp(suc, "ICRS") ||
               !strcmp(suc, "J2000") ||
               !strcmp(suc, "2000") ||
               !strcmp(suc, "2000.0")) {
        *frame = FK5;
        return TCL_OK;
    } else if (!strcmp(suc, "APPT") ||
               !strcmp(suc, "APPARENT") ||
               !strcmp(suc, "GEOCENTRIC APPARENT") ||
               !strcmp(suc, "APPARENT PLACE")) {
        *frame = APPT;
        return TCL_OK;
    } else if (!strcmp(suc, "AZ/EL") ||
               !strcmp(suc, "AZEL_TOPO") ||
               !strcmp(suc, "AZEL") ||
               !strcmp(suc, "TOPOCENTRIC AZ/EL") ||
               !strcmp(suc, "TOPOCENTRIC AZ EL")) {
        *frame = AZEL_TOPO;
        return TCL_OK;
    } else {
        Tcl_SetResult( interp, "unrecognized frame", TCL_VOLATILE);
        return TCL_ERROR;
    }
}

/* *INDENT-OFF* */
/*
**  - - - - - - - -
**   t c c D c L c
**  - - - - - - - -
**
**  Convert a string to lowercase.
**
**  Given:
**     in      char*    string to be converted
**     lout    int      length of output string (including terminator)
**
**  Returned:
**     out     char*    string after conversion
**
**  Notes:
**
**  1  The *out pointer is also returned as the function value.
**
**  2  The two arguments can refer to the same string.
**
*/
/* *INDENT-ON* */



char* tccDcLc(const char *in, int lout, char *out)
{
    int i, c;

    out[lout - 1] = (char) '\0';
    for (i = 0; i < lout; i++) {
        c = (int) in[i];
        out[i] = (char) tolower(c);
        if (c == '\0')
            break;
    }
    return out;
}
/* *INDENT-OFF* */
/*
**  - - - - - - - - - -
**   t c c D c L e n
**  - - - - - - - - - -
**
**  Length of a string excluding any trailing whitespace.
**
**  Given:
**     *string      char     the string
**
**  Returned (function value):
**                  int      length excluding trailing spaces
**
**  The minimum length returned is 0.
**
*/
/* *INDENT-ON* */


int tccDcLen(const char *string)
{
    int l, i, c;

    l = 0;
    for (i = 0;
         (c = (int) string[i++]) != '\0';
         l = isspace(c) ? l : i);
    return l;
}

/* *INDENT-OFF* */
/*
**  - - - - - - - - - - -
**   t c c D c R a d e c
**  - - - - - - - - - - -
**
**  Decode two strings containing, respectively, either right ascension
**  and declination or azimuth and elevation.
**
**  Given:
**    interp    Tcl_Interp*
**    frame     int      frame ID
**    string1   char*    string to decode, containing RA/Az
**    string2   char*    string to decode, containing Dec/El
**
**  Returned (arguments):
**    r         double*  RA/Az (radians)
**    d         double*  Dec/El (radians)
**
**  Returned (function value):
**              int       TCL status
**
**  Defined in tccConstants.h:
**    AZEL_MNT     FRAMETYPE      frame ID for mount az/el
**    AZEL_TOPO    FRAMETYPE        "   "   "  topocentric az/el
**    APPT         FRAMETYPE        "   "   "  geocentric apparent RA/dec
**    FK5          FRAMETYPE        "   "   "  IAU 1976 RA/dec
**    FK4          FRAMETYPE        "   "   "  pre IAU 1976 RA/dec
**    D2R          double         degrees to radians
**
**  Called:  tccDcLen, slaDfltin, slaIntin
**
**  Notes:
**
**    1  The strings contain decimal fields (in free format):
**
**         RA         hours minutes seconds
**         Dec        +/- degrees arcminutes arcseconds
**         Az         degrees
**         El         degrees
**
**    2  The allowed ranges are as follows.
**
**       frame = AZEL_MNT, AZEL_TOPO:
**
**          r from -359.999... to +359.999
**          d from 0 to 90
**
**       frame = APPT, FK4, FK5:
**
**          r from 00 00 00.0 to 23 59 59.999...
**          d from -90 00 00.0 to +90 00 00.0
**
**    3  Azimuth is interpreted as follows.
**
**       frame = AZEL_MNT:
**
**         Supplied values in the range -270 to +360 are interpreted
**         literally.  Supplied values in the range -360 to -270 are
**         expressed in the range 0-90.
**
**       frame = AZEL_TOPO:
**
**         Supplied values are folded into the range 0-360.
**
**    4  If any error occurs the output arguments are not changed
**       and a function value of TCL_ERROR is returned.
**
**    5  See the code for the various messages that can result.
**
*/
/* *INDENT-ON* */



int tccDcRadec(Tcl_Interp *interp, FRAMETYPE frame, const char *string1, 
               const char *string2, double *r, double *d)
{
    char lstr1[LSUC], lstr2[LSUC];
    int n, i, j, m;
    double ar, ed;

/*
   ** -------------
   ** PRELIMINARIES
   ** -------------
 */

/* Look for blank input. */
    if (!tccDcLen(string1)) {
        Tcl_SetResult( interp, "blank RA/Azimuth", TCL_VOLATILE);
        return TCL_ERROR;
    }
    if (!tccDcLen(string2)) {
        Tcl_SetResult( interp, "blank Dec/Elevation", TCL_VOLATILE);
        return TCL_ERROR;
    }

/* Copy input strings replacing colons ith commas. */
    for ( i = 0; i < LSUC; i++ ) {
        lstr1[i] = string1[i] == ':' ? ',' : string1[i];
        if ( lstr1[i] == '\0' ) break;
    }
    if ( i == LSUC) lstr1[ LSUC - 1 ] = '\0';
    for ( i = 0; i < LSUC; i++ ) {
        lstr2[i] = string2[i] == ':' ? ',' : string2[i];
        if ( lstr2[i] == '\0' ) break;
    }
    if ( i == LSUC) lstr2[ LSUC - 1 ] = '\0';
/*
   ** ----------------------------
   ** SWITCH ACCORDING TO FRAME ID
   ** ----------------------------
 */

    switch (frame) {
    case AZEL_MNT:
    case AZEL_TOPO:


        /*
           ** Decode azimuth
           ** --------------
         */

        m = 1;
        slaDafin(lstr1, &m, &ar, &j);

        /* OK if number supplied */
        if (j > 0) {
            Tcl_SetResult( interp, "illegal azimuth", TCL_VOLATILE);
            return TCL_ERROR;
        }

        /*
           ** Decode elevation
           ** ----------------
         */

        n = 1;
        slaDafin(lstr2, &n, &ed, &j);

        /* OK if number supplied. */
        if (j > 0) {
            Tcl_SetResult( interp, "illegal elevation", TCL_VOLATILE);
            return TCL_ERROR;
        }

        break;

    case APPT:
    case FK5:
    case FK4:


        /*
           ** Decode right ascension
           ** ----------------------
         */

        m = 1;
        slaDafin(lstr1, &m, &ar, &j);
        ar *= 15.0;

        /* Give up if error already. */
        if (j < 0) {
            Tcl_SetResult( interp, "illegal RA", TCL_VOLATILE);
            return TCL_ERROR;
        } else {
            if (ar > PI2 || ar < 0.0 ) {
                Tcl_SetResult( interp, "RA out of range", TCL_VOLATILE);
                return TCL_ERROR;
            }
        }

        /*
           ** Decode declination
           ** ------------------
         */

        n = 1;
        slaDafin(lstr2, &n, &ed, &j);

        /* Give up if error already. */
        if (j < 0) {
            Tcl_SetResult( interp, "illegal Dec", TCL_VOLATILE);
            return TCL_ERROR;
        } else {
            if (ed > D90 || ed < -D90 ) {
                Tcl_SetResult( interp, "Dec out of range", TCL_VOLATILE);
                return TCL_ERROR;
            }
        }

        break;

    default:
        Tcl_SetResult( interp, "illegal frame ID", TCL_VOLATILE);
        return TCL_ERROR;
    }


/*
   ** -------
   ** WRAP UP
   ** -------
 */

/* Return the values and verify end of string reached. */
    if ((int) lstr1[m - 1] == '\0' &&
        (int) lstr2[n - 1] == '\0') {
        *r = ar;
        *d = ed;
        return TCL_OK;
    } else {
        Tcl_SetResult( interp, "leftover text", TCL_VOLATILE);
        return TCL_ERROR;
    }
}

/* *INDENT-OFF* */
/*
**  - - - - - - - -
**   t c c D c T 0
**  - - - - - - - -
**
**  Decode a string containing a reference epoch.
**
**  Given:
**    interp    Tcl_Interp*
**    string    char*    string to decode
**
**  Returned (arguments):
**    t0        double*  reference epoch (TT MJD)
**
**  Returned (function value):
**              int       TCL status
**
**  Called:  tccDcLen, slaDfltin, slaDfltin, slaDtf2d, slaCaldj
**
**  Notes:
**
**    1  Several different formats are accepted, as follows (all
**       numbers are free-format and any number of digits can be
**       used):
**
**       string                      value
**
**       blank                       0.0
**       yyyy mm dd hh mm ss.s       MJD
**       yy mm dd hh mm ss.s         MJD, interpretation 1950-2049
**       hh mm ss.s                  fraction of a day
**
**       If a blank or empty string is supplied, a zero result 
**       is returned.  It is suggested that under these circumstances 
**       the calling program sets t0 equal to the current MJD TT, 
**       obtained from the Time Service by calling timeNowD.  Thus 
**       blank means "now".
**
**       It is suggested that the calling program tests for t0 <= 1.0
**       and, if true, obtains the current MJD TT from the Time Service
**       by calling timeNowD and adds the integer part to t0.  Thus
**       time-only means "today".  (Use of this technique close to
**       0 hours is clearly risky in a real-time sense.)
**
**    2  If the epoch is outside the allowed range (see below),
**       it is not changed and a function value of TCL_ERROR is returned.
**
**    3  If the epoch has been decoded successfully but the
**       string has not been exhausted, the value is returned
**       but with a function value of TCL_ERROR.
**
**    4  See the code for the various messages that can result.
**
**    5  MJD means JD minus 2400000.5.
**
*/
/* *INDENT-ON* */



/* Allowed MJD range */
#define MINT0 10000.0
#define MAXT0 24100000.0

int tccDcT0(Tcl_Interp *interp, const char *string, double *t0)
{
    int n, i, j, k;
    long iy, imo, id, ih, im, test;
    double d, s, t, df;


/* Look for blank input. */
    if (!tccDcLen(string)) {
        *t0 = 0.0;
        return TCL_OK;
    }
/* Decode up to seven +ve doubles. */
    i = 1;
    for (n = 0; n < 7; n++) {
        slaDfltin((char*)string, &i, &d, &j);
        if (j == 1) {
            break;
        } else if (j) {
            n = 0;
            break;
        }
    }

/* If all is well, the field count should now be 1, 3 or 6. */

/* Examine number of fields. */
    if (n == 3) {

        /* Handle time-of-day and date with fractions of a day cases. */
        d = 0.0;

        /* In order to distinguish between a time of day and a date we
           need to examine the fist number. */
        k = 1;
        test = 0.0;
        slaIntin((char*)string, &k, &test, &j);
        if (j)
            n = 0;

        if ( test > 24 ) {

           /* Handle date */
           i = 1;
           slaIntin((char*)string, &i, &iy, &j);
           if (j)
               n = 0;
           slaIntin((char*)string, &i, &imo, &j);
           if (j)
               n = 0;
           slaDfltin((char*)string, &i, &df, &j);
           if (j)
               n = 0;
           slaCaldj((int) iy, (int) imo, (int) floor( df ), &d, &j);
           if (j)
               n = 0;
           d += df - floor( df );

        } else {

           /* Handle time of day. */
           i = 1;
           slaIntin((char*)string, &i, &ih, &j);
           if (j)
               n = 0;
           slaIntin((char*)string, &i, &im, &j);
           if (j)
               n = 0;
           slaDfltin((char*)string, &i, &d, &j);
           if (j)
               n = 0;
           slaDtf2d((int) ih, (int) im, s, &d, &j);
           if (j)
               n = 0;
        }

    } else if (n == 6) {

        /* Handle full date and time case. */
        d = 0.0;
        i = 1;
        slaIntin((char*)string, &i, &iy, &j);
        if (j)
            n = 0;
        slaIntin((char*)string, &i, &imo, &j);
        if (j)
            n = 0;
        slaIntin((char*)string, &i, &id, &j);
        if (j)
            n = 0;
        slaCaldj((int) iy, (int) imo, (int) id, &d, &j);
        if (j)
            n = 0;
        slaIntin((char*)string, &i, &ih, &j);
        if (j)
            n = 0;
        slaIntin((char*)string, &i, &im, &j);
        if (j)
            n = 0;
        slaDfltin((char*)string, &i, &s, &j);
        if (j)
            n = 0;
        slaDtf2d((int) ih, (int) im, s, &t, &j);
        if (j)
            n = 0;
        d += t;

    } else if (n != 1) {
        n = 0;
    }
/* Return value and status as appropriate. */
    if (n > 0) {
        if (n == 3 || (d >= MINT0 && d <= MAXT0)) {

            /* OK: return the value and verify end of string reached. */
            *t0 = d;
            if ((int) string[i - 1] == '\0') {
                return TCL_OK;
            } else {
                Tcl_SetResult( interp, "leftover text", TCL_VOLATILE);
                return TCL_ERROR;
            }
        } else {
            Tcl_SetResult( interp, "epoch out of range", TCL_VOLATILE);
        }
    } else {
        Tcl_SetResult( interp, "illegal epoch", TCL_VOLATILE);
    }
    return TCL_ERROR;
}
/* *INDENT-OFF* */
/*
**  - - - - - - - -
**   t c c D c U c
**  - - - - - - - -
**
**  Convert a string to uppercase.
**
**  Given:
**     in      char*    string to be converted
**     lout    int      length of output string (including terminator)
**
**  Returned:
**     out     char*    string after conversion
**
**  Notes:
**
**  1  The *out pointer is also returned as the function value.
**
**  2  The two arguments can refer to the same string.
**
*/
/* *INDENT-ON* */



char *tccDcUc(const char *in, int lout, char *out)
{
    int i, c;

    out[lout - 1] = (char) '\0';
    for (i = 0; i < lout; i++) {
        c = (int) in[i];
        out[i] = (char) toupper(c);
        if (c == '\0')
            break;
    }
    return out;
}

/* *INDENT-OFF* */
/*
**  - - - - - - - - - - - -
**   t c s D c P l a n e t
**  - - - - - - - - - - - -
**
**  Decode a string containing a planet name.
**
**  Given:
**    interp    Tcl_Interp*
**    string    char*    string to decode
**
**  Returned (arguments):
**    planet    int*     code for planet
**
**  Returned (function value):
**              int     TCL status
**
**  Called:  tcsDcUc, tcsDcLen
**
**  Notes:
**
**    1  If a blank or empty string is supplied, a planert name
**       of NONE is returned.
**
**    2  If the string does not contain a recognized planet name,
**       the planet id is not changed and a function value of
**       TCL_ERROR is returned.
**
*/
/* *INDENT-ON* */

int tccDcPlanet(Tcl_Interp *interp, const char *string, int *planet)
{
    int n, ls;
    char suc[LSUC];

/* Fold the planet name to uppercase, stripping trailing blanks. */
    n = LSUC - 1;
    if ((ls = tccDcLen(string)) > n)
        ls = n;
    tccDcUc(string, ls + 1, suc);

/* Identify planet, return ID and status. */
    if (!ls) {
        return TCL_ERROR;
    } else if (!strcmp(suc, "MERCURY") ) {
        *planet = 1;
        return TCL_OK;
    } else if (!strcmp(suc, "VENUS") ) {
        *planet = 2;
        return TCL_OK;
    } else if (!strcmp(suc, "MOON") ) {
        *planet = 3;
        return TCL_OK;
    } else if (!strcmp(suc, "MARS") ) {
        *planet = 4;
        return TCL_OK;
    } else if (!strcmp(suc, "JUPITER") ) {
        *planet = 5;
        return TCL_OK;
    } else if (!strcmp(suc, "SATURN") ) {
        *planet = 6;
        return TCL_OK;
    } else if (!strcmp(suc, "URANUS") ) {
        *planet = 7;
        return TCL_OK;
    } else if (!strcmp(suc, "NEPTUNE") ) {
        *planet = 8;
        return TCL_OK;
    } else if (!strcmp(suc, "PLUTO") ) {
        *planet = 9;
        return TCL_OK;
    } else {
        Tcl_SetResult(interp, "unrecognized planet", TCL_VOLATILE);
        return TCL_ERROR;
    }
}
