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
   if { [llength [info commands epics]] == 0 } {
      service epics
   }

# Create the command senders.
   foreach command $args {
      epics cs $command
   }
}
