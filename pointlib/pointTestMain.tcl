proc pointTestMain {w} {
#+
#  Function name:
#  pointTestMain
#
#  Creates the user interface for the pointTest application and creates the
#  command senders and status acceptors needed to drive the TCS.
#
#  D Terrett 22 June 2001
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

# pointLog command
   epics cs pointLog

# Guideing related commands
   epics cs mountGuide
   epics cs m2Guide
   epics cs pwfs1Observe
   epics cs pwfs2Observe
   epics cs collAdjust

# Status acceptors we want to monitor.
   epics sa tcssad
   epics sa agssad
   epics sa m2ssad

# Wait to get connected to the TCS.
   waitConnect

# Create the user interface
   pack [PointTest .pointtest]

   return
}
