static char rcsid[] = "$Id: tccUtil.c,v 1.2 2001-02-22 16:26:37 dlt Exp $";
/* *INDENT-OFF* */
/*
*   FILENAME
*   tccUtil.c
*
*   FUNCTION NAME(S)
*   tccLimAzel2Hadec - Converts Az/El to HA/Dec
*   tccLimEl - Computes hour angle of elevation limit
*   tccLimPa - Computes hour angle of position angle limit
*   tccDh2e -  Horizon to equitorial conversion
*   tccDe2e -  Equitorial to horizon conversion
*   tccPa -    Equitorial to paralactic angle
*   tccXy2Probe - Convert focal plane X/Y to probe X/Y
*   tccProbe2Xy - Convert focal plane X/Y to probe X/Y
*
*/
/*
 * $Log:
*/
/* *INDENT-ON* */


#include <slalib.h>
#include <astLib.h>
#include "tccUtil.h"
#include "tccConstants.h"

void tccLimAzel2Hadec( double x0, double y0, double z0, 
   double clat, double slat, double *ha, double *dec)
{
/* *INDENT-OFF* */
/*+
 *   Function name:
 *   tccLimAzel2Hadec
 *
 *   Purpose:
 *   Convert Az/El to hour angle and declination
 *
 *   Description:
 *   Converts an Az/El expressed as a vector into an hour angle and
 *   declination.
 *
 *   Invocation:
 *   tccLimAzel2Hadec( x, y, z, &ha, &dec);
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (>)    x0       (double)   X component of Az/El
 *      (>)    y0       (double)   Y component of Az/El
 *      (>)    z0       (double)   Z component of Az/El
 *      (>)    clat     (double)   cosine of latitude
 *      (>)    slat     (double)   sine of latitude
 *      (<)    ha       (double*)  Hour angle
 *      (<)    dec      (double*)  Declination
 *
 *   Function value:
 *      none
 * 
 *   External functions:
 *      none
 *
 *   External variables:
 *      none
 *
 *   Prior requirements:
 *      none
 *
 *   Deficiencies:
 *
 *-
 */
/* *INDENT-ON* */

   double x1, y1, z1;

/* To HA/Dec cartesian */
   x1 = x0 * slat + z0 * clat;
   y1 = y0;
   z1 = - x0 * clat + z0 * slat;

/* To spherical */
   *ha =   - ( x1 == 0.0 && y1 == 0.0 ? 0.0 : atan2( y1, x1) );
   *dec = atan2( z1, sqrt( x1 * x1 + y1 * y1) );
}

/* *INDENT-OFF* */
/*+
 *   Function name:
 *   tccLimEl
 *
 *   Purpose:
 *   Returns the hour angle at which a star at a given declination sets
 *
 *   Description:
 *   Deterimines whether a star reaches a given elevation, and if it does
 *   returns the hour angle at which the star sets.
 *
 *   Invocation:
 *   istat = tccLimEl( cdec, sdec, el, clat, slat, &ha);
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (>)    cdec     (double)   cosine declination of object
 *      (>)    sdec     (double)   sine declination of object
 *      (>)    el       (double)   elevation limit
 *      (>)    clat     (double)   cosine of latitude
 *      (>)    slat     (double)   sine of latitude
 *      (<)    ha       (double*)  hour angle of setting
 *
 *   Function value:
 *   Return value from function e.g.
 *   (<)  status  (int)  Return status, 0 = OK
 *                                     -1 = never rises
 *                                     +1 = never sets
 * 
 *   External functions:
 *   none
 *
 *   External variables:
 *   none
 *
 *   Prior requirements:
 *   none
 *
 *   Deficiencies:
 *   Fails if clat = 0 (a telescope on the equator)
 *
 *-
 */
/* *INDENT-ON* */

int tccLimEl( double cdec, double sdec, double el, double clat, double slat, 
   double *ha)
{
   double gamma;

/* dec +/-90 are a special case to avoid divide by zero */
   if ( cdec == 0.0 ) {
      if ( sdec > 0.0 && slat > 0.0 ) return 1;
      else return -1;
   }

   gamma = ( sin( el ) - sdec * slat ) / ( cdec * clat );
   if ( gamma < -1.0 )
      return 1;             /* never sets */
   else if ( gamma > 1.0 ) 
      return -1;            /* never rises */
   else
      *ha = acos( gamma );
   return 0;
}

