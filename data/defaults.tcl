createPointingOrigin "Rotator axis" -x "0.0" -y "0.0"
createPointingOrigin "Aperture A" -x "0.0" -y "0.0"
createPointingOrigin "Aperture B" -x "0.0" -y "0.0"

createCosys FK5/J2000 -frame FK5 -equinox J2000

createRotator north -cosys FK5/J2000 -ipa 0
createRotator vertical -cosys "" -ipa 0

createGuide "Off"

createSlewOptions "All" \
   -chopThrow "On" \
   -sourceOffset "On" \
   -sourceDifTr "On" \
   -mountOffset "On" \
   -mountDifTr "On" \
   -filter1 "On" \
   -filter2 "On" \
   -pointing "On" \
   -guiding "On"

createConfig default \
   -rotator north \
   -pointOrig "Rotator axis" \
   -slewOptions All \
   -trackFrame FK5/J2000
