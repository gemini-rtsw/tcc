static char rcsid[]="$Id:";
/* *INDENT-OFF* */
/*
*   FILENAME
*   tsdDomeView.c
*
*   FUNCTION NAME(S)
*   tsdDomeView - Implements the DomeView canvas item.
*
*   DESCRIPTION
*   The domeview canvas item draws a picture of the Gemini dome with the 
*   position of the shutters and widn gates, the projection of the mirror
*   onto the dome surface, the position and track of the current target and
*   the position of the moon and sun.
*
*   The only configuration option is -display which merely serves as a way
*   of forcing the item to be redrawn and new data fetched from the TCS.
*
*   WARNING: THIS CODE WILL BREAK AT TCL 8.4
*            The interface to Tk_ItemCreatProc, Tk_ItemConfigureProc and
*            Tk_ItemCoordProc have been changed to use object instead of
*            strings for the command arguments.
*
*   D L Terrett 27 June 2001
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
#include <timeLib.h>
#include "tccConstants.h"
#include "tccDecode.h"
#include "tccUtil.h"

/* Procedure definitions for static routines */

static int CreateDV( Tcl_Interp *interp, Tk_Canvas canvas, Tk_Item *itemPtr,
   int argc, char **argv);
static int ConfigureDV( Tcl_Interp *interp, Tk_Canvas canvas, Tk_Item *itemPtr,
   int argc, char **argv, int flags);
static void DeleteDV( Tk_Canvas canvas, Tk_Item *itemPtr, Display *display);
static void DisplayDV( Tk_Canvas canvas, Tk_Item *itemPtr, Display *display,
   Drawable drawable, int x, int y, int width, int height);
static int OverlapDV( Tk_Canvas canvas, Tk_Item *itemPtr, double *areaPtr);
static void BboxDV( Tk_Canvas canvas, Tk_Item *itemPtr);
static double PointDV( Tk_Canvas canvas, Tk_Item *itemPtr, double *pointPtr);
static void project( short xc, short yc, double r, double azr, double el, 
   double w, double h, XPoint *point);

/* This structure defines the record for a domeview item */

typedef struct DomeViewItem {
   Tk_Item header;                      /* Generic stuff */
   double x0, y0;                       /* Centre coordinates */
   int display;
   double amLimEl;                      /* Airmass limit elevation */
   double domeaz;                       /* Dome azimuth */
   double telEl;                        /* Telescope elevation */
   double telAz;                        /* Telescope azimuth */
   double oldTelEl;
   double oldTelAz;
   double oldEl;
   double oldAz;
   double evgPos;                       /* East vent gate position */
   double wvgPos;                       /* West vent gate position */
   double botShutEl;                    /* Bottom shutter elevation */
   double topShutEl;                    /* Top shutter elevation */
   double ra;                           /* Target RA */
   double dec;                          /* Target declination */
   FRAMETYPE system;                    /* System */
   struct EPOCH equinox;                /* Equinox */
   double oldRa;
   double oldDec;
   FRAMETYPE oldSystem;
   struct EPOCH oldEquinox;
   double aoprms[15];                   /* Apparent to observed params */
   short xc;
   short yc;
   XPoint mp[101];
   int mpvalid;
   XPoint tp[20];
   int tpvalid;
   XColor* domeLowColor;
   GC domeLowGC;
   XColor* domeMidColor;
   GC domeMidGC;
   XColor* domeHiColor;
   GC domeHiGC;
   XColor* mirrorColor;
   GC mirrorGC;
   XColor* blackColor;
   GC blackGC;
   XColor* targetColor;
   GC targetGC;
   XColor* moonColor;
   GC moonGC;
   GC blackdotGC;
   GC evgGC;
   GC wvgGC;
   GC vglabGC;
   XColor* vglabColor;
} DomeViewItem;

static Tk_CustomOption tagsOption;

/* Configuration specs structure */

static Tk_ConfigSpec configspecs[] = {

   {TK_CONFIG_BOOLEAN, "-display", (char *) NULL, (char *) NULL, (char *) NULL,
      Tk_Offset(DomeViewItem, display), TK_CONFIG_NULL_OK},

   {TK_CONFIG_CUSTOM, "-tags", (char *) NULL, (char *) NULL, (char *) NULL,
      0, TK_CONFIG_NULL_OK, &tagsOption},

   {TK_CONFIG_END, (char *) NULL, (char *) NULL, (char *) NULL, (char *) NULL, 
      0, 0}
};


/* This structure defines the procedures that the generic canvas item code
   calls to implement the domeview canvas item.
*/

