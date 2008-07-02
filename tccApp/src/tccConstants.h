/*
*   FILENAME
*   tccConstants.h
*
*   DESCRIPTION
*   This include file defines the constants that are needed within the Gemini
*   TCC code.
*
*/
/* *INDENT-OFF* */
/*
 * $Log: not supported by cvs2svn $
 */
/* *INDENT-ON* */

#ifndef TCCCONSTANTS_H
#define TCCCONSTANTS_H

#define PI    3.1415926535897932384626433832795
#define PI2   (PI*2.0)
#define D90   (PI/2.0)
#define D2R   (PI/180.0)
#define S2R   (PI/43200.0)
#define AS2R  (PI/648000.0)
#define C     173.14463331
#define AU2KM 149597870.66
#define UTST  1.00273790934
#define R2D   (1.0/D2R)
#define TSLOW 60.0
#define STRPS (UTST * S2R)
#define ST2MIN (1.0 / UTST / S2R / 60.0)

#endif
