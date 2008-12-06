#+
#  waitConnect.tcl
#
#  Procedures:
#     waitConnect - waits for a connection to the TCS to be established by 
#                   attempting to initialise the time system at 1 second 
#                   intervals.
#
#  D Terrett 5 December 2000
#
#  Copyright CCLRC
#-

proc waitConnect {} {

   puts -nonewline "Waiting for connection to tcs..."
   flush stdout
   while { [catch {tccTime init}] } {
      after 1000
   }
   puts " connected."
   return
}
