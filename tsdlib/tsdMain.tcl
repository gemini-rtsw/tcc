#  tsdMain.tcl
#
#  The main routine of the Gemini telescope status display application.
#
#  D Terrett 1 September 1998
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
   set layout ""
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

# Wait to get connected to the TCS.
   waitConnect

# Create all the panel objects.
   PanelMgr graphicPanel -name .graphicPanel -class GraphicPanel \
         -title "Gemini North"
   PanelMgr numericPanel -name .numericPanel -class NumericPanel \
         -title "Gemini North"
   PanelMgr rcPanel -name .rcPanel -class RcPanel -title "Display configure"

# Link updating the panels to the UTC sad record.
   sa tcssad proc UTC updateDisplays

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
