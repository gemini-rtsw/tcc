#+
#  timeIit
#
#  Attempts to initialise the time system. If it fails it reschedules itself
#  one second later.
#
#  Arguments:
#             none
#
#  D Terrett 29 January 2003
#
#  Copyright CCLRC
#-

proc timeInit {} {
   if { [catch {tccTime init}] } {
      after 1000 timeInit
   }
   return
}
