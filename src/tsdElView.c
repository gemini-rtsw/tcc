static char rcsid[]="$Id:";
/* *INDENT-OFF* */
/*
*   FILENAME
*   tsdElView.c
*
*   FUNCTION NAME(S)
*   tsdElView - Implements the ElView canvas item.
*
*   DESCRIPTION
*
*   WARNING: THIS CODE WILL BREAK AT TCL 8.4
*            The interface to Tk_ItemCreatProc, Tk_ItemConfigureProc and
*            Tk_ItemCoordProc have been changed to use object instead of
*            strings for the command arguments.
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

static int CreateEV( Tcl_Interp *interp, Tk_Canvas canvas, Tk_Item *itemPtr,
   int argc, char **argv);
static int ConfigureEV( Tcl_Interp *interp, Tk_Canvas canvas, Tk_Item *itemPtr,
   int argc, char **argv, int flags);
static void DeleteEV( Tk_Canvas canvas, Tk_Item *itemPtr, Display *display);
static void DisplayEV( Tk_Canvas canvas, Tk_Item *itemPtr, Display *display,
   Drawable drawable, int x, int y, int width, int height);
static void BboxEV( Tk_Canvas canvas, Tk_Item *itemPtr);
static double PointEV( Tk_Canvas canvas, Tk_Item *itemPtr, double *pointPtr);

/* This structure defines the record for a elview item */

typedef struct ElViewItem {
   Tk_Item header;                      /* Generic stuff */
   double x0, y0;                       /* Centre coordinates */
   short xc, yc;                        /* Canvas coords of centre */
   double sphi;                         /* Sine latitude */
   double cphi;                         /* Cosine latitude */
   XColor* elpathColor;
   GC elpathGC;
   XColor* amlimColor;
   GC amlimGC;
   XColor* pointerColor;
   GC pointerGC;
   XColor* mechptrColor;
   GC mechptrGC;
   XPoint elpath[145];
   XPoint amlim[2];
   XPoint pointer[2];
   XPoint mechptr[2];
   double el;
   double az;
   double maz;
   double mel;
   double amlimel;
   FRAMETYPE frame;
   int npc;
   int display;
} ElViewItem;

static Tk_CustomOption tagsOption;

/* Configuration specs structure */

static Tk_ConfigSpec configspecs[] = {

   {TK_CONFIG_BOOLEAN, "-display", (char *) NULL, (char *) NULL, (char *) NULL,
      Tk_Offset(ElViewItem, display), TK_CONFIG_NULL_OK},

   {TK_CONFIG_CUSTOM, "-tags", (char *) NULL, (char *) NULL, (char *) NULL,
      0, TK_CONFIG_NULL_OK, &tagsOption},

   {TK_CONFIG_END, (char *) NULL, (char *) NULL, (char *) NULL, (char *) NULL, 
      0, 0}
};


/* This structure defines the procedures that the generic canvas item code
   calls to implement the domeview canvas item.
*/

