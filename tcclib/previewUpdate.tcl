#+
#  previewUpdate
#
#  Calls the tccPreview update command with data gathered from the tcs sad.
#  and then re-schedules itself after 10 minutes.
#
#  Arguments:
#             none
#
#  D Terrett 27 July 2000
#
#  Copyright CCLRC
#-

proc previewUpdate {} {

# Initialise the time library
   set interval 600000
   if { [catch {tccTime init}] } {
      set interval 1000
   } else {
      set data(delut) [sa tcssad get delut value]
      set data(tlongm) [sa tcssad get tlongm value]
      set data(tlatm) [sa tcssad get tlatm value]
      set data(hm) [sa tcssad get height value]

      set data(xpm) [sa tcssad get xpm value]
      set data(ypm) [sa tcssad get ypm value]
      set data(tlr) [sa tcssad get tlr value]

      set data(fl) [sa tcssad get focalLength value]
      set data(an) 0.0
      set data(aw) 0.0
      set data(pnpae) 0.0
      set data(ca) [sa tcssad get collErrAz value]
      set data(ce) [sa tcssad get collErrEl value]

      set data(tdc) [sa tcssad get currentExternalTemp value]
      set data(pmb) [sa tcssad get currentAirPressure value]
      set data(rh) [sa tcssad get currentRH value]

      if { [catch {tccPreview update data}] } {
         set interval 1000
      }
   }

   after $interval previewUpdate

   return
}
