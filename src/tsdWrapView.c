static char rcsid[]="$Id:";
/* *INDENT-OFF* */
/*
*   FILENAME
*   tsdWrapView.c
*
*   FUNCTION NAME(S)
*   tsdWrapView - Implements the WrapView canvas item.
*
*   DESCRIPTION
*
*   D L Terrett 10 April 2000
*
*   Copyright CCLRC
*/
/*
 * $Log:
 */
/* *INDENT-ON* */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <tcl.h>
#include <tk.h>
#include <slalib.h>
#include <astLib.h>
#include "tccConstants.h"
#include "tccDecode.h"
#include "tccUtil.h"

/* Procedure definitions for static routines */

static int CreateWV( Tcl_Interp *interp, Tk_Canvas canvas, Tk_Item *itemPtr,
   int argc, char **argv);
static int ConfigureWV( Tcl_Interp *interp, Tk_Canvas canvas, Tk_Item *itemPtr,
   int argc, char **argv, int flags);
static void DeleteWV( Tk_Canvas canvas, Tk_Item *itemPtr, Display *display);
static void DisplayWV( Tk_Canvas canvas, Tk_Item *itemPtr, Display *display,
   Drawable drawable, int x, int y, int width, int height);
static void BboxWV( Tk_Canvas canvas, Tk_Item *itemPtr);
static double PointWV( Tk_Canvas canvas, Tk_Item *itemPtr, double *pointPtr);

/* This structure defines the record for a wrapview item */

typedef struct WrapViewItem {
   Tk_Item header;                      /* Generic stuff */
   double x0, y0;                       /* Centre coordinates */
   short xc, yc;                        /* Canvas coords of centre */
   double sphi;                         /* Sine latitude */
   double cphi;                         /* Cosine latitude */
   double lolimit, hilimit;             /* Cable wrap limits */
   XColor* backgroundColor;
   GC backgroundGC;
   XColor* foregroundColor;
   GC foregroundGC;
   XColor* cableColor;
   GC cableGC;
   XColor* pointerColor;
   GC pointerGC;
   XColor* mechptrColor;
   GC mechptrGC;
   XPoint outline[1001];
   XPoint cable[1450];
   double az;
   double el;
   double ma;
   double mech;
   double mel;
   FRAMETYPE frame;
   int px1, px2, py1, py2;
   int mpx1, mpx2, mpy1, mpy2;
   int npc;
   int rotator;
   int display;
} WrapViewItem;

static Tk_CustomOption tagsOption;

/* Configuration specs structure */

static Tk_ConfigSpec configspecs[] = {

   {TK_CONFIG_COLOR, "-background", (char *) NULL, (char *) NULL, (char *)NULL,
      Tk_Offset(WrapViewItem, backgroundColor), TK_CONFIG_NULL_OK},

   {TK_CONFIG_COLOR, "-foreground", (char *) NULL, (char *) NULL, (char *)NULL,
      Tk_Offset(WrapViewItem, foregroundColor), TK_CONFIG_NULL_OK},

   {TK_CONFIG_DOUBLE, "-lolimit", (char *) NULL, (char *) NULL, "0",
      Tk_Offset(WrapViewItem, lolimit), TK_CONFIG_DONT_SET_DEFAULT},

   {TK_CONFIG_DOUBLE, "-hilimit", (char *) NULL, (char *) NULL, "0",
      Tk_Offset(WrapViewItem, hilimit), TK_CONFIG_DONT_SET_DEFAULT},

   {TK_CONFIG_BOOLEAN, "-rotator", (char *) NULL, (char *) NULL, "0",
      Tk_Offset(WrapViewItem, rotator), TK_CONFIG_DONT_SET_DEFAULT},

   {TK_CONFIG_BOOLEAN, "-display", (char *) NULL, (char *) NULL, (char *) NULL,
      Tk_Offset(WrapViewItem, display), TK_CONFIG_NULL_OK},

   {TK_CONFIG_CUSTOM, "-tags", (char *) NULL, (char *) NULL, (char *) NULL,
      0, TK_CONFIG_NULL_OK, &tagsOption},

   {TK_CONFIG_END, (char *) NULL, (char *) NULL, (char *) NULL, (char *) NULL, 
      0, 0}
};


