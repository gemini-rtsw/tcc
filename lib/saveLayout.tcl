#+
#  saveLayout.tcl
#
#  The saveLayout procedure saves the current panel layout to a file.
#
#  D Terrett 17 September 1999
#
#  Copyright CCLRC
#-
proc saveLayout {} {

# cd to save file directory.
   global SAVE_FILE_DIR
   set pwd [pwd]
   cd $SAVE_FILE_DIR

# Create a dialog box for selecting the file.
   set dialog [iwidgets::fileselectiondialog .fsd \
         -modality application -title "Layout File Selection Dialog"]
   $dialog center

   if { [$dialog activate] } {

# Create the file.
      set filename [$dialog get]
      if { [catch {set file [open $filename {WRONLY CREAT TRUNC}]} \
            msg] } {
         tk_messageBox -icon error -message $msg
      } else {

# Save the layout parameters for every panel object that exists.
         foreach panelmgr [itcl::find objects -isa PanelMgr] {

# Update the panel manager's geometry and state options from the window.
            $panelmgr updategeometry

            set s [$panelmgr cget -screen]
            if { $s != "" } {
               puts $file "$panelmgr configure -screen $s"
            }
            set g [$panelmgr cget -geometry]
            if { $g != "" } {
               puts $file "$panelmgr configure -geometry $g"
            }

            set state [$panelmgr cget -state]
            puts $file "$panelmgr configure -state $state"

         }

# Map the panels which have createset to 1.
         foreach panelmgr [itcl::find objects -class PanelMgr] {
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

# Do the same for the EditShadows (if such a class exists).
         if { [itcl::find classes EditShadow] != "" } {
            foreach editshadow [itcl::find objects -class EditShadow] {
               set panelmgr [$editshadow cget -panel]
               if { [$panelmgr cget -create] } {
                  puts $file "$panelmgr configure -create 1"
                  puts $file "$editshadow map"
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
         }

         close $file

# Update save file directory.
         set dir [file dirname $filename]
         if { [string compare [file pathtype $dir] relative] == 0 } {
            set SAVE_FILE_DIR [pwd]/$dir
         } else {
            set SAVE_FILE_DIR $dir
         }
      }
   }
   destroy $dialog

# Restore default directory
   cd $pwd
}
