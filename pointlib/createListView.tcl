#+
# createListView.tcl
#
# Creates the list view of the star catalog if it doesn't already exist.
#
# D Terrett 16 October 1998
#
# Copyrigt CCLRC
#-

proc createListView {} {

   global Widgets Catalog

# Create the widget.
   if { ! [winfo exists .listtop] } {
      toplevel .listtop
      set Widgets(cat) [iwidgets::scrolledlistbox .listtop.cat]
      pack $Widgets(cat) -fill both -expand yes

# Bind a mouse click in the list box to giving the focus to the list box and
# selecting the star under the mouse.
      bind [$Widgets(cat) component listbox] <Button-1> {
         focus %W
      }

# Make selecting a star in the listbox select the star on the plot.
      $Widgets(cat) configure -selectmode single -selectioncommand \
           "$Widgets(azelplot) select \[$Widgets(cat) index anchor\]"
   }

# Load the catalog into the widget.
   foreach line $Catalog {
      $Widgets(cat) insert end $line
   }
   $Widgets(cat) selection anchor 0
   $Widgets(cat) selection set 0
   $Widgets(cat) see 0
}