/* This structure defines the procedures that the generic canvas item code
   calls to implement the wrapview canvas item.
*/

static Tk_ItemType wrapViewType = {
   "wrapview",                          /* Name */
   sizeof(WrapViewItem),                /* Item structure size */
   CreateWV,                            /* Create item proc */
   configspecs,                         /* Configuration specs array */
   ConfigureWV,                         /* Configure proc */
   (Tk_ItemCoordProc *)NULL,            /* Coord proc */
   DeleteWV,                            /* Delete proc */
   DisplayWV,                           /* Display proc */
   0,                                   /* alwaysRedraw */
   PointWV,                             /* Point proc */
   (Tk_ItemAreaProc *) NULL,            /* Area proc */
   (Tk_ItemPostscriptProc *) NULL,      /* Postscript proc */
   (Tk_ItemScaleProc *)NULL,            /* Scale proc */
   (Tk_ItemTranslateProc *)NULL,        /* Translate proc */
   (Tk_ItemIndexProc *) NULL,           /* Index proc */
   (Tk_ItemCursorProc *) NULL,          /* Cursor proc */
   (Tk_ItemSelectionProc *) NULL,       /* Selection proc */
   (Tk_ItemInsertProc *) NULL,          /* Insert proc */
   (Tk_ItemDCharsProc *) NULL,          /* Delete chars proc */
   (Tk_ItemType *) NULL                 /* nextPtr */
};

void tsdWrapView()
{
/* Initialize the tagsOptions structure.  */
   tagsOption.parseProc = Tk_CanvasTagsParseProc;
   tagsOption.printProc = Tk_CanvasTagsPrintProc;
   tagsOption.clientData = (ClientData) NULL;

/* Create the canvas item type. */
   Tk_CreateItemType( &wrapViewType );
   return;
}

