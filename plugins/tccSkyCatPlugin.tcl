#+
#  tccSkyCatPlugin.tcl
#
#  D Terrett 1 December 1998
#
#  Copyright CCLRC
#-

proc SkyCat_plugin {this} {

# Set the application name to the name that the consoles use to send commands
# to skycat.
   tk appname tccSkycat

# Get the top level window name and call the procedure that stores the name
# inside the TccSkyCat namespace.
   set w [namespace tail $this]
   TccSkyCat::store_names $w

# Add the Gemini specific menu to the menubar.
   $w add_menubutton Gemini
   set gemmenu [$w get_menu Gemini]
   $w add_menuitem $gemmenu command "Pick Science Target" \
         "Pick an object and define it as a science target" \
         -command "TccSkyCat::pick_target science"
   $w add_menuitem $gemmenu command "Pick PWFS1 Target" \
         "Pick an object and define it as the peripheral wave front sensor 1 target" \
         -command "TccSkyCat::pick_target guidepwfs1"
   $w add_menuitem $gemmenu command "Pick PWFS2 Target" \
         "Pick an object and define it as the peripheral wave front sensor 2 target" \
         -command "TccSkyCat::pick_target guidepwfs2"
   $w add_menuitem $gemmenu command "Pick OIWFS Target" \
         "Pick an object and define it as the on instrument wave front sensor target" \
         -command "TccSkyCat::pick_target guideoiwfs"
   $w add_menuitem $gemmenu command "Set IPD" \
         "Set Instrument Principle Direction" \
         -command "TccSkyCat::pick_angle"

# Set the name of the command called when a new image is loaded.
   set image [$w component image]
   $image configure -newimagecmd TccSkyCat::draw_field
}

proc tccDisplayField {ra dec equinox} {

   TccSkyCat::display_image $ra $dec $equinox
}

proc tccSetTcc interp {
   global tcc_interp
   set tcc_interp $interp
}

namespace eval TccSkyCat {

