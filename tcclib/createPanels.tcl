#+
#  createPanels
#
#  Creates all the panel objects that make up the tcc application.
#
#  Arguments:
#             none
#
#  D Terrett 6 June 2001
#
#  Copyright CCLRC
#-

proc createPanels {} {

# Create all the simple command panels.
   tcclib::PanelMgr observePanel -name .observePanel -class ObservePanel \
         -title "Acquisition & Guiding"
   tcclib::PanelMgr skycatPanel -name .skycatPanel -class SkycatPanel \
         -title "SkyCat"
   tcclib::PanelMgr tcsPanel -name .tcsPanel -class TcsPanel \
         -title "System Control"
   tcclib::PanelMgr mcsPanel -name .mcsPanel -class McsPanel \
         -title "Mount Control"
   tcclib::PanelMgr crsPanel -name .crsPanel -class CrsPanel \
         -title "Cass Rotator Control"
   tcclib::PanelMgr ecsPanel -name .ecsPanel -class EcsPanel \
         -title "Enclosure Control"
#   tcclib::PanelMgr m1sPanel -name .m1sPanel -class M1sPanel \
#         -title "Primary Mirror Control"
#   tcclib::PanelMgr m2sPanel -name .m2sPanel -class M2sPanel \
#         -title "Secondary Mirror Control"
#   tcclib::PanelMgr agsPanel -name .agsPanel -class AgsPanel \
#         -title "A & G Control"
#   tcclib::PanelMgr aosPanel -name .aosPanel -class AosPanel \
#         -title "Active Optics Control"
   tcclib::PanelMgr previewPanel -name .previewPanel -class PreviewPanel \
         -title "Configuration Preview"
   tcclib::PanelMgr logPanel -name .logPanel -class tcclib::LogPanel \
         -title "Command Log"
   tcclib::PanelMgr guideLoopPanel -name .guideLoopPanel \
         -class GuideLoopPanel -title "Guiding Loop Control"
#   tcclib::PanelMgr guideHandsetPanel -name .guideHandsetPanel \
#         -class GuideHandsetPanel -title "Pointing Handset"
#   tcclib::PanelMgr targetHandsetPanel -name .targetHandsetPanel \
#         -class TargetHandsetPanel -title "Target Handset"
#   tcclib::PanelMgr poHandsetPanel -name .poHandsetPanel \
#         -class PoHandsetPanel -title "Instrument Handset"
   tcclib::PanelMgr handsetPanel -name .handsetPanel \
         -class HandsetPanel -title "Handsets"
   tcclib::PanelMgr pointUpdatePanel -name .pointUpdatePanel \
         -class PointUpdatePanel -title "Pointing model update"
   tcclib::PanelMgr starSelectPanel -name .starSelectPanel \
         -class StarSelectPanel -title "Star Selector"

# Create all the edit panels.
   createEditPanels "" tcsconfig TcsConfig TcsConfigNames \
         TcsConfigList "" ""

# Zap the Target edit panel managers.
   delete object tcsconfigscienceTargetPanel
   delete object tcsconfigguidepwfs1TargetPanel
   delete object tcsconfigguidepwfs2TargetPanel
   delete object tcsconfigguideoiwfsTargetPanel

# Create a field panel in its place.
    tcclib::EditPanelMgr tcsconfigfieldPanel \
          -namespace {ScienceTargetNames WfsTargetNames WfsTargetNames \
             WfsTargetNames} \
          -applyarg {sourceA pwfs1 pwfs2 oiwfs} \
          -componentlist { ScienceTargetList WfsTargetList WfsTargetList \
             WfsTargetList} \
          -titlelist [list "Science target" "PWFS1 target" "PWFS2 target" \
             "OIWFS target"] \
          -class FieldPanel -name .tcsconfigfieldPanel \
          -title "Target Manager"

# Redirect the target edit shadows.
    tcsconfigscienceTarget configure -panel tcsconfigfieldPanel \
          -control scitarget -alsomaps {tcsconfigguidepwfs1Target \
          tcsconfigguidepwfs2Target tcsconfigguideoiwfsTarget}
    tcsconfigguidepwfs1Target configure -panel tcsconfigfieldPanel \
          -control pwfs1target -alsomaps {tcsconfigscienceTarget \
          tcsconfigguidepwfs2Target tcsconfigguideoiwfsTarget}
    tcsconfigguidepwfs2Target configure -panel tcsconfigfieldPanel \
          -control pwfs2target -alsomaps {tcsconfigscienceTarget \
          tcsconfigguidepwfs1Target tcsconfigguideoiwfsTarget}
    tcsconfigguideoiwfsTarget configure -panel tcsconfigfieldPanel \
          -control oiwfstarget -alsomaps {tcsconfigscienceTarget \
          tcsconfigguidepwfs1Target tcsconfigguidepwfs1Target}

# Configure the title of the observation editor.
   tcsconfigPanel configure -title "Configuration Manager"
}