static int CreateWV( Tcl_Interp *interp, Tk_Canvas canvas, Tk_Item *itemPtr,
   int argc, char **argv)
{
   WrapViewItem *wrapviewPtr = (WrapViewItem *) itemPtr;
   Tk_Window tkwin = Tk_CanvasTkwin( canvas );
   XGCValues gcvalues;
   unsigned long mask;
   int i;
   double phi, step, s, c, r;

   if (argc < 2 ) {
      Tcl_AppendResult( interp, "wrong # args:  should be \"",
         Tk_PathName(Tk_CanvasTkwin(canvas)), " create ",
         itemPtr->typePtr->name, " x y ?options?\"", (char *) NULL);
      return TCL_ERROR;
   }

/* Initialise the domeview structure so that it can be deleted if something
   goes wrong during the item creation. */
   wrapviewPtr->backgroundColor = NULL;
   wrapviewPtr->backgroundGC = None;
   wrapviewPtr->foregroundColor = NULL;
   wrapviewPtr->foregroundGC = None;
   wrapviewPtr->cableColor = NULL;
   wrapviewPtr->cableGC = None;
   wrapviewPtr->pointerColor = NULL;
   wrapviewPtr->pointerGC = None;
   wrapviewPtr->mechptrColor = NULL;
   wrapviewPtr->mechptrGC = None;

/* Process arguments. */
   if ( (Tk_CanvasGetCoord( interp, canvas, argv[0], 
      &wrapviewPtr->x0) != TCL_OK ) ||
        (Tk_CanvasGetCoord( interp, canvas, argv[1], 
      &wrapviewPtr->y0) != TCL_OK ) ) {
      return TCL_ERROR;
   }

/* Configure options. */
   if ( ConfigureWV( interp, canvas, itemPtr, argc-2, argv+2, 0) != TCL_OK ) {
      DeleteWV( canvas, itemPtr, Tk_Display( Tk_CanvasTkwin(canvas) ) );
      return TCL_ERROR;
   }

/* Set the bounding box */
   BboxWV( canvas, itemPtr);

/* Create fixed GCs */

   mask = GCForeground | GCLineWidth;
   gcvalues.line_width = 2;
   wrapviewPtr->cableColor = Tk_GetColor( interp, tkwin, Tk_GetUid("Orange") );
   gcvalues.foreground = wrapviewPtr->cableColor->pixel;
   wrapviewPtr->cableGC = Tk_GetGC( tkwin, mask, &gcvalues );

   wrapviewPtr->pointerColor = 
      Tk_GetColor( interp, tkwin, Tk_GetUid("DarkGreen") );
   gcvalues.line_width = 4;
   gcvalues.foreground = wrapviewPtr->pointerColor->pixel;
   wrapviewPtr->pointerGC = Tk_GetGC( tkwin, mask, &gcvalues );

   wrapviewPtr->mechptrColor = 
      Tk_GetColor( interp, tkwin, Tk_GetUid("White") );
   gcvalues.line_width = 6;
   gcvalues.foreground = wrapviewPtr->mechptrColor->pixel;
   wrapviewPtr->mechptrGC = Tk_GetGC( tkwin, mask, &gcvalues );

/* Get latitude */
   if ( Tcl_Eval( interp, "sa tcssad get tlatm value" ) != TCL_OK ) 
       return TCL_ERROR;
   if ( Tcl_GetDouble( interp, interp->result, &phi ) != TCL_OK )
       return TCL_ERROR;
   wrapviewPtr->sphi = sin( phi * D2R );
   wrapviewPtr->cphi = cos( phi * D2R );

/* Generate the outline */
#define START_R 40.0
#define PITCH 4.0
#define WIDTH 20.0

   step = (wrapviewPtr->hilimit - wrapviewPtr->lolimit ) * D2R / 500.0;
   for ( i = 0; i < 500; i++ ) {
      s = sin( wrapviewPtr->lolimit * D2R + step * (double) i );
      c = cos( wrapviewPtr->lolimit * D2R + step * (double) i );
      r = START_R + ( step * (double) i * PITCH );
      wrapviewPtr->outline[i].x = (short) ( s * r );
      wrapviewPtr->outline[i].y = - (short) ( c * r );
      r += WIDTH;
      wrapviewPtr->outline[999 - i].x = (short) ( s * r );
      wrapviewPtr->outline[999 - i].y = - (short) ( c * r );
   }
   wrapviewPtr->outline[1000].x = wrapviewPtr->outline[0].x;
   wrapviewPtr->outline[1000].y = wrapviewPtr->outline[0].y;
   wrapviewPtr->xc = 0;
   wrapviewPtr->yc = 0;

/* Set tcs data to safe values. */
   wrapviewPtr->az = 0.0;
   wrapviewPtr->el = 90.0 * D2R;
   wrapviewPtr->frame = AZEL_MNT;
   wrapviewPtr->ma = 0.0;
   wrapviewPtr->mech = 0.0;
   wrapviewPtr->mel = 90.0 * D2R;

   return TCL_OK;
}

