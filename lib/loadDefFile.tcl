#+
#  loadDefFile
#
#  Loads a file of compoment definitions.
#
#  Arguments:
#             chanid      The channel id to read from.
#
#  D Terrett 30 January 2001
#
#  Copyright CCLRC
#-

proc loadDefFile chanid {
   set command ""

   set lineno 0
   while { ![eof $chanid] } {
      incr lineno
      if { [gets $chanid line] > 1 } {
         set command ${command}\n${line}
         if { [info complete $command] && [string index $command \
                  [expr {[string length $command] - 1}]] != "\\"} {
            if { [catch {eval $command} msg] } {
               if { [string equal [tk_messageBox -title "tcc error" \
                     -icon error -type okcancel \
                     -message "Error in component definition file at line \
                     $lineno: \"$msg\"" ] cancel] } {
                   set command ""
                   break;
               }
            }
            set command ""
         }
      }
   }
   if { ! [string equal $command ""] } {
      tk_messageBox -title "tcc error" -icon error -type ok -message \
            "Incomplete command in component definition file: \"$command\""
   }
}
