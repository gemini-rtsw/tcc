/*
*   FILENAME
*   tccLimits.h
*
*   DESCRIPTION
*   Routine definitions for tcc utility routines
*
*/
/* *INDENT-OFF* */
/*
 * $Log:
 */
/* *INDENT-ON* */

#ifndef TCCUTIL_H
#define TCSCUTIL_H

#include "tccConstants.h"

void tccLimAzel2Hadec( double x, double y, double z, double clat, double slat,
   double *ha, double *dec);
int tccLimEl( double cdec, double sdec, double el, double clat, double slat, 
   double *ha);
int tccLimAz( double cdec, double sdec, double al, double clat, double slat,
   double *ha);
int tccLimPa( double cdec, double sdec, double pl, double clat, double slat,
   double *ha);
void tccDh2e ( double az, double el, double sp, double cp,
   double *ha, double *dec );
void tccDe2h ( double ha, double sd, double cd, double sp, double cp,
    double *az, double *el );
double tccPa ( double ha, double sd, double cd, double sp, double cp );
void tccXy2Probe( double *x, double *y );
void tccProbe2Xy( double *x, double *y );

#endif
