proc pointUpdateMain {w} {
#+
#  Function name:
#  pointUpdateMain
#
#  Creates the user interface for the pointUpdate application and creates the
#  command senders and status acceptors needed to drive the TCS.
#
#  D Terrett 12 November 1998
#
#  Copyright CCLRC
#-

# TCS apply status.
   epics sa apply

# Command for logging pointing data.
   epics cs pointLog

# Command for setting pointing model terms.
   epics cs pointParam

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
      menubutton options -text Options -menu {
        options -tearoff 0
        checkbutton ia -label "Include IA in fit" -variable _ia \
              -command {.ptu configure -ia \$_ia}
      }
   }
   pack .mb -fill x -expand yes -anchor w

   PointUpdate .ptu
   pack .ptu -fill y -expand yes

   return
}
