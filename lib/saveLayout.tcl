#+
#  saveLayout.tcl
#
#  Procedures:
#     saveLayout  - saves the current panel layout to a file selected with
#                   a dialog box.
#
#  D Terrett 8 May 2002
#
#  Copyright CCLRC
#-
proc saveLayout {} {

# Save the current directory and cd to save file directory.
   set pwd [pwd]
   cd $tcclib::SaveFileDirectory

# Create a dialog box for selecting the file and center it.
   set dialog [iwidgets::fileselectiondialog .fsd \
         -modality application -title "Layout File Selection Dialog"]
   $dialog center

# Activate the dialog
   if { [$dialog activate] } {

# Create the file.
      set filename [$dialog get]
      if { [catch {set file [open $filename {WRONLY CREAT TRUNC}]} \
            msg] } {
         tk_messageBox -icon error -message $msg
      } else {

# Save the layout parameters for every panel object that exists.
         foreach panelmgr [itcl::find objects -isa PanelMgr] {

# Update the panel manager's geometry 
            $panelmgr updategeometry

# Write the panel's screen option.
            set screen [$panelmgr cget -screen]
            if { ! [string is space $screen] } {
               puts $file "$panelmgr configure -screen $screen"
            }

# Write the window size part of the geometry.
            set geom [$panelmgr cget -geometry]
            if { ! [string is space $geom] } {
               if { [regexp {\+.*} $geom pos] } {
                  puts $file "$panelmgr configure -geometry $pos"
               }
            }

# Write the panel's state option.
            set state [$panelmgr cget -state]
            puts $file "$panelmgr configure -state $state"

# Write a map command for the panels which have create set to 1.
            if { [$panelmgr cget -create] } {
               puts $file "$panelmgr configure -create 1"
               puts $file "$panelmgr map"
               switch state {
                  iconified {
                     puts $file "$panelmgr iconify"
                  }
                  withdrawn {
                     puts $file "$panelmgr withdraw"
                  }
               }
            }
         }

# Close the file.
         close $file

# Save the name of the save file directory.
         set dir [file dirname $filename]
         if { [string compare [file pathtype $dir] relative] == 0 } {
            set tcclib::SaveFileDirectory [pwd]/$dir
         } else {
            set tcclib::SaveFileDirectory $dir
         }
      }
   }

# Destroy the dialog box.
   destroy $dialog

# Restore default directory
   cd $pwd
}
