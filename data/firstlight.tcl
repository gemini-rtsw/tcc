catch {delete object "::TcsConfigNames::default"}
ConfigComponent "::TcsConfigNames::default"  TcsConfigList \
   -scienceTarget "" \
   -guide "" \
   -rotator "north" \
   -pointOrig "Rotator axis" \
   -slewOptions "All" \
   -trackFrame "FK5/J2000" \
   -instrument ""
catch {delete object "::TcsConfigNames::MS0302+16"}
ConfigComponent "::TcsConfigNames::MS0302+16"  TcsConfigList \
   -scienceTarget "MS0302+16" \
   -guide "MS0302+16" \
   -rotator "vertical" \
   -pointOrig "Rotator axis" \
   -slewOptions "All" \
   -trackFrame "FK5/J2000" \
   -instrument ""
catch {delete object "::TcsConfigNames::HCG-31"}
ConfigComponent "::TcsConfigNames::HCG-31"  TcsConfigList \
   -scienceTarget "HCG-31" \
   -guide "HCG-31" \
   -rotator "vertical" \
   -pointOrig "" \
   -slewOptions "All" \
   -trackFrame "" \
   -instrument ""
catch {delete object "::TcsConfigNames::M42"}
ConfigComponent "::TcsConfigNames::M42"  TcsConfigList \
   -scienceTarget "M42" \
   -guide "" \
   -rotator "vertical" \
   -pointOrig "" \
   -slewOptions "All" \
   -trackFrame "" \
   -instrument ""
catch {delete object "::TcsConfigNames::N2071"}
ConfigComponent "::TcsConfigNames::N2071"  TcsConfigList \
   -scienceTarget "N2071" \
   -guide "N2071" \
   -rotator "vertical" \
   -pointOrig "" \
   -slewOptions "All" \
   -trackFrame "" \
   -instrument ""
catch {delete object "::TcsConfigNames::MonR2"}
ConfigComponent "::TcsConfigNames::MonR2"  TcsConfigList \
   -scienceTarget "MonR2" \
   -guide "MonR2" \
   -rotator "vertical" \
   -pointOrig "" \
   -slewOptions "All" \
   -trackFrame "" \
   -instrument ""
catch {delete object "::TcsConfigNames::M1-16"}
ConfigComponent "::TcsConfigNames::M1-16"  TcsConfigList \
   -scienceTarget "M1-16" \
   -guide "M1-16" \
   -rotator "vertical" \
   -pointOrig "" \
   -slewOptions "All" \
   -trackFrame "" \
   -instrument ""
catch {delete object "::TcsConfigNames::N2623"}
ConfigComponent "::TcsConfigNames::N2623"  TcsConfigList \
   -scienceTarget "N2623" \
   -guide "N2623" \
   -rotator "vertical" \
   -pointOrig "" \
   -slewOptions "All" \
   -trackFrame "" \
   -instrument ""
catch {delete object "::TcsConfigNames::Arp299a"}
ConfigComponent "::TcsConfigNames::Arp299a"  TcsConfigList \
   -scienceTarget "Arp299a" \
   -guide "Arp299a" \
   -rotator "vertical" \
   -pointOrig "" \
   -slewOptions "All" \
   -trackFrame "" \
   -instrument ""
catch {delete object "::TcsConfigNames::Arp299b"}
ConfigComponent "::TcsConfigNames::Arp299b"  TcsConfigList \
   -scienceTarget "Arp299b" \
   -guide "Arp299b" \
   -rotator "vertical" \
   -pointOrig "" \
   -slewOptions "All" \
   -trackFrame "" \
   -instrument ""
catch {delete object "::TcsConfigNames::CL1322+31"}
ConfigComponent "::TcsConfigNames::CL1322+31"  TcsConfigList \
   -scienceTarget "CL1322+31" \
   -guide "CL1322+31" \
   -rotator "vertical" \
   -pointOrig "" \
   -slewOptions "All" \
   -trackFrame "" \
   -instrument ""
catch {delete object "::TcsConfigNames::MS1512+36"}
ConfigComponent "::TcsConfigNames::MS1512+36"  TcsConfigList \
   -scienceTarget "MS1512+36" \
   -guide "MS1512+36" \
   -rotator "vertical" \
   -pointOrig "" \
   -slewOptions "All" \
   -trackFrame "" \
   -instrument ""
