#+
# default_components.tcl
#
# Defines default configuration and configuration components.
#+
createPointingOrigin "Rotator axis" -x "0.0" -y "0.0" -refcount 1

createCosys FK5/J2000 -frame FK5 -equinox J2000 -refcount 1
createCosys FK4/B1950 -frame FK4 -equinox B1950
createCosys FK4/B1900 -frame FK4 -equinox B1900

createRotator north -cosys FK5/J2000 -ipa 180
createRotator vertical -cosys "" -ipa 180

createGuide "None" -refcount 1

createSlewOptions "All" -refcount 1 \
   -chopThrow "On" \
   -sourceOffset "On" \
   -sourceDifTr "On" \
   -mountOffset "On" \
   -mountDifTr "On" \
   -filter1 "On" \
   -filter2 "On" \
   -pointing "On" \
   -guiding "On"

createSlewOptions "Normal" -refcount 1 \
   -chopThrow "On" \
   -sourceOffset "On" \
   -sourceDifTr "On" \
   -mountOffset "On" \
   -mountDifTr "On" \
   -filter1 "On" \
   -filter2 "On" \
   -pointing "Off" \
   -guiding "On"

createTrackFrame FK5 -refcount 1 \
   -mountCosys "FK5/J2000" \
   -sourceACosys "FK5/J2000" \
   -sourceBCosys "FK5/J2000" \
   -sourceCCosys "FK5/J2000" \
   -pwfs1Cosys "FK5/J2000" \
   -pwfs2Cosys "FK5/J2000" \
   -oiwfsCosys "FK5/J2000" 

createConfig default -refcount 1 \
   -rotator north \
   -slewOptions Normal \
   -trackFrame FK5 \
   -guide None