static int ConfigureWV( Tcl_Interp *interp, Tk_Canvas canvas, Tk_Item *itemPtr,
   int argc, char **argv, int flags)
{
   WrapViewItem *wrapviewPtr = (WrapViewItem *) itemPtr;
   Tk_Window tkwin = Tk_CanvasTkwin( canvas );
   Display *display = Tk_Display( tkwin );
   GC newGC;
   XGCValues gcvalues;
   unsigned long mask;
   char savest[40];
   int i;
   double az, el, ma, da, daoff, ha, dec, sdec, cdec;
   double s, c, r, r0, r1, a, a0, a1, delta;

   if ( Tk_ConfigureWidget( interp, tkwin, configspecs, argc, argv, 
      (char *) wrapviewPtr, flags ) != TCL_OK ) {
      return TCL_ERROR;
   }

/* Allocate colours and GCs */
   mask = GCForeground;

   if ( configspecs[0].specFlags & TK_CONFIG_OPTION_SPECIFIED ) {
      if ( wrapviewPtr->backgroundColor == NULL ) {
         newGC = None;
      } else {
         gcvalues.foreground = wrapviewPtr->backgroundColor->pixel;
         newGC = Tk_GetGC( tkwin, mask, &gcvalues );
      }
      if ( wrapviewPtr->backgroundGC != None ) {
         Tk_FreeGC( display, wrapviewPtr->backgroundGC );
      }
      wrapviewPtr->backgroundGC = newGC;
   }

   if ( configspecs[1].specFlags & TK_CONFIG_OPTION_SPECIFIED ) {
      if ( wrapviewPtr->foregroundColor == NULL ) {
         newGC = None;
      } else {
         gcvalues.foreground = wrapviewPtr->foregroundColor->pixel;
         newGC = Tk_GetGC( tkwin, mask, &gcvalues );
      }
      if ( wrapviewPtr->foregroundGC != None ) {
         Tk_FreeGC( display, wrapviewPtr->foregroundGC );
      }
      wrapviewPtr->foregroundGC = newGC;
   }

/* Get data from the TCS */
   if ( Tcl_Eval( interp, "sa tcssad get demandAz value" ) == TCL_OK ) {
      strcpy( savest, interp->result);
      if ( Tcl_Eval( interp, "sa tcssad get demandEl value" ) == TCL_OK )
          tccDcRadec( interp, AZEL_MNT, savest, interp->result, 
             &wrapviewPtr->az, &wrapviewPtr->el ); 
   }
   if ( Tcl_Eval( interp, "sa tcssad get mountTrackFrame value" ) == TCL_OK ) 
      tccDcFrame( interp, interp->result, &wrapviewPtr->frame);
   if ( wrapviewPtr->rotator ) {
      if ( Tcl_Eval( interp, "sa tcssad get demandRma value" ) == TCL_OK ) 
         if ( Tcl_GetDouble( interp, interp->result, &wrapviewPtr->ma ) 
            == TCL_OK ) wrapviewPtr->ma *= D2R;
      if ( Tcl_Eval( interp, "sa crssad get currentRma value" ) == TCL_OK ) 
         if ( Tcl_GetDouble( interp, interp->result, &wrapviewPtr->mech ) 
            == TCL_OK ) wrapviewPtr->mech *= D2R;
      if ( wrapviewPtr->frame != AZEL_TOPO ) {
         if ( Tcl_Eval( interp, "sa tcssad get rotTrackFrame value" ) ==
            TCL_OK ) tccDcFrame( interp, interp->result, &wrapviewPtr->frame);
      }
   } else {
      if ( Tcl_Eval( interp, "sa tcssad get currentAz value" ) == TCL_OK ) {
         strcpy( savest, interp->result);
         if ( Tcl_Eval( interp, "sa tcssad get currentEl value" ) == TCL_OK )
            tccDcRadec( interp, AZEL_MNT, savest, interp->result, 
               &wrapviewPtr->mech, &wrapviewPtr->mel );
      }
   }

/* Mechanical position pointer */
   s = sin( wrapviewPtr->mech );
   c = cos( wrapviewPtr->mech );
   r = START_R + ( ( wrapviewPtr->mech - wrapviewPtr->lolimit * D2R ) * PITCH );
   wrapviewPtr->mpx1 = (short) ( s * r ) + wrapviewPtr->xc;
   wrapviewPtr->mpy1 = - (short) ( c * r ) + wrapviewPtr->yc;
   wrapviewPtr->mpx2 = (short) ( s * ( r + WIDTH ) ) + wrapviewPtr->xc;
   wrapviewPtr->mpy2 = - (short) ( c * ( r + WIDTH ) ) + wrapviewPtr->yc;

/* Generate the cable path */
   ma = wrapviewPtr->rotator ? wrapviewPtr->ma : wrapviewPtr->az;
   s = sin( ma );
   c = cos( ma );
   r0 = START_R + ( ( ma - wrapviewPtr->lolimit * D2R ) * PITCH );
   a0 = ma;

/* Demand pointer */
   wrapviewPtr->px1 = (short) ( s * r0 ) + wrapviewPtr->xc;
   wrapviewPtr->py1 = - (short) ( c * r0 ) + wrapviewPtr->yc;
   wrapviewPtr->px2 = (short) ( s * ( r0 + WIDTH ) ) + wrapviewPtr->xc;
   wrapviewPtr->py2 = - (short) ( c * ( r0 + WIDTH ) ) + wrapviewPtr->yc;

   wrapviewPtr->cable[0].x = wrapviewPtr->px1;
   wrapviewPtr->cable[0].y = wrapviewPtr->py1;
   wrapviewPtr->npc = 1;
   if ( wrapviewPtr->frame != AZEL_TOPO ) {
      tccDh2e( wrapviewPtr->az, wrapviewPtr->el, wrapviewPtr->sphi, 
         wrapviewPtr->cphi, &ha, &dec);
      sdec = sin ( dec );
      cdec = cos ( dec );
      if ( wrapviewPtr->rotator )
         da = tccPa( ha, sdec, cdec, wrapviewPtr->sphi, wrapviewPtr->cphi);
      else
         da = wrapviewPtr->az;
      daoff = ma - ( PI - da );
      for ( i = 1; i < 144; i++ ) {
         ha += PI / 144.0;
         tccDe2h( ha, sdec, cdec, wrapviewPtr->sphi, wrapviewPtr->cphi, 
            &az, &el);
         if ( el < 15.0 * D2R ) break;
         if ( wrapviewPtr->rotator ) {
            da = tccPa( ha, sdec, cdec, wrapviewPtr->sphi, wrapviewPtr->cphi);
            ma += slaDrange( ( ( PI - da ) + daoff ) -  ma );
         } else {
            ma += slaDrange( az - ma );
         }
         if ( ma < wrapviewPtr->lolimit * D2R || 
            ma > wrapviewPtr->hilimit * D2R ) break;
         r1 = START_R + ( ( ma - wrapviewPtr->lolimit * D2R ) * PITCH ) +
            (double) i * WIDTH / 144.0;
         a1 = ma;
         if ( a0 < a1 ) {
            delta = 2.0 / r0;
            a = a0;
            r = r0;
            while ( a < a1 ) {
               s = sin( a );
               c = cos( a );
               wrapviewPtr->cable[wrapviewPtr->npc].x = 
                  (short) ( s * r ) + wrapviewPtr->xc;
               wrapviewPtr->cable[wrapviewPtr->npc].y = 
                  - (short) ( c * r ) + wrapviewPtr->yc;
               wrapviewPtr->npc++;
               a += delta;
               r += ( r1 - r0 ) * delta / ( a1 - a0 );
            }
         } else {
            delta = - 2.0 / r0;
            a = a0;
            r = r0;
            while ( a > a1 ) {
               s = sin( a );
               c = cos( a );
               wrapviewPtr->cable[wrapviewPtr->npc].x = 
                  (short) ( s * r ) + wrapviewPtr->xc;
               wrapviewPtr->cable[wrapviewPtr->npc].y = 
                  - (short) ( c * r ) + wrapviewPtr->yc;
               wrapviewPtr->npc++;
               a += delta;
               r += ( r1 - r0 ) * delta / ( a1 - a0 );
            }
         }
         a0 = a1;
         r0 = r1;
      }
   }
   return TCL_OK;
}

