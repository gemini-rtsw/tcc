#+
#  createEditPanels.tcl
#
#  Creates and EditPanel and then calls itself to create the edit panels
#  for any subcompents.
#
#  D Terrett 9 September 1998
#
#  Copyright CCLRC
#-

proc createEditPanels {parent component class namespace list title vt} {

# Create the edit panel that edits this component.
   EditPanelMgr ${parent}${component}Panel \
         -name .${parent}${component}Panel -class ${class}Panel \
         -title $title -componentlist $list -component ${component} \
         -controls ${parent}${component} -namespace $namespace -vt $vt \

# Create the EditShadow.
   EditShadow ${parent}${component} ${parent}${component}Panel \
         -parent $parent -component $component

# Create all the edit panels for the subcomponents.
   set children ""
   foreach subcomp [${class}::subcomponents] {
      lappend children [lindex $subcomp 0]
      if { [lindex $subcomp 5] != "" } {
         set subvt [lindex $subcomp 5]
      } else {
         set subvt $vt
      }
      createEditPanels ${parent}${component} [lindex $subcomp 0] \
         [lindex $subcomp 1] [lindex $subcomp 2] [lindex $subcomp 3] \
         "$title [lindex $subcomp 4]" $subvt
   }

# Configure the EditShadows children.
   ${parent}${component} configure -children $children
}
