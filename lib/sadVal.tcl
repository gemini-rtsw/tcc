#+
#  sadVal
#
#  Returns the value of a record in a sad, ensuring that the value is a
#  valid number. If it is not, the supplied default is returned instead.
#
#  Arguments:
#            sad       The name of the status acceptor containing the record.
#
#            rec       The name of the record.
#
#            default   The default value.
#
#  D Terrett 10 November 1999
#
#  Copyright CCLRC
#-
proc sadVal {sad rec default} {

# Get the value of the the record.
   set val [string trim [sa $sad get $rec]]

# Test the value in an expression.
   if { [catch {expr 0 + $val}] } {
      return $default
   } else {
      return $val
   }
}