static void DeleteWV( Tk_Canvas canvas, Tk_Item *itemPtr, Display *display)
{
   WrapViewItem *wrapviewPtr = (WrapViewItem *) itemPtr;

   if ( wrapviewPtr->backgroundColor != NULL ) {
      Tk_FreeColor( wrapviewPtr->backgroundColor );
   }
   if ( wrapviewPtr->backgroundGC != None ) {
      Tk_FreeGC( display, wrapviewPtr->backgroundGC );
   }
   if ( wrapviewPtr->foregroundColor != NULL ) {
      Tk_FreeColor( wrapviewPtr->foregroundColor );
   }
   if ( wrapviewPtr->foregroundGC != None ) {
      Tk_FreeGC( display, wrapviewPtr->foregroundGC );
   }
   if ( wrapviewPtr->cableColor != NULL ) {
      Tk_FreeColor( wrapviewPtr->cableColor );
   }
   if ( wrapviewPtr->cableGC != None ) {
      Tk_FreeGC( display, wrapviewPtr->cableGC );
   }
   if ( wrapviewPtr->pointerGC != None ) {
      Tk_FreeGC( display, wrapviewPtr->pointerGC );
   }
   if ( wrapviewPtr->pointerColor != NULL ) {
      Tk_FreeColor( wrapviewPtr->pointerColor );
   }
   if ( wrapviewPtr->mechptrGC != None ) {
      Tk_FreeGC( display, wrapviewPtr->mechptrGC );
   }
   if ( wrapviewPtr->mechptrColor != NULL ) {
      Tk_FreeColor( wrapviewPtr->mechptrColor );
   }
}

