proc slewTel {} {
#+
#  Procedure:
#  slewTel
#
#  Purpose:
#  Slews the telescope to the next star
#
#  Description:
#  The next star position is loaded into the TCS from the entry widgets
#  and the slew started.
#
#  D Terrett 17 January 1999
#
#  Copyright CCLRC
#-
   global Target Options

   cs sourceA set objName value \"$Target(objname)\"
   cs sourceA set system value $Target(frame)
   cs sourceA set theta1 value \"$Target(ra)\"
   cs sourceA set theta2 value \"$Target(dec)\"
   cs sourceA set equinox value $Target(equinox)
   cs sourceA set epoch value $Target(epoch)
   cs sourceA set parallax value $Target(parallax)
   cs sourceA set dtheta1 value $Target(pmra)
   cs sourceA set dtheta2 value $Target(pmdec)
   cs sourceA set rv value $Target(rv)

   if { $Options(zlpm) } {
      cs slew set pointing value On
   } else {
      cs slew set pointing value Off
   }

   if [catch {cs sourceA postCallback endSlew} info] {
      if $Options(skip) {
         after 2000
         after idle selectNext
      } {
         tk_messageBox -icon error -message "TCS reports $info"
      }
   }

   return
}
