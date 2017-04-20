#+
#  createPanels
#
#  Creates all the panel objects that make up the tcc application.
#
#  Arguments:
#             none
#
#  D Terrett 17 January 2004
#
#  Copyright CCLRC
#-

proc createPanels {} {

# Create all the simple command panels.
   PanelMgr observePanel -name .observePanel -class ObservePanel \
         -title "Acquisition & Guiding"
   PanelMgr skycatPanel -name .skycatPanel -class SkycatPanel \
         -title "SkyCat"
   PanelMgr tcsPanel -name .tcsPanel -class TcsPanel \
         -title "System Control"
   PanelMgr mcsPanel -name .mcsPanel -class McsPanel \
         -title "Mount And Rotator Control"
   PanelMgr m1sPanel -name .m1sPanel -class M1sPanel \
         -title "Primary Mirror Control"
   PanelMgr aosPanel -name .aosPanel -class AosPanel \
         -title "Active Optics Control"
   PanelMgr previewPanel -name .previewPanel -class PreviewPanel \
         -title "Configuration Preview"
   PanelMgr logPanel -name .logPanel -class LogPanel \
         -title "Command Log"
   PanelMgr aoPanel -name .aoPanel \
         -class AosPanel -title "Adaptive Optics Control"
   PanelMgr handsetPanel -name .handsetPanel \
         -class HandsetPanel -title "Handsets"
   PanelMgr pointUpdatePanel -name .pointUpdatePanel \
         -class PointUpdatePanel -title "Pointing model update"
   PanelMgr skyPanel -name .skyPanel \
         -class SkyPanel -title Catalogs

# Create all the edit panel managers.
   EditPanelMgr tcsConfigPanel -name .tcsConfigPanel \
         -class TcsConfigPanel -title "Configuration Manager" \
         -components tcs
   EditPanelMgr tcsFieldPanel -name .tcsFieldPanel \
         -class FieldPanel -title Field -components tcs.field

   EditPanelMgr tcsFieldCosysPanel \
         -name .tcsFieldCosysPanel -class CosysPanel \
         -title "Coordinate System" \
         -components tcs.field.cosys
   EditPanelMgr tcsFieldTrackRatePanel \
         -name .tcsFieldTrackRatePanel -class TrackRatePanel \
         -title "Differential Track Rates" \
         -components tcs.field.trackrate
   EditPanelMgr tcsFieldTrackRateCosysPanel \
         -name .tcsFieldTrackRateCosysPanel -class CosysPanel \
         -title "Coordinate System" \
         -components tcs.field.trackrate.cosys

	EditPanelMgr tcsFieldScienceTargetCosysPanel \
         -name .tcsFieldScienceTargetCosysPanel -class CosysPanel \
         -title "Science Target Coordinate System" \
         -components tcs.field.sciencetarget.cosys
   EditPanelMgr tcsFieldScienceTargetTrackRatePanel \
         -name .tcsFieldScienceTargetTrackRatePanel -class TrackRatePanel \
         -title "Science Target Differential Track Rates" \
         -components tcs.field.sciencetarget.trackrate
   EditPanelMgr tcsFieldScienceTargetTrackRateCosysPanel \
         -name .tcsFieldScienceTargetTrackRateCosysPanel -class CosysPanel \
         -title "Science Target Differential Track Rate Coordinate System" \
         -components tcs.field.sciencetarget.trackrate.cosys
   EditPanelMgr tcsFieldPwfs1TargetCosysPanel \
         -name .tcsFieldPwfs1TargetCosysPanel -class CosysPanel \
         -title "Pwfs1 Target Coordinate System" \
         -components tcs.field.pwfs1target.cosys
   EditPanelMgr tcsFieldPwfs1TargetTrackRatePanel \
         -name .tcsFieldPwfs1TargetTrackRatePanel -class TrackRatePanel \
         -title "Pwfs1 Target Differential Track Rates" \
         -components tcs.field.pwfs1target.trackrate
   EditPanelMgr tcsFieldPwfs1TargetTrackRateCosysPanel \
         -name .tcsFieldPwfs1TargetTrackRateCosysPanel -class CosysPanel \
         -title "Pwfs1 Target Differential Track Rate Coordinate System" \
         -components tcs.field.pwfs1target.trackrate.cosys
   EditPanelMgr tcsFieldPwfs2TargetCosysPanel \
         -name .tcsFieldPwfs2TargetCosysPanel -class CosysPanel \
         -title "Pwfs2 Target Coordinate System" \
         -components tcs.field.pwfs2target.cosys
   EditPanelMgr tcsFieldPwfs2TargetTrackRatePanel \
         -name .tcsFieldPwfs2TargetTrackRatePanel -class TrackRatePanel \
         -title "Pwfs2 Target Differential Track Rates" \
         -components tcs.field.pwfs2target.trackrate
   EditPanelMgr tcsFieldPwfs2TargetTrackRateCosysPanel \
         -name .tcsFieldPwfs2TargetTrackRateCosysPanel -class CosysPanel \
         -title "Pwfs2 Target Differential Track Rate Coordinate System" \
         -components tcs.field.pwfs2target.trackrate.cosys
   EditPanelMgr tcsFieldOiwfsTargetCosysPanel \
         -name .tcsFieldOiwfsTargetCosysPanel -class CosysPanel \
         -title "Oiwfs Target Coordinate System" \
         -components tcs.field.oiwfstarget.cosys
   EditPanelMgr tcsFieldOiwfsTargetTrackRatePanel \
         -name .tcsFieldOiwfsTargetTrackRatePanel -class TrackRatePanel \
         -title "Oiwfs Target Differential Track Rates" \
         -components tcs.field.oiwfstarget.trackrate
   EditPanelMgr tcsFieldOiwfsTargetTrackRateCosysPanel \
         -name .tcsFieldOiwfsTargetTrackRateCosysPanel -class CosysPanel \
         -title "Oiwfs Target Differential Track Rate Coordinate System" \
         -components tcs.field.oiwfstarget.trackrate.cosys
   EditPanelMgr tcsFieldAowfsTargetCosysPanel \
         -name .tcsFieldAowfsTargetCosysPanel -class CosysPanel \
         -title "AOWFS Target Coordinate System" \
         -components tcs.field.pwfs2target.cosys
   EditPanelMgr tcsFieldAowfsTargetTrackRatePanel \
         -name .tcsFieldAowfsTargetTrackRatePanel -class TrackRatePanel \
         -title "AOWFS Target Differential Track Rates" \
         -components tcs.field.aowfstarget.trackrate
   EditPanelMgr tcsFieldAowfsTargetTrackRateCosysPanel \
         -name .tcsFieldAowfsTargetTrackRateCosysPanel -class CosysPanel \
         -title "AOWFS Target Differential Track Rate Coordinate System" \
         -components tcs.field.pwfs2target.trackrate.cosys
   EditPanelMgr tcsGuidePanel -name .tcsGuidePanel \
         -class GuidePanel -title "Guide Configuration" \
         -components tcs.field.guide 
   EditPanelMgr tcsGuideOiwfsWavelengthPanel \
         -name .tcsGuideOiwfsWavelengthPanel -class WavelengthPanel \
         -title "Oiwfs Guiding Wavelength" \
         -components tcs.field.guide.oiwfswavelength
   EditPanelMgr tcsGuideAowfsWavelengthPanel \
         -name .tcsGuideAowfsWavelengthPanel -class WavelengthPanel \
         -title "Aowfs Guiding Wavelength" \
         -components tcs.field.guide.aowfswavelength
   EditPanelMgr tcsRotatorPanel -name .tcsRotatorPanel \
         -class RotatorPanel -title "Rotator Configuration" \
         -components tcs.field.rotator 
   EditPanelMgr tcsRotatorCosysPanel -name .tcsRotatorCosysPanel \
         -class CosysPanel -title "Rotator Coordinate System" \
         -components tcs.field.rotator.cosys
   EditPanelMgr tcsPointOrigPanel -name .tcsPointOrigPanel \
         -class PointOrigPanel -title "Instrument Origin" \
         -components tcs.pointorig 
   EditPanelMgr tcsChopPanel -name .tcsChopPanel \
         -class ChopPanel -title "Chop Configuration" \
         -components tcs.chop 
   EditPanelMgr tcsChopCosysPanel -name .tcsChopCosysPanel \
         -class CosysPanel -title "Chop Coordinate System" \
         -components tcs.chop.cosys
   EditPanelMgr tcsGaosPanel -name .tcsGaosPanel \
         -class GaosPanel -title "GAOS Configuration" \
         -components tcs.gaos 
   EditPanelMgr tcsSlewOptionsPanel -name .tcsSlewOptionsPanel \
         -class SlewOptionsPanel -title "Slew Options" \
         -components tcs.slewoptions 
   EditPanelMgr tcsTrackFramePanel -name .tcsTrackFramePanel \
         -class TrackFramePanel -title "Tracking Frames" \
         -components tcs.trackframe 
   EditPanelMgr tcsTrackFrameMountCosysPanel \
         -name .tcsTrackFrameMountCosysPanel \
         -class CosysPanel -title "Mount Tracking Frame Coordinate System" \
         -components tcs.trackframe.mountcosys
   EditPanelMgr tcsTrackFrameSourceACosysPanel \
         -name .tcsTrackFrameSourceACosysPanel \
         -class CosysPanel -title "Source A Tracking Frame Coordinate System" \
         -components tcs.trackframe.sourceAcosys
   EditPanelMgr tcsTrackFrameSourceBCosysPanel \
         -name .tcsTrackFrameSourceBCosysPanel \
         -class CosysPanel -title "Source B Tracking Frame Coordinate System" \
         -components tcs.trackframe.sourceBcosys
   EditPanelMgr tcsTrackFrameSourceCCosysPanel \
         -name .tcsTrackFrameSourceCCosysPanel \
         -class CosysPanel -title "Source C Tracking Frame Coordinate System" \
         -components tcs.trackframe.sourceCcosys
   EditPanelMgr tcsTrackFramePwfs1CosysPanel \
         -name .tcsTrackFramePwfs1CosysPanel \
         -class CosysPanel -title "PWFS1 Tracking Frame Coordinate System" \
         -components tcs.trackframe.pwfs1cosys
   EditPanelMgr tcsTrackFramePwfs2CosysPanel \
         -name .tcsTrackFramePwfs2CosysPanel \
         -class CosysPanel -title "PWFS2 Tracking Frame Coordinate System" \
         -components tcs.trackframe.pwfs2cosys
   EditPanelMgr tcsTrackFrameOiwfsCosysPanel \
         -name .tcsTrackFrameOiwfsCosysPanel \
         -class CosysPanel -title "OIWFS Tracking Frame Coordinate System" \
         -components tcs.trackframe.oiwfscosys
   EditPanelMgr tcsTrackFrameAowfsCosysPanel \
         -name .tcsTrackFrameAowfsCosysPanel \
         -class CosysPanel -title "AOWFS Tracking Frame Coordinate System" \
         -components tcs.trackframe.aowfscosys
   EditPanelMgr tcsInstrumentPanel -name .tcsInstrumentPanel \
         -class InstrumentPanel -title "Instrument Configuration" \
         -components tcs.instrument 
   EditPanelMgr tcsFieldWavelengthPanel \
         -name .tcsFieldWavelengthPanel -class WavelengthPanel \
         -title "Observing Wavelength" \
         -components tcs.field.wavelength
}
