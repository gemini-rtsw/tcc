proc endSlew {monitorName cmdStatus info} {
#+
#  Procedure
#  endSlew
#
#  Purpose
#  Callback procedure that monitors the progress of a slew.
#
#  Description:
#  If the slew completes successfully and the "Log observation at end of slew"
#  option was selected, the "log observation" button in invoked.
#
#  D Terrett 21 October 1998
#
#  Copyright CCLRC
#-
   global Options Widgets
   switch $cmdStatus {
      idle { 
         if $Options(log) {after idle $Widgets(log) invoke}
      }
      error {
         if $Options(skip) {
            after 2000
         } {
            set info [sa apply get mess value]
            tk_messageBox -icon error -message "Slew failed with: \"$info\""
         }
      }
      timeout {
         set info [sa apply get mess value]
         tk_messageBox -icon error -message "Slew timed out"
      }
   }
}
