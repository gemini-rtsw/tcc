#  tsdMain
#
#  The main routine of the Gemini telescope status display application.
#
#  Arguments:
#             Any command line options and values.
#
#  D Terrett 13 July 2001
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

# Create status acceptors needed.
   epics sa tcssad
   epics sa mcssad
   epics sa crssad
   epics sa m2ssad
   epics sa agssad
   epics sa ecssad
   epics sa gissad
   epics sa pcssad
   epics sa gcalsad

# Wait to get connected to the TCS.
   waitConnect

# Set the title for the windows.
   global env
   if { [info exists env(TCC_TCSNAME)] } {
      wm title . "Simulated Gemini TCS ($env(TCC_TCSNAME))"
   } else {
      wm title . [sa tcssad get name]
   }

# Link updating the panels to the UTC sad record.
   RecordMonitor tcssad.UTC tcssad UTC
   tcssad.UTC add updateDisplays

# Pack the numeric and graphic displays.
   pack [NumericDisplay .numericDisplay] -anchor w
   pack [GraphicDisplay .graphicDisplay] -anchor w

# Set the icon.
   wm iconname . TSD

# It is now safe to display the main window.
   wm deiconify .

# Fix the window size.
   update
   wm resizable . 0 0

   return
}
