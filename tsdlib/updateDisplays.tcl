#+
#  updateDisplays
#
#  updateDisplays updates any of the displays in the tsd application that
#  exist.
#
#  Arguments:
#            All arguments are ignored.
#
#  D Terrett 9 June 2000
#
#  Copyright CCLRC
#-
proc updateDisplays {record value} {

# Update the time system with the latest tcs context
   tccTime set [ast update]

# Update all panels that exist.
   foreach panel {.graphicPanel .numericPanel} {
      if { [winfo exists $panel] } {
         $panel update
      }
   }
}
