proc closeLogFile {} {
#+
#  Procedure:
#  closeLogFile
#
#  Purpose:
#  Closes a pointing log file
#
#  Description:
#  The END record is written to the log file and the file closed.
#
#  D Terrett 26 May 1999
#
#  Copyright CCLRC
#-
   global Logchan Widgets Menus LogFileName

# Disable the logging of observations.
   sa tcssad unproc pointObs saveObs

# Write and END record to the log file.
   puts $Logchan \
         "END ! [clock format [clock seconds] -format "%H %M %S" -gmt 1]"
   set Logchan [close $Logchan]

# Prompt for a file name for the tpoint input data file.
   set types {
      {{TPOINT Input Files} {.tpd} {}}
      {{All Files} * {}}
   }

   file stat $LogFileName logstats
   while 1 {

# Get a name for the TPOINT input file.
      set tpname [tk_getSaveFile -defaultextension .tpd -parent . \
                -filetypes $types -title "Tpoint input file"]

# Check that it is a different file from the log file.
      if {[file exists $tpname]} {
         file stat $tpname tpstats
         if { $tpstats(ino) != $logstats(ino) || 
               $tpstats(dev) != $logstats(dev) } {
            break
         }
         tk_messageBox -icon error -parent . -message \
               "The TPOINT input file must be different from the log file; \
               please try again"
      } else {
         break
      }
   }

# Convert the log file to tpoint format.
   if { $tpname != {} } {
      exec /gemini/tcs/bin/solaris/ptconv < $LogFileName > $tpname
   }

# Adjust the menu.
   $Menus(file) entryconfigure "Open*" -state normal
   $Menus(file) entryconfigure "Close*" -state disabled
   $Widgets(log) configure -state disabled
   return
}