   variable toplevel
   variable canvas
   variable image
   variable rtdimage

# This procedure stores the names of the skycat top level window, the
# rtd_image object and the canvas that the image is plotted on. 
   proc store_names w {

      global toplevel image canvas rtdimage
      set toplevel $w
      set image [$toplevel component image]
      set canvas [$toplevel component image component canvas]
      set rtdimage [$toplevel component image get_image]
   }

# This procedure draws the Gemini field centred on the supplied position.
   proc draw_field {} {
      global tcc_interp canvas toplevel rtdimage

# Get the field centre in world coordinates.
      set centre [$rtdimage wcscenter -format 1]

# Get the field centre in canvas coordinates.
      $rtdimage convert coords [lindex $centre 0] [lindex $centre 1] deg \
            xc yc canvas

# Get the size of the rectangle that encloses the field radius (7 arcmin)
# as canvas coords.
      $rtdimage convert dist 0.117 0.117 deg xs ys canvas

# Plot the field size.
      $canvas create oval [expr $xc - $xs] [expr $yc - $ys] \
            [expr $xc + $xs] [expr $yc + $ys] -outline white -tags objects

# Radius (in pixels) of the WFS table - same as field size for now.
      set r $xs

# Plot the wave front sensor positions
      foreach wfs {pwfs1_target pwfs2_target} {
         set target [send $tcc_interp tccPreview get $wfs]
         if { $target != "" } {
            if { [catch {$rtdimage convert coords [lindex $target 0] \
                   [lindex $target 1] {wcs J2000} x y canvas} ] } {
            } else {
               $canvas create polygon $x [expr $y - 6] [expr $x - 6] $y \
                     $x [expr $y + 6] [expr $x + 6] $y -fill white -tags objects
   
# Plot the two possible WFS arm positions.
   
# WFS position relative to field centre.
               set dx [expr $x - $xc]
               set dy [expr $y - $yc]

# Half the distance of the WFS from the field centre.
               set s2 [expr sqrt ( $dx * $dx + $dy * $dy ) / 2]

# Bearing of WFS.
               set b [expr atan2 ( - $dy , $dx )]

# Angle between WFS bearing and arm pivot.
               set d [expr acos ( $s2 / $r )]

# X/Y of pivot.
               set x1 [expr $xc + $r * cos ( $b + $d )]
               set y1 [expr $yc - $r * sin ( $b + $d )]
               $canvas create line $x $y $x1 $y1 -fill white -tags objects

# Similarly for the other possible position.
               set x1 [expr $xc + $r * cos ( $b - $d )]
               set y1 [expr $yc - $r * sin ( $b - $d )]
               $canvas create line $x $y $x1 $y1 -fill white -tags objects
            }
         }
      }
   }

# This procedure displays a dialog box for the user to specify the object
# name and sends the result of the pick to the tcc is a target definition.
   proc pick_target type {
      global tcc_interp

      set result [::skycat::SkyCat::pick_object]

# Activate the appropriate editing panel and get the name of the scratch
# target object associated with that edit control.
      send $tcc_interp tcsconfig${type}Target map
      set control [send $tcc_interp tcsconfig${type}Target cget -window]
      set name [send $tcc_interp $control scratchobj]

# Update the definition of the scratch object.
      send $tcc_interp $name configure \
            -objName \"$name\" \
            -radec \"[list [lindex $result 2] [lindex $result 3]]\" \
            -cosys \"FK5/[lindex $result 4]\"

# Clear the current target and load the contents of the scratch object
      send $tcc_interp tcsconfig${type}Target select \"\"
      send $tcc_interp $control selectobj $name
   }

# This procedure displays an image or replots the field if the image has
# already been drawn.
   proc display_image {ra dec equinox} {
      global rtdimage canvas

      set centre [$rtdimage wcscenter -format 1]
      set x [lindex $centre 0]
      set y [lindex $centre 1]
      if { $x != "" && $y != "" } {
         $rtdimage convert coords $x $y deg xc yc canvas
         if { [catch {$rtdimage convert coords $ra $dec {wcs $equinox} \
               x y canvas} ] } {
            ::skycat::SkyCat::display_image $ra $dec 16.0 16.0 $equinox
         } else {
            if { abs($x - $xc) < 100.0 && abs($y - $yc) < 100.0 } {
               $canvas delete objects
               draw_field
            } else {
               ::skycat::SkyCat::display_image $ra $dec 16.0 16.0 $equinox
            }
         }
      } else {
         ::skycat::SkyCat::display_image $ra $dec 16.0 16.0 $equinox
      }
   }

# Procedure to select an IPD.
   proc pick_angle {} {
      global rtdimage canvas

# Draw a line starting at the image centre.
      set centre [$rtdimage wcscenter -format 1]
      set x [lindex $centre 0]
      set y [lindex $centre 1]
      if { $x != "" && $y != "" } {
         $rtdimage convert coords $x $y deg xc yc canvas
         $canvas create line $xc $yc $xc $yc -fill white -arrow last \
               -tags {tcc objects}

# Tie mouse motion to the other end of the line.
         bind $canvas <Motion> {
            set coords [%W coords tcc]
            set x1 [%W canvasx %x]
            set y1 [%W canvasy %y]
            %W coords tcc  [lindex $coords 0] [lindex $coords 1] $x1 $y1
         }
      }

# Tie button 1 to a procedure that sets the IPD and removes the bindings.
         bind $canvas <Button-1> {TccSkyCat::set_ipd %W %x %y}
   }

   proc set_ipd {canvas x y} {
      global rtdimage

# Cancel the bindings.
      bind $canvas <Motion> {}
      bind $canvas <Button-1> {}

# Find the angle of the line (up through left).
      set coords [$canvas coords tcc]
      set a0 [expr atan2( [lindex $coords 0] - [lindex $coords 2], \
            [lindex $coords 1] - [lindex $coords 3] )]
      if { $a0 < 0.0 } {set a0 [expr $a0 + 6.283185307]}

# Get the bearing of North.
      $rtdimage convert coords [lindex $coords 0] [lindex $coords 1] \
            canvas xc yc deg
      $rtdimage convert coords [expr $xc + 0.1] $yc deg x1 y1 canvas
      set a1 [expr atan2( [lindex $coords 0] - $x1, \
            [lindex $coords 1] - $y1 ) - 1.570796327]
      if { $a1 < 0.0 } {set a1 [expr $a1 + 6.283185307]}

# IPA
      set ipa [expr $a0 - $a1]
      if { $ipa < 0.0 } {
         set ipa [expr $ipa + 6.283185307]
      }

# Activate the rotator editing panel and get the name of the scratch
# rotator object associated with that edit control.
      global tcc_interp
      send $tcc_interp tcsconfigrotator map
      set control [send $tcc_interp tcsconfigrotator cget -window]
      set name [send $tcc_interp $control scratchobj]

# Update the definition of the scratch object.
      send $tcc_interp $name configure \
            -ipa \"[expr $ipa * 57.295779513]\" \
            -cosys FK5/J2000

# Select it.
      send $tcc_interp tcsconfigrotator select \"\"
      send $tcc_interp $control selectobj $name
   }
}
