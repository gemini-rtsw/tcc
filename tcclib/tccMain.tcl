#+
#  tccMain
#
#  The main program for the Gemini control console. It creates the top level
#  "toolbar" window.
#
#  Arguments:
#            Any command line options and values.
#
#  D Terrett  18 October 2002
#
#  Copyright CCLRC
#-

proc tccMain args {

# Withdraw the main window so that it doesn't appear if some error happens
# during startup and a message box is created and so that the windows that
# need access to the TCS can't get created while we are waiting for a
# connection.
   wm withdraw .

# Create the epics service.
   service epics

# Process command line arguments.
   set layout $::ROOT/default_layout.tcc
   set init ""
   foreach {opt val} $args {
      switch -- $opt {
         -layout {
            set layout $val
         }
         -init {
            lappend init $val
         }
         default {
            tk_messageBox -icon warning -message \
                  "Unknown command line option \"$opt\" being ignored."
         }
      }
   }

# Create the TCS principal system.
   seq::PrincipalSystem tcs tcsApply -debug 0

# Create status acceptors for sad databases.
   epics sa tcssad
   epics sa ecssad
   epics sa gpolsad

# Create status acceptors for tcs status and messages.
   epics sa apply
   epics sa applyC
   epics sa errMessage

# Create status senders for setting SIR records
   global env
   epics ss $env(TCC_TCSNAME):sad:programID
   epics ss $env(TCC_TCSNAME):sad:sourceAObjectName

# Create the configuration structures.
   createConfig

# Create all the panel manager objects.
   createPanels

# Create the command senders for all the TCS commands that are used by
# the configuration components.
   epics cs mount
   epics cs sourceA
   epics cs sourceB
   epics cs pwfs1
   epics cs pwfs2
   epics cs pwfs1Park
   epics cs pwfs2Park
   epics cs oiwfs
   epics cs wavelMount
   epics cs wavelSourceA
   epics cs wavelSourceB
   epics cs wavelPwfs1
   epics cs pwfs1Filter
   epics cs wavelPwfs2
   epics cs pwfs2Filter
   epics cs wavelOiwfs
   epics cs difTrMount
   epics cs difTrSourceA
   epics cs difTrSourceB
   epics cs difTrPwfs1
   epics cs difTrPwfs2
   epics cs difTrOiwfs
   epics cs trackMount
   epics cs trackSourceA
   epics cs trackSourceB
   epics cs trackSourceC
   epics cs trackPwfs1
   epics cs trackPwfs2
   epics cs trackOiwfs
   epics cs pwfs1Guide
   epics cs pwfs2Guide
   epics cs oiwfsGuide
   epics cs poriginA
   epics cs poriginB
   epics cs poriginC
   epics cs poriginM
   epics cs slew
   epics cs rotator
   epics cs wrap
   epics cs dtelFocus
   epics cs m2Baffle
   epics cs hrwfs
   epics cs hrwfsPark
   epics cs aoFoldPark
   epics cs aoFold
   epics cs scienceFold
   epics cs scienceFoldPark
   epics cs gpolPark
   epics cs airmassLimit
   epics cs guideHandset
   epics cs guideHandConfig
   epics cs tarHandset
   epics cs tarHandConfig
   epics cs poHandset
   epics cs poHandConfig
   epics cs orbit
   epics cs planet
   epics cs chopRelative
   epics cs chopConfig
   epics cs pointParam

# Set the timeout period for posting commands to the TCS.
   cs tcsApply setTimeout 3

# Create the object the represents the configuration of the mirrors within
# the instrument support structure.
   ISS iss

# Create the object the controls the Acquisition Camera/HRWFS.
   Acqcam acqcam

# Create the ocs session object for the one and only session.
   OcsSession ocssession sessionQueue

# Create the object that controls the active optics.
   ActiveOptics AO

# Create the logger objects.
   Logger logger tcs apply applyC errMessage
   Logger aclogger hrwfs acqcam::apply acqcam::applyC

# Create the CalParam object.
   CalParam calparam

# Pack the toolbar.
   pack [TccToolbar .toolbar]

# Set the icon.
   wm iconname . TCC

# Load default components.
   set file [open $::ROOT/default_components.xml RDONLY]
   set config [TcsConfigFile #auto [read $file] .]
   itcl::delete object $config
   close $file

# Load initialisation files.
   if { ! [string equal $init ""] } {
      foreach filespec $init {
         if { [file isdirectory $filespec] } {
            if { [catch {glob $filespec/*} filenames] } {
               tk_messageBox -icon error -parent . -message \
                  "initialization directory \"$filespec\" is empty"
            } else {
               foreach filename $filenames {
                  if { ! [file isdirectory $filename] } {
                     puts "processing $filename"
                     if { [catch {set file [open $filename RDONLY]} msg] } {
                        tk_messageBox -icon error -parent . -message \
                        "error opening initialization file \"$filename\": $msg"
                     } else {
                        set config [TcsConfigFile #auto $file .]
                        itcl::delete object $config
                        close $file
                     }
                  }
               }
            }
         } else {
            if { [catch {set file [open $filespec RDONLY]} msg] } {
               tk_messageBox -icon error -parent . -message \
                  "error opening initialization file \"$filespec\": $msg"
            } else {
               puts "processing $filespec"
               set config [TcsConfigFile #auto $file .]
               itcl::delete object $config
               close $file
            }
         }
      }
   }

# Select the tcs configuration called default by pretending that it was
# selected by its non-existent parent.
#   tcsconfig selectfromparent default

# Wait to connect to the tcs.
   waitConnect

# Load the layout definition.
   if { ! [string equal $layout ""] } {
      if [catch {source $layout} msg] {
         puts "Error processing layout \"$msg\""
      }
   }

# Set the title for the windows.
   global env
   if { $env(TCC_TCSNAME) != "tcs" } {
      wm title . "Simulated Gemini TCS ($env(TCC_TCSNAME))"
   } else {
      wm title . [sa tcssad get name]
   }

# Start the preview update process.
   previewUpdate

# It is now safe to display the main window.
   wm deiconify .

# Fix the window size.
   update
   wm resizable . 0 0

   return
}
