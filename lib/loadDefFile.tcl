#+
#  loadDefFile
#
#  Loads a file of compoment definitions.
#
#  Arguments:
#             chanid      The channel id to read from.
#
#  D Terrett 19 February 2000
#
#  Copyright CCLRC
#-

proc loadDefFile chanid {
   set command ""

   while { ![eof $chanid] } {
      if { [gets $chanid line] > 1 } {
         set command ${command}\n${line}
         if { [info complete $command] && [string index $command \
                  [expr {[string length $command] - 1}]] != "\\"} {
            if { [catch {eval $command} msg] } {
               tk_messageBox -title "tcc error" -icon error -type ok \
                     -message "Error in component definition file: \"$msg\""
            }
            set command ""
         }
      }
   }
   if { [llength $command] != 0 } {
      tk_messageBox -title "tcc error" -icon error -type ok -message \
            "Incomplete command in component definition file: \"$command\""
   }
}
