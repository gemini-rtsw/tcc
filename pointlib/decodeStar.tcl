proc decodeStar {index} {
#+
#  Procedure:
#  decodeStar
#
#  Purpose:
#  Decodes an entry in the star catalogue
#
#  Description:
#  The specified entry in the catalogue listbox is decoded
#
#  D Terrett 1 Febraury 1999
#
#  Copyright CCLRC
#-
   global CatForm Catalog
   set rec [lindex $Catalog $index]
   if { $rec != "" } {
      switch $CatForm {
         tpoint {
            scan $rec "%s %s %s %s %s %s %s %s %s %s" \
                 objname rah ram ras decd decm decs pmra pmdec equinox
            set i [string first ! $rec]
            if { $i >= 0 } {
               set comment [string trimleft [string range $rec $i end] !]
            } else {
               set comment ""
            }
            set ra "$rah $ram $ras"
            set dec "$decd $decm $decs"
            set epoch $equinox
            set frame FK5
            set parallax 0.0
            set rv 0.0
         }
      }
   }

   return [list $objname $ra $dec $frame $equinox $epoch $pmra $pmdec \
         $parallax $rv $comment]
}
