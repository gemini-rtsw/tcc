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
#  D Terrett 29 April 1999
#
#  Copyright CCLRC
#-
   global Logchan Widgets Menus LogFileName

# Disable the loggin of observations.
   sa tcssad unproc pointObs saveObs

# Write and END record to the log file.
   puts $Logchan \
         "END ! [clock format [clock seconds] -format "%H %M %S" -gmt 1]"
   set Logchan [close $Logchan]

# Prompt for a file name for the tpoint input data file.
   set types {
      {{Data Files} {.dat} {}}
      {{All Files} * {}}
   }
   set tpname [tk_getSaveFile -defaultextension .tpd -parent . \
                -filetypes $types -title "Tpoint input file"]

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