/* *INDENT-OFF* */
/*+
 *   Function name:
 *   tccLimAz
 *
 *   Purpose:
 *   Returns the hour angles at which a star at a given declination reaches
 *   a given azimuth.
 *
 *   Description:
 *   Determines whether a star reaches a given azimuth, and if it does
 *   returns the hour angles at which it reaches it.
 *
 *   Invocation:
 *   istat = tccLimAz( cdec, sdec, al, clat, slat, ha);
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (>)    cdec     (double)   cosine declination of object
 *      (>)    sdec     (double)   sine declination of object
 *      (>)    al       (double)   azimuth limit
 *      (>)    clat     (double)   cosine of latitude
 *      (>)    slat     (double)   sine of latitude
 *      (<)    ha[2]    (double)   hour angles of setting
 *
 *   Function value:
 *   Return value from function e.g.
 *   (<)  status  (int)  Number of hour angles.
 * 
 *   External functions:
 *   none
 *
 *   External variables:
 *   none
 *
 *   Prior requirements:
 *   none
 *
 *   Deficiencies:
 *   Fails if slat = 0 (a telescope at the N or S pole) and al = PI/2.
 *
 *-
 */
/* *INDENT-ON* */

int tccLimAz( double cdec, double sdec, double al, double clat, double slat, 
   double ha[2])
{
   double a, b, t, h1, h2;
   double sal, cal, asinab;
   int i;

   sal = sin( al );
   cal = cos( al );
   a = sdec * sal * clat;
   b = cdec * sqrt( cal * cal + sal * sal * slat *slat );

   if ( fabs(a) > fabs(b) || b == 0 ) return 0;

   t = atan2( sal * slat, - cal);

   asinab = asin( a / b );
   h1 = slaDrange( asinab - t );
   h2 = slaDrange( PI - ( asinab + t ) );

   i = 0;
   if ( ( sal > 0.0 && h1 < 0.0 ) || ( sal < 0.0 && h1 > 0.0 ) ) ha[i++] = h1;
   if ( ( sal > 0.0 && h2 < 0.0 ) || ( sal < 0.0 && h2 > 0.0 ) ) ha[i++] = h2;

   return i;
}

/* *INDENT-OFF* */
/*+
 *   Function name:
 *   tccLimPa
 *
 *   Purpose:
 *   Returns the hour angles at which a star at a given declination reaches
 *   a given parallactic angle.
 *
 *   Description:
 *   Determines whether a star reaches a given parallactic andge, and if 
 *   it does returns the hour angle at which it reaches it.
 *
 *   Invocation:
 *   istat = tccLimPa( cdec, sdec, ap, clat, slat, ha);
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (>)    cdec     (double)   cosine declination of object
 *      (>)    sdec     (double)   sine declination of object
 *      (>)    pl       (double)   parallactic angle limit
 *      (>)    clat     (double)   cosine of latitude
 *      (>)    slat     (double)   sine of latitude
 *      (<)    ha[2]    (double)   hour angles of setting
 *
 *   Function value:
 *   Return value from function e.g.
 *   (<)  status  (int)  Number of hour angles.
 * 
 *   External functions:
 *   none
 *
 *   External variables:
 *   none
 *
 *   Prior requirements:
 *   none
 *
 *   Deficiencies:
 *
 *-
 */
/* *INDENT-ON* */

int tccLimPa( double cdec, double sdec, double pl, double clat, double slat, 
   double ha[2])
{
   double a, b, t, h1, h2;
   double spl, cpl, asinab;
   int i;

   spl = sin( pl );
   cpl = cos( pl );
   a = slat * spl * cdec;
   b = clat * sqrt( cpl * cpl + spl * spl * sdec *sdec );

   if ( fabs(a) > fabs(b) || b == 0.0 ) return 0;

   t = atan2( spl * sdec, cpl);

   asinab = asin( a / b );
   h1 = slaDrange( asinab - t );
   h2 = slaDrange( PI - ( asinab + t ) );

   i = 0;
   if ( ( spl > 0.0 && h1 > 0.0 ) || ( spl < 0.0 && h1 < 0.0 ) ) ha[i++] = h1;
   if ( ( spl > 0.0 && h2 > 0.0 ) || ( spl < 0.0 && h2 < 0.0 ) ) ha[i++] = h2;

   return i;
}

