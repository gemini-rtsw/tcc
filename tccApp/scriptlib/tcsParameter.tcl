#+
#  tcsParameter.tcl
#
#  Defines sets a tcs command parameter value.
#
#  D Terrett 18 March 1999
#
#  Copyright CCLRC
#-

proc tcsParameter { command par value} {

   cs $command set $par value $value
}
