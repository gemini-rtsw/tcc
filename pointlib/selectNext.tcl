proc selectNext {} {
#+
#  Procedure
#  selectNext
#
#  Purpose
#  Selects the next star.
#
#  Description
#
#  D Terrett 21 October 1998
#
#  Copyright CCLRC
#+
   global Widgets NextStyle
   set tag [$Widgets(azelplot) next $NextStyle]
   if { $tag != "" && [winfo exists $Widgets(cat)]} {
       $Widgets(cat) selection clear [$Widgets(cat) index anchor]
       $Widgets(cat) selection anchor $tag
       $Widgets(cat) selection set $tag
       $Widgets(cat) see $tag
   }
   $Widgets(azelplot) select $tag
   return
}
