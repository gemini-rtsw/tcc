#+
#  tccMain
#
#  The main program for the Gemini control console. It creates the top level
#  "toolbar" window.
#
#  Arguments:
#            Any command line options and values.
#
#  D Terrett  17 December 2004
#
#  Copyright CCLRC
#-

proc tccMain args {
global env
# Withdraw the main window so that it doesn't appear if some error happens
# during startup and a message box is created and so that the windows that
# need access to the TCS can't get created while we are waiting for a
# connection.
   wm withdraw .

# Initialise the time service.
   timeInit


# Process command line arguments.
   set layout $::ROOT/default_layout.tcc
   set init ""
   if { [string compare [exec uname] "Linux"] == 0 } {
		set calfile [file join $env(GEMINI_TOP) etc tcc calparams calparams.dat]
	} else {
		set calfile "/gemini/files/calparams/calparams.dat"
	}

	set simgems 0
	set ignoreao ""
	
	set ocsproxy ""
   foreach {opt val} $args {
      switch -- $opt {
         -layout {
            set layout $val
         }
         -init {
            lappend init $val
         }
         -cal {
            set calfile "$val"
         }
			-ocsproxy {
				set ocsproxy "$val"
			}
			-simgems {
				set simgems $val
			}
			-nogems {
				set ignoreao $val
			}
         default {
            tk_messageBox -icon warning -message \
                  "Unknown command line option \"$opt\" being ignored."
         }
      }
   }

# Create the CalParam object.
   CalParam calparam -calFile $calfile

	appData add configFile tcc[calparam cget -site].ca
	puts "Using ca file [appData lookup configPath]/[appData lookup configFile]"
	# Create the epics service.
	service epics

# Create the TCS principal system.
   seq::PrincipalSystem tcs tcsApply -debug 0


#OCS proxy can be overwriten with an environment variable or command line option
	if { [info exists env(OCSPROXY)] && ![string is space $env(OCSPROXY)] } {
		calparam configure -ocs_proxy $env(OCSPROXY)
	}
	if { ![string is space $ocsproxy] } {
		calparam configure -ocs_proxy $ocsproxy
	}

	GemsSys::useSimulator $simgems
	
# AWE use the site parameter to load site specific channels
   puts "site is [calparam cget -site]"
   switch [calparam cget -site] {
       MK {
	   #Altair sad records
	   epics sa aossad

	   #Altair Commands
	   epics cs aoUseLwfs
	   epics cs aoInitWfs
	   epics cs aoDeployAdc
	   epics cs aoFlatten
	   epics cs aoPrepareCm
	   epics cs aoFlex
	   epics cs aoSFOdefocus
	   epics cs aoMoveAdc
	   epics cs aoMoveGim
	   epics cs aoFollow
	   epics cs aoExitShutter
	   epics cs aoEntShutter
	   epics cs aoDmVolt
	   epics cs aoPark
	   epics cs aoLgsPark
	   epics cs aoDatum
	   epics cs aoCalSource
	   epics cs aoCentreWfs
	   epics cs aoCorrect
	   epics cs aoOiwfsSource
           epics cs aoLgsP1Source
	   epics cs aoGimOffsets
	   epics cs aongsNDFilter
	   epics cs aolgsNDFilter
	   epics cs aoFLens
	   epics cs aoTTGSiris
	   epics cs ttgsloop
	   epics cs sfoloop
	   epics cs gaosTTGS
	   epics cs gaosSFO
	   epics cs gaosBTO
	   epics cs gaosObserve
	   epics cs m2deadband

       }
		CP {
	   # NICI channels
			epics sa nici
			epics cs niciMag
			epics cs niciAOLoop
			
			#GEMS guide channels
			epics cs g1
			epics cs wavelG1
			epics cs difTrG1
			epics cs trackG1
			epics cs g1Guide
			epics cs g1orbit
			epics cs g2
			epics cs wavelG2
			epics cs difTrG2
			epics cs trackG2
			epics cs g2Guide
			epics cs g2orbit
			epics cs g3
			epics cs wavelG3
			epics cs difTrG3
			epics cs trackG3
			epics cs g3Guide
			epics cs g3orbit
			epics cs g4
			epics cs wavelG4
			epics cs difTrG4
			epics cs trackG4
			epics cs g4Guide
			epics cs g4orbit
					
			epics cs guidersMapping
		}
   }


# Create status acceptors for sad databases.
   epics sa tcssad
   epics sa ecssad
   epics sa agssad
   epics sa gpolsad

# Create status acceptors for tcs status and messages.
   epics sa apply
   epics sa applyC
   epics sa errMessage

# AWE monitor Shutter Drive Currents
   epics sa shutterDriveCurrents

# AWE monitor Guide Counts
   epics sa guidects

# Create status senders for setting SIR records
   global env
   epics ss $env(TCC_TCSNAME):sad:programID
   epics ss $env(TCC_TCSNAME):sad:sourceAObjectName

# Create the configuration structures.
   createConfig

# Create the Action Logger
   LogActions logactions

# Create all the panel manager objects.
   createPanels
   
# Create the command senders for all the TCS commands that are used by
# the configuration components.
   epics cs configForAO
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
   epics cs pwfs1FldStop
   epics cs wavelPwfs2
   epics cs pwfs2Filter
   epics cs pwfs2FldStop
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
   epics cs offsetPoA1
   epics cs rotator
   epics cs wrap
   epics cs dtelFocus
   epics cs m2Baffle
   epics cs hrwfs
   epics cs hrwfsPark
   epics cs aoFoldPark
   epics cs aoFold
   epics cs agInPosOver
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
   epics cs pwfs1orbit
   epics cs pwfs2orbit
   epics cs oiwfsorbit
   epics cs planet
   epics cs chopRelative
   epics cs chopConfig
   epics cs p2Seq
   epics cs pointParam
   epics cs filter1
   epics cs filter2
   epics cs oiwfsSelect
	epics cs zeroRotCorr

# Set the timeout period for posting commands to the TCS.
   cs tcsApply setTimeout 3

# Create the object the represents the configuration of the mirrors within
# the instrument support structure.
   ISS iss

# Create the object the controls the Acquisition Camera/HRWFS.
   Hrwfs hrwfs

# Create the object the configures the Peripheral WFS
   Pwfs pwfs

# Create the object that controls the active optics.
   ActiveOptics AO
   #Set P1 default for MK
   if {[calparam cget -site] eq "MK"}  {
      AO configure -pwfs1Expose 0.005
   } else {
      AO configure -pwfs1Expose 0.01
   }

# Create the objects that control the wave-front sensors
   WaveFrontSensor Pwfs1 pwfs1
   WaveFrontSensor Pwfs2 pwfs2
   OiWaveFrontSensor Oiwfs oiwfs
   AoWaveFrontSensor Aowfs

# Create object for executing nods
   Nod Nodder

# Create the ocs session object for the one and only session.
   OcsSessionXmlRpc ocssession sessionQueue

# Create the logger objects.
   Logger logger tcs apply applyC errMessage
   Logger aclogger hrwfs hrwfs::apply hrwfs::applyC

# Pack the toolbar.
   pack [TccToolbar .toolbar]

# Set the icon.
   wm iconname . TCC

# Load default components.
   set file [open $::ROOT/default_components.xml RDONLY]
   set config [TcsConfigFile #auto [read $file] .]
   itcl::delete object $config
   close $file

   #Command line option -nogems overrides default slew options
   if { $ignoreao ne "" } {
      ::SlewOptionsNames::Normal configure -ignoreao $ignoreao
   }
   set ::Flags(ignoreao) [::SlewOptionsNames::Normal cget -ignoreao]
      
   
# Load initialisation files.
   if { ! [string equal $init ""] } {
      foreach filespec $init {
         if { [file isdirectory $filespec] } {
            if { [catch {glob $filespec/*.xml} filenames] } {
               tk_messageBox -icon error -parent . -message \
                  "initialization directory \"$filespec\" is empty"
            } else {
		# sort the list
		set filenames [lsort $filenames]
               foreach filename $filenames {
                  if { ! [file isdirectory $filename] } {
                     puts "processing $filename"
                     if { [catch {set file [open $filename RDONLY]} msg] } {
                        tk_messageBox -icon error -parent . -message \
                        "error opening initialization file \"$filename\": $msg"
                     } else {
                        set config [TcsConfigFile #auto [read $file] .]
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
               set config [TcsConfigFile #auto [read $file] .]
               itcl::delete object $config
               close $file
            }
         }
      }
   }

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

# It is now safe to display the main window.
   wm deiconify .

# Fix the window size.
   update
   wm resizable . 0 0

# Add Global variables
   # putting AOreuseGains here since I can't think of a better place at the moment (low O2)... might reconsider
   global sfPos instPO AOreuseGains AOtryingToReuseGain
   set sfPos empty
   set instPO rotator_axis
   set AOreuseGains 0
   set AOtryingToReuseGain 0

   return
}