catch {delete object "::TcsConfigNames::N2419"}
ConfigComponent "::TcsConfigNames::N2419"  TcsConfigList \
   -scienceTarget "N2419" \
   -guide "" \
   -rotator "vertical" \
   -pointOrig "" \
   -slewOptions "All" \
   -trackFrame "" \
   -instrument ""
catch {delete object "::TcsConfigNames::N5272"}
ConfigComponent "::TcsConfigNames::N5272"  TcsConfigList \
   -scienceTarget "N5272" \
   -guide "" \
   -rotator "vertical" \
   -pointOrig "" \
   -slewOptions "All" \
   -trackFrame "" \
   -instrument ""
catch {delete object "::TcsConfigNames::N5904"}
ConfigComponent "::TcsConfigNames::N5904"  TcsConfigList \
   -scienceTarget "N5904" \
   -guide "" \
   -rotator "vertical" \
   -pointOrig "" \
   -slewOptions "All" \
   -trackFrame "" \
   -instrument ""
catch {delete object "::TcsConfigNames::M45"}
ConfigComponent "::TcsConfigNames::M45"  TcsConfigList \
   -scienceTarget "M45" \
   -guide "" \
   -rotator "vertical" \
   -pointOrig "" \
   -slewOptions "All" \
   -trackFrame "" \
   -instrument ""
catch {delete object "::ScienceTargetNames::Vela Pulsar"}
TargetComponent "::ScienceTargetNames::Vela Pulsar"  ScienceTargetList \
   -objName "Vela Pulsar" \
   -cosys "FK4/B1950" \
   -radec "08:33:39.22 -45:00:10.1" \
   -epoch "1977" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::NGC 224 (M31)"}
TargetComponent "::ScienceTargetNames::NGC 224 (M31)"  ScienceTargetList \
   -objName "M31" \
   -cosys "FK5/J1997.5" \
   -radec "0:42:36 41:15:18" \
   -epoch "J1997.5" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::3C 273"}
TargetComponent "::ScienceTargetNames::3C 273"  ScienceTargetList \
   -objName "3C 273" \
   -cosys "FK5/J2000" \
   -radec "12:29:06.67 2:03:08.59" \
   -epoch "J2000" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::BL Lac"}
TargetComponent "::ScienceTargetNames::BL Lac"  ScienceTargetList \
   -objName "BL Lac" \
   -cosys "FK5/J2000" \
   -radec "22:02:43.291 42:16:39.98" \
   -epoch "J2000" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::NGC 5921"}
TargetComponent "::ScienceTargetNames::NGC 5921"  ScienceTargetList \
   -objName "" \
   -cosys "FK5/J1993.5" \
   -radec "15:21.62 +5:05.6" \
   -epoch "" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::FS1"}
TargetComponent "::ScienceTargetNames::FS1"  ScienceTargetList \
   -objName "FS1" \
   -cosys "FK4/B1950" \
   -radec "00:31:22.7 -12:24:29" \
   -epoch "B1950" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::FS2"}
TargetComponent "::ScienceTargetNames::FS2"  ScienceTargetList \
   -objName "FS2" \
   -cosys "FK4/B1950" \
   -radec "00:52:36 00:26:58" \
   -epoch "B1950" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::FS3"}
TargetComponent "::ScienceTargetNames::FS3"  ScienceTargetList \
   -objName "FS3" \
   -cosys "FK4/B1950" \
   -radec "01:01:46.6 03:57:34" \
   -epoch "B1950" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::FS4"}
TargetComponent "::ScienceTargetNames::FS4"  ScienceTargetList \
   -objName "FS4" \
   -cosys "FK4/B1950" \
   -radec "01:52:03.7 00:28:20" \
   -epoch "B1950" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::FS5"}
TargetComponent "::ScienceTargetNames::FS5"  ScienceTargetList \
   -objName "FS5" \
   -cosys "FK4/B1950" \
   -radec "01:52:04.7 -7:00:47" \
   -epoch "B1950" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::FS6"}
