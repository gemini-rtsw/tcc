proc loadCatalog {} {
#+
#  Procedure:
#  LoadCatalog
#
#  Purpose:
#  Loads the contents of a star catalog into the catalog list box
#
#  Description:
#  This procedure opens a star catalog file and loads its contents into
#  the catalogue list box, one line per entry. Comment line (lines starting
#  with !), blank lines and the END line are discarded.
#
#  Each star is loaded into the Az/El plot widget.
#
#  The first star in the catalog is then selected.
#
#  D Terrett 26 October 1998
#
#  Copyright CCLRC
#-
   global Widgets Catalog

# Get the catalogue name.
   set types {
      {{Data Files} {.dat} {}}
      {{All Files} * {}}
   }
   set catname [tk_getOpenFile -defaultextension .dat -parent . \
                -filetypes $types -title "Open star catalog"]

   if { $catname != {} } {

# Open the catalogue.
      set chan [open $catname RDONLY]

# Clear the current catalogue and delete the contents of the catalogue
# list widget (if it exists).
      unset Catalog
      $Widgets(azelplot) remove ""
      if { [winfo exists $Widgets(cat)] } {
         $Widgets(cat) delete 0 end
      }

# Read the catalogue a line at a time.
      while { [gets $chan line] != -1 } {

# The END keyword marks the end of the catalogue.
         if { [string compare $line END] == 0 } break
         set line [string trim $line]

# Comment lines start with !.
         if { [string length $line] != 0 && [string range $line 0 0] != "!" } {

# Add the line to the catalogue and load the line into the list widget.
            lappend Catalog $line
            if { [winfo exists $Widgets(cat)] } {
               $Widgets(cat) insert end $line
            }
         }
      }
      close $chan

      set n [llength $Catalog]

# For each star in the catalogue.
      for { set i 0} { $i < $n} {incr i} {

# Decode the line.
         set def [decodeStar $i]

# Decode the RA and Dec.
         set ra [expr [slaDafin [lindex $def 1]] * 15.0]
         set dec [slaDafin [lindex $def 2]]

# Add the star to the Az/El plot widget.
         $Widgets(azelplot) add $i $ra $dec
         update
      }

# Set the current selection in the view widget to the first line.
      if { [winfo exists $Widgets(cat)] } {
         $Widgets(cat) selection anchor 0
         $Widgets(cat) selection set 0
         $Widgets(cat) see 0
         $Widgets(azelplot) select 0
      }
   }
   return
}
