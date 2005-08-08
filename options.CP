# Options database for the tcc & tsd - this version is for Cerro Pachon

# Foreground for all windows.
*foreground:				Black

# Background for displays.
*numericDisplay.background:	Wheat
*graphicDisplay.background:	Wheat

# Background for the "direct action" panels.
*observePanel*background:		PeachPuff1
*ecsPanel*background:		PeachPuff1
*guideLoopPanel*background:	PeachPuff1
*handsetPanel*background:	PeachPuff1
*mcsPanel*background:		PeachPuff1
*crsPanel*background:		PeachPuff1
*tcsPanel*background:		PeachPuff1
*m1sPanel*background:		PeachPuff1
*agsPanel*background:		PeachPuff1

# Handset pages
*handsetPanel*page1*background:	SkyBlue
*handsetPanel*page2*background:	Pink
*handsetPanel*page3*background:	LemonChiffon

# Background for the configuration editing panels.
*CosysPanel*background:		LightGray
*GuidePanel*background:		LightGrey
*InstrumentPanel*background:		LightGray
*PointOrigPanel*background:		LightGray
*ChopPanel*background:		LightGray
*RotatorPanel*background:		LightGray
*FieldPanel*background:		LightGray
*SlewOptionsPanel*background:	LightGray
*TcsConfigPanel*background:		LightGray
*TrackFramePanel*background:		LightGray
*TrackRatePanel*background:		LightGray
*WavelengthPanel*background:		LightGray

# Target entry pages
*FieldPanel*data*page1*background: 	LightGray
*FieldPanel*data*page2*background: 	LightGray
*FieldPanel*data*page3*background: 	LightGray
*FieldPanel*data*page4*background: 	LightGray
*FieldPanel*data*page5*background: 	LightGray

# Font for "major" tcc items.
*Font:			-*-*-bold-r-normal-sans-12-*-*-*-*-*-*-*

# Fonts for tsd displays.
*numericDisplay.Font:		-*-*-bold-r-normal-sans-18-*-*-*-*-*-*-*
*graphicDisplay.Font:		-*-*-bold-r-normal-sans-18-*-*-*-*-*-*-*

# Disable AO related features
*tcsConfigPanel*control*altair*state:   disabled
*tcsPanel*control*altair*state:         disabled
*tcsPanel*control*aodatum*state:        disabled
*tcsPanel*control*aopark*state:         disabled
*observePanel*control*altair*state:     disabled
*observePanel*control*aopark*state:     disabled
*observePanel*control*gaosframe*state:  disabled
*observePanel*control*lpath*1*state:    disabled
*observePanel*control*lpath*4*state:    disabled
*observePanel*guideloop*m2ao*0*state:   disabled
*observePanel*guideloop*m2ao*1*state:   disabled
*observePanel*guideloop*m1src*3*state:  disabled
*tcsFieldPanel*notebook*page5*state:    disabled