TargetComponent "::ScienceTargetNames::FS6"  ScienceTargetList \
   -objName "FS6" \
   -cosys "FK4/B1950" \
   -radec "02:27:39.2 05:02:34" \
   -epoch "B1950" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::FS7"}
TargetComponent "::ScienceTargetNames::FS7"  ScienceTargetList \
   -objName "FS7" \
   -cosys "FK4/B1950" \
   -radec "02:54:47.2 00:06:39" \
   -epoch "B1950" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::FS8"}
TargetComponent "::ScienceTargetNames::FS8"  ScienceTargetList \
   -objName "FS8" \
   -cosys "FK4/B1950" \
   -radec "02:55:12.9 00:04:04" \
   -epoch "B1950" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::FS9"}
TargetComponent "::ScienceTargetNames::FS9"  ScienceTargetList \
   -objName "FS9" \
   -cosys "FK4/B1950" \
   -radec "02:55:38.8 00:58:54" \
   -epoch "B1950" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::FS11"}
TargetComponent "::ScienceTargetNames::FS11"  ScienceTargetList \
   -objName "FS11" \
   -cosys "FK4/B1950" \
   -radec "04:50:25.4 -00:19:34" \
   -epoch "B1950" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::FS12"}
TargetComponent "::ScienceTargetNames::FS12"  ScienceTargetList \
   -objName "FS12" \
   -cosys "FK4/B1950" \
   -radec "05:49:34.8 15:52:37" \
   -epoch "B1950" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::FS13"}
TargetComponent "::ScienceTargetNames::FS13"  ScienceTargetList \
   -objName "FS13" \
   -cosys "FK4/B1950" \
   -radec "05:54:33.8 00:00:53" \
   -epoch "B1950" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::FS14"}
TargetComponent "::ScienceTargetNames::FS14"  ScienceTargetList \
   -objName "FS14" \
   -cosys "FK4/B1950" \
   -radec "07:21:41.2 -00:27:10" \
   -epoch "B1950" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::FS15"}
TargetComponent "::ScienceTargetNames::FS15"  ScienceTargetList \
   -objName "FS15" \
   -cosys "FK4/B1950" \
   -radec "08:48:21.9 11:55:02" \
   -epoch "B1950" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::FS16"}
TargetComponent "::ScienceTargetNames::FS16"  ScienceTargetList \
   -objName "FS16" \
   -cosys "FK4/B1950" \
   -radec "08:48:31.0 12:00:36" \
   -epoch "B1950" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::FS17"}
TargetComponent "::ScienceTargetNames::FS17"  ScienceTargetList \
   -objName "FS17" \
   -cosys "FK4/B1950" \
   -radec "08:48:35.4 12:03:26" \
   -epoch "B1950" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::FS19"}
TargetComponent "::ScienceTargetNames::FS19"  ScienceTargetList \
   -objName "FS19" \
   -cosys "FK4/B1950" \
   -radec "10:31:14.5 -11:26:08" \
   -epoch "B1950" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::FS20"}
TargetComponent "::ScienceTargetNames::FS20"  ScienceTargetList \
   -objName "FS20" \
   -cosys "FK4/B1950" \
   -radec "11:05:27.6 -04:53:04" \
   -epoch "B1950" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::FS21"}
TargetComponent "::ScienceTargetNames::FS21"  ScienceTargetList \
   -objName "FS11" \
   -cosys "FK4/B1950" \
   -radec "11:34:27.6 30:04:35" \
   -epoch "B1950" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::FS23"}
TargetComponent "::ScienceTargetNames::FS23"  ScienceTargetList \
   -objName "FS23" \
   -cosys "FK4/B1950" \
   -radec "13:39:25.7 28:44:59" \
   -epoch "B1950" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::FS24"}
TargetComponent "::ScienceTargetNames::FS24"  ScienceTargetList \
   -objName "FS24" \
   -cosys "FK4/B1950" \
   -radec "14:37:33.3 00:14:36" \
   -epoch "B1950" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::FS26"}
TargetComponent "::ScienceTargetNames::FS26"  ScienceTargetList \
   -objName "FS26" \
   -cosys "FK4/B1950" \
   -radec "16:34:26.3.0 -00:28:39" \
   -epoch "B1950" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::FS27"}
