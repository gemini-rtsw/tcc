#+
# default_components.tcl
#
# Defines default configuration and configuration components.
#+
createPointingOrigin "Rotator axis" -x 0.0 -y 0.0 -refcount 1

createCosys FK5/J2000 -frame FK5 -equinox J2000 -refcount 1
createCosys FK4/B1950 -frame FK4 -equinox B1950
createCosys FK4/B1900 -frame FK4 -equinox B1900
createCosys AzEl -frame AZEL_TOPO -equinox ""

createRotator tracking -cosys FK5/J2000 -ipa 180
createRotator fixed -cosys AzEl -ipa 180

createGuide "None" -pwfs1Active never -pwfs2Active never -oiwfsActive never \
   -refcount 1

createSlewOptions All -refcount 1 \
   -chopThrow On \
   -sourceOffset On \
   -sourceDifTr On \
   -mountOffset On \
   -mountDifTr On \
   -filter1 On \
   -filter2 On \
   -pointing On \
   -guiding On

createSlewOptions "Normal" -refcount 1 \
   -chopThrow On \
   -sourceOffset On \
   -sourceDifTr On \
   -mountOffset On \
   -mountDifTr On \
   -filter1 On \
   -filter2 On \
   -pointing Off \
   -guiding On

createTrackFrame FK5/J2000 -refcount 1 \
   -mountCosys FK5/J2000 \
   -sourceACosys FK5/J2000 \
   -sourceBCosys FK5/J2000 \
   -sourceCCosys FK5/J2000 \
   -pwfs1Cosys FK5/J2000 \
   -pwfs2Cosys FK5/J2000 \
   -oiwfsCosys FK5/J2000 

createTrackFrame FK4/B1950 -refcount 1 \
   -mountCosys FK4/B1950 \
   -sourceACosys FK4/B1950 \
   -sourceBCosys FK4/B1950 \
   -sourceCCosys FK4/B1950 \
   -pwfs1Cosys FK4/B1950 \
   -pwfs2Cosys FK4/B1950 \
   -oiwfsCosys FK4/B1950 

createTrackFrame FK4/B1900 -refcount 1 \
   -mountCosys FK4/B1900 \
   -sourceACosys FK4/B1900 \
   -sourceBCosys FK4/B1900 \
   -sourceCCosys FK4/B1900 \
   -pwfs1Cosys FK4/B1900 \
   -pwfs2Cosys FK4/B1900 \
   -oiwfsCosys FK4/B1900 

createTrackFrame AzEL -refcount 1 \
   -mountCosys AzEl \
   -sourceACosys AzEl \
   -sourceBCosys AzEl \
   -sourceCCosys AzEl \
   -pwfs1Cosys AzEl \
   -pwfs2Cosys AzEl \
   -oiwfsCosys AzEl 

createConfig default -refcount 1 \
   -rotator tracking \
   -slewOptions Normal \
   -trackFrame FK5/J2000 \
   -guide None
