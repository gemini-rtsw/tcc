#+
#  saveLayout.tcl
#
#  The saveLayout procedure saves the current panel layout to a file.
#
#  D Terrett 25 November 1998
#
#  Copyright CCLRC
#-
proc saveLayout {} {

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
         foreach panelmgr [info objects -isa PanelMgr] {

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
         foreach panelmgr [info objects -class PanelMgr] {
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
         if { [info classes EditShadow] != "" } {
            foreach editshadow [info objects -class EditShadow] {
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
      }
   }
   destroy $dialog
}