TargetComponent "::ScienceTargetNames::FS27"  ScienceTargetList \
   -objName "FS27" \
   -cosys "FK4/B1950" \
   -radec "16:38:54.2 36:26:56" \
   -epoch "B1950" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::FS29"}
TargetComponent "::ScienceTargetNames::FS29"  ScienceTargetList \
   -objName "FS29" \
   -cosys "FK4/B1950" \
   -radec "21:49:53.0 02:09:16" \
   -epoch "B1950" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::FS30"}
TargetComponent "::ScienceTargetNames::FS30"  ScienceTargetList \
   -objName "FS30" \
   -cosys "FK4/B1950" \
   -radec "22:39:11.3.6 00:56:55" \
   -epoch "B1950" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::FS31"}
TargetComponent "::ScienceTargetNames::FS31"  ScienceTargetList \
   -objName "FS31" \
   -cosys "FK4/B1950" \
   -radec "23:09:50.4 10:30:46" \
   -epoch "B1950" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::FS32"}
TargetComponent "::ScienceTargetNames::FS32"  ScienceTargetList \
   -objName "FS32" \
   -cosys "FK4/B1950" \
   -radec "23:13:38.2 -02:06:58" \
   -epoch "B1950" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::N2419"}
TargetComponent "::ScienceTargetNames::N2419"  ScienceTargetList \
   -objName "N2419" \
   -cosys "FK5/J2000" \
   -radec "07:38:08.45 38:52:55" \
   -epoch "J2000" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::N5272"}
TargetComponent "::ScienceTargetNames::N5272"  ScienceTargetList \
   -objName "N5272" \
   -cosys "FK5/J2000" \
   -radec "13:42:11.2 28:22:32" \
   -epoch "J2000" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::N5904"}
TargetComponent "::ScienceTargetNames::N5904"  ScienceTargetList \
   -objName "N5904" \
   -cosys "FK5/J2000" \
   -radec "15:18:33.7 02:04:58" \
   -epoch "J2000" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::M45"}
TargetComponent "::ScienceTargetNames::M45"  ScienceTargetList \
   -objName "M45" \
   -cosys "FK5/J2000" \
   -radec "03:47:00 24:07:00" \
   -epoch "J2000" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::MS0302+16"}
TargetComponent "::ScienceTargetNames::MS0302+16"  ScienceTargetList \
   -objName "MS0302+16" \
   -cosys "FK5/J2000" \
   -radec "03:05:31.6 17:10:03.3" \
   -epoch "J2000" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::HCG-31"}
TargetComponent "::ScienceTargetNames::HCG-31"  ScienceTargetList \
   -objName "HCG-31" \
   -cosys "FK5/J2000" \
   -radec "05:01:38.0 -04:15:27" \
   -epoch "J2000" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::M42"}
TargetComponent "::ScienceTargetNames::M42"  ScienceTargetList \
   -objName "M42" \
   -cosys "FK5/J2000" \
   -radec "05:35:12.8 -05:22:44" \
   -epoch "J2000" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::N2071"}
TargetComponent "::ScienceTargetNames::N2071"  ScienceTargetList \
   -objName "N2071" \
   -cosys "FK5/J2000" \
   -radec "05:47:04.45 00:21:44" \
   -epoch "J2000" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::MonR2"}
TargetComponent "::ScienceTargetNames::MonR2"  ScienceTargetList \
   -objName "MonR2" \
   -cosys "FK5/J2000" \
   -radec "06:07:45.3 -06:22:54.8" \
   -epoch "J2000" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::M1-16"}
TargetComponent "::ScienceTargetNames::M1-16"  ScienceTargetList \
   -objName "M1-16" \
   -cosys "FK5/J2000" \
   -radec "07:37:18.3 -09:38:43" \
   -epoch "J2000" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::N2623"}
TargetComponent "::ScienceTargetNames::N2623"  ScienceTargetList \
   -objName "N2623" \
   -cosys "FK5/J2000" \
   -radec "08:38:24 25:45:17" \
   -epoch "J2000" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::Arp299a"}
