#+
#  updateDisplays.tcl
#
#  updateDisplays updates any of the displays in the tsd application that
#  exist.
#
#  D Terrett 28 August 1998
#
#  Copyright CCLRC
#-
proc updateDisplays args {

# Update the time system with the latest tcs context
   tccTime set [ast update]

# Update all panels that exist.
   foreach panel {.graphicPanel .numericPanel} {
      if { [winfo exists $panel] } {
         $panel update
      }
   }
}
