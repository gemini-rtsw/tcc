proc saveObs {name alarm time values} {
#+
#  Procedure
#  saveObs
#
#  Purpose
#  Logs a pointing observation to a file.
#
#  Description
#  A pointing observation is logged to the log file
#
#  D Terrett 29 April 1999
#
#  Copyright CCLRC
#-
   global Logchan
   if {$Logchan != "" } {
      puts $Logchan OBS
      foreach item $values {
         puts $Logchan $item
      }
      flush $Logchan
   }
   return
}
