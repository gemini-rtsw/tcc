#+
#  createObjects
#
#  This file contains a set of convenience routines for creating tcc objects
#  designed to be used in initialisation files or via "send" from other
#  tcl applications. If an object of the same name and type already exists 
#  it is deleted.
#
#  D Terrett 21 February 2000
#
#  Copyright CCLRC
#-

proc createConfig {name args} {
   if { [llength [itcl::find  objects ::TcsConfigNames::$name \
         -class ConfigComponent]] != 0} {
      delete object ::TcsConfigNames::$name
   }
   eval ConfigComponent \"::TcsConfigNames::$name\" TcsConfigList $args
}

proc createScienceTarget {name args} {
   if { [llength [itcl::find  objects ::ScienceTargetNames::$name \
         -class SciTargetComponent]] != 0} {
      delete object ::ScienceTargetNames::$name
   }
   eval SciTargetComponent \"::ScienceTargetNames::$name\" ScienceTargetList $args
}

proc createWfsTarget {name args} {
   if { [llength [itcl::find  objects ::WfsTargetNames::$name \
         -class TargetComponent]] != 0} {
      delete object ::WfsTargetNames::$name
   }
   eval TargetComponent \"::WfsTargetNames::$name\" WfsTargetList $args
}

proc createPointingOrigin {name args} {
   if { [llength [itcl::find  objects ::PointOrigNames::$name \
         -class PointOrigComponent]] != 0} {
      delete object ::PointOrigNames::$name
   }
   eval PointOrigComponent \"::PointOrigNames::$name\" PointOrigList $args
}

proc createSlewOptions {name args} {
   if { [llength [itcl::find  objects ::SlewOptionsNames::$name \
         -class SlewOptionsComponent]] != 0} {
      delete object ::SlewOptionsNames::$name
   }
   eval SlewOptionsComponent \"::SlewOptionsNames::$name\" SlewOptionsList $args
}

proc createTrackFrame {name args} {
   if { [llength [itcl::find  objects ::TrackFrameNames::$name \
         -class TrackFrameComponent]] != 0} {
      delete object ::TrackFrameNames::$name
   }
   eval TrackFrameComponent \"::TrackFrameNames::$name\" TrackFrameList $args
}

proc createWavelength {name args} {
   if { [llength [itcl::find  objects ::WavelengthNames::$name \
         -class WavelengthComponent]] != 0} {
      delete object ::WavelengthNames::$name
   }
   eval WavelengthComponent \"::WavelengthNames::$name\" WavelengthList $args
}

proc createInstrument {name args} {
   if { [llength [itcl::find  objects ::InstrumentNames::$name \
         -class InstrumentComponent]] != 0} {
      delete object ::InstrumentNames::$name
   }
   eval InstrumentComponent \"::InstrumentNames::$name\" InstrumentList $args
}

proc createTrackRate {name args} {
   if { [llength [itcl::find  objects ::TrackRateNames::$name \
         -class TrackRateComponent]] != 0} {
      delete object ::TrackRateNames::$name
   }
   eval TrackRateComponent \"::TrackRateNames::$name\" TrackRateList $args
}

proc createRotator {name args} {
   if { [llength [itcl::find  objects ::RotatorNames::$name \
            -class RotatorComponent]] != 0} {
      delete object ::RotatorNames::$name
   }
   eval RotatorComponent \"::RotatorNames::$name\" RotatorList $args
}

proc createGuide {name args} {
   if { [llength [itcl::find  objects ::GuideNames::$name \
         -class GuideComponent]] != 0} {
      delete object ::GuideNames::$name
   }
   eval GuideComponent \"::GuideNames::$name\" GuideList $args
}


proc createCosys {name args} {
   if { [llength [itcl::find  objects ::CosysNames::$name \
         -class CosysComponent]] != 0} {
      delete object ::CosysNames::$name
   }
   eval CosysComponent \"::CosysNames::$name\" CosysList $args
}

