proc starSelectMain {w} {
#+
#  Function name:
#  starSelectMain
#
#  Creates the user interface for the starSelect application and creates the
#  command senders and status acceptors needed to drive the TCS.
#
#  D Terrett 20 July 2000
#
#  Copyright CCLRC
#-

# Create the TCS principal system.
   seq::PrincipalSystem tcs tcsApply

# Connect to TCS status
   epics sa applyC

# source A and slew commands
   epics cs sourceA
   epics cs slew

# Status acceptors
   epics sa tcssad

# Wait to get connected to the TCS.
   waitConnect

# Create the user interface
   pack [StarSelect .starselect]

   return
}
