#+
#  tccSkyCatPlugin.tcl
#
#  D Terrett & C Mayer  11 February 2000
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
   $w add_menuitem $gemmenu command "Toggle PWFS1 arm" \
         "Select the other orientation for PWFS1 probe arm" \
         -command "TccSkyCat::toggle_arm pwfs1"
   $w add_menuitem $gemmenu command "Toggle PWFS2 arm" \
         "Select the other orientation for PWFS2 probe arm" \
         -command "TccSkyCat::toggle_arm pwfs2"

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
      global togglePwfs1 togglePwfs2

      set pwfs1Color     white       ;# Plot colour for pwfs1
      set pwfs2Color     yellow      ;# Plot colour for pwfs2

      if {![info exists togglePwfs1]} {
        set togglePwfs1 subtract
      }

      if {![info exists togglePwfs2]} {
        set togglePwfs2 subtract
      }

# Get the field centre in world coordinates.
      set centre [$rtdimage wcscenter -format 1]

# Get the field centre in canvas coordinates.
      $rtdimage convert coords [lindex $centre 0] [lindex $centre 1] deg \
            xc yc canvas

# Get the size of the rectangle that encloses the field radius (418")
# of PWFS1 as canvas coords.
      $rtdimage convert dist 0.1161 0.1161 deg xs ys canvas

# Plot the field size.
      $canvas create oval [expr $xc - $xs] [expr $yc - $ys] \
            [expr $xc + $xs] [expr $yc + $ys] -outline $pwfs1Color -tags objects

# Derive conversion factor to go from mm in focal plane to pixels
      set mm2Pixels [expr $xs * 1.611 / (0.1161 * 3600.0)]

# Radius (in pixels) of the WFS table - same as field size of PWFS1 for now.
      set r $xs

# Get the size of the rectangle that encloses the field radius (421")
# of PWFS2 as canvas coords.
      $rtdimage convert dist 0.1169 0.1169 deg xs ys canvas

# Plot the field size.
      $canvas create oval [expr $xc - $xs] [expr $yc - $ys] \
            [expr $xc + $xs] [expr $yc + $ys] -outline $pwfs2Color -tags objects