static Tk_ItemType elViewType = {
   "elview",                            /* Name */
   sizeof(ElViewItem),                  /* Item structure size */
   CreateEV,                            /* Create item proc */
   configspecs,                         /* Configuration specs array */
   ConfigureEV,                         /* Configure proc */
   (Tk_ItemCoordProc *)NULL,            /* Coord proc */
   DeleteEV,                            /* Delete proc */
   DisplayEV,                           /* Display proc */
   0,                                   /* alwaysRedraw */
   PointEV,                             /* Point proc */
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

void tsdElView()
{
/* Initialize the tagsOptions structure.  */
   tagsOption.parseProc = Tk_CanvasTagsParseProc;
   tagsOption.printProc = Tk_CanvasTagsPrintProc;
   tagsOption.clientData = (ClientData) NULL;

/* Create the canvas item type. */
   Tk_CreateItemType( &elViewType );
   return;
}

static int CreateEV( Tcl_Interp *interp, Tk_Canvas canvas, Tk_Item *itemPtr,
   int argc, char **argv)
{
   ElViewItem *elviewPtr = (ElViewItem *) itemPtr;
   Tk_Window tkwin = Tk_CanvasTkwin( canvas );
   XGCValues gcvalues;
   unsigned long mask;
   double phi;

   if (argc < 2 ) {
      Tcl_AppendResult( interp, "wrong # args:  should be \"",
         Tk_PathName(Tk_CanvasTkwin(canvas)), " create ",
         itemPtr->typePtr->name, " x y ?options?\"", (char *) NULL);
      return TCL_ERROR;
   }

/* Initialise the elview structure so that it can be deleted if something
   goes wrong during the item creation. */
   elviewPtr->elpathColor = NULL;
   elviewPtr->elpathGC = None;
   elviewPtr->amlimColor = NULL;
   elviewPtr->amlimGC = None;
   elviewPtr->pointerColor = NULL;
   elviewPtr->pointerGC = None;
   elviewPtr->mechptrColor = NULL;
   elviewPtr->mechptrGC = None;

/* Process arguments. */
   if ( (Tk_CanvasGetCoord( interp, canvas, argv[0], 
      &elviewPtr->x0) != TCL_OK ) ||
        (Tk_CanvasGetCoord( interp, canvas, argv[1], 
      &elviewPtr->y0) != TCL_OK ) ) {
      return TCL_ERROR;
   }

/* Configure options. */
   if ( ConfigureEV( interp, canvas, itemPtr, argc-2, argv+2, 0) != TCL_OK ) {
      DeleteEV( canvas, itemPtr, Tk_Display( Tk_CanvasTkwin(canvas) ) );
      return TCL_ERROR;
   }

/* Set the bounding box */
   BboxEV( canvas, itemPtr);

/* Create fixed GCs */

   mask = GCForeground | GCLineWidth;
   gcvalues.line_width = 2;
   elviewPtr->elpathColor = Tk_GetColor( interp, tkwin, Tk_GetUid("Orange") );
   gcvalues.foreground = elviewPtr->elpathColor->pixel;
   elviewPtr->elpathGC = Tk_GetGC( tkwin, mask, &gcvalues );

   elviewPtr->pointerColor = 
      Tk_GetColor( interp, tkwin, Tk_GetUid("DarkGreen") );
   gcvalues.line_width = 4;
   gcvalues.foreground = elviewPtr->pointerColor->pixel;
   elviewPtr->pointerGC = Tk_GetGC( tkwin, mask, &gcvalues );

   elviewPtr->mechptrColor = 
      Tk_GetColor( interp, tkwin, Tk_GetUid("White") );
   gcvalues.line_width = 6;
   gcvalues.foreground = elviewPtr->mechptrColor->pixel;
   elviewPtr->mechptrGC = Tk_GetGC( tkwin, mask, &gcvalues );

   elviewPtr->amlimColor = 
      Tk_GetColor( interp, tkwin, Tk_GetUid("DarkGreen") );
   mask &= ~GCLineWidth;
   mask |= GCLineStyle;
   gcvalues.line_style = LineOnOffDash;
   gcvalues.foreground = elviewPtr->amlimColor->pixel;
   elviewPtr->amlimGC = Tk_GetGC( tkwin, mask, &gcvalues );

/* Get latitude */
   if ( Tcl_Eval( interp, "sa tcssad get tlatm value" ) != TCL_OK ) 
       return TCL_ERROR;
   if ( Tcl_GetDouble( interp, interp->result, &phi ) != TCL_OK )
       return TCL_ERROR;
   elviewPtr->sphi = sin( phi * D2R );
   elviewPtr->cphi = cos( phi * D2R );

/* Initialise drawable offset */
   elviewPtr->xc = 0;
   elviewPtr->yc = 0;

/* Initialise telescope data to safe values. */
   elviewPtr->el = 90.0 * D2R;
   elviewPtr->az = 0.0;
   elviewPtr->mel = 90.0 * D2R;
   elviewPtr->maz = 0.0;
   elviewPtr->frame = AZEL_MNT;
   elviewPtr->amlimel = 90.0 * D2R;

   return TCL_OK;
}

static int ConfigureEV( Tcl_Interp *interp, Tk_Canvas canvas, Tk_Item *itemPtr,
   int argc, char **argv, int flags)
{
   ElViewItem *elviewPtr = (ElViewItem *) itemPtr;
   Tk_Window tkwin = Tk_CanvasTkwin( canvas );
   Display *display = Tk_Display( tkwin );
   GC newGC;
   XGCValues gcvalues;
   unsigned long mask;
   char savest[40];
   int i;
   double az, el, ha, dec, sdec, cdec;

   if ( Tk_ConfigureWidget( interp, tkwin, configspecs, argc, argv, 
      (char *) elviewPtr, flags ) != TCL_OK ) {
      return TCL_ERROR;
   }

/* Get data from the TCS */
   if ( Tcl_Eval( interp, "sa tcssad get demandAz value" ) == TCL_OK ) {
       strcpy( savest, interp->result);
       if ( Tcl_Eval( interp, "sa tcssad get demandEl value" ) == TCL_OK )
           tccDcRadec( interp, AZEL_MNT, savest, interp->result, 
              &elviewPtr->az, &elviewPtr->el);
   }
   if ( Tcl_Eval( interp, "sa tcssad get mountTrackFrame value" ) == TCL_OK ) 
      tccDcFrame( interp, interp->result, &elviewPtr->frame);
   if ( Tcl_Eval( interp, "sa tcssad get airMassLimitEl value" ) == TCL_OK ) 
       if ( Tcl_GetDouble( interp, interp->result, &elviewPtr->amlimel ) 
           == TCL_OK ) elviewPtr->amlimel *= D2R;
   if ( Tcl_Eval( interp, "sa tcssad get currentAz value" ) == TCL_OK ) {
       strcpy( savest, interp->result);
       if ( Tcl_Eval( interp, "sa tcssad get currentEl value" ) == TCL_OK )
           tccDcRadec( interp, AZEL_MNT, savest, interp->result, 
               &elviewPtr->maz, &elviewPtr->mel );
   }

/* Generate the pointers */
   elviewPtr->pointer[0].x = 1 + elviewPtr->xc;
   elviewPtr->pointer[0].y = 
            (short) ( ( D90 - elviewPtr->el ) * 110.0 / ( 75.0 * D2R ) ) + 
            elviewPtr->yc;
   elviewPtr->pointer[1].x = 20 + elviewPtr->xc;
   elviewPtr->pointer[1].y = elviewPtr->pointer[0].y;

   elviewPtr->mechptr[0].x = 1 + elviewPtr->xc;
   elviewPtr->mechptr[0].y = 
            (short) ( ( D90 - elviewPtr->mel ) * 110.0 / ( 75.0 * D2R ) ) 
            + elviewPtr->yc;
   elviewPtr->mechptr[1].x = 20 + elviewPtr->xc;
   elviewPtr->mechptr[1].y = elviewPtr->mechptr[0].y;

/* Generate the elevation path */
   elviewPtr->npc = 0;
   if ( elviewPtr->frame != AZEL_TOPO ) {
      tccDh2e( elviewPtr->az, elviewPtr->el, elviewPtr->sphi, elviewPtr->cphi, 
          &ha, &dec);
      sdec = sin ( dec );
      cdec = cos ( dec );
      for ( i = 0; i < 144; i++ ) {
         tccDe2h( ha, sdec, cdec, elviewPtr->sphi, elviewPtr->cphi, 
            &az, &el);
         elviewPtr->elpath[i].x = (short) ( (double) i * 280.0 / 144.0 ) + 20
            + elviewPtr->xc;
         elviewPtr->elpath[i].y = 
            (short) ( ( D90 - el ) * 110.0 / ( 75.0 * D2R ) ) + elviewPtr->yc;
         elviewPtr->npc++;
         if ( el < 14.99 * D2R ) break;
         ha += PI / 144.0;
      }
   }

/* Generate the airmass limit */
   elviewPtr->amlim[0].x = 20 + elviewPtr->xc;
   elviewPtr->amlim[0].y =
      (short) ( ( D90 - elviewPtr->amlimel ) * 110.0 / ( 75.0 * D2R ) )
      + elviewPtr->yc;
   elviewPtr->amlim[1].x = 300 + elviewPtr->xc;
   elviewPtr->amlim[1].y = elviewPtr->amlim[0].y;

   return TCL_OK;
}

static void DeleteEV( Tk_Canvas canvas, Tk_Item *itemPtr, Display *display)
{
   ElViewItem *elviewPtr = (ElViewItem *) itemPtr;

   if ( elviewPtr->elpathColor != NULL ) {
      Tk_FreeColor( elviewPtr->elpathColor );
   }
   if ( elviewPtr->elpathGC != None ) {
      Tk_FreeGC( display, elviewPtr->elpathGC );
   }
   if ( elviewPtr->amlimColor != NULL ) {
      Tk_FreeColor( elviewPtr->amlimColor );
   }
   if ( elviewPtr->amlimGC != None ) {
      Tk_FreeGC( display, elviewPtr->amlimGC );
   }
   if ( elviewPtr->pointerColor != NULL ) {
      Tk_FreeColor( elviewPtr->pointerColor );
   }
   if ( elviewPtr->pointerGC != None ) {
      Tk_FreeGC( display, elviewPtr->pointerGC );
   }
   if ( elviewPtr->mechptrColor != NULL ) {
      Tk_FreeColor( elviewPtr->mechptrColor );
   }
   if ( elviewPtr->mechptrGC != None ) {
      Tk_FreeGC( display, elviewPtr->mechptrGC );
   }
}

static void DisplayEV( Tk_Canvas canvas, Tk_Item *itemPtr, Display *display,
   Drawable drawable, int xunused, int yunused, int wunused, int hunused)
{
   ElViewItem *elviewPtr = (ElViewItem *) itemPtr;
   Tk_Window tkwin = Tk_CanvasTkwin( canvas );
   short xc, yc;
   int i;

   Tk_CanvasDrawableCoords( canvas, elviewPtr->x0, elviewPtr->y0, &xc, &yc);

/* Adjust the coordinates */
   elviewPtr->pointer[0].x += xc - elviewPtr->xc;
   elviewPtr->pointer[0].y += yc - elviewPtr->yc;
   elviewPtr->pointer[1].x += xc - elviewPtr->xc;
   elviewPtr->pointer[1].y += yc - elviewPtr->yc;
   elviewPtr->mechptr[0].x += xc - elviewPtr->xc;
   elviewPtr->mechptr[0].y += yc - elviewPtr->yc;
   elviewPtr->mechptr[1].x += xc - elviewPtr->xc;
   elviewPtr->mechptr[1].y += yc - elviewPtr->yc;
   elviewPtr->amlim[0].x += xc - elviewPtr->xc;
   elviewPtr->amlim[0].y += yc - elviewPtr->yc;
   elviewPtr->amlim[1].x += xc - elviewPtr->xc;
   elviewPtr->amlim[1].y += yc - elviewPtr->yc;
   for ( i = 0; i < elviewPtr->npc; i++ ) {
      elviewPtr->elpath[i].x += xc - elviewPtr->xc;
      elviewPtr->elpath[i].y += yc - elviewPtr->yc;
   }
   
/* Draw the ponters */
   XDrawLine( display, drawable, elviewPtr->mechptrGC, 
      elviewPtr->mechptr[0].x, elviewPtr->mechptr[0].y,
      elviewPtr->mechptr[1].x, elviewPtr->mechptr[1].y);
   XDrawLine( display, drawable, elviewPtr->pointerGC, 
      elviewPtr->pointer[0].x, elviewPtr->pointer[0].y,
      elviewPtr->pointer[1].x, elviewPtr->pointer[1].y);

/* Draw the elevation path */
   XDrawLines( display, drawable, elviewPtr->elpathGC, 
         elviewPtr->elpath, elviewPtr->npc, CoordModeOrigin);

/* Draw the airmass limit */
   XDrawLine( display, drawable, elviewPtr->amlimGC, 
      elviewPtr->amlim[0].x, elviewPtr->amlim[0].y,
      elviewPtr->amlim[1].x, elviewPtr->amlim[1].y);

/* Save the drawable coordinate offset */
   elviewPtr->xc = xc;
   elviewPtr->yc = yc;
}

static void BboxEV( Tk_Canvas canvas, Tk_Item *itemPtr)
{
   ElViewItem *elviewPtr = (ElViewItem *) itemPtr;
   elviewPtr->header.x1 = elviewPtr->x0;
   elviewPtr->header.y1 = elviewPtr->y0 - 3;
   elviewPtr->header.x2 = elviewPtr->x0 + 300;
   elviewPtr->header.y2 = elviewPtr->y0 + 116;
}

static double PointEV( Tk_Canvas canvas, Tk_Item *itemPtr, double *pointPtr)
{
   ElViewItem *elviewPtr = (ElViewItem *) itemPtr;
   return sqrt( (elviewPtr->x0 - pointPtr[0]) *
                (elviewPtr->x0 - pointPtr[0]) +
                (elviewPtr->y0 - pointPtr[1]) *
                (elviewPtr->y0 - pointPtr[1]) );
}
