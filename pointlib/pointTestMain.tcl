proc pointTestMain {w} {
#+
#  Function name:
#  pointTestMain
#
#  Creates the user interface for the pointTest application and creates the
#  command senders and status acceptors needed to drive the TCS.
#
#  D Terrett 12 July 2000
#
#  Copyright CCLRC
#-

# Create the TCS principal system.
   seq::PrincipalSystem tcs tcsApply tcssad inPosition TRUE

# Connect to TCS status
   epics sa applyC

# source A and slew commands
   epics cs sourceA
   epics cs slew

# pointLog command
   epics cs pointLog

# Guideing related commands
   epics cs mountGuide
   epics cs m2Guide
   epics cs pwfs1Observe
   epics cs pwfs2Observe

# Wait to get connected to the TCS.
   waitConnect

# Create the user interface
   pack [PointTest .pointtest]

   return
}
