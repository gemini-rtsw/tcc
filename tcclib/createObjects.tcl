#+
#  createObjects
#
#  This file contains a set of convenience routines for creating tcc objects
#  designed to be used in initialisation files or via "send" from other
#  tcl applications.
#
#  D Terrett 1 November 2000
#
#  Copyright CCLRC
#-

proc createConfig {name args} {
   catch {ConfigComponent ::TcsConfigNames::$name TcsConfigList}
   ::TcsConfigNames::$name init
   eval \"::TcsConfigNames::$name\" configure $args
}

proc createScienceTarget {name args} {
   catch {SciTargetComponent ::ScienceTargetNames::$name ScienceTargetList}
   ::ScienceTargetNames::$name init
   eval \"::ScienceTargetNames::$name\" configure $args
}

proc createWfsTarget {name args} {
   catch {WfsTargetComponent ::WfsTargetNames::$name WfsTargetList}
   ::WfsTargetNames::$name init
   eval \"::WfsTargetNames::$name\" configure $args
}

proc createPointingOrigin {name args} {
   catch {PointOrigComponent ::PointOrigNames::$name PointOrigList}
   ::PointOrigNames::$name init
   eval \"::PointOrigNames::$name\" configure $args
}

proc createChop {name args} {
   catch {ChopComponent ::ChopNames::$name ChopList}
   ::ChopNames::$name init
   eval \"::ChopNames::$name\" configure $args
}

proc createSlewOptions {name args} {
   catch {SlewOptionsComponent ::SlewOptionsNames::$name SlewOptionsList}
   ::SlewOptionsNames::$name init
   eval \"::SlewOptionsNames::$name\" configure $args
}

proc createTrackFrame {name args} {
   catch {TrackFrameComponent ::TrackFrameNames::$name TrackFrameList}
   ::TrackFrameNames::$name init
   eval \"::TrackFrameNames::$name\" configure $args
}

proc createWavelength {name args} {
   catch {WavelengthComponent ::WavelengthNames::$name WavelengthList}
   ::WavelengthNames::$name init
   eval \"::WavelengthNames::$name\" configure $args
}

proc createInstrument {name args} {
   catch {InstrumentComponent ::InstrumentNames::$name InstrumentList}
   ::InstrumentNames::$name init
   eval \"::InstrumentNames::$name\" configure $args
}

proc createTrackRate {name args} {
   catch {TrackRateComponent ::TrackRateNames::$name TrackRateList}
   ::TrackRateNames::$name init
   eval \"::TrackRateNames::$name\" configure $args
}

proc createRotator {name args} {
   catch {RotatorComponent ::RotatorNames::$name RotatorList}
   ::RotatorNames::$name init
   eval \"::RotatorNames::$name\" configure $args
}

proc createGuide {name args} {
   catch {GuideComponent ::GuideNames::$name GuideList}
   ::GuideNames::$name init
   eval \"::GuideNames::$name\" configure $args
}

proc createCosys {name args} {
   catch {CosysComponent ::CosysNames::$name CosysList}
   ::CosysNames::$name init
   eval \"::CosysNames::$name\" configure $args
}
