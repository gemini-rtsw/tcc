#+
#  tclComamnds.tcl
#
#  Defines tcs command senders.
#
#  D Terrett 18 March 1999
#
#  Copyright CCLRC
#-

proc tcsCommands args {

# Create the epics service if necessary.
   if { [info commands epics] == "" } {
      service epics
   }

# Create the command senders.
   foreach command $args {
      epics cs $command
   }
}
