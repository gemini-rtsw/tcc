proc pointTestMain {w} {
#+
#  Function name:
#  pointTestMain
#
#  Creates the user interface for the pointTest application and creates the
#  command senders and status acceptors needed to drive the TCS.
#
#  D Terrett 30 May 1999
#
#  Copyright CCLRC
#-
   global Menus Widgets

# Connect to TCS status
   epics sa applyC
   sa applyC link val Status

# source A and slew commands
   epics cs sourceA
   epics cs slew

# pointLog command
   epics cs pointLog

# tcs SAD database
   epics sa tcssad
   epics sa mcssad

# Wait to get connected to the TCS.
   waitConnect

# Create widgets - menu bar and menus
   set row 0
   grid [set w [frame $w.mbar -relief raised -bd 2]] -columnspan 2 -sticky ew \
        -row $row
   pack [set w [menubutton $w.file -text File -underline 0]] -side left
   set Menus(file) [menu $w.menu -tearoff 0]
   $w configure -menu $Menus(file)
   $Menus(file) add command -label "Open log file" -underline 0
   $Menus(file) add command -label "Close log file" -underline 0
   $Menus(file) add command -label "Load catalogue" -underline 0
   $Menus(file) add command -label Exit -underline 1
   set w [winfo parent $w]

   pack [set w [menubutton $w.view -text View -underline 0]] -side left
   set Menus(view) [menu $w.menu -tearoff 0]
   $w configure -menu $Menus(view)
   $Menus(view) add command -label "Options..." -underline 0
   $Menus(view) add command -label "Catalog" -underline 0

# Options dialog
   set Dialogs(options) [iwidgets::dialog $w.options]
   $Dialogs(options) hide "Apply"
   $Dialogs(options) hide "Cancel"
   $Dialogs(options) hide "Help"
   $Dialogs(options) buttonconfigure "OK" -text "Dismiss"
   set c [$Dialogs(options) childsite]
   pack [frame $c.f -relief sunken -bd 2] -expand yes -fill both
   pack [checkbutton $c.f.slew -variable Options(slew) \
         -text "Slew when star is selected"] -anchor w
   pack [checkbutton $c.f.log -variable Options(log) \
         -text "Log observation at end of slew"] -anchor w
   pack [checkbutton $c.f.select -variable Options(select) \
         -text "Select next star after logging"] -anchor w
   pack [checkbutton $c.f.skip -variable Options(skip) \
         -text "Skip star on error"] -anchor w
   pack [checkbutton $c.f.zlpm -variable Options(zlpm) \
         -text "Zero local pointing model on slew"] -anchor w
   pack [iwidgets::radiobox $c.nextstyle -labeltext "Star selection mode" \
         -command "set NextStyle \[$c.nextstyle get\]"] -expand yes -fill both
   $c.nextstyle add minslew -text "Shortest slew"
   $c.nextstyle add catorder -text "Next in catalog"
   $c.nextstyle select minslew
   
   pack [iwidgets::radiobox $c.catform -labeltext "Catalog format" \
         -command "set CatForm \[$c.catform get\]"] -expand yes -fill both
   $c.catform add tpoint -text "tpoint"
   $c.catform select tpoint

# Unpack the catalog format radio box because only one format is currently
# supported.
   pack forget $c.catform
   
   wm resizable $Dialogs(options) 0 0

   set w [string trimright [winfo parent $w] .]
   set w [string trimright [winfo parent $w] .]

# - entry widgets and labels
   incr row
   grid [set Widgets(objname) \
        [iwidgets::entryfield $w.objname -labeltext "Object name:"]] \
        -columnspan 2 -sticky ew -row $row

   incr row
   grid [set Widgets(ra) [iwidgets::entryfield $w.ra -labeltext "RA:"]] \
        [set Widgets(dec) [iwidgets::entryfield $w.dec -labeltext "Dec:"]] \
        -sticky ew -row $row

   incr row
   grid [set Widgets(frame) \
         [iwidgets::entryfield $w.frame -labeltext "Frame:"]] \
         [set Widgets(equinox) \
         [iwidgets::entryfield $w.equinox -labeltext "Equinox:"]] \
        -sticky ew -row $row

   incr row
   grid [set w [frame $w.f41]] -sticky ew -row $row
   pack [label $w.pmlabel -text "Proper motion:"] -side left
   pack [set Widgets(pmra) \
         [iwidgets::entryfield $w.pmra -labeltext "RA:" -width 10]] \
        -expand 1 -fill x -padx 2 -pady 2 -side left
   set w [string trimright [winfo parent $w] .]

   grid [set w [frame $w.f42]] -sticky ew -row $row -column 1
   pack [set Widgets(pmdec) \
        [iwidgets::entryfield $w.pmdec -labeltext "Dec:" -width 6]] \
        -expand 1 -fill x -padx 2 -pady 2 -side left
   pack [set Widgets(epoch) \
        [iwidgets::entryfield $w.epoch -labeltext "Epoch" -width 6]] \
        -expand 1 -fill x -padx 2 -pady 2 -side left
   set w [string trimright [winfo parent $w] .]

   incr row
   grid [set Widgets(parallax) \
        [iwidgets::entryfield $w.px -labeltext "Parallax:" -width 6]] \
        [set Widgets(rv) \
        [iwidgets::entryfield $w.rv -labeltext "Radial velocity:" -width 6]] \
        -sticky ew -row $row

   incr row
   grid [set w [frame $w.f6]] -columnspan 2 -sticky ew -row $row
   pack [label $w.statuslabel -text "TCS status"] -side left -padx 2 -pady 2
   pack [label $w.status -textvariable Status(value) -bg grey54 \
               -relief sunken] \
        -side left -padx 2 -pady 2
   pack [label $w.comentlabel -text "Comment:"] -side left -padx 2 -pady 2
   pack [label $w.comment -bg grey54 -relief sunken -anchor w \
        -textvariable Comment] \
        -side left -padx 2 -pady 2 -expand 1 -fill x
   set w [string trimright [winfo parent $w] .]

   incr row

# Skyplot window.
   grid [set Widgets(azelplot) [AzElPlot $w.azelplot]] -columnspan 2 \
        -sticky ewns -row $row
 
   incr row
   grid [set Widgets(slew) \
             [button $w.slew -text "Slew to target" -underline 0]] \
        [set Widgets(log) \
             [button $w.log -text "Log pointing data" -underline 0]] -pady 4

   if { $w == "" } {set w .}
   grid columnconfigure $w 0 -weight 1
   grid columnconfigure $w 1 -weight 1


# Define behaviour - Menus
   $Menus(file) entryconfigure "Open*" -command openLogFile
   $Menus(file) entryconfigure "Close*" -command closeLogFile
   $Menus(file) entryconfigure "Load*" -command loadCatalog
   $Menus(file) entryconfigure "Exit" -command pointTestExit

   $Menus(view) entryconfigure "Options..." \
         -command "$Dialogs(options) activate"
   $Menus(view) entryconfigure "Catalog" -command createListView

# - Entry widgets
   $Widgets(objname) configure -textvariable Target(objname)
   $Widgets(ra) configure -textvariable Target(ra)
   $Widgets(dec) configure -textvariable Target(dec)
   $Widgets(frame) configure -textvariable Target(frame)
   $Widgets(equinox) configure -textvariable Target(equinox)
   $Widgets(pmra) configure -textvariable Target(pmra)
   $Widgets(pmdec) configure -textvariable Target(pmdec)
   $Widgets(epoch) configure -textvariable Target(epoch)
   $Widgets(parallax) configure -textvariable Target(parallax)
   $Widgets(rv) configure -textvariable Target(rv)

# - Stop keypresses from triggering button bindings
   bind Entry <Key> "[bind Entry <Key>]; break"

# - Star selection
   $Widgets(azelplot) configure -selectioncommand selectStar
   $Widgets(azelplot) configure -hitcommand loadPosition

# - buttons
   $Widgets(slew) configure -command slewTel
   $Widgets(log) configure -command logObs
   bind all <s> "$Widgets(slew) invoke"
   bind all <l> "$Widgets(log) invoke"

# Set initial state of interface
   $Menus(file) entryconfigure "Close*" -state disabled
   $Widgets(log) configure -state disabled
   global CatForm
   set CatForm tpoint
   global NextStyle
   set NextStyle minslew

# Initialise global variables
   global Version Logchan Retries Catalog
   set Version 1.0
   set Logchan {}
   set Retries 0
   set Catalog {}
   set Widgets(cat) ""

   return
}
