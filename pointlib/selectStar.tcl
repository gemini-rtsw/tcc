proc selectStar index {
#+
#  Procedure:
#  selectStar
#
#  Purpose:
#  Selects a star from the catalog listbox
#
#  Description:
#  The entry in the catalogue listbox is decoded and
#  loaded into the target position entry widgets.
#
#  If the "Slew when star selected" option has been selected the telescope
#  is slewed to new target.
#
#  D Terrett 1 February 1999
#
#  Copyright CCLRC
#-
   global Widgets Target Options Comment
   foreach {Target(objname) Target(ra) Target(dec) Target(frame) \
         Target(equinox) Target(epoch) Target(pmra) Target(pmdec) \
         Target(parallax) Target(rv) Comment} [decodeStar $index] {
   }

   if $Options(slew) {after idle $Widgets(slew) invoke}
   return
}
