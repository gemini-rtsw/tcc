#+
#  validate.tcl
#
#  This file contains a package of routines that check that a string is
#  a valid representation of some quantity.
#
#  D Terrett 2 September 1998
#
#  Copyright CCLRC
#-

proc valRA ra {
   set h 0.0
   set m 0.0
   set s 0.0
   set n [scan $ra "%d %c %s" h sep rest]
   if { $n == 0 } {
      error "\"$ra\" is not a valid right ascension."
   } elseif { $n == 2 } {
      set sep [format %c $sep]
      if { $sep != ":" && $sep != "," && $sep != " " } {
         error "Invalid separator \"$sep\""
      }
   }
   if { $n > 2 } {
      set n [scan $rest "%d %c %s" m sep rest]
      if { $n == 0 } {
         error "Extra text \"$rest\" after valid right ascension."
      } elseif { $n == 2 } {
         set sep [format %c $sep]
         if { $sep != ":" && $sep != "," && $sep != " " } {
            error "Invalid separator \"$sep\""
         }
      }
      if { $n > 2 } {
         set n [scan $rest "%f %s" s rest]
         if { $n != 1 } {
            error "Extra text \"$rest\" after valid right ascension."
         } 
      }
      if { $s > 60.0 || $s < 0.0 } {
         error "Seconds field out of range"
      }
      if { $m > 60.0 || $s < 0.0 } {
         error "Minutes field out of range"
      }
      if { $h > 24 || $s < 0.0 } {
         error "Hours field out of range"
      }
      if { $h == 24 && ( $m != 0.0 || $s != 0.0 ) } {
         error "Right acension greater than 24 hours"
      }
   }
   return $ra
}

proc valDec dec {
   set d 0.0
   set m 0.0
   set s 0.0
   set n [scan $dec "%d %c %s" d sep rest]
   if { $n == 0 } {
      error "\"$dec\" is not a valid declination."
   } elseif { $n == 2 } {
      set sep [format %c $sep]
      if { $sep != ":" && $sep != "," && $sep != " " } {
         error "Invalid separator \"$sep\""
      }
   }
   if { $n > 2 } {
      set n [scan $rest "%d %c %s" m sep rest]
      if { $n == 0 } {
         error "Extra text \"$rest\" after valid declination."
      } elseif { $n == 2 } {
         set sep [format %c $sep]
         if { $sep != ":" && $sep != "," && $sep != " " } {
            error "Invalid separator \"$sep\""
         }
      }
      if { $n > 2 } {
         set n [scan $rest "%f %s" s rest]
         if { $n != 1 } {
            error "Extra text \"$rest\" after valid declination."
         } 
      }
      if { $s > 60.0 || $s < 0.0 } {
         error "Seconds field out of range"
      }
      if { $m > 60.0 || $s < 0.0 } {
         error "Minutes field out of range"
      }
      if { $d > 90.0 || $s < 0.0 } {
         error "Degrees field out of range"
      }
      if { $d == 90 && ( $m != 0.0 || $s != 0.0 ) } {
         error "Declination greater than 90 degrees"
      }
   }
   return $dec
}

proc valWavelength wavel {

# Allow an empty string.
   if { $wavel != "" } {

# Decode the value.
      set n [scan $wavel "%f %s" v s]
      if { $n == 0 } {
          error "\"$wavel\" is not a valid wavelength."
      } elseif { $n > 1 } {
         error "extra text \"$s\" after wavelength."
      } else {
   
# Check the value against the allowed range.
         if { $v < 0.2 } {
            error "wavelengths must be longer than 0.2 microns."
         }
      }
   }
   return $wavel
}
