proc probeCalMain {w} {
#+
#  Function name:
#  probeCalMain
#
#  Creates the user interface for the probe calibration application and 
#  creates the tcs principal system  and status acceptors needed to drive 
#  the TCS.
#
#  D Terrett 4 October 2000
#
#  Copyright CCLRC
#-

# TCS apply status.
   epics sa apply

# tcs SAD database
   epics sa tcssad
   epics sa agssad
   epics sa crssad

# TCS principal system object.
   seq::PrincipalSystem tcs tcsApply

# Calibration parameter object
   CalParam calparam

# Create interface.
   iwidgets::menubar .mb -menubuttons {
      menubutton file -text File -menu {
         options -tearoff false
         command exit -label Exit -command exit
      }
   }
   pack .mb -fill x -expand yes -anchor w

   ProbeCal .probecal
   pack .probecal -fill y -expand yes

   return
}
