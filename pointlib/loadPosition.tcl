#+
#  loadPosition.tcl
#
#  Loads an Apparent RA/Dec into the target position widgets.
#
#  D Terrett 22 October 1998
#
#  Copyright CCLRC
#-
proc loadPosition {ra dec} {

   global Target

# Format the RA/Dec as HH:MM and DD
   set ram [expr round($ra*229.18)]
   if { $ram < 0 } {set ram [expr $ram + 1440]}
   set rah [expr round(floor($ram / 60.0))]
   set ram [expr round($ram - $rah * 60.0)]
   set decm [expr round($dec*3437.746)]
   set dech [expr round(floor($decm / 60.0))]
   set decm [expr round($decm - $dech * 60.0)]

# Load the RA, Dec and frame target widgets and clear the rest.
   set Target(objname) ""
   set Target(ra) [format %02d:%02d $rah $ram]
   set Target(dec) [format %0+3d:%02d $dech $decm]
   set Target(frame) APPT
   set Target(equinox) ""
   set Target(pmra) ""
   set Target(pmdec) ""
   set Target(parallax) ""
   set Target(rv) ""
}

