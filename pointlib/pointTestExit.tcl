proc pointTestExit {} {
#+
#  Procedure:
#  pointTestExit
#
#  Purpose:
#  Exits the pointTest application.
#
#  Description:
#  The log file is closed (if open) and exit called.
#
#  D Terrett 21 October 1998
#
#  Copyright CCLRC
#-
   global Logchan
   if { $Logchan != "" } closeLogFile
   exit
}