TargetComponent "::ScienceTargetNames::Arp299a"  ScienceTargetList \
   -objName "Arp299a" \
   -cosys "FK5/J2000" \
   -radec "11:28:32.5 58:33:43" \
   -epoch "J2000" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::Arp299b"}
TargetComponent "::ScienceTargetNames::Arp299b"  ScienceTargetList \
   -objName "Arp299b" \
   -cosys "FK5/J2000" \
   -radec "11:28:31 58:33:43" \
   -epoch "J2000" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::CL1322+31"}
TargetComponent "::ScienceTargetNames::CL1322+31"  ScienceTargetList \
   -objName "CL1322+31" \
   -cosys "FK5/J2000" \
   -radec "13:24:47.2 30:59:00.1" \
   -epoch "J2000" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::ScienceTargetNames::MS1512+36"}
TargetComponent "::ScienceTargetNames::MS1512+36"  ScienceTargetList \
   -objName "MS1512+36" \
   -cosys "FK5/J2000" \
   -radec "15:14:22.4 36:36:21" \
   -epoch "J2000" \
   -parallax "0" \
   -pm "" \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::WfsTargetNames::GSC0122500571"}
TargetComponent "::WfsTargetNames::GSC0122500571"  WfsTargetList \
   -objName "GSC0122500571" \
   -cosys "FK5/J2000" \
   -radec "03:05:36.031 +17:15:42.19" \
   -epoch "" \
   -parallax "0" \
   -pm "  " \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::WfsTargetNames::GSC0122500930"}
TargetComponent "::WfsTargetNames::GSC0122500930"  WfsTargetList \
   -objName "GSC0122500930" \
   -cosys "FK5/J2000" \
   -radec "03:05:50.376 +17:06:56.81" \
   -epoch "" \
   -parallax "0" \
   -pm "  " \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::WfsTargetNames::GSC0475800962"}
TargetComponent "::WfsTargetNames::GSC0475800962"  WfsTargetList \
   -objName "GSC0475800962" \
   -cosys "FK5/J2000" \
   -radec "05:01:41.431 -04:10:13.51" \
   -epoch "" \
   -parallax "0" \
   -pm "  " \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::WfsTargetNames::GSC0011601247"}
TargetComponent "::WfsTargetNames::GSC0011601247"  WfsTargetList \
   -objName "GSC0011601247" \
   -cosys "FK5/J2000" \
   -radec "05:46:45.864 +00:17:03.55" \
   -epoch "" \
   -parallax "0" \
   -pm "  " \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::WfsTargetNames::GSC0479501284"}
TargetComponent "::WfsTargetNames::GSC0479501284"  WfsTargetList \
   -objName "GSC0479501284" \
   -cosys "FK5/J2000" \
   -radec "06:07:48.816 -06:16:35.98" \
   -epoch "" \
   -parallax "0" \
   -pm "  " \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::WfsTargetNames::GSC0479500991"}
TargetComponent "::WfsTargetNames::GSC0479500991"  WfsTargetList \
   -objName "GSC0479500991" \
   -cosys "FK5/J2000" \
   -radec "06:07:59.393 -06:26:27.82" \
   -epoch "" \
   -parallax "0" \
   -pm "  " \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::WfsTargetNames::GSC0540100199"}
TargetComponent "::WfsTargetNames::GSC0540100199"  WfsTargetList \
   -objName "GSC0540100199" \
   -cosys "FK5/J2000" \
   -radec "07:37:15.228 -09:32:46.28" \
   -epoch "" \
   -parallax "0" \
   -pm "  " \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::WfsTargetNames::GSC0540101325"}
TargetComponent "::WfsTargetNames::GSC0540101325"  WfsTargetList \
   -objName "GSC0540101325" \
   -cosys "FK5/J2000" \
   -radec "07:37:37.990 -09:40:07.82" \
   -epoch "" \
   -parallax "0" \
   -pm "  " \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::WfsTargetNames::GSC0194501654"}
TargetComponent "::WfsTargetNames::GSC0194501654"  WfsTargetList \
   -objName "GSC0194501654" \
   -cosys "FK5/J2000" \
   -radec "08:38:22.306 +25:49:47.68" \
   -epoch "" \
   -parallax "0" \
   -pm "  " \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::WfsTargetNames::GSC0194502062"}
