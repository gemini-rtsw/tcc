#+
#  tccMain.tcl
#
#  The main program for the Gemini control console. It creates the top level
#  "toolbar" window.
#
#  D Terrett 26 May 1999
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
   set layout ""
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

# Create status acceptors for sad databases.
   epics sa tcssad
   epics sa ecssad

# Create status acceptors for tcs status and messages.
   epics sa apply
   epics sa applyC
   epics sa errMessage

# Create the configuration component lists and the namespaces for the
# components.
   ComponentList TcsConfigList
   namespace eval TcsConfigNames {}
   ComponentList ScienceTargetList
   namespace eval ScienceTargetNames {}
   ComponentList WfsTargetList
   namespace eval WfsTargetNames {}
   ComponentList PointOrigList
   namespace eval PointOrigNames {}
   ComponentList WavelengthList
   namespace eval WavelengthNames {}
   ComponentList TrackRateList
   namespace eval TrackRateNames {}
   ComponentList SlewOptionsList
   namespace eval SlewOptionsNames {}
   ComponentList RotatorList
   namespace eval RotatorNames {}
   ComponentList TrackFrameList
   namespace eval TrackFrameNames {}
   ComponentList GuideList
   namespace eval GuideNames {}
   ComponentList CosysList
   namespace eval CosysNames {}
   ComponentList InstrumentList
   namespace eval InstrumentNames {}

# Create all the panel manager objects.
   createPanels

# Create the command senders for all the TCS commands that are used by
# the configuration components.
   epics cs mount
   epics cs sourceA
   epics cs sourceB
   epics cs pwfs1
   epics cs pwfs2
   epics cs oiwfs
   epics cs wavelMount
   epics cs wavelSourceA
   epics cs wavelSourceB
   epics cs wavelPwfs1
   epics cs wavelPwfs2
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
   epics cs azwrap
   epics cs rotwrap
   epics cs dtelFocus
   epics cs m2Baffle
   epics cs hrwfs
   epics cs aoFold
   epics cs scienceFold
   epics cs scienceFoldPark
   epics cs airmassLimit
   epics cs guideHandset
   epics cs guideHandConfig
   epics cs tarHandset
   epics cs tarHandConfig
   epics cs poHandset
   epics cs poHandConfig

# Set the timeput period for posting commands to the TCS.
   cs tcsApply setTimeout 3

# Create the object the represents the configuration of the mirrors within
# the instrument support structure.
   ISS iss

# Create the logger object.
   Logger logger

# Pack the toolbar.
   pack [TccToolbar .toolbar]

# Set the title, icon etc.
   wm title . "Gemini North"
   wm iconname . TCC

# Load initialisation files.
   if { $init != "" } {
      foreach filespec $init {
         if { [file isdirectory $filespec] } {
            if { [catch {glob $filespec/*} filenames] } {
               tk_messageBox -icon error -parent . -message \
                  "initialization directory \"$filespec\" is empty"
            } else {
               foreach file $filenames {
                  if { ! [file isdirectory $file] } {
                     if { [catch {source $file} msg] } {
                        tk_messageBox -icon error -parent . -message \
                 "error while processing initialization file \"$file\": $msg"
                     }
                  }
               }
            }
         } else {
            if { [catch {source $filespec} msg] } {
               tk_messageBox -icon error -parent . -message \
                "error while processing initialization file \"$filespec\": $msg"
            }
         }
      }
   }

# Wait to connect to the tcs.
   waitConnect

# Load the layout definition.
   if { $layout != "" } {
      if [catch {source $layout} msg] {
         puts "Error processing layout \"$msg\""
      }
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
