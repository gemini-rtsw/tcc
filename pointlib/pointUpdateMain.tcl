proc pointUpdateMain {w} {
#+
#  Function name:
#  pointUpdateMain
#
#  Creates the user interface for the pointUpdate application and creates the
#  command senders and status acceptors needed to drive the TCS.
#
#  D Terrett 28 May 1999
#
#  Copyright CCLRC
#-

# TCS apply status.
   epics sa apply

# Command for logging pointing data.
   epics cs pointLog

# Command for setting pointing model terms.
   epics cs pointParam

# Command for zeroing the guiding offsets.
   epics cs zeroGuide

# tcs SAD database
   epics sa tcssad

# tcs pointing model records
   epics sa session
   epics sa local
   epics sa net
   epics sa guide

# Wait to get connected to the TCS.
   waitConnect

# Create interface.
   iwidgets::menubar .mb -menubuttons {
      menubutton file -text File -menu {
         options -tearoff false
         command exit -label Exit -command exit
      }
   }
   pack .mb -fill x -expand yes -anchor w

   PointUpdate .ptu
   pack .ptu -fill y -expand yes

   return
}