void tccDe2h ( double ha, double sd, double cd, double sp, double cp,
    double *az, double *el )
/*
**  - - - - - - - -
**   t c c D e 2 h
**  - - - - - - - -
**
**  Equatorial to horizon coordinates:  HA,Dec to Az,El
**
**  (double precision)
**
**  Given:
**     ha          double       hour angle
**     sd          double       sine declination
**     cd          double       cosine declination
**     sp          double       sine observatory latitude
**     cp          double       cosine observatory latitude
**
**  Returned:
**     *az         double       azimuth
**     *el         double       elevation
**
**  Notes:
**
**  1)  All the arguments are angles in radians.
**
**  2)  Azimuth is returned in the range 0-2pi;  north is zero,
**      and east is +pi/2.  Elevation is returned in the range
**      +/-pi/2.
**
**  3)  The latitude must be geodetic.  In critical applications,
**      corrections for polar motion should be applied.
**
**  4)  In some applications it will be important to specify the
**      correct type of hour angle and declination in order to
**      produce the required type of azimuth and elevation.  In
**      particular, it may be important to distinguish between
**      elevation as affected by refraction, which would
**      require the "observed" HA,Dec, and the elevation
**      in vacuo, which would require the "topocentric" HA,Dec.
**      If the effects of diurnal aberration can be neglected, the
**      "apparent" HA,Dec may be used instead of the topocentric
**      HA,Dec.
**
**  5)  No range checking of arguments is carried out.
**
**
**  Defined in tccConstants.h:  PI2
**
**  Copyright P.T.Wallace.  All rights reserved.
**  Copyright D.L.Terrett.  All rights reserved.
*/
{
   double sh, ch, x, y, z, r, a;

/* Useful trig functions */
   sh = sin ( ha );
   ch = cos ( ha );

/* Az,El as x,y,z */
   x = - ch * cd * sp + sd * cp;
   y = - sh * cd;
   z = ch * cd * cp + sd * sp;

/* To spherical */
   r = sqrt ( x * x + y * y );
   a = ( r == 0.0 ) ? 0.0 : atan2 ( y, x ) ;
   *az = ( a < 0.0 ) ? a + PI2 : a;
   *el = atan2 ( z, r );
}

void tccDh2e ( double az, double el, double sp, double cp, 
   double *ha, double *dec )
/*
**  - - - - - - - -
**   t c c D h 2 e
**  - - - - - - - -
**
**  Horizon to equatorial coordinates:  Az,El to HA,Dec
**
**  (double precision)
**
**  Given:
**     az          double       azimuth
**     el          double       elevation
**     sp          double       sine observatory latitude
**     cp          double       cosine observatory latitude
**
**  Returned:
**     *ha         double       hour angle
**     *dec        double       declination
**
**  Notes:
**
**  1)  All the arguments are angles in radians.
**
**  2)  The sign convention for azimuth is north zero, east +pi/2.
**
**  3)  HA is returned in the range +/-pi.  Declination is returned
**      in the range +/-pi/2.
**
**  4)  The is latitude is (in principle) geodetic.  In critical
**      applications, corrections for polar motion should be applied.
**
**  5)  In some applications it will be important to specify the
**      correct type of elevation in order to produce the required
**      type of HA,Dec.  In particular, it may be important to
**      distinguish between the elevation as affected by refraction,
**      which will yield the "observed" HA,Dec, and the elevation
**      in vacuo, which will yield the "topocentric" HA,Dec.  If the
**      effects of diurnal aberration can be neglected, the
**      topocentric HA,Dec may be used as an approximation to the
**      "apparent" HA,Dec.
**
**  6)  No range checking of arguments is done.
**
**  Last revision:   7 May 1998
**
**  Copyright P.T.Wallace.  All rights reserved.
**  Copyright D.L.Terrett.  All rights reserved.
*/
{
   double sa, ca, se, ce, x, y, z, r;

/* Useful trig functions */
   sa = sin ( az );
   ca = cos ( az );
   se = sin ( el );
   ce = cos ( el );

/* HA,Dec as x,y,z */
   x = - ca * ce * sp + se * cp;
   y = - sa * ce;
   z = ca * ce * cp + se * sp;

/* To spherical */
   r = sqrt ( x * x + y * y );
   *ha = ( r == 0.0 ) ? 0.0 : atan2 ( y, x ) ;
   *dec = atan2 ( z, r );
}

