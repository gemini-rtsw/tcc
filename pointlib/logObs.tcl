proc logObs {} {
#+
#  Procedure:
#  logObs
#
#  Purpose:
#  Logs a pointing observation.
#
#  Description:
#  The pointLog command is sent to the TCS and the next star
#  selected if the "Select next star" option is selected.
#
#  D Terrett 5 january 1999
#
#  Temporary workround for inPosition not implemented in the TCS.
#
#  Copyright CCLRC
#-
   global TimeStamp Options

# Check that the telescope is in position.
#   for { set j 0 } { $j < 10 } { incr j} {
#      if { [sa tcssad get inPosition value] == "TRUE" } break
#      after 1000
#   }
#   if { $j == 10 } {
#      if { ! $Options(skip) } {
#         tk_messageBox -icon error -message "Telescope not it position"
#      }
#   }

# Log the pointing by sending a pointLog command to the tcs.
   set TimeStamp [sa tcssad get inPosition time]
   cs pointLog set log 0
   if [catch {cs pointLog postWait}] {
      set info [sa apply get mess value]
      tk_messageBox -icon error -message "Pointlog command failed: \"$info\""
   }

# Select the next star.
   if $Options(select) {after idle selectNext}

   return
}
