#+
#  tccSkySearchPlugin.tcl
#
#  D Terrett 13 July 2001
#
#  Copyright CCLRC
#-

proc SkySearch_plugin {this} {

# Get the top level window name and call the procedure that stores the name
# inside the TccSkyQuery namespace.
   set w [namespace tail $this]
   TccSkyQuery::store_names $w

   set w [namespace tail $this]

# Add the Gemini menu
   $w add_menubutton Gemini
   set gemmenu [$w get_menu Gemini]
   $w add_menuitem $gemmenu command "Selection -> Science target" \
         "Define the selected entry as a science target" \
         -command "TccSkyQuery::define_selected science"
   $w add_menuitem $gemmenu command "Selection -> PWFS1 target" \
         "Define the selected entry as the peripheral wave front sensor 1 target" \
         -command "TccSkyQuery::define_selected guidepwfs1"
   $w add_menuitem $gemmenu command "Selection -> PWFS2 target" \
         "Define the selected entry as the peripheral wave front sensor 2 target" \
         -command "TccSkyQuery::define_selected guidepwfs2"
   $w add_menuitem $gemmenu command "Selection -> OIWFS target" \
         "Define the selected entry as the on instrument wave front sensor target" \
         -command "TccSkyQuery::define_selected guideoiwfs"
   $w add_menuitem $gemmenu command "Result -> Science targets" \
         "Define the search result as science targets" \
         -command "TccSkyQuery::define_contents science"
   $w add_menuitem $gemmenu command "Result -> WFS targets" \
         "Define the search result as wave front sensor targets" \
         -command "TccSkyQuery::define_contents wfs"
}

namespace eval TccSkyQuery {

   variable toplevel

# This procedure stores the names of the SkyQuery top level window.
   proc store_names w {

      global toplevel
      set toplevel $w
   }

   proc define_selected type {
      global toplevel
      set results [$toplevel component results]
      define_targets $type [$results get_selected]
   }

   proc define_contents type {
      global toplevel
      set results [$toplevel component results]
      define_targets $type [$results get_contents]
   }

   proc define_targets {type targets} {

      global tcc_interp toplevel
      set results [$toplevel component results]
      set searchopts [$toplevel component searchopts]
      set astrocat [$searchopts cget -astrocat]
      set headings [$results get_headings]
      set id [$astrocat id_col]
      set cooSystem [lsearch $headings cooSystem]
      set cooType [lsearch $headings cooType]
      set ra [$astrocat ra_col]
      set dec [$astrocat dec_col]
      set epoch [lsearch $headings epoch]
      set pma [lsearch $headings pma]
      set pmd [lsearch $headings pmd]
      set radvel [lsearch $headings radvel]
      set parallax [lsearch $headings parallax]

      if { $type == "science" } {
         set list Science
      } else {
         set list Wfs
      }

      foreach target $targets {
         set args [list -objName [lindex $target $id]]
         if { $ra != -1 && $dec != -1 } {
            set rav [lindex $target $ra]
            set decv [lindex $target $dec]
            if { $rav != "" && $decv != "" } {
               lappend args -radec [list $rav $decv]
            }
         }
         set_frame $target $cooSystem $cooType
         if { $epoch != -1 } {
            set epochv [lindex $target $epoch]
            if { $epochv != "" } {
               lappend args -epoch $epochv
            }
         }
         if { $pma != -1  && $pmd != -1 } {
            set pmav [lindex $target $pma]
            set pmdv [lindex $target $pmd]
            if { $pmav != "" && $pmdv != "" } {
               lappend args -pm \
                    "[expr $pmav * 13750.987] [expr $pmdv * 206264.806]"
            }
         }
         if { $radvel != -1 } {
            set radvalv [lindex $target $radvel]
            if { $radvalv != "" } {
               lappend args -rv $radvelv
            }
         }
         if { $parallax != -1 } {
            set parallaxv [lindex $target $parallax]
            if { $parallaxv != "" } {
               lappend args -parallax $parallaxv
            }
         }
         
         send $tcc_interp create${list}Target \"[lindex $target $id]\" $args
      }

      if { [llength $targets] == 1 } {
         send $tcc_interp tcsconfig${type}Target map
         send $tcc_interp tcsconfig${type}Target \
               selectfromcontrol \"[lindex [lindex $targets 0] $id]\"
      }
   }

   proc set_frame {target cooSystem cooType} {
      upvar 1 args args

      if { $cooType != -1 } {
         if { [lindex $target $cooType] == "A" } {
            lappend args -cosys Apparent
            return
         }
      }
      if { $cooSystem != -1 } {
         set equinox [lindex $target $cooSystem]
         if { [string range $equinox 0 0] == "B" } {
            lappend args -cosys FK4/$equinox
            return
         }
         lappend args -cosys FK5/$equinox
         return
      }
      lappend args -cosys FK5/J2000
   }
}
