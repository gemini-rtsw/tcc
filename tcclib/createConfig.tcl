#+
#  createConfig
#
#  Creates the global array, namespaces and ComponentList objects that
#  implement the tcs configuration hierachy.
#
#  Arguments:
#             none
#
#  D Terrett 11 April 2002
#
#  Copyright CCLRC
#-

proc createConfig {} {

# Build the global array that underlies the current TCS configuration.
   set ::Config(tcs,value) default
   set ::Config(tcs,anon) ::[TcsConfig #auto]
   set ::Config(tcs,class) TcsConfigComponent
   set ::Config(tcs,namespace) ::TcsConfigNames
   set ::Config(tcs,list) TcsConfigList
   set ::Config(tcs,panel) tcsConfigPanel
   set ::Config(tcs,subcomp) [list field pointorig slewoptions trackframe \
         instrument ]
   set ::Config(tcs,label) "TCS configuration"

   set ::Config(tcs.field,value) ""
   set ::Config(tcs.field,anon) ::[Field #auto]
   set ::Config(tcs.field,class) FieldComponent
   set ::Config(tcs.field,namespace) ::FieldNames
   set ::Config(tcs.field,list) FieldList
   set ::Config(tcs.field,panel) tcsFieldPanel
   set ::Config(tcs.field,subcomp) [list sciencetarget pwfs1target pwfs2target \
         oiwfstarget guide rotator wavelength]
   set ::Config(tcs.field,label) Field

   set ::Config(tcs.field.sciencetarget,value) ""
   set ::Config(tcs.field.sciencetarget,anon) ::[Target #auto]
   set ::Config(tcs.field.sciencetarget,class) TargetComponent
   set ::Config(tcs.field.sciencetarget,namespace) \
         $::Config(tcs.field,anon)TargetNames
   set ::Config(tcs.field.sciencetarget,list) \
         $::Config(tcs.field,anon)TargetList
   set ::Config(tcs.field.sciencetarget,panel) tcsFieldPanel
   set ::Config(tcs.field.sciencetarget,subcomp) [list cosys trackrate]
   set ::Config(tcs.field.sciencetarget,label) "Science target"

   set ::Config(tcs.field.sciencetarget.cosys,value) ""
   set ::Config(tcs.field.sciencetarget.cosys,anon) ::[Cosys #auto]
   set ::Config(tcs.field.sciencetarget.cosys,class) CosysComponent
   set ::Config(tcs.field.sciencetarget.cosys,namespace) ::CosysNames
   set ::Config(tcs.field.sciencetarget.cosys,list) CosysList
   set ::Config(tcs.field.sciencetarget.cosys,panel) \
      tcsFieldScienceTargetCosysPanel
   set ::Config(tcs.field.sciencetarget.cosys,subcomp) ""
   set ::Config(tcs.field.sciencetarget.cosys,label) "Coordinate system"

   set ::Config(tcs.field.sciencetarget.trackrate,value) ""
   set ::Config(tcs.field.sciencetarget.trackrate,anon) [TrackRate #auto]
   set ::Config(tcs.field.sciencetarget.trackrate,class) TrackRateComponent
   set ::Config(tcs.field.sciencetarget.trackrate,namespace) ::TrackRateNames
   set ::Config(tcs.field.sciencetarget.trackrate,list) TrackRateList
   set ::Config(tcs.field.sciencetarget.trackrate,panel) \
      tcsFieldScienceTargetTrackRatePanel
   set ::Config(tcs.field.sciencetarget.trackrate,subcomp) cosys
   set ::Config(tcs.field.sciencetarget.trackrate,label) "Coordinate system"

   set ::Config(tcs.field.sciencetarget.trackrate.cosys,value) ""
   set ::Config(tcs.field.sciencetartet.trackrate.cosys,anon) ::[Cosys #auto]
   set ::Config(tcs.field.sciencetartet.trackrate.cosys,class) CosysComponent
   set ::Config(tcs.field.sciencetarget.trackrate.cosys,namespace) ::CosysNames
   set ::Config(tcs.field.sciencetarget.trackrate.cosys,list) CosysList
   set ::Config(tcs.field.sciencetarget.trackrate.cosys,panel) \
      tcsFieldScienceTargetTrackRateCosysPanel
   set ::Config(tcs.field.sciencetarget.trackrate.cosys,subcomp) ""
   set ::Config(tcs.field.sciencetarget.trackrate.cosys,label) \
         "Differential track rate"
   
   set ::Config(tcs.field.pwfs1target,value) ""
   set ::Config(tcs.field.pwfs1target,anon) ::[Target #auto]
   set ::Config(tcs.field.pwfs1target,class) TargetComponent
   set ::Config(tcs.field.pwfs1target,namespace) \
         $::Config(tcs.field,anon)TargetNames
   set ::Config(tcs.field.pwfs1target,list) \
         $::Config(tcs.field,anon)TargetList
   set ::Config(tcs.field.pwfs1target,panel) tcsFieldPanel
   set ::Config(tcs.field.pwfs1target,subcomp) [list cosys trackrate]
   set ::Config(tcs.field.pwfs1target,label) "PWFS1 target"

   set ::Config(tcs.field.pwfs1target.cosys,value) ""
   set ::Config(tcs.field.pwfs1target.cosys,anon) ::[Cosys #auto]
   set ::Config(tcs.field.pwfs1target.cosys,class) CosysComponent
   set ::Config(tcs.field.pwfs1target.cosys,namespace) ::CosysNames
   set ::Config(tcs.field.pwfs1target.cosys,list) CosysList
   set ::Config(tcs.field.pwfs1target.cosys,panel) \
      tcsFieldPwfs1TargetCosysPanel
   set ::Config(tcs.field.pwfs1target.cosys,subcomp) ""
   set ::Config(tcs.field.pwfs1target.cosys,label) "Coordinate system"

   set ::Config(tcs.field.pwfs1target.trackrate,value) ""
   set ::Config(tcs.field.pwfs1target.trackrate,anon) ::[TrackRate #auto]
   set ::Config(tcs.field.pwfs1target.trackrate,class) TrackRateComponent
   set ::Config(tcs.field.pwfs1target.trackrate,namespace) ::TrackRateNames
   set ::Config(tcs.field.pwfs1target.trackrate,list) TrackRateList
   set ::Config(tcs.field.pwfs1target.trackrate,panel) \
      tcsFieldPwfs1TargetTrackRatePanel
   set ::Config(tcs.field.pwfs1target.trackrate,subcomp) cosys
   set ::Config(tcs.field.pwfs1target.trackrate,label) \
         "Differential track rate"

   set ::Config(tcs.field.pwfs1target.trackrate.cosys,value) ""
   set ::Config(tcs.field.pwfs1target.trackrate.cosys,anon) ::[Cosys #auto]
   set ::Config(tcs.field.pwfs1target.trackrate.cosys,class) CosysComponent
   set ::Config(tcs.field.pwfs1target.trackrate.cosys,namespace) ::CosysNames
   set ::Config(tcs.field.pwfs1target.trackrate.cosys,list) CosysList
   set ::Config(tcs.field.pwfs1target.trackrate.cosys,panel) \
      tcsFieldPwfs1TargetTrackRateCosysPanel
   set ::Config(tcs.field.pwfs1target.trackrate.cosys,subcomp) ""
   set ::Config(tcs.field.pwfs1target.trackrate.cosys,label) \
         "Coordiante system"
   
   set ::Config(tcs.field.pwfs2target,value) ""
   set ::Config(tcs.field.pwfs2target,anon) ::[Target #auto]
   set ::Config(tcs.field.pwfs2target,class) TargetComponent
   set ::Config(tcs.field.pwfs2target,namespace) \
         $::Config(tcs.field,anon)TargetNames
   set ::Config(tcs.field.pwfs2target,list) \
         $::Config(tcs.field,anon)TargetList
   set ::Config(tcs.field.pwfs2target,panel) tcsFieldPanel
   set ::Config(tcs.field.pwfs2target,subcomp) [list cosys trackrate]
   set ::Config(tcs.field.pwfs2target,label) "PWFS2 target"

   set ::Config(tcs.field.pwfs2target.cosys,value) ""
   set ::Config(tcs.field.pwfs2target.cosys,anon) ::[Cosys #auto]
   set ::Config(tcs.field.pwfs2target.cosys,class) CosysComponent
   set ::Config(tcs.field.pwfs2target.cosys,namespace) ::CosysNames
   set ::Config(tcs.field.pwfs2target.cosys,list) CosysList
   set ::Config(tcs.field.pwfs2target.cosys,panel) \
         tcsFieldPwfs2TargetCosysPanel
   set ::Config(tcs.field.pwfs2target.cosys,subcomp) ""
   set ::Config(tcs.field.pwfs2target.cosys,label) "Coordinate system"

   set ::Config(tcs.field.pwfs2target.trackrate,value) ""
   set ::Config(tcs.field.pwfs2target.trackrate,anon) ::[TrackRate #auto]
   set ::Config(tcs.field.pwfs2target.trackrate,class) TrackRateComponent
   set ::Config(tcs.field.pwfs2target.trackrate,namespace) ::TrackRateNames
   set ::Config(tcs.field.pwfs2target.trackrate,list) TrackRateList
   set ::Config(tcs.field.pwfs2target.trackrate,panel) \
         tcsFieldPwfs2TargetTrackRatePanel
   set ::Config(tcs.field.pwfs2target.trackrate,subcomp) cosys
   set ::Config(tcs.field.pwfs2target.trackrate,label) \
         "Differential track rate"

   set ::Config(tcs.field.pwfs2target.trackrate.cosys,value) ""
   set ::Config(tcs.field.pwfs2target.trackrate.cosys,anon) ::[Cosys #auto]
   set ::Config(tcs.field.pwfs2target.trackrate.cosys,class) CosysComponent
   set ::Config(tcs.field.pwfs2target.trackrate.cosys,namespace) ::CosysNames
   set ::Config(tcs.field.pwfs2target.trackrate.cosys,list) CosysList
   set ::Config(tcs.field.pwfs2target.trackrate.cosys,panel) \
      tcsFieldPwfs2TargetTrackRateCosysPanel
   set ::Config(tcs.field.pwfs2target.trackrate.cosys,subcomp) ""
   set ::Config(tcs.field.pwfs2target.trackrate.cosys,label) \
         "Coordinate system"

   set ::Config(tcs.field.oiwfstarget,value) ""
   set ::Config(tcs.field.oiwfstarget,anon) ::[Target #auto]
   set ::Config(tcs.field.oiwfstarget,class) TargetComponent
   set ::Config(tcs.field.oiwfstarget,namespace) \
         $::Config(tcs.field,anon)TargetNames
   set ::Config(tcs.field.oiwfstarget,list) \
         $::Config(tcs.field,anon)TargetList
   set ::Config(tcs.field.oiwfstarget,panel) tcsFieldPanel
   set ::Config(tcs.field.oiwfstarget,subcomp) [list cosys trackrate]
   set ::Config(tcs.field.oiwfstarget,label) "OIWFS target"

   set ::Config(tcs.field.oiwfstarget.cosys,value) ""
   set ::Config(tcs.field.oiwfstarget.cosys,anon) ::[Cosys #auto]
   set ::Config(tcs.field.oiwfstarget.cosys,class) CosysComponent
   set ::Config(tcs.field.oiwfstarget.cosys,namespace) ::CosysNames
   set ::Config(tcs.field.oiwfstarget.cosys,list) CosysList
   set ::Config(tcs.field.oiwfstarget.cosys,panel) \
      tcsFieldOiwfsTargetCosysPanel
   set ::Config(tcs.field.oiwfstarget.cosys,subcomp) ""
   set ::Config(tcs.field.oiwfstarget.cosys,label) "Coordinate system"

   set ::Config(tcs.field.oiwfstarget.trackrate,value) ""
   set ::Config(tcs.field.oiwfstarget.trackrate,anon) ::[TrackRate #auto]
   set ::Config(tcs.field.oiwfstarget.trackrate,class) TrackRateComponent
   set ::Config(tcs.field.oiwfstarget.trackrate,namespace) ::TrackRateNames
   set ::Config(tcs.field.oiwfstarget.trackrate,list) TrackRateList
   set ::Config(tcs.field.oiwfstarget.trackrate,panel) \
         tcsFieldOiwfsTargetTrackRatePanel
   set ::Config(tcs.field.oiwfstarget.trackrate,subcomp) cosys
   set ::Config(tcs.field.oiwfstarget.trackrate,label) \
         "Differential track rate"

   set ::Config(tcs.field.oiwfstarget.trackrate.cosys,value) ""
   set ::Config(tcs.field.oiwfstarget.trackrate.cosys,anon) ::[Cosys #auto]
   set ::Config(tcs.field.oiwfstarget.trackrate.cosys,class) CosysComponent
   set ::Config(tcs.field.oiwfstarget.trackrate.cosys,namespace) ::CosysNames
   set ::Config(tcs.field.oiwfstarget.trackrate.cosys,list) CosysList
   set ::Config(tcs.field.oiwfstarget.trackrate.cosys,panel) \
         tcsFieldOiwfsTargetTrackRateCosysPanel
   set ::Config(tcs.field.oiwfstarget.trackrate.cosys,subcomp) ""
   set ::Config(tcs.field.oiwfstarget.trackrate.cosys,label) \
         "Coordinate system"

   set ::Config(tcs.field.guide,value) ""
   set ::Config(tcs.field.guide,anon) ::[Guide #auto]
   set ::Config(tcs.field.guide,class) GuideComponent
   set ::Config(tcs.field.guide,namespace) ::GuideNames
   set ::Config(tcs.field.guide,list) GuideList
   set ::Config(tcs.field.guide,panel) tcsGuidePanel
   set ::Config(tcs.field.guide,subcomp) "oiwfswavelength"
   set ::Config(tcs.field.guide,label) "Guide configuration"

   set ::Config(tcs.field.guide.oiwfswavelength,value) ""
   set ::Config(tcs.field.guide.oiwfswavelength,anon) ::[Wavelength #auto]
   set ::Config(tcs.field.guide.oiwfswavelength,class) WavelengthComponent
   set ::Config(tcs.field.guide.oiwfswavelength,namespace) \
         ::OIWFSWavelengthNames
   set ::Config(tcs.field.guide.oiwfswavelength,list) OIWFSWavelengthList
   set ::Config(tcs.field.guide.oiwfswavelength,panel) \
         tcsGuideOiwfsWavelengthPanel
   set ::Config(tcs.field.guide.oiwfswavelength,subcomp) ""
   set ::Config(tcs.field.guide.oiwfswavelength,label) "OIWFS Filter"

   set ::Config(tcs.field.rotator,value) ""
   set ::Config(tcs.field.rotator,anon) ::[Rotator #auto]
   set ::Config(tcs.field.rotator,class) RotatorComponent
   set ::Config(tcs.field.rotator,namespace) ::RotatorNames
   set ::Config(tcs.field.rotator,list) RotatorList
   set ::Config(tcs.field.rotator,panel) tcsRotatorPanel
   set ::Config(tcs.field.rotator,subcomp) cosys
   set ::Config(tcs.field.rotator,label) "Rotator configuration"

   set ::Config(tcs.field.rotator.cosys,value) ""
   set ::Config(tcs.field.rotator.cosys,anon) ::[Cosys #auto]
   set ::Config(tcs.field.rotator.cosys,class) CosysComponent
   set ::Config(tcs.field.rotator.cosys,namespace) ::CosysNames
   set ::Config(tcs.field.rotator.cosys,list) CosysList
   set ::Config(tcs.field.rotator.cosys,panel) tcsRotatorCosysPanel
   set ::Config(tcs.field.rotator.cosys,subcomp) ""
   set ::Config(tcs.field.rotator.cosys,label) "Coordinate system"

   set ::Config(tcs.field.wavelength,value) ""
   set ::Config(tcs.field.wavelength,anon) ::[Wavelength #auto]
   set ::Config(tcs.field.wavelength,class) WavelengthComponent
   set ::Config(tcs.field.wavelength,namespace) ::WavelengthNames
   set ::Config(tcs.field.wavelength,list) WavelengthList
   set ::Config(tcs.field.wavelength,panel) \
         tcsFieldWavelengthPanel
   set ::Config(tcs.field.wavelength,subcomp) ""
   set ::Config(tcs.field.wavelength,label) "Observing wavelength"


   set ::Config(tcs.pointorig,value) ""
   set ::Config(tcs.pointorig,anon) ::[PointOrig #auto]
   set ::Config(tcs.pointorig,class) PointOrigComponent
   set ::Config(tcs.pointorig,namespace) ::PointOrigNames
   set ::Config(tcs.pointorig,list) PointOrigList
   set ::Config(tcs.pointorig,panel) tcsPointOrigPanel
   set ::Config(tcs.pointorig,subcomp) ""
   set ::Config(tcs.pointorig,label) "Instrument origin"

   set ::Config(tcs.chop,value) ""
   set ::Config(tcs.chop,anon) ::[Chop #auto]
   set ::Config(tcs.chop,class) ChopComponent
   set ::Config(tcs.chop,namespace) ::ChopNames
   set ::Config(tcs.chop,list) ChopList
   set ::Config(tcs.chop,panel) tcsChopPanel
   set ::Config(tcs.chop,subcomp) cosys
   set ::Config(tcs.chop,label) "Chop configuration"

   set ::Config(tcs.chop.cosys,value) ""
   set ::Config(tcs.chop.cosys,anon) ::[Cosys #auto]
   set ::Config(tcs.chop.cosys,class) CosysComponent
   set ::Config(tcs.chop.cosys,namespace) ::CosysNames
   set ::Config(tcs.chop.cosys,list) CosysList
   set ::Config(tcs.chop.cosys,panel) tcsChopCosysPanel
   set ::Config(tcs.chop.cosys,subcomp) ""
   set ::Config(tcs.chop.cosys,label) "Coordinate system"

   set ::Config(tcs.slewoptions,value) ""
   set ::Config(tcs.slewoptions,anon) ::[SlewOptions #auto]
   set ::Config(tcs.slewoptions,class) SlewOptionsComponent
   set ::Config(tcs.slewoptions,namespace) ::SlewOptionsNames
   set ::Config(tcs.slewoptions,list) SlewOptionsList
   set ::Config(tcs.slewoptions,panel) tcsSlewOptionsPanel
   set ::Config(tcs.slewoptions,subcomp) ""
   set ::Config(tcs.slewoptions,label) "Slew options"

   set ::Config(tcs.trackframe,value) ""
   set ::Config(tcs.trackframe,anon) ::[TrackFrame #auto]
   set ::Config(tcs.trackframe,class) TrackFrameComponent
   set ::Config(tcs.trackframe,namespace) ::TrackFrameNames
   set ::Config(tcs.trackframe,list) TrackFrameList
   set ::Config(tcs.trackframe,panel) tcsTrackFramePanel
   set ::Config(tcs.trackframe,subcomp) [list mountcosys sourceAcosys \
         sourceBcosys sourceCcosys pwfs1cosys pwfs2cosys oiwfscosys]
   set ::Config(tcs.trackframe,label) "Tracking frames"

   set ::Config(tcs.trackframe.mountcosys,value) ""
   set ::Config(tcs.trackframe.mountcosys,anon) ::[Cosys #auto]
   set ::Config(tcs.trackframe.mountcosys,class) CosysComponent
   set ::Config(tcs.trackframe.mountcosys,namespace) ::CosysNames
   set ::Config(tcs.trackframe.mountcosys,list) CosysList
   set ::Config(tcs.trackframe.mountcosys,panel) tcsTrackFrameMountCosysPanel
   set ::Config(tcs.trackframe.mountcosys,subcomp) ""
   set ::Config(tcs.trackframe.mountcosys,label) "Coordinate system"

   set ::Config(tcs.trackframe.sourceAcosys,value) ""
   set ::Config(tcs.trackframe.sourceAcosys,anon) ::[Cosys #auto]
   set ::Config(tcs.trackframe.sourceAcosys,class) CosysComponent
   set ::Config(tcs.trackframe.sourceAcosys,namespace) ::CosysNames
   set ::Config(tcs.trackframe.sourceAcosys,list) CosysList
   set ::Config(tcs.trackframe.sourceAcosys,panel) \
         tcsTrackFrameSourceACosysPanel
   set ::Config(tcs.trackframe.sourceAcosys,subcomp) ""
   set ::Config(tcs.trackframe.sourceAcosys,label) "Coordinate system"

   set ::Config(tcs.trackframe.sourceBcosys,value) ""
   set ::Config(tcs.trackframe.sourceBcosys,anon) ::[Cosys #auto]
   set ::Config(tcs.trackframe.sourceBcosys,class) CosysComponent
   set ::Config(tcs.trackframe.sourceBcosys,namespace) ::CosysNames
   set ::Config(tcs.trackframe.sourceBcosys,list) CosysList
   set ::Config(tcs.trackframe.sourceBcosys,panel) \
         tcsTrackFrameSourceBCosysPanel
   set ::Config(tcs.trackframe.sourceBcosys,subcomp) ""
   set ::Config(tcs.trackframe.sourceBcosys,label) "Coordinate system"

   set ::Config(tcs.trackframe.sourceCcosys,value) ""
   set ::Config(tcs.trackframe.sourceCcosys,anon) ::[Cosys #auto]
   set ::Config(tcs.trackframe.sourceCcosys,class) CosysComponent
   set ::Config(tcs.trackframe.sourceCcosys,namespace) ::CosysNames
   set ::Config(tcs.trackframe.sourceCcosys,list) CosysList
   set ::Config(tcs.trackframe.sourceCcosys,panel) \
         tcsTrackFrameSourceCCosysPanel
   set ::Config(tcs.trackframe.sourceCcosys,subcomp) ""
   set ::Config(tcs.trackframe.sourceCcosys,label) "Coordinate system"

   set ::Config(tcs.trackframe.pwfs1cosys,value) ""
   set ::Config(tcs.trackframe.pwfs1cosys,anon) ::[Cosys #auto]
   set ::Config(tcs.trackframe.pwfs1cosys,class) CosysComponent
   set ::Config(tcs.trackframe.pwfs1cosys,namespace) ::CosysNames
   set ::Config(tcs.trackframe.pwfs1cosys,list) CosysList
   set ::Config(tcs.trackframe.pwfs1cosys,panel) tcsTrackFramePwfs1CosysPanel
   set ::Config(tcs.trackframe.pwfs1cosys,subcomp) ""
   set ::Config(tcs.trackframe.pwfs1cosys,label) "Coordinate system"

   set ::Config(tcs.trackframe.pwfs2cosys,value) ""
   set ::Config(tcs.trackframe.pwfs2cosys,anon) ::[Cosys #auto]
   set ::Config(tcs.trackframe.pwfs2cosys,class) CosysComponent
   set ::Config(tcs.trackframe.pwfs2cosys,namespace) ::CosysNames
   set ::Config(tcs.trackframe.pwfs2cosys,list) CosysList
   set ::Config(tcs.trackframe.pwfs2cosys,panel) tcsTrackFramePwfs2CosysPanel
   set ::Config(tcs.trackframe.pwfs2cosys,subcomp) ""
   set ::Config(tcs.trackframe.pwfs2cosys,label) "Coordinate system"

   set ::Config(tcs.trackframe.oiwfscosys,value) ""
   set ::Config(tcs.trackframe.oiwfscosys,anon) ::[Cosys #auto]
   set ::Config(tcs.trackframe.oiwfscosys,class) CosysComponent
   set ::Config(tcs.trackframe.oiwfscosys,namespace) ::CosysNames
   set ::Config(tcs.trackframe.oiwfscosys,list) CosysList
   set ::Config(tcs.trackframe.oiwfscosys,panel) tcsTrackFrameOiwfsCosysPanel
   set ::Config(tcs.trackframe.oiwfscosys,subcomp) ""
   set ::Config(tcs.trackframe.oiwfscosys,label) "Coordinate system"

   set ::Config(tcs.instrument,value) ""
   set ::Config(tcs.instrument,anon) ::[Instrument #auto]
   set ::Config(tcs.instrument,class) InstrumentComponent
   set ::Config(tcs.instrument,namespace) ::InstrumentNames
   set ::Config(tcs.instrument,list) InstrumentList
   set ::Config(tcs.instrument,panel) tcsInstrumentPanel
   set ::Config(tcs.instrument,subcomp) ""
   set ::Config(tcs.instrument,label) "Instrument"


# Create the namespaces for storing the components.
   foreach name [array names ::Config *,namespace] {
      namespace eval $::Config($name) {}
   }

# Create the ComponentList objects
   foreach name [array names ::Config *,list] {
      if {  $::Config($name) != "" } {
         if { [find objects $::Config($name)] == "" } {
            tcclib::ComponentList $::Config($name)
         }
      }
   }

# Set up a trace on Config(tcs.field,name) that changes the list and namespace
# used for targets whenever the current field changes.
   trace variable ::Config w fieldTrace
}
