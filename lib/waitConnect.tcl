#+
#  waitConnect.tcl
#
#  Procedures:
#     waitConnect - waits for a connection to the TCS to be established by 
#                   attempting to initialise the time system at 1 second 
#                   intervals.
#
#  Globals:
#     TZOFFSET - Time offset between local time and UTC in hours.
#
#  D Terrett 4 September 1998
#
#  Copyright CCLRC
#-

proc waitConnect {} {

   global TZOFFSET
   if { [info exists TZOFFSET] } {
      set offset $TZOFFSET
   } else {
      set offset 0
   }

   puts -nonewline "Waiting for connection to tcs..."
   flush stdout
   while { [catch {tccTime init $offset}] } {
      after 1000
   }
   puts " connected."
   return
}
