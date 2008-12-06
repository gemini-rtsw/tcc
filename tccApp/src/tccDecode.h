/*
*   FILENAME
*   tccDecode.h
*
*   DESCRIPTION
*   Function prototypes for the tcs input decoding routines.
*
*/
/* *INDENT-OFF* */
/*
 * $Log: tccDecode.h,v $
 * Revision 1.3  2005/01/20 00:03:25  dlt
 * Remove deprecated access to interp->result
 *
 * Revision 1.2  2000/02/18 02:29:19  dlt
 * New routine for decoding planet names
 *
 * Revision 1.1.1.1  1999/05/27 04:53:39  dlt
 * Initial load of Gemini telescope consoles
 *
 */
/* *INDENT-ON* */

#ifndef DECODELIBHDEF
#define DECODELIBHDEF

/*
**
**  TCS string decode library macros and prototype function declarations.
**
**  D Terrett  16 February 2000
**
**  Copyright CCLRC
*/

#include "tcl.h"
#include "astLib.h"

/* Function prototypes */
int tccDcDt ( Tcl_Interp*, FRAMETYPE, const char*, const char*, 
        double*, double*);
int tccDcEpoch ( Tcl_Interp*, const char*, char*, double*);
int tccDcFrame ( Tcl_Interp*, const char*, FRAMETYPE*);
int tccDcLen ( const char* );
int tccDcPmra ( Tcl_Interp*, const char*, double*);
int tccDcPmdec ( Tcl_Interp*, const char*, double*);
int tccDcRadec ( Tcl_Interp*, FRAMETYPE, const char*, const char*, 
        double*, double*);
int tccDcT0 ( Tcl_Interp*, const char*, double*);
char* tccDcUc ( const char*, int, char* );
char* tccDcLc ( const char*, int, char* );
int tccDcPlanet( Tcl_Interp*, const char*, int* );

#endif