TargetComponent "::WfsTargetNames::GSC0194502062"  WfsTargetList \
   -objName "GSC0194502062" \
   -cosys "FK5/J2000" \
   -radec "08:38:38.854 +25:41:41.57" \
   -epoch "" \
   -parallax "0" \
   -pm "  " \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::WfsTargetNames::GSC0383100412"}
TargetComponent "::WfsTargetNames::GSC0383100412"  WfsTargetList \
   -objName "GSC0383100412" \
   -cosys "FK5/J2000" \
   -radec "11:28:28.951 +58:39:02.16" \
   -epoch "" \
   -parallax "0" \
   -pm "  " \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::WfsTargetNames::GSC0383100795"}
TargetComponent "::WfsTargetNames::GSC0383100795"  WfsTargetList \
   -objName "GSC0383100795" \
   -cosys "FK5/J2000" \
   -radec "11:28:49.286 +58:29:24.97" \
   -epoch "" \
   -parallax "0" \
   -pm "  " \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::WfsTargetNames::GSC0253600418"}
TargetComponent "::WfsTargetNames::GSC0253600418"  WfsTargetList \
   -objName "GSC0253600418" \
   -cosys "FK5/J2000" \
   -radec "13:25:16.382 +30:58:13.87" \
   -epoch "" \
   -parallax "0" \
   -pm "  " \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::WfsTargetNames::GSC0253600692"}
TargetComponent "::WfsTargetNames::GSC0253600692"  WfsTargetList \
   -objName "GSC0253600692" \
   -cosys "FK5/J2000" \
   -radec "13:24:25.447 +30:57:32.90" \
   -epoch "" \
   -parallax "0" \
   -pm "  " \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::WfsTargetNames::GSC0256900287"}
TargetComponent "::WfsTargetNames::GSC0256900287"  WfsTargetList \
   -objName "GSC0256900287" \
   -cosys "FK5/J2000" \
   -radec "15:14:16.975 +36:41:55.57" \
   -epoch "" \
   -parallax "0" \
   -pm "  " \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::WfsTargetNames::GSC0256900087"}
TargetComponent "::WfsTargetNames::GSC0256900087"  WfsTargetList \
   -objName "GSC0256900087" \
   -cosys "FK5/J2000" \
   -radec "15:14:14.870 +36:30:21.89" \
   -epoch "" \
   -parallax "0" \
   -pm "  " \
   -rv "0" \
   -azel "" \
   -wavelength_name "" \
   -wavelength_value "" \
   -trackrate ""
catch {delete object "::RotatorNames::north"}
RotatorComponent "::RotatorNames::north"  RotatorList \
   -ipa "0" \
   -cosys "FK5/J2000"
catch {delete object "::RotatorNames::vertical"}
RotatorComponent "::RotatorNames::vertical"  RotatorList \
   -ipa "180.02" \
   -cosys ""
catch {delete object "::SlewOptionsNames::All"}
SlewOptionsComponent "::SlewOptionsNames::All"  SlewOptionsList \
   -chopThrow "On" \
   -sourceOffset "On" \
   -sourceDifTr "On" \
   -mountOffset "On" \
   -mountDifTr "On" \
   -filter1 "On" \
   -filter2 "On" \
   -pointing "On" \
   -guiding "On"
catch {delete object "::GuideNames::Off"}
GuideComponent "::GuideNames::Off"  GuideList \
   -pwfs1Target "" \
   -pwfs1Chop A \
   -pwfs1Nod AB \
   -pwfs2Target "" \
   -pwfs2Chop A \
   -pwfs2Nod AB \
   -oiwfsTarget "" \
   -oiwfsChop A \
   -oiwfsNod AB
catch {delete object "::GuideNames::MS0302+16"}
GuideComponent "::GuideNames::MS0302+16"  GuideList \
   -pwfs1Target "GSC0122500571" \
   -pwfs1Chop A \
   -pwfs1Nod AB \
   -pwfs2Target "GSC0122500930" \
   -pwfs2Chop A \
   -pwfs2Nod AB \
   -oiwfsTarget "" \
   -oiwfsChop A \
   -oiwfsNod AB
