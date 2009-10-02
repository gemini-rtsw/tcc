# Options database for the tcc & tsd - this version is for Cerro Pachon

# Foreground for all windows.
*foreground:				Black

# Background for displays.
*numericDisplay.background:	Wheat
*graphicDisplay.background:	Wheat

# Background for the "direct action" panels.
*observePanel*background:	PeachPuff1
*ecsPanel*background:		PeachPuff1
*guideLoopPanel*background:	PeachPuff1
*handsetPanel*background:	PeachPuff1
*mcsPanel*background:		PeachPuff1
*crsPanel*background:		PeachPuff1
*tcsPanel*background:		PeachPuff1
*m1sPanel*background:		PeachPuff1
*agsPanel*background:		PeachPuff1
*aosPanel*background:		PeachPuff1

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

# TrackRate entry pages
*tcsFieldScienceTargetTrackRatePanel*background:  Pink
*tcsFieldPwfs1TargetTrackRatePanel*background:    SkyBlue
*tcsFieldPwfs2TargetTrackRatePanel*background:    OldLace
*tcsFieldOiwfsTargetTrackRatePanel*background:    Wheat
*tcsFieldAowfsTargetTrackRatePanel*background:   LemonChiffon

# Target entry pages
*FieldPanel*data*page1*background:    Pink
*FieldPanel*data*page2*background:    SkyBlue
*FieldPanel*data*page3*background:    OldLace
*FieldPanel*data*page4*background:    Wheat
*FieldPanel*data*page5*background:    LemonChiffon

# WFS config pages
*wfsconfig*page1*background:   Pink
*wfsconfig*page2*background:   SkyBlue
*wfsconfig*page3*background:   OldLace
*wfsconfig*page4*background:   Wheat
*wfsconfig*page5*background:   LemonChiffon

# GACQ Offset request background
*gacq*background:    Red

# Font for "major" tcc items.
*Font:			-*-*-bold-r-normal-sans-12-*-*-*-*-*-*-*
*alarm*Font:			-*-*-bold-r-normal-sans-10-*-*-*-*-*-*-*

# Fonts for tsd displays.
*numericDisplay.Font:		-*-*-bold-r-normal-sans-18-*-*-*-*-*-*-*
*graphicDisplay.Font:		-*-*-bold-r-normal-sans-18-*-*-*-*-*-*-*

# Disable AO related features
*toolbar*ao*state:                      disabled
#*tcsConfigPanel*control*gaos*state:   disabled
*tcsPanel*control*gaos*state:         disabled
*tcsPanel*control*aodatum*state:        disabled
*tcsPanel*control*aopark*state:         disabled
*observePanel*control*altair*state:     disabled
*observePanel*control*aopark*state:     disabled
*observePanel*control*gaosframe*state:  disabled
*observePanel*control*ttgsframe*state:  disabled
*observePanel*control*sfoframe*state:  disabled
*observePanel*control*niciframe*state:  disabled
*observePanel*control*lpath*1*state:    disabled
*observePanel*control*lpath*4*state:    disabled
*observePanel*guideloop*m2ao*state:   disabled
*observePanel*guideloop*oiao*state:     disabled
*observePanel*guideloop*ttao*state:     disabled
*observePanel*guideloop*sfao*state:     disabled
*observePanel*guideloop*aobto*state:     disabled
*observePanel*guideloop*aosfo*state:     disabled
*observePanel*guideloop*sfo_ex*state:     disabled
*observePanel*guideloop*aottgs*state:     disabled
*observePanel*guideloop*ttgs_ex*state:     disabled
*observePanel*guideloop*gaoson*state:   disabled
*observePanel*guideloop*gaos_ex*state:   disabled
*observePanel*guideloop*m1src*3*state:  disabled
*tcsSlewOptionsPanel*ltcscheck*state:   disabled
