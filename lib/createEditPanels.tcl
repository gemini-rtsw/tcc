#+
#  createEditPanels
#
#  Creates an EditPanel and its shadow and then calls itself to create 
#  the edit panels for any subcomponents.
#
#  Arguments:
#            parent     The parent widget for the edit panel.
#
#            component  The name of the configuration component that the edit
#                       panel will edit.
#
#            class      The class of the component.
#
#            namespace  The namespace in which the components are created.
#
#            list       The component list to which the component belongs.
#
#            title      The title for the edit panel.
#
#            vt         The virtual telescope argument for the component.
#
#  D Terrett 6 June 2001
#
#  Copyright CCLRC
#-

proc createEditPanels {parent component class namespace list title vt} {

# Create the edit panel that edits this component.
   tcclib::EditPanelMgr ${parent}${component}Panel \
         -name .${parent}${component}Panel -class ${class}Panel \
         -title $title -componentlist $list -namespace $namespace -applyarg $vt

# Create the EditShadow.
   tcclib::EditShadow ${parent}${component} ${parent}${component}Panel \
         -parent $parent -component $component -namespace $namespace

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

# Configure the EditShadows -children option.
   ${parent}${component} configure -children $children
}