double tccPa ( double ha, double sd, double cd, double sp, double cp )
/*+
**  - - - - - -
**   t c c P a
**  - - - - - -
**
**  HA, Dec to Parallactic Angle.
**
**  (double precision)
**
**  Given:
**     ha     d     hour angle in radians (geocentric apparent)
**     sd     d     sine declination (geocentric apparent)
**     cd     d     cosine declination (geocentric apparent)
**     sp     d     sine observatory latitude (geodetic)
**     cp     d     cosine observatory latitude (geodetic)
**
**  The result is in the range -pi to +pi
**
**  Notes:
**
**  1)  The parallactic angle at a point in the sky is the position
**      angle of the vertical, i.e. the angle between the direction to
**      the pole and to the zenith.  In precise applications care must
**      be taken only to use geocentric apparent HA,Dec and to consider
**      separately the effects of atmospheric refraction and telescope
**      mount errors.
**
**  2)  At the pole a zero result is returned.
**
**  Last revision:   7 May 1998
**
**  Copyright P.T.Wallace.  All rights reserved.
**  Copyright D.L.Terrett.  All rights reserved.
*/
{
   double cqsz, sqsz;

   sqsz = cp * sin ( ha );
   cqsz = sp * cd - cp * sd * cos ( ha );
   return ( ( sqsz != 0.0 || cqsz != 0.0 ) ? atan2 ( sqsz, cqsz ) : 0.0 );
}

/* *INDENT-OFF* */
/*+
 *   Function name:
 *   tccXy2Probe
 *
 *   Purpose:
 *   Converts an X/Y in the focal plane to a probe position corrected for
 *   the curvature of the focal plane.
 *
 *   Description:
 *   Assuming no focus corrections then the correction for the curved surface
 *   on which the probes move is
 *
 *   r(corrected) = r1 (1 - beta * r1 * r1)
 *
 *   where beta is curvature * curvature / 2.0 and curvature is 1/16539.326.
 *   6539.326mm is the default separation of M2 and the focal plane.
 *
 *   Invocation:
 *   tccXy2Probe( &x, &y);
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)    x     (double)   Probe X
 *      (!)    y     (double)   Probe Y
 *
 *   External functions:
 *   none
 *-
 */
/* *INDENT-ON* */

#define BETA ( 1.0/16539.326 * 1.0/16539.326 / 2.0 )

void tccXy2Probe( double *x, double *y)
{
   double f;

   f = ( 1.0 - BETA * ( *x * *x + *y * *y ) );
   *x *= f;
   *y *= f;
}

/* *INDENT-OFF* */
/*+
 *   Function name:
 *   tccProbe2Xy
 *
 *   Purpose:
 *   Converts a probe position to an X/Y in the focal plane corrected for
 *   the curvature of the focal plane.
 *
 *   Description:
 *
 *   Invocation:
 *   tccProbe2Xy( &x, &y);
 *
 *   Parameters: (">" input, "!" modified, "<" output)  
 *      (!)    x     (double)   Probe X
 *      (!)    y     (double)   Probe Y
 *
 *   External functions:
 *   none
 *-
 */
/* *INDENT-ON* */

void tccProbe2Xy( double *x, double *y)
{
   double f;

   f = ( 1.0 - BETA * ( *x * *x + *y * *y ) );
   *x /= f;
   *y /= f;
}
