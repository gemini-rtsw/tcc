proc openLogFile {} {
#+
#  Procedure:
#  openLogFile
#
#  Purpose:
#  Creates a new pointing log file and writes the header and links the
#  saveObs proc to changes in the pointObs status acceptot.
#
#  Description:
#
#  D Terrett 17 September 1999
#
#  Copyright CCLRC
#-
   global Widgets Menus LogFileName

# cd to the save file directory.
   set pwd [pwd]
   global SAVE_FILE_DIR
   cd $SAVE_FILE_DIR

   set types {
      {{Data Files} {.dat} {}}
      {{All Files} * {}}
   }
   set LogFileName [tk_getSaveFile -defaultextension .dat -parent . \
                -initialdir $SAVE_FILE_DIR \
                -filetypes $types -title "Open log file"]

   if { $LogFileName != {} } {
      global Logchan
      set Logchan [open $LogFileName {WRONLY CREAT TRUNC}]
      $Menus(file) entryconfigure "Open*" -state disabled
      $Menus(file) entryconfigure "Close*" -state normal
      $Widgets(log) configure -state normal
   
      global Version

# Write a header line.
      puts $Logchan \
           "! Pointing test log file created by tcsPointTest.tk version\
            $Version"

# Create a dialog box for entering comments.
      iwidgets::promptdialog .pd -title Comments -labeltext Comments: \
            -modality global
      .pd buttonconfigure OK -command {.pd deactivate 1}
      .pd buttonconfigure Cancel -command {.pd deactivate 0}
      .pd buttonconfigure Apply -command {
            puts $Logchan "! [.pd get]"
            .pd delete 0 end
      }
      .pd center
      if { [.pd activate] } {
         puts $Logchan "! [.pd get]"
      }
      destroy .pd

# Write a description of the keywords.
      puts $Logchan "!"
      puts $Logchan "! Keywords are:"
      puts $Logchan "!               VERSION  file format version"
      puts $Logchan \
           "!               DATE     calandar date on which test started"
      puts $Logchan "!               UTC      time at which test started"
      puts $Logchan \
           "!               TLATM    Mean latitude of observatory (degrees)"
      puts $Logchan \
           "!               TLONGM   Mean longitude of observatory (degrees)"
      puts $Logchan "!               HEIGHT   Height above sea level (metres)"
      puts $Logchan \
           "!               XPM      X component of polar motion (arcsec)"
      puts $Logchan \
           "!               YPM      Y component of polar motion (arcsec)"
      puts $Logchan "!               TTMTAI   TT-TAI (seconds)"
      puts $Logchan "!               DJMLS    MJD following next leap second"
      puts $Logchan "!               DELAT    TAI-UTC (seconds)"
      puts $Logchan "!               DELUT    UT1-UTC (seconds)"
      puts $Logchan "!"
      puts $Logchan "! Records following the OBS keyword contain the following:"
      puts $Logchan "!"
      puts $Logchan "!  TAI of observation as MJD and time of day in seconds"
      puts $Logchan "!  FK5/J2000 coordinates of object (radians)"
      puts $Logchan "!  Effective wavelength (microns)"
      puts $Logchan "!  Az and El of telescope (radians)"
      puts $Logchan "!  Predicted rotator position angle (radians)"
      puts $Logchan "!  Pointing origin x & y (millimetres)"
      puts $Logchan "!  Telescope focal length (millimetres)"
      puts $Logchan "!  Temperature (degrees K)"
      puts $Logchan "!  Pressure (mBar)"
      puts $Logchan "!  Relative humidity"
      puts $Logchan "!  Tropospheric lapse rate"
      puts $Logchan "!"
      puts $Logchan "VERSION $Version"
      set date [clock seconds]
      puts $Logchan "DATE [clock format $date -format "%Y %b %d" -gmt 1]"
      puts $Logchan "UTC [clock format $date -format "%H %M %S" -gmt 1]"
      puts $Logchan "TLATM [sa tcssad get tlatm value]"
      puts $Logchan "TLONGM [sa tcssad get tlongm value]"
      puts $Logchan "HEIGHT [sa tcssad get height value]"
      puts $Logchan "XPM [sa tcssad get xpm value]"
      puts $Logchan "YPM [sa tcssad get ypm value]"
      puts $Logchan "TTMTAI [sa tcssad get ttmtai value]"
      puts $Logchan "DJMLS [sa tcssad get djmls value]"
      puts $Logchan "DELAT [sa tcssad get delat value]"
      puts $Logchan "DELUT [sa tcssad get delut value]"
      flush $Logchan

# Update save file directory.
      set dir [file dirname $LogFileName]
      if { [string compare [file pathtype $dir] relative] == 0 } {
         set SAVE_FILE_DIR [pwd]/$dir
      } else {
         set SAVE_FILE_DIR $dir
      }

# Link the proc to the pointLog status acceptor.
      sa tcssad proc pointObs saveObs
   }

# Restore the default directory
   cd $pwd
   return
}
