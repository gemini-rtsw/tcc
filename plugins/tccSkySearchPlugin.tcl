#+
#  tccSkySearchPlugin.tcl
#
#  D Terrett 24 May 2002
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
         -command "TccSkyQuery::define_selected pwfs1"
   $w add_menuitem $gemmenu command "Selection -> PWFS2 target" \
         "Define the selected entry as the peripheral wave front sensor 2 target" \
         -command "TccSkyQuery::define_selected pwfs2"
   $w add_menuitem $gemmenu command "Selection -> OIWFS target" \
         "Define the selected entry as the on instrument wave front sensor target" \
         -command "TccSkyQuery::define_selected oiwfs"
   $w add_menuitem $gemmenu command "Result -> targets" \
         "Define the search result as targets" \
         -command "TccSkyQuery::define_contents science"
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

      foreach target $targets {
         set args [list -objname [lindex $target $id]]
         lappend args -type hmsdegTarget
         if { $ra != -1 && $dec != -1 } {
            set rav [lindex $target $ra]
            set decv [lindex $target $dec]
            if { $rav != "" && $decv != "" } {
               lappend args -c1 $rav -c2 $decv
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
               lappend args -pm1 [expr $pmav * 13750.987] \
                     -pm2 [expr $pmdv * 206264.806] -pmunits sec-arcsec/year
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
         
# Activate the appropriate editing panel and get the name of the scratch
# target object associated with that edit control.
         set panelmgr [send $tcc_interp set ::Config(tcs.field,panel)]
         send $tcc_interp $panelmgr map ${type}target
         set panel [send $tcc_interp $panelmgr cget -name]

         send $tcc_interp $panel newtarget [list [lindex $target $id]] \
               ${type}target $args
      }
   }

   proc set_frame {target cooSystem cooType} {
      upvar 1 args args

      if { $cooType != -1 } {
         if { [lindex $target $cooType] == "A" } {
            lappend args -system apparent
            return
         }
      }
      if { $cooSystem != -1 } {
         set equinox [lindex $target $cooSystem]
         lappend args -system $equinox
         return
      }
      lappend args -system J2000
   }
}
