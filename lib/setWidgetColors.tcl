
#  setWidgetColors is a procedure that sets the colour related options
#  of a list of widgets on the basis of the supplied background and foreground
#  colours. The algorithms for generating the colour values come from
#  pallet.tcl in the tk distribution.

proc setWidgetColors {widgets background foreground} {

   set new(background) $background
   set new(foreground) $foreground

   set bg [winfo rgb . $background]
   set fg [winfo rgb . $foreground]

   set darkerBg [format #%02x%02x%02x [expr (9*[lindex $bg 0])/2560] \
         [expr (9*[lindex $bg 1])/2560] [expr (9*[lindex $bg 2])/2560]]

   foreach i {activeForeground insertBackground selectForeground \
         highlightColor} {
      set new($i) $new(foreground)
   }

   set new(disabledforeground) [format #%02x%02x%02x \
         [expr (3*[lindex $bg 0] + [lindex $fg 0])/1024] \
         [expr (3*[lindex $bg 1] + [lindex $fg 1])/1024] \
         [expr (3*[lindex $bg 2] + [lindex $fg 2])/1024]]

   foreach i {0 1 2} {
      set light($i) [expr [lindex $bg $i]/256]
      set inc1 [expr ($light($i)*15)/100]
      set inc2 [expr (255-$light($i))/3]
      if {$inc1 > $inc2} {
         incr light($i) $inc1
      } else {
         incr light($i) $inc2
      }
      if {$light($i) > 255} {
         set light($i) 255
      }
   }
   set new(activeBackground) [format #%02x%02x%02x $light(0) \
         $light(1) $light(2)]

   set new(selectBackground) $darkerBg
   set new(troughColor) $darkerBg
   set new(textBackground) $darkerBg
   set new(selectColor) #b03060

   set appname [winfo name .]
   foreach option [array names new] {
      foreach w $widgets {
         option add ${appname}$w*$option $new($option)
      }
   }
}