catch {delete object "::GuideNames::HCG-31"}
GuideComponent "::GuideNames::HCG-31"  GuideList \
   -pwfs1Target "GSC0475800962" \
   -pwfs1Chop A \
   -pwfs1Nod AB \
   -pwfs2Target "" \
   -pwfs2Chop A \
   -pwfs2Nod AB \
   -oiwfsTarget "" \
   -oiwfsChop A \
   -oiwfsNod AB
catch {delete object "::GuideNames::N2071"}
GuideComponent "::GuideNames::N2071"  GuideList \
   -pwfs1Target "GSC0011601247" \
   -pwfs1Chop A \
   -pwfs1Nod AB \
   -pwfs2Target "" \
   -pwfs2Chop A \
   -pwfs2Nod AB \
   -oiwfsTarget "" \
   -oiwfsChop A \
   -oiwfsNod AB
catch {delete object "::GuideNames::MonR2"}
GuideComponent "::GuideNames::MonR2"  GuideList \
   -pwfs1Target "GSC0479501284" \
   -pwfs1Chop A \
   -pwfs1Nod AB \
   -pwfs2Target "GSC0479500991" \
   -pwfs2Chop A \
   -pwfs2Nod AB \
   -oiwfsTarget "" \
   -oiwfsChop A \
   -oiwfsNod AB
catch {delete object "::GuideNames::M1-16"}
GuideComponent "::GuideNames::M1-16"  GuideList \
   -pwfs1Target "GSC0540100199" \
   -pwfs1Chop A \
   -pwfs1Nod AB \
   -pwfs2Target "GSC0540101325" \
   -pwfs2Chop A \
   -pwfs2Nod AB \
   -oiwfsTarget "" \
   -oiwfsChop A \
   -oiwfsNod AB
catch {delete object "::GuideNames::N2623"}
GuideComponent "::GuideNames::N2623"  GuideList \
   -pwfs1Target "GSC0194501654" \
   -pwfs1Chop A \
   -pwfs1Nod AB \
   -pwfs2Target "GSC0194502062" \
   -pwfs2Chop A \
   -pwfs2Nod AB \
   -oiwfsTarget "" \
   -oiwfsChop A \
   -oiwfsNod AB
catch {delete object "::GuideNames::Arp299a"}
GuideComponent "::GuideNames::Arp299a"  GuideList \
   -pwfs1Target "GSC0383100412" \
   -pwfs1Chop A \
   -pwfs1Nod AB \
   -pwfs2Target "GSC0383100795" \
   -pwfs2Chop A \
   -pwfs2Nod AB \
   -oiwfsTarget "" \
   -oiwfsChop A \
   -oiwfsNod AB
catch {delete object "::GuideNames::Arp299b"}
GuideComponent "::GuideNames::Arp299b"  GuideList \
   -pwfs1Target "GSC0383100412" \
   -pwfs1Chop A \
   -pwfs1Nod AB \
   -pwfs2Target "GSC0383100795" \
   -pwfs2Chop A \
   -pwfs2Nod AB \
   -oiwfsTarget "" \
   -oiwfsChop A \
   -oiwfsNod AB
catch {delete object "::GuideNames::CL1322+31"}
GuideComponent "::GuideNames::CL1322+31"  GuideList \
   -pwfs1Target "GSC0253600418" \
   -pwfs1Chop A \
   -pwfs1Nod AB \
   -pwfs2Target "GSC0253600692" \
   -pwfs2Chop A \
   -pwfs2Nod AB \
   -oiwfsTarget "" \
   -oiwfsChop A \
   -oiwfsNod AB
catch {delete object "::GuideNames::MS1512+36"}
GuideComponent "::GuideNames::MS1512+36"  GuideList \
   -pwfs1Target "GSC0256900287" \
   -pwfs1Chop A \
   -pwfs1Nod AB \
   -pwfs2Target "GSC0256900087" \
   -pwfs2Chop A \
   -pwfs2Nod AB \
   -oiwfsTarget "" \
   -oiwfsChop A \
   -oiwfsNod AB
