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
 * $Log: not supported by cvs2svn $
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
int tccDcDt ( Tcl_Interp*, FRAMETYPE, char*, char*, double*, double*);
int tccDcEpoch ( Tcl_Interp*, char*, char*, double*);
int tccDcFrame ( Tcl_Interp*, char*, FRAMETYPE*);
int tccDcLen ( char* );
int tccDcPmra ( Tcl_Interp*, char*, double*);
int tccDcPmdec ( Tcl_Interp*, char*, double*);
int tccDcRadec ( Tcl_Interp*, FRAMETYPE, char*, char*, double*, double*);
int tccDcT0 ( Tcl_Interp*, char*, double*);
char* tccDcUc ( char*, int, char* );
char* tccDcLc ( char*, int, char* );
int tccDcPlanet( Tcl_Interp*, char*, int* );

#endif
