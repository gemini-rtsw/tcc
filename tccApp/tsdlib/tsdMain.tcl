#  tsdMain
#
#  The main routine of the Gemini telescope status display application.
#
#  Arguments:
#             Any command line options and values.
#
#  D Terrett 19 January 2004
#
#  Copyright CCLRC
#-
proc tsdMain args {

# Withdraw the main window so that it doesn't appear if some error happens
# during startup and a message box is created and so that the windows that
# need access to the TCS can't get created while we are waiting for a
# connection.
   wm withdraw .

# Parse command line args
   #puts "args: $args"
   set eng 1
   set dst 0
   set calfile [file join $::env(GEMINI_TOP) etc tcc calparams calparams.dat]
   
   set args [ join $args ]
  
   for { set i 0 } { $i < [llength $args] } { incr i } {
      set arg [lindex $args $i]
      switch -- $arg {
         -eng {
            set eng 1
            if { [ expr "$i + 1" ] < [ llength $args ] } {
               set val [lindex $args [expr "$i + 1" ] ]
               if { [ string is integer -strict $val ]} {
                  set eng $val
                  incr i
               }
            }
         }
         -dst {
            set dst 1
            if { [ expr "$i + 1" ] < [ llength $args ] } {
               set val [lindex $args [expr "$i + 1" ] ]
               if { [ string is integer -strict $val ]} {
                  set dst $val
                  incr i
               }
            }
         }
         -cal {
            if { [ expr "$i + 1" ] < [ llength $args ] } {
               set val [lindex $args [expr "$i + 1" ] ]
               if { [string length $val] > 0 && [ string index $val 0 ] ne "-" } {
                  set calfile $val
                  incr i
               }
            }
         }
      }
   }
   
   # Create the CalParam object.
   CalParam calparam  -calFile $calfile

   appData add configFile tsd[calparam cget -site].ca

   
   set skycalc_path [calparam cget -skycalc_dir]
   puts "using skycalc path: $skycalc_path"
   
   if { [catch {set fid [open "|$skycalc_path/skycalc < lib/skycalc_input_file" r]}]} {
      puts "Unable to determine whether Chile is using daylight saving time \
            - assuming not. Please specify skycalc dir in calparams.<site>."
      set ::CHILE_DAYLIGHT_SAVING 0
   } else {
      set almanac [read $fid]
      if { [string first CDT $almanac] == -1 } {
         set ::CHILE_DAYLIGHT_SAVING 0
      } else {
         set ::CHILE_DAYLIGHT_SAVING 1
      }
   }

# Create the epics service.
   service epics

# Create status acceptors needed.
   epics sa tcssad
   epics sa mcssad
   epics sa crssad
   epics sa m2ssad
   epics sa agssad
   epics sa ecssad
   epics sa gissad
   epics sa pcssad
   epics sa aossad
   epics sa gcalsad
   epics sa gpolsad

#AWE  Below will not exist at GS until next year  
   if { [catch {epics sa bfo}] } {
      #AWE should not generate an error this way
   }

   if { [catch {epics sa vtk}] } {
      #AWE should not generate an error this way
   }
   
   if { $::env(GEMINI_SITE) eq "CP" } {
      epics sa aomsad
      epics sa gemssad
   }

# Wait to get connected to the TCS.
   waitConnect

# Set the title for the windows.
   global env
   if { [info exists env(TCC_TCSNAME)] } {
      wm title . "Simulated Gemini TCS ($env(TCC_TCSNAME))"
   } else {
      wm title . [sa tcssad get name]
   }

# Link updating the panels to the UTC sad record.
   RecordMonitor tcssad.UTC tcssad UTC
   tcssad.UTC add updateDisplays

# Pack the numeric and graphic displays.
   pack [NumericDisplay .numericDisplay -eng $eng -dst $dst] -anchor w -fill x -expand y
   pack [GraphicDisplay .graphicDisplay] -anchor w -fill x -expand y

# Set the icon.
   wm iconname . TSD

# It is now safe to display the main window.
   wm deiconify .

# Fix the window size.
   update
   wm resizable . 0 0

   return
}