static Tk_ItemType domeViewType = {
   "domeview",                          /* Name */
   sizeof(DomeViewItem),                /* Item structure size */
   CreateDV,                            /* Create item proc */
   configspecs,                         /* Configuration specs array */
   ConfigureDV,                         /* Configure proc */
   (Tk_ItemCoordProc *)NULL,            /* Coord proc */
   DeleteDV,                            /* Delete proc */
   DisplayDV,                           /* Display proc */
   0,                                   /* alwaysRedraw */
   PointDV,                             /* Point proc */
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

void tsdDomeView()
{
/* Initialize the tagsOptions structure.  */
   tagsOption.parseProc = Tk_CanvasTagsParseProc;
   tagsOption.printProc = Tk_CanvasTagsPrintProc;
   tagsOption.clientData = (ClientData) NULL;

/* Create the canvas item type. */
   Tk_CreateItemType( &domeViewType );
   return;
}

static int CreateDV( Tcl_Interp *interp, Tk_Canvas canvas, Tk_Item *itemPtr,
   int argc, char **argv)
{
   DomeViewItem *domeviewPtr = (DomeViewItem *) itemPtr;
   Tk_Window tkwin = Tk_CanvasTkwin( canvas );
   XGCValues gcvalues;
   unsigned long mask;
   double date, dut, elongm, elatm, hm;
   double elat, elong;

   if (argc < 2 ) {
      Tcl_AppendResult( interp, "wrong # args:  should be \"",
         Tk_PathName(Tk_CanvasTkwin(canvas)), " create ",
         itemPtr->typePtr->name, " x y ?options?\"", (char *) NULL);
      return TCL_ERROR;
   }

/* Initialise the domeview structure so that it can be deleted if something
   goes wrong during the item creation. */
   domeviewPtr->domeLowColor = NULL;
   domeviewPtr->domeMidColor = NULL;
   domeviewPtr->domeHiColor = NULL;
   domeviewPtr->mirrorColor = NULL;
   domeviewPtr->blackColor = NULL;
   domeviewPtr->targetColor = NULL;
   domeviewPtr->moonColor = NULL;
   domeviewPtr->vglabColor = NULL;
   domeviewPtr->domeLowGC = None;
   domeviewPtr->domeMidGC = None;
   domeviewPtr->domeHiGC = None;
   domeviewPtr->mirrorGC = None;
   domeviewPtr->targetGC = None;
   domeviewPtr->moonGC = None;
   domeviewPtr->blackGC = None;
   domeviewPtr->blackdotGC = None;
   domeviewPtr->evgGC = None;
   domeviewPtr->wvgGC = None;
   domeviewPtr->vglabGC = None;

   domeviewPtr->mpvalid = 0;
   domeviewPtr->tpvalid = 0;

/* Process arguments. */
   if ( (Tk_CanvasGetCoord( interp, canvas, argv[0], 
      &domeviewPtr->x0) != TCL_OK ) ||
        (Tk_CanvasGetCoord( interp, canvas, argv[1], 
      &domeviewPtr->y0) != TCL_OK ) ) {
      return TCL_ERROR;
   }

/* Allocate colours and GCs */
   mask = GCForeground;

   domeviewPtr->domeLowColor = 
      Tk_GetColor( interp, tkwin, Tk_GetUid("LightGrey") );
   gcvalues.foreground = domeviewPtr->domeLowColor->pixel;
   domeviewPtr->domeLowGC = Tk_GetGC( tkwin, mask, &gcvalues );

   domeviewPtr->domeMidColor = 
      Tk_GetColor( interp, tkwin, Tk_GetUid("#F0F0F0") );
   gcvalues.foreground = domeviewPtr->domeMidColor->pixel;
   domeviewPtr->domeMidGC = Tk_GetGC( tkwin, mask, &gcvalues );

   domeviewPtr->domeHiColor = Tk_GetColor( interp, tkwin, Tk_GetUid("white") );
   gcvalues.foreground = domeviewPtr->domeHiColor->pixel;
   domeviewPtr->domeHiGC = Tk_GetGC( tkwin, mask, &gcvalues );

   domeviewPtr->mirrorColor = Tk_GetColor( interp, tkwin, Tk_GetUid("white") );
   gcvalues.foreground = domeviewPtr->mirrorColor->pixel;
   domeviewPtr->mirrorGC = Tk_GetGC( tkwin, mask, &gcvalues );

   domeviewPtr->targetColor = Tk_GetColor( interp, tkwin, Tk_GetUid("orange") );
   gcvalues.foreground = domeviewPtr->targetColor->pixel;
   domeviewPtr->targetGC = Tk_GetGC( tkwin, mask, &gcvalues );

   domeviewPtr->moonColor = 
      Tk_GetColor( interp, tkwin, Tk_GetUid("PowderBlue") );
   gcvalues.foreground = domeviewPtr->moonColor->pixel;
   domeviewPtr->moonGC = Tk_GetGC( tkwin, mask, &gcvalues );

   domeviewPtr->vglabColor = 
      Tk_GetColor( interp, tkwin, Tk_GetUid("Yellow") );
   gcvalues.foreground = domeviewPtr->vglabColor->pixel;
   gcvalues.line_width = 3.0;
   mask |= GCLineWidth;
   domeviewPtr->vglabGC = Tk_GetGC( tkwin, mask, &gcvalues );

   domeviewPtr->blackColor = Tk_GetColor( interp, tkwin, Tk_GetUid("black") );
   gcvalues.foreground = domeviewPtr->blackColor->pixel;
   domeviewPtr->blackGC = Tk_GetGC( tkwin, mask, &gcvalues );
   gcvalues.line_style = LineOnOffDash;
   gcvalues.line_width = 0.0;
   mask |= GCLineStyle;
   domeviewPtr->blackdotGC = Tk_GetGC( tkwin, mask, &gcvalues );

/* Configure options. */
   if ( ConfigureDV( interp, canvas, itemPtr, argc-2, argv+2, 0) != TCL_OK ) {
      DeleteDV( canvas, itemPtr, Tk_Display( Tk_CanvasTkwin(canvas) ) );
      return TCL_ERROR;
   }

/* Set the bounding box */
   BboxDV( canvas, itemPtr);

/* Fill out the appt to obs parameters array. Canonical values (except for
   the telescope location) are used as nothing else matters at the
   resolution we get from an X display. */
   timeNowD( UTC, &date );
   if ( Tcl_Eval( interp, "sa tcssad get delut value" ) != TCL_OK ) 
       return TCL_ERROR;
   if ( Tcl_GetDouble( interp, interp->result, &dut ) != TCL_OK )
       return TCL_ERROR;
   if ( Tcl_Eval( interp, "sa tcssad get tlongm value" ) != TCL_OK ) 
       return TCL_ERROR;
   if ( Tcl_GetDouble( interp, interp->result, &elongm ) != TCL_OK )
       return TCL_ERROR;
   if ( Tcl_Eval( interp, "sa tcssad get tlatm value" ) != TCL_OK ) 
       return TCL_ERROR;
   if ( Tcl_GetDouble( interp, interp->result, &elatm ) != TCL_OK )
       return TCL_ERROR;
   if ( Tcl_Eval( interp, "sa tcssad get height value" ) != TCL_OK ) 
       return TCL_ERROR;
   if ( Tcl_GetDouble( interp, interp->result, &hm ) != TCL_OK )
       return TCL_ERROR;
   slaAoppa( date, dut, elongm * D2R, elatm * D2R, hm, 0.0, 0.0, 275.0, 605.0, 
      0.8, 0.5, 0.0065, domeviewPtr->aoprms );
   slaPolmo( elongm * D2R, elatm * D2R, 0.0, 0.0, &elong, &elat, 
      &domeviewPtr->aoprms[14] );

/* Initial safe values in case the TCS is not ready with valid data. */
   domeviewPtr->amLimEl = 30.0 * D2R;
   domeviewPtr->domeaz = 0.0;
   domeviewPtr->telEl = D90;
   domeviewPtr->telAz = D90;
   domeviewPtr->oldTelEl = D90;
   domeviewPtr->oldTelAz = D90;
   domeviewPtr->oldEl = D90;
   domeviewPtr->oldAz = D90;
   domeviewPtr->evgPos = 0.0;
   domeviewPtr->wvgPos = 0.0;
   domeviewPtr->botShutEl = 0.0;
   domeviewPtr->topShutEl = 0.0;
   domeviewPtr->ra = D90;
   domeviewPtr->dec = D90;
   domeviewPtr->system = AZEL_TOPO;
   return TCL_OK;
}

#define GATEWIDTH 40.0

static int ConfigureDV( Tcl_Interp *interp, Tk_Canvas canvas, Tk_Item *itemPtr,
   int argc, char **argv, int flags)
{
/* This routine is called each time the -display option is configured. We
   use it as an opportunity to refresh the data from the TCS. */

   DomeViewItem *domeviewPtr = (DomeViewItem *) itemPtr;
   Tk_Window tkwin = Tk_CanvasTkwin( canvas );
   Display *display = Tk_Display( tkwin );
   unsigned long mask;
   XGCValues gcvalues;
   double vgpos;
   char savest[40];

   if ( Tk_ConfigureWidget( interp, tkwin, configspecs, argc, argv, 
      (char *) domeviewPtr, flags ) != TCL_OK ) {
      return TCL_ERROR;
   }

/* If the vent gate positions have changed new GC for drawing them have to
   be allocated. */
   vgpos = 0.0;
   if ( Tcl_Eval( interp, "sa ecssad get eastVentGatePos value" ) == TCL_OK )
       Tcl_GetDouble( interp, interp->result, &vgpos );
   if ( vgpos > 100.0 ) vgpos = 100.0;
   mask = GCForeground | GCLineWidth;
   gcvalues.foreground = domeviewPtr->blackColor->pixel;
   if ( fabs( domeviewPtr->evgPos - vgpos ) > 0.05 ) {
      if ( domeviewPtr->evgGC != None ) {
         Tk_FreeGC( display, domeviewPtr->evgGC );
         domeviewPtr->evgGC = None;
      }
      if ( vgpos > 0.0 ) {
         gcvalues.line_width = (int) ceil( GATEWIDTH * vgpos / 100.0 );
         domeviewPtr->evgGC = Tk_GetGC( tkwin, mask, &gcvalues );
      }
      domeviewPtr->evgPos = vgpos;
   }
   vgpos = 0.0;
   if ( Tcl_Eval( interp, "sa ecssad get westVentGatePos value" ) == TCL_OK )
       Tcl_GetDouble( interp, interp->result, &vgpos );
   if ( vgpos > 100.0 ) vgpos = 100.0;
   if ( fabs( domeviewPtr->wvgPos - vgpos ) > 0.05 ) {
      if ( domeviewPtr->wvgGC != None ) {
         Tk_FreeGC( display, domeviewPtr->wvgGC );
         domeviewPtr->wvgGC = None;
      }
      if ( vgpos > 0.0 ) {
         gcvalues.line_width = (int) ceil( GATEWIDTH * vgpos / 100.0 );
         domeviewPtr->wvgGC = Tk_GetGC( tkwin, mask, &gcvalues );
      }
      domeviewPtr->wvgPos = vgpos;
   }

   if ( Tcl_Eval( interp, "sa tcssad get airMassLimitEl value" ) == TCL_OK )
       Tcl_GetDouble( interp, interp->result, &domeviewPtr->amLimEl );
   if ( Tcl_Eval( interp, "sa ecssad get topShtrPos value" ) == TCL_OK ) 
       Tcl_GetDouble( interp, interp->result, &domeviewPtr->topShutEl );
   if ( Tcl_Eval( interp, "sa ecssad get botShtrPos value" ) == TCL_OK ) 
       Tcl_GetDouble( interp, interp->result, &domeviewPtr->botShutEl );

   if ( Tcl_Eval( interp, "sa ecssad get domePos value" ) == TCL_OK ) 
       Tcl_GetDouble( interp, interp->result, &domeviewPtr->domeaz );

   if ( Tcl_Eval( interp, "sa tcssad get currentAz value" ) == TCL_OK ) {
       strcpy( savest, interp->result);
       if ( Tcl_Eval( interp, "sa tcssad get currentEl value" ) == TCL_OK ) 
           tccDcRadec( interp, AZEL_MNT, savest, interp->result, 
               &domeviewPtr->telAz, &domeviewPtr->telEl );
   }

   if ( Tcl_Eval( interp, "sa tcssad get mountTrackFrame value" ) == TCL_OK ) 
       tccDcFrame( interp, interp->result, &domeviewPtr->system ); 
   if ( domeviewPtr->system != AZEL_TOPO ) {
      if ( Tcl_Eval( interp, "sa tcssad get mountTrackEq value" ) == TCL_OK ) 
          tccDcEpoch( interp, interp->result, &domeviewPtr->equinox.type, 
              &domeviewPtr->equinox.year );
   }
   if ( Tcl_Eval( interp, "sa tcssad get demandRA value" ) == TCL_OK ) {
       strcpy( savest, interp->result);
       if ( Tcl_Eval( interp, "sa tcssad get demandDec value" ) == TCL_OK ) 
           tccDcRadec( interp, domeviewPtr->system, savest, interp->result, 
               &domeviewPtr->ra, &domeviewPtr->dec );
   }
   return TCL_OK;
}

static void DeleteDV( Tk_Canvas canvas, Tk_Item *itemPtr, Display *display)
{
   DomeViewItem *domeviewPtr = (DomeViewItem *) itemPtr;

   if ( domeviewPtr->domeLowColor != NULL ) {
      Tk_FreeColor( domeviewPtr->domeLowColor );
   }
   if ( domeviewPtr->domeMidColor != NULL ) {
      Tk_FreeColor( domeviewPtr->domeMidColor );
   }
   if ( domeviewPtr->domeHiColor != NULL ) {
      Tk_FreeColor( domeviewPtr->domeHiColor );
   }
   if ( domeviewPtr->mirrorColor != NULL ) {
      Tk_FreeColor( domeviewPtr->mirrorColor );
   }
   if ( domeviewPtr->blackColor != NULL ) {
      Tk_FreeColor( domeviewPtr->blackColor );
   }
   if ( domeviewPtr->targetColor != NULL ) {
      Tk_FreeColor( domeviewPtr->targetColor );
   }
   if ( domeviewPtr->moonColor != NULL ) {
      Tk_FreeColor( domeviewPtr->moonColor );
   }
   if ( domeviewPtr->vglabColor != NULL ) {
      Tk_FreeColor( domeviewPtr->vglabColor );
   }
   if ( domeviewPtr->domeLowGC != None ) {
      Tk_FreeGC( display, domeviewPtr->domeLowGC );
   }
   if ( domeviewPtr->domeMidGC != None ) {
      Tk_FreeGC( display, domeviewPtr->domeMidGC );
   }
   if ( domeviewPtr->domeHiGC != None ) {
      Tk_FreeGC( display, domeviewPtr->domeHiGC );
   }
   if ( domeviewPtr->mirrorGC != None ) {
      Tk_FreeGC( display, domeviewPtr->mirrorGC );
   }
   if ( domeviewPtr->blackGC != None ) {
      Tk_FreeGC( display, domeviewPtr->blackGC );
   }
   if ( domeviewPtr->blackdotGC != None ) {
      Tk_FreeGC( display, domeviewPtr->blackdotGC );
   }
   if ( domeviewPtr->evgGC != None ) {
      Tk_FreeGC( display, domeviewPtr->evgGC );
   }
   if ( domeviewPtr->wvgGC != None ) {
      Tk_FreeGC( display, domeviewPtr->wvgGC );
   }
   if ( domeviewPtr->targetGC != None ) {
      Tk_FreeGC( display, domeviewPtr->targetGC );
   }
   if ( domeviewPtr->moonGC != None ) {
      Tk_FreeGC( display, domeviewPtr->moonGC );
   }
   if ( domeviewPtr->vglabGC != None ) {
      Tk_FreeGC( display, domeviewPtr->vglabGC );
   }
}

/* Polar coordinates of dome features need to define the drawing.
   The dome is 18m in diameter, the slit is 4.5m wide and the mirror 8m
   diameter. */

#define D2X 64.0                         /* Degrees to X angle */
#define D90X 5760.0                      /* 90 degrees as X angle */
#define SLITWIDTH2 0.25                  /* Semi-width of slit (4.5m/18m) */
#define SLITWIDTH2X 916.7324723          /*  "     "   "   " as X angle */
#define MIRROR 0.22222222222             /* Radius of mirror (4m/18m) */
#define DOMERADIUS 1.570796326794896     /* 90 degrees */
#define ELLIMIT 1.308996939              /* 75 degrees */
#define TOPSHUTMAX (105.5 *D2R)          /* Top shutter maximum */
#define BOTSHUTMIN (0.5 * D2R)           /* Bottom shutter minimum */

#define PICSIZE 160.0                    /* "Radius" of picture in pixels */
#define R2L(R) (PICSIZE * sin(R)/sqrt( 1.0 + cos(R) ) )
                                         /* Radius to x/y conversion */

static void DisplayDV( Tk_Canvas canvas, Tk_Item *itemPtr, Display *display,
   Drawable drawable, int xunused, int yunused, int wunused, int hunused)
{
   DomeViewItem *domeviewPtr = (DomeViewItem *) itemPtr;
   Tk_Window tkwin = Tk_CanvasTkwin( canvas );
   short xc, yc, a1, a2;
   int x, y, r, rel, rgate, i;
   double t, xi, eta;
   double domeazr, domeaz, amlimel;
   unsigned int width, height;
   XPoint point[7], w[5], e[7];
   XPoint p;
   struct EPOCH epochNow, eqx_unused;
   struct TELP tel_unused;
   struct PMPXRV pmotion = {0, 0.0, 0.0, 0.0, 0.0};
   double az, el, ra, dec, diam;
   double rawt, utc, tt;
   double ha1, ha2, timestep;

/* Center of picture in drawable coordinates */
   Tk_CanvasDrawableCoords( canvas, domeviewPtr->x0, domeviewPtr->y0, &xc, &yc);

/* The dome */
   r = R2L(DOMERADIUS);
   domeazr = domeviewPtr->domeaz * D2R;
   a1 = (short) ( D90X + SLITWIDTH2X - domeviewPtr->domeaz * D2X );
   a2 = 23040 - (short) ( SLITWIDTH2X * 2.0 );
   x = xc - r;
   y = yc - r;
   width = height = (unsigned) ( r * 2.0 );
   XFillArc( display, drawable, domeviewPtr->domeLowGC, x, y, width, height, 
      a1, a2);
   point[0].x = xc;
   point[0].y = yc;
   point[1].x = xc + (short) ( r * sin( domeazr + SLITWIDTH2 ) );
   point[1].y = yc - (short) ( r * cos( domeazr + SLITWIDTH2 ) );
   point[2].x = xc + (short) ( r * sin( domeazr - SLITWIDTH2 ) );
   point[2].y = yc - (short) ( r * cos( domeazr - SLITWIDTH2 ) );
   XFillPolygon( display, drawable, domeviewPtr->domeLowGC, point, 3, Convex,
      CoordModeOrigin);

/* Elevation limit */
   rel = R2L(ELLIMIT);
   a1 = 0;
   a2 = 360 * 64;
   x = xc - rel;
   y = yc - rel;
   width = height = (unsigned) ( rel * 2.0 );
   XFillArc( display, drawable, domeviewPtr->domeMidGC, x, y, width, height, 
      a1, a2);

/* Air mass limit */
   rel = R2L(D90 - domeviewPtr->amLimEl * D2R);
   x = xc - rel;
   y = yc - rel;
   width = height = (unsigned) ( rel * 2.0 );
   XFillArc( display, drawable, domeviewPtr->domeHiGC, x, y, width, height, 
      a1, a2);

/* Shutter opening */
   project( xc, yc, r, domeazr, domeviewPtr->topShutEl * D2R, -SLITWIDTH2, 0.0,
      &point[0]);
   project( xc, yc, r, domeazr, domeviewPtr->topShutEl * D2R, 0.0, 0.0,
      &point[1]);
   project( xc, yc, r, domeazr, domeviewPtr->topShutEl * D2R, SLITWIDTH2, 0.0,
      &point[2]);
   project( xc, yc, r, domeazr, domeviewPtr->botShutEl * D2R, SLITWIDTH2, 0.0,
      &point[3]);
   project( xc, yc, r, domeazr, domeviewPtr->botShutEl * D2R, 0.0, 0.0,
      &point[4]);
   project( xc, yc, r, domeazr, domeviewPtr->botShutEl * D2R, -SLITWIDTH2, 0.0,
      &point[5]);
   point[6] = point[0];
   XFillPolygon( display, drawable, domeviewPtr->blackGC, point, 7, 
      Nonconvex, CoordModeOrigin);

/* Vent gates */
   if ( domeviewPtr->evgGC != None ) {
      a1 = 30 * 64 + (short) ( D90X - domeviewPtr->domeaz * D2X );
      a2 = 120 * 64;
      rgate = r - GATEWIDTH/2.0;
      x = xc - rgate;
      y = yc - rgate;
      width = height = (unsigned) ( rgate * 2.0 );
      XDrawArc( display, drawable, domeviewPtr->evgGC, x, y, width, height, 
         a1, a2);
   }
   if ( domeviewPtr->wvgGC != None ) {
      a1 = 210 * 64 + (short) ( D90X - domeviewPtr->domeaz * D2X );
      a2 = 120 * 64;
      rgate = r - GATEWIDTH/2.0;
      x = xc - rgate;
      y = yc - rgate;
      width = height = (unsigned) ( rgate * 2.0 );
      XDrawArc( display, drawable, domeviewPtr->wvgGC, x, y, width, height, 
         a1, a2);
   }

/* Vent gate labels */
   x = xc - (short) ( 0.9 * r * cos( domeazr ) );
   y = yc - (short) ( 0.9 * r * sin( domeazr ) );
   e[0].x = x + 8.0;
   e[0].y = y + 8.0;
   e[1].x = x;
   e[1].y = y + 8.0;
   e[2].x = x;
   e[2].y = y;
   e[3].x = x + 8.0;
   e[3].y = y;
   e[4].x = x;
   e[4].y = y;
   e[5].x = x;
   e[5].y = y - 8.0;
   e[6].x = x + 8.0;
   e[6].y = y - 8.0;
   XDrawLines( display, drawable, domeviewPtr->vglabGC, e, 7, CoordModeOrigin);
   x = 2.0 * xc - x;
   y = 2.0 * yc - y;
   w[0].x = x + 10.0;
   w[0].y = y;
   w[1].x = x + 5.0;
   w[1].y = y + 10.0;
   w[2].x = x;
   w[2].y = y + 2.0;
   w[3].x = x - 5.0;
   w[3].y = y + 10.0;
   w[4].x = x - 10.0;
   w[4].y = y;
   XDrawLines( display, drawable, domeviewPtr->vglabGC, w, 5, CoordModeOrigin);


/* Mirror */
   if ( !(domeviewPtr->mpvalid) ||
        fabs( domeviewPtr->oldTelEl - domeviewPtr->telEl ) > D2R ||
        fabs( domeviewPtr->oldTelAz - domeviewPtr->telAz ) > D2R ) {
      for ( i = 0; i < 100; i++ ) {
         t = PI2 * (double)i / 100.0;
         xi = MIRROR * cos(t);
         eta = MIRROR * sin(t);
         project( xc, yc, r, domeviewPtr->telAz, domeviewPtr->telEl, xi, eta, 
            &domeviewPtr->mp[i]);
      }
      domeviewPtr->mp[100] = domeviewPtr->mp[0];
      domeviewPtr->oldTelEl = domeviewPtr->telEl;
      domeviewPtr->oldTelAz = domeviewPtr->telAz;
      domeviewPtr->mpvalid = 1;
   } else {
      for ( i = 0; i < 101; i++ ) {
         domeviewPtr->mp[i].x += xc - domeviewPtr->xc;
         domeviewPtr->mp[i].y += yc - domeviewPtr->yc;
      }
   }
   XFillPolygon( display, drawable, domeviewPtr->mirrorGC, domeviewPtr->mp, 
      100, Nonconvex, CoordModeOrigin);
   XDrawLines( display, drawable, domeviewPtr->blackdotGC, domeviewPtr->mp, 
      101, CoordModeOrigin);

/* Shutter opening outline on top of mirror */
   XDrawLines( display, drawable, domeviewPtr->blackGC, point, 7,
      CoordModeOrigin);

/* Times for celestial positions. */
   timeNow( &rawt );
   timeThenD( rawt, TT, &tt);
   timeThenD( rawt, UTC, &utc);
   slaAoppat( utc, domeviewPtr->aoprms );

/* Sun */
   slaRdplan( tt, 0, domeviewPtr->aoprms[12], domeviewPtr->aoprms[0], 
      &ra, &dec, &diam);
   slaDe2h( domeviewPtr->aoprms[13] - ra, dec, domeviewPtr->aoprms[0],
      &az, &el);
   if ( el > 0.0 ) {
      project( xc, yc, r, az, el, 0.0, 0.0, &p);
      XFillArc( display, drawable, domeviewPtr->targetGC, p.x - 10, p.y - 10, 
         30, 30, 0, 360 * 64);
   }

/* Moon */
   slaRdplan( tt, 3, domeviewPtr->aoprms[12], domeviewPtr->aoprms[0], 
      &ra, &dec, &diam);
   slaDe2h( domeviewPtr->aoprms[13] - ra, dec, domeviewPtr->aoprms[0],
      &az, &el);
   if ( el > 0.0 ) {
      project( xc, yc, r, az, el, 0.0, 0.0, &p);
      XFillArc( display, drawable, domeviewPtr->moonGC, p.x - 10, p.y - 10, 
         30, 30, 0, 360 * 64);
   }

/* Target position */
   if ( domeviewPtr->system != AZEL_TOPO ) {
      epochNow.year = slaEpj( tt );
      epochNow.type = 'J';
      if ( astCoco( domeviewPtr->ra, domeviewPtr->dec, pmotion,
         domeviewPtr->system, domeviewPtr->equinox, epochNow, AZEL_TOPO, 
         eqx_unused, tt, domeviewPtr->aoprms, tel_unused, &az, &el) == 0 ) {

         if ( el > 0.0 ) {
            if ( !(domeviewPtr->tpvalid) ||
               fabs( domeviewPtr->oldAz - az ) > D2R ||
               fabs( domeviewPtr->oldEl - el ) > D2R ) {
               domeviewPtr->tpvalid = 1;
               domeviewPtr->oldAz = az;
               domeviewPtr->oldEl = el;
               project( xc, yc, r, az, el, 0.0, 0.0, &domeviewPtr->tp[0]);

/* Target track (assuming siderial tracking) */
               ha1 = slaDrange( domeviewPtr->aoprms[13] - domeviewPtr->ra );
               if ( tccLimEl( cos( domeviewPtr->dec ), 
                  sin( domeviewPtr->dec), 0.0, domeviewPtr->aoprms[2], 
                  domeviewPtr->aoprms[1], &ha2)
                  == 1 ) {
                  ha2 = ha1 + PI2;
               }
               timestep = (ha2 - ha1) / PI2 / 20.0;
               for ( i = 1; i < 20; i++ ) {
                  tt += timestep;
                  utc += timestep;
                  slaAoppat( utc, domeviewPtr->aoprms );
                  astCoco( domeviewPtr->ra, domeviewPtr->dec, pmotion, 
                     domeviewPtr->system, domeviewPtr->equinox, epochNow, 
                     AZEL_TOPO, eqx_unused, tt, domeviewPtr->aoprms, 
                     tel_unused, &az, &el);
                  project( xc, yc, r, az, el, 0.0, 0.0, &domeviewPtr->tp[i]);
               }
            } else {
               for ( i = 0; i < 20; i++ ) {
                  domeviewPtr->tp[i].x += xc - domeviewPtr->xc;
                  domeviewPtr->tp[i].y += yc - domeviewPtr->yc;
               }
            }
         } else {
            domeviewPtr->tpvalid = 0;
         }
      }
   }
   if ( domeviewPtr->system != AZEL_TOPO && domeviewPtr->tpvalid ) {
      XFillArc( display, drawable, domeviewPtr->targetGC, 
         domeviewPtr->tp[0].x - 5, domeviewPtr->tp[0].y - 5, 10, 10, 
            0, 360 * 64);
      XDrawLines( display, drawable, domeviewPtr->targetGC, 
                  domeviewPtr->tp, 20, CoordModeOrigin);
   }

/* Save the drawable coordinate offset */
   domeviewPtr->xc = xc;
   domeviewPtr->yc = yc;
}

static void project( short xc, short yc, double r, double azr, double el, 
   double w, double h, XPoint *point)
{
   double ap, ep, rp;
   slaDtp2s( w, h, azr, el, &ap, &ep);
   rp = r * sin(D90 - ep) / sqrt( 1.0 + cos(D90 - ep) );
   point->x = xc + (short) ( rp * sin(ap) );
   point->y = yc - (short) ( rp * cos(ap) );
}

static int OverlapDV( Tk_Canvas canvas, Tk_Item *itemPtr, double *areaPtr)
{
   DomeViewItem *domeviewPtr = (DomeViewItem *) itemPtr;
   int x1, x2, y1, y2;

   x1 = domeviewPtr->x0 - PICSIZE - 10;
   x2 = domeviewPtr->x0 + PICSIZE + 10;
   y1 = domeviewPtr->y0 - PICSIZE - 10;
   y2 = domeviewPtr->y0 + PICSIZE + 10;

   if ( ( areaPtr[0] < x1 && areaPtr[2] < x1 ) ||
        ( areaPtr[0] > x2 && areaPtr[2] > x2 ) ||
        ( areaPtr[1] < y1 && areaPtr[3] < y1 ) ||
        ( areaPtr[1] > y2 && areaPtr[3] > y2 ) ) return -1;
   else if ( ( areaPtr[0] > x1 && areaPtr[0] < x2 ) &&
             ( areaPtr[2] > x1 && areaPtr[2] < x2 ) &&
             ( areaPtr[1] > y1 && areaPtr[1] < y2 ) &&
             ( areaPtr[3] > y1 && areaPtr[3] < y2 ) ) return 1;
   else
      return 0;
}

static void BboxDV( Tk_Canvas canvas, Tk_Item *itemPtr)
{
   DomeViewItem *domeviewPtr = (DomeViewItem *) itemPtr;
   domeviewPtr->header.x1 = domeviewPtr->x0 - PICSIZE - 10;
   domeviewPtr->header.y1 = domeviewPtr->y0 - PICSIZE - 10;
   domeviewPtr->header.x2 = domeviewPtr->x0 + PICSIZE + 10;
   domeviewPtr->header.y2 = domeviewPtr->y0 + PICSIZE + 10;
}

static double PointDV( Tk_Canvas canvas, Tk_Item *itemPtr, double *pointPtr)
{
   DomeViewItem *domeviewPtr = (DomeViewItem *) itemPtr;
   return sqrt( (domeviewPtr->x0 - pointPtr[0]) *
                (domeviewPtr->x0 - pointPtr[0]) +
                (domeviewPtr->y0 - pointPtr[1]) *
                (domeviewPtr->y0 - pointPtr[1]) );
}
