#+
#  waitConnect.tcl
#
#  Waits for a connection to the TCS to be established by repeated attempting
#  to initialise the time system.
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
