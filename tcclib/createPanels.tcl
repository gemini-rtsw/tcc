#+
#  createPanels
#
#  Creates all the panel objects that make up the tcc application.
#
#  Arguments:
#             none
#
#  D Terrett 2 October 2000
#
#  Copyright CCLRC
#-

proc createPanels {} {

# Create all the simple command panels.
   PanelMgr observePanel -name .observePanel -class ObservePanel \
         -title "Acquisition & Guiding"
   PanelMgr skycatPanel -name .skycatPanel -class SkycatPanel \
         -title "SkyCat"
   PanelMgr tcsPanel -name .tcsPanel -class TcsPanel \
         -title "System Control"
   PanelMgr mcsPanel -name .mcsPanel -class McsPanel \
         -title "Mount Control"
   PanelMgr crsPanel -name .crsPanel -class CrsPanel \
         -title "Cass Rotator Control"
   PanelMgr ecsPanel -name .ecsPanel -class EcsPanel \
         -title "Enclosure Control"
#   PanelMgr m1sPanel -name .m1sPanel -class M1sPanel \
#         -title "Primary Mirror Control"
#   PanelMgr m2sPanel -name .m2sPanel -class M2sPanel \
#         -title "Secondary Mirror Control"
#   PanelMgr agsPanel -name .agsPanel -class AgsPanel \
#         -title "A & G Control"
#   PanelMgr aosPanel -name .aosPanel -class AosPanel \
#         -title "Active Optics Control"
   PanelMgr previewPanel -name .previewPanel -class PreviewPanel \
         -title "Configuration Preview"
   PanelMgr logPanel -name .logPanel -class LogPanel \
         -title "Command Log"
   PanelMgr guideLoopPanel -name .guideLoopPanel \
         -class GuideLoopPanel -title "Guiding Loop Control"
   PanelMgr guideHandsetPanel -name .guideHandsetPanel \
         -class GuideHandsetPanel -title "Pointing Handset"
   PanelMgr offsetHandsetPanel -name .offsetHandsetPanel \
         -class OffsetHandsetPanel -title "Target Handset"
   PanelMgr poHandsetPanel -name .poHandsetPanel \
         -class PoHandsetPanel -title "Instrument Handset"
   PanelMgr pointUpdatePanel -name .pointUpdatePanel \
         -class PointUpdatePanel -title "Pointing model update"

# Create all the edit panels.
   createEditPanels "" tcsconfig TcsConfig TcsConfigNames \
         TcsConfigList "" ""

# Configure the title of the observation editor.
   tcsconfigPanel configure -title "Target Selector"
}
