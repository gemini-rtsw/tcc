#+
#  createPanels
#
#  Creates all the panel objects that make up the tcc application.
#
#  Arguments:
#             none
#
#  D Terrett 24 April 2001
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
#   PanelMgr guideHandsetPanel -name .guideHandsetPanel \
#         -class GuideHandsetPanel -title "Pointing Handset"
#   PanelMgr targetHandsetPanel -name .targetHandsetPanel \
#         -class TargetHandsetPanel -title "Target Handset"
#   PanelMgr poHandsetPanel -name .poHandsetPanel \
#         -class PoHandsetPanel -title "Instrument Handset"
   PanelMgr handsetPanel -name .handsetPanel \
         -class HandsetPanel -title "Handsets"
   PanelMgr pointUpdatePanel -name .pointUpdatePanel \
         -class PointUpdatePanel -title "Pointing model update"
   PanelMgr starSelectPanel -name .starSelectPanel \
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
    EditPanelMgr tcsconfigfieldPanel \
          -namespace {ScienceTargetNames WfsTargetNames WfsTargetNames \
             WfsTargetNames} \
          -vt {sourceA pwfs1 pwfs2 oiwfs} \
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