# Plot the wave front sensor positions
      foreach wfs {pwfs1_target pwfs2_target} {
         if {$wfs == "pwfs1_target"} {
           set pwfsColor $pwfs1Color
           set toggle $togglePwfs1
         } else {
           set pwfsColor $pwfs2Color
           set toggle $togglePwfs2
         }
         set target [send $tcc_interp tccPreview get $wfs]
         if { $target != "" } {
            if { [catch {$rtdimage convert coords [lindex $target 0] \
                   [lindex $target 1] {wcs J2000} x y canvas} ] } {
            } else {
   
# Plot the two possible WFS arm positions.

               plotWfs $canvas $wfs $x $y $xc $yc $mm2Pixels $toggle
   
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

# This procedure takes a  WFS object and draws its projected
# shape in the focal plane of the image

  proc plotWfs {canvas wfs x y xc yc mm2Pix toggle} {

# Define constants that define geometry of the two PWFS

      set m2FplaneSep 16539.326      ;# Distance from M2 to focal plane
      set m2Radius    511.0          ;# Radius of M2 (mm)
      set armLength      400.0       ;# Pivot to mirror centre (mm)
      set pwfs1FplaneSep 1900.0      ;# Distance of pwfs1 from focal plane
      set pwfs2FplaneSep 1615.0      ;# Distance of pwfs2 from focal plane
      set pwfs1Mwidth    146.0       ;# Width of pickoff mirror (mm)
      set pwfs1Mlength   146.0       ;# Length of pickoff mirror (mm)
      set pwfs2Mwidth    122.0       ;# Width of pickoff mirror (mm)
      set pwfs2Mlength   122.0       ;# Length of pickoff mirror (mm)
      set pwfs1Awidth    196.0       ;# Width of probe arm (mm)
      set pwfs2Awidth    196.0       ;# Width of probe arm (mm)
      set pwfs1Adepth    196.0       ;# Depth of pwfs1 support (mm)
      set pwfs2Adepth    196.0       ;# Depth of pwfs2 support (mm)
      set pwfs1Color     white       ;# colour for pwfs1
      set pwfs2Color     yellow      ;# colour for pwfs2

# Set scale to convert mm in focal plane to mm at height of pwfs

      set pwfs1Scale [expr ($m2FplaneSep - $pwfs1FplaneSep)/$m2FplaneSep]
      set pwfs2Scale [expr ($m2FplaneSep - $pwfs2FplaneSep)/$m2FplaneSep]

# Set up parameters for appropriate wfs

      if {$wfs == "pwfs1_target"} {
        set pwfsScale $pwfs1Scale
        set pwfsColor $pwfs1Color
        set pwfsFplaneSep $pwfs1FplaneSep
        set pwfsMwidth $pwfs1Mwidth
        set pwfsMlength $pwfs1Mlength
        set pwfsAwidth $pwfs1Awidth
      } else {
        set pwfsScale $pwfs2Scale
        set pwfsColor $pwfs2Color
        set pwfsFplaneSep $pwfs2FplaneSep
        set pwfsMwidth $pwfs2Mwidth
        set pwfsMlength $pwfs2Mlength
        set pwfsAwidth $pwfs2Awidth
      }

# WFS position relative to field centre with allowance for height of wfs
# above focal plane.
      set dx [expr ($x - $xc)*$pwfsScale]
      set dy [expr ($y - $yc)*$pwfsScale]

# Half the distance of the WFS from the field centre.
      set s2 [expr sqrt ( $dx * $dx + $dy * $dy ) / 2]

# Bearing of WFS.
      set b [expr atan2 ( - $dy , $dx )]

# Radius (in pixels) of the WFS table
      set r [expr $armLength * $mm2Pix]

# Angle between WFS bearing and arm pivot.
      set d [expr acos ( $s2 / $r)]

# X/Y of pivot.
      set xp [expr $xc + $r * cos ( $b + $d )]
      set yp [expr $yc - $r * sin ( $b + $d )]
#      $canvas create line $x $y $xp $yp -fill $pwfsColor -tags objects
      if {$toggle == "add" } {
        set xpSave $xp
        set ypSave $yp
      }

# Similarly for the other possible position.
      set xp [expr $xc + $r * cos ( $b - $d )]

      set yp [expr $yc - $r * sin ( $b - $d )]
#      $canvas create line $x $y $xp $yp -fill $pwfsColor -tags objects
      if {$toggle == "subtract" } {
        set xpSave $xp
        set ypSave $yp
      }

# Set offset for determining the completely vignetted/unvignetted coords
# of a point
      set dr [expr $m2Radius * $pwfsFplaneSep / $m2FplaneSep]

# Coords of corners of object for fully vignetted case when it is aligned
# along the x axis with the end of the object furthest from the pivot
# point. 

# Partially vignetted region

      set x1 [expr ($pwfsMlength/2.0 + $dr) * $mm2Pix + $r]
      set y1 [expr (-$pwfsMwidth/2.0 - $dr) * $mm2Pix ]

      set x2 $x1
      set y2 [expr -$y1]

      set x3 [expr (-$pwfsMlength/2.0 + $dr) * $mm2Pix + $r]
      set y3 $y2

      set x4 $x3
      set y4 $y1

      set vigType partial 
      plotShape $canvas $pwfsColor $pwfsScale $xc $yc $xpSave $ypSave $dx $dy \
                $x1 $y1 $x2 $y2 $x3 $y3 $x4 $y4 $vigType

      set x1 $x4
      set y1 [expr (-$pwfsAwidth/2.0 - $dr) * $mm2Pix ]

      set x2 $x1
      set y2 [expr -$y1]

      set x3 0.0
      set y3 $y2

      set x4 $x3
      set y4 $y1

      set vigType partial 
      plotShape $canvas $pwfsColor $pwfsScale $xc $yc $xpSave $ypSave $dx $dy \
                $x1 $y1 $x2 $y2 $x3 $y3 $x4 $y4 $vigType

# Mirror

      set x1 [expr ($pwfsMlength/2.0 - $dr) * $mm2Pix + $r]
      set y1 [expr (-$pwfsMwidth/2.0 + $dr) * $mm2Pix ]

      set x2 $x1
      set y2 [expr -$y1]

      set x3 [expr (-$pwfsMlength/2.0 + $dr) * $mm2Pix + $r]
      set y3 $y2


      set x4 $x3
      set y4 $y1

      set vigType clear 
      plotShape $canvas $pwfsColor $pwfsScale $xc $yc $xpSave $ypSave $dx $dy \
                $x1 $y1 $x2 $y2 $x3 $y3 $x4 $y4 $vigType

# Arm

      set x1 $x4
      set y1 [expr (-$pwfsAwidth/2.0 + $dr) * $mm2Pix ]

      set x2 $x1
      set y2 [expr -$y1]

      set x3 0.0
      set y3 $y2

      set x4 $x3
      set y4 $y1

      set vigType blocked 
      plotShape $canvas $pwfsColor $pwfsScale $xc $yc $xpSave $ypSave $dx $dy \
                $x1 $y1 $x2 $y2 $x3 $y3 $x4 $y4 $vigType

  }

# Routine to draw a polygon as with four corners in the correct orientation
# and projection in the focal plane

  proc plotShape {canvas pwfsColor pwfsScale xc yc xp yp dx dy
                  x1 y1 x2 y2 x3 y3 x4 y4 vigType} {

# Rotate each x,y pair to a frame aligned along the axis of the probe

      set dxp [expr $dx + $xc - $xp]
      set dyp [expr $dy + $yc - $yp]
      set rot [expr atan2 ( -$dyp, $dxp)]

      set cosrot [expr cos($rot)]
      set sinrot [expr sin($rot)]

      set x1p [expr ($x1 * $cosrot + $y1 * $sinrot) + $xp]
      set y1p [expr (-$x1 * $sinrot + $y1 * $cosrot) + $yp]

      set x2p [expr ($x2 * $cosrot + $y2 * $sinrot) + $xp]
      set y2p [expr (-$x2 * $sinrot + $y2 * $cosrot) + $yp]

      set x3p [expr ($x3 * $cosrot + $y3 * $sinrot) + $xp]
      set y3p [expr (-$x3 * $sinrot + $y3 * $cosrot) + $yp]

      set x4p [expr ($x4 * $cosrot + $y4 * $sinrot) + $xp]
      set y4p [expr (-$x4 * $sinrot + $y4 * $cosrot) + $yp]

# Project these points into the focal plane

      set x1p [expr ($x1p - $xc)/$pwfsScale + $xc ]
      set y1p [expr ($y1p - $yc)/$pwfsScale + $yc ]

      set x2p [expr ($x2p - $xc)/$pwfsScale + $xc ]
      set y2p [expr ($y2p - $yc)/$pwfsScale + $yc ]

      set x3p [expr ($x3p - $xc)/$pwfsScale + $xc ]
      set y3p [expr ($y3p - $yc)/$pwfsScale + $yc ]

      set x4p [expr ($x4p - $xc)/$pwfsScale + $xc ]
      set y4p [expr ($y4p - $yc)/$pwfsScale + $yc ]

# Draw a polygon connecting these points

      if {$vigType == "partial" } {
        $canvas create polygon $x1p $y1p $x2p $y2p $x3p $y3p $x4p $y4p \
          -fill $pwfsColor -stipple gray12  \
          -tags objects
      } elseif {$vigType == "blocked"} {
        $canvas create polygon $x1p $y1p $x2p $y2p $x3p $y3p $x4p $y4p \
          -fill $pwfsColor -stipple gray50 \
          -tags objects
      } else {
        $canvas create polygon $x1p $y1p $x2p $y2p $x3p $y3p $x4p $y4p \
          -outline $pwfsColor -fill "" -tags objects
      }
  }

  proc toggle_arm {pwfs} {

   global togglePwfs1 togglePwfs2
   global canvas

   if {$pwfs == "pwfs1"} {
     if {$togglePwfs1 == "add"} {
       set togglePwfs1 subtract
     } else {
       set togglePwfs1 add
     }
   } else {
     if {$togglePwfs2 == "add"} {
       set togglePwfs2 subtract
     } else {
       set togglePwfs2 add
     }
   }
  
   $canvas delete objects
   draw_field

  }
}
