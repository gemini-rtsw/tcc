#  tsdMain
#
#  The main routine of the Gemini telescope status display application.
#
#  Arguments:
#             Any command line options and values.
#
#  D Terrett 9 June 2000
#
#  Copyright CCLRC
#-
proc tsdMain args {

# Withdraw the main window so that it doesn't appear if some error happens
# during startup and a message box is created and so that the windows that
# need access to the TCS can't get created while we are waiting for a
# connection.
   wm withdraw .

# Create the epics service.
   service epics

# Process command line arguments.
   global ROOT
   set layout $ROOT/default_layout.tsd
   foreach {opt val} $args {
      switch -- $opt {
         -layout {
            set layout $val
         }
         default {
            tk_messageBox -icon warning -message \
                  "Unknown command line option \"$opt\" being ignored."
         }
      }
   }

# Create status acceptors needed.
   epics sa tcssad
   epics sa mcssad
   epics sa crssad
   epics sa m2ssad
   epics sa agssad
   epics sa ecssad
   epics sa gissad

# Wait to get connected to the TCS.
   waitConnect

# Set the title for the windows.
   global env
   if { [info exists env(TCC_TCSNAME)] } {
      set title "Simulated Gemini TCS ($env(TCC_TCSNAME))"
   } else {
      set title "Gemini North"
   }

# Create all the panel objects.
   PanelMgr graphicPanel -name .graphicPanel -class GraphicPanel \
         -title $title
   PanelMgr numericPanel -name .numericPanel -class NumericPanel \
         -title $title
   PanelMgr rcPanel -name .rcPanel -class RcPanel -title "Display configure"

# Link updating the panels to the UTC sad record.
   RecordMonitor tcssad.UTC tcssad UTC
   tcssad.UTC add updateDisplays

# Pack the toolbar.
   pack [TsdToolbar .toolbar]

# Set the title, icon etc.
   wm title . "Gemini North"
   wm iconname . TSD

# Load the layout definition.
   if { $layout != "" } {
      if [catch {source $layout} msg] {
         puts "Error processing layout \"$msg\""
      }
   }

# It is now safe to display the main window.
   wm deiconify .

# Fix the window size.
   update
   wm resizable . 0 0

   return
}
