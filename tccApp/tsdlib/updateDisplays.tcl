#+
#  updateDisplays
#
#  updateDisplays updates any of the displays in the tsd application that
#  exist.
#
#  Arguments:
#            All arguments are ignored.
#
#  D Terrett 7 July 2000
#
#  Copyright CCLRC
#-
proc updateDisplays {record value} {

# Update the time system with the latest tcs context
   tccTime set [ast update]

# Update all the displays.
   foreach display {.graphicDisplay .numericDisplay} {
      $display update
   }
}
