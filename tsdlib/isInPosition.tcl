#+
#  isInPosition.tcl
#
#  Tests the value of a subsystem in position record to determine if the
#  mechanism is in position or not.
#
#  D Terrett 10 November 1999
#
#  Copyright CCLRC
#-
proc isInPosition {sad rec} {

# Get the value of the the record.
   set val [string toupper [string trim [sa $sad get $rec]]]

# Test against the various possible representations of "True".
   if { [string compare $val "TRUE"] == 0 } {return 1}
   if { $val == 1 } {return 1}

# No match so return false.
   return 0
}
