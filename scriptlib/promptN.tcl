#+
#  promptN.tcl
#
#  Prompts for a numeric value in a specified range; if the range limits are
#  identical, no range checking is done. If no value is entered, the variable
#  is unchanged.
#
#  D Terrett 18 March 1999
#
#  Copyright CCLRC
#-

proc promptN { prompt min max help varname } {
   upvar $varname val

   set done 0
   while { ! $done } {
      if { $val == "" } {
         puts -nonewline "$prompt: "
      } else {
         puts -nonewline "$prompt ($val): "
      }
      flush stdout
      gets stdin reply
      if { $reply == "" } {
         if { $val != "" } {
            incr done
         }
      } else {
         if { [string first ? $reply] != -1 } {
            puts $help
         } else {
            if { [catch {expr $reply + 0}] } {
               puts "\"$reply\" isn't a number"
               puts $help
            } else {
               if { $min != $max } {
                  if { $reply < $min } {
                     puts "\"$reply\" is too small"
                     puts $help
                  } else {
                     if { $reply > $max } {
                        puts "\"$reply\" is too big"
                        puts $help
                     } else {
                        set val $reply
                        incr done
                     }
                  }
               } else {
                  set val $reply
                  incr done
               }
            }
         }
      }
   }
}