static void DisplayWV( Tk_Canvas canvas, Tk_Item *itemPtr, Display *display,
   Drawable drawable, int xunused, int yunused, int wunused, int hunused)
{
   WrapViewItem *wrapviewPtr = (WrapViewItem *) itemPtr;
   Tk_Window tkwin = Tk_CanvasTkwin( canvas );
   short xc, yc;
   int i;

   Tk_CanvasDrawableCoords( canvas, wrapviewPtr->x0, wrapviewPtr->y0, &xc, &yc);

/* Adjust the outline coordinates */
   for ( i = 0; i < 1001; i++ ) {
      wrapviewPtr->outline[i].x += xc - wrapviewPtr->xc;
      wrapviewPtr->outline[i].y += yc - wrapviewPtr->yc;
   }

/* Draw the outline */
   XFillPolygon( display, drawable, wrapviewPtr->backgroundGC, 
      wrapviewPtr->outline, 1001, Nonconvex, CoordModeOrigin);
   XDrawLines( display, drawable, wrapviewPtr->foregroundGC, 
      wrapviewPtr->outline, 1001, CoordModeOrigin);

/* Adjust the pointer and cable coordinates */
   wrapviewPtr->mpx1 += xc - wrapviewPtr->xc;
   wrapviewPtr->mpy1 += yc - wrapviewPtr->yc;
   wrapviewPtr->mpx2 += xc - wrapviewPtr->xc;
   wrapviewPtr->mpy2 += yc - wrapviewPtr->yc;
   wrapviewPtr->px1 += xc - wrapviewPtr->xc;
   wrapviewPtr->py1 += yc - wrapviewPtr->yc;
   wrapviewPtr->px2 += xc - wrapviewPtr->xc;
   wrapviewPtr->py2 += yc - wrapviewPtr->yc;
   for ( i = 0; i < wrapviewPtr->npc; i++ ) {
      wrapviewPtr->cable[i].x += xc - wrapviewPtr->xc;
      wrapviewPtr->cable[i].y += yc - wrapviewPtr->yc;
   }
   
/* Draw the pointers */
   XDrawLine( display, drawable, wrapviewPtr->mechptrGC, wrapviewPtr->mpx1,
      wrapviewPtr->mpy1, wrapviewPtr->mpx2, wrapviewPtr->mpy2);
   XDrawLine( display, drawable, wrapviewPtr->pointerGC, wrapviewPtr->px1,
      wrapviewPtr->py1, wrapviewPtr->px2, wrapviewPtr->py2);

/* Draw the cable path */
   for ( i = 0; i < wrapviewPtr->npc; i++ ) {
      XDrawLine( display, drawable, wrapviewPtr->cableGC,
         wrapviewPtr->cable[i].x - 1, wrapviewPtr->cable[i].y - 1,
         wrapviewPtr->cable[i].x + 1, wrapviewPtr->cable[i].y + 1);
   }

/* Save the drawable coordinate offset */
   wrapviewPtr->xc = xc;
   wrapviewPtr->yc = yc;
}

#define PICSIZE 100

static void BboxWV( Tk_Canvas canvas, Tk_Item *itemPtr)
{
   WrapViewItem *wrapviewPtr = (WrapViewItem *) itemPtr;
   wrapviewPtr->header.x1 = wrapviewPtr->x0 - PICSIZE;
   wrapviewPtr->header.y1 = wrapviewPtr->y0 - PICSIZE;
   wrapviewPtr->header.x2 = wrapviewPtr->x0 + PICSIZE;
   wrapviewPtr->header.y2 = wrapviewPtr->y0 + PICSIZE;
}

static double PointWV( Tk_Canvas canvas, Tk_Item *itemPtr, double *pointPtr)
{
   WrapViewItem *wrapviewPtr = (WrapViewItem *) itemPtr;
   return sqrt( (wrapviewPtr->x0 - pointPtr[0]) *
                (wrapviewPtr->x0 - pointPtr[0]) +
                (wrapviewPtr->y0 - pointPtr[1]) *
                (wrapviewPtr->y0 - pointPtr[1]) );
}
