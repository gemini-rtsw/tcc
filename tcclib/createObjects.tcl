#+
#  createObjects
#
#  This file contains a set of convenience routines for creating tcc objects
#  designed to be used in initialisation files or via "send" from other
#  tcl applications. If an object of the same name and type already exists 
#  it is deleted.
#
#  D Terrett 22 May 2000
#
#  Copyright CCLRC
#-

proc createConfig {name args} {
   catch {delete object ::TcsConfigNames::$name}
   eval ConfigComponent \"::TcsConfigNames::$name\" TcsConfigList $args
}

proc createScienceTarget {name args} {
   catch {delete object ::ScienceTargetNames::$name}
   eval SciTargetComponent \"::ScienceTargetNames::$name\" ScienceTargetList $args
}

proc createWfsTarget {name args} {
   catch {delete object ::WfsTargetNames::$name}
   eval WfsTargetComponent \"::WfsTargetNames::$name\" WfsTargetList $args
}

proc createPointingOrigin {name args} {
   catch {delete object ::PointOrigNames::$name}
   eval PointOrigComponent \"::PointOrigNames::$name\" PointOrigList $args
}

proc createChop {name args} {
   catch {delete object ::ChopNames::$name}
   eval ChopComponent \"::ChopNames::$name\" ChopList $args
}

proc createSlewOptions {name args} {
   catch {delete object ::SlewOptionsNames::$name}
   eval SlewOptionsComponent \"::SlewOptionsNames::$name\" SlewOptionsList $args
}

proc createTrackFrame {name args} {
   catch {delete object ::TrackFrameNames::$name}
   eval TrackFrameComponent \"::TrackFrameNames::$name\" TrackFrameList $args
}

proc createWavelength {name args} {
   catch {delete object ::WavelengthNames::$name}
   eval WavelengthComponent \"::WavelengthNames::$name\" WavelengthList $args
}

proc createInstrument {name args} {
   catch {delete object ::InstrumentNames::$name}
   eval InstrumentComponent \"::InstrumentNames::$name\" InstrumentList $args
}

proc createTrackRate {name args} {
   catch {delete object ::TrackRateNames::$name}
   eval TrackRateComponent \"::TrackRateNames::$name\" TrackRateList $args
}

proc createRotator {name args} {
   catch {delete object ::RotatorNames::$name}
   eval RotatorComponent \"::RotatorNames::$name\" RotatorList $args
}

proc createGuide {name args} {
   catch {delete object ::GuideNames::$name}
   eval GuideComponent \"::GuideNames::$name\" GuideList $args
}

proc createCosys {name args} {
   catch {delete object ::CosysNames::$name}
   eval CosysComponent \"::CosysNames::$name\" CosysList $args
}

