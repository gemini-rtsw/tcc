#+
#  formatEpoch
#
#  Formats an Epoch so that it always has a leading B or J and has no
#  trailing zeros after the decimal point and no trailing dot if it 
#  has no fractional part.
#
#  D Terrett 2 February 2001
#
#  Copyright CCLRC
#-
proc formatEpoch {frame epoch} {

# Extract the component parts of the epoch (prefix, year, fraction of year).
   if { [regexp {^ *(B|J)?([0-9]+)\.?([0-9]*) *$} $epoch match prefix year \
         frac] } {

# Apply defaults.
      if { [string is space $prefix] } {
         if { [string equal -nocase $frame FK5] } {
            set prefix J
         } elseif { [string equal -nocase $frame FK4] } {
            set prefix B
         } else {
            if { $year < 1984 } {
               set prefix B
            } else {
               set prefix J
            }
         }
      }
      if { [string is space $frac] } {
         set frac 0
      }

# Format result.
      set result ${prefix}${year}
      if { $frac != 0 } {
         set result ${result}.[string trimright $frac 0]
      }
      return $result
   } else {
      error "\"$epoch\" is not a valid epoch"
   }
}
