createCosys ICRS -frame FK5 -equinox J2000
createCosys Appt -frame Appt -equinox ""

createTrackFrame "ICRS" \
   -mountCosys "ICRS" \
   -sourceACosys "ICRS" \
   -sourceBCosys "ICRS" \
   -pwfs1Cosys "ICRS" \
   -pwfs2Cosys "ICRS" \
   -oiwfsCosys "ICRS" 
createTrackFrame "Apparent place" \
   -mountCosys "Appt" \
   -sourceACosys "Appt" \
   -sourceBCosys "Appt" \
   -pwfs1Cosys "Appt" \
   -pwfs2Cosys "Appt" \
   -oiwfsCosys "Appt"
createTrackFrame "FK4/B1950" \
   -mountCosys "FK4/B1950" \
   -sourceACosys "FK4/B1950" \
   -sourceBCosys "FK4/B1950" \
   -pwfs1Cosys "FK4/B1950" \
   -pwfs2Cosys "FK4/B1950" \
   -oiwfsCosys "FK4/B1950"
createTrackFrame "FK5/J2000" \
   -mountCosys "FK5/J2000" \
   -sourceACosys "FK5/J2000" \
   -sourceBCosys "FK5/J2000" \
   -pwfs1Cosys "FK5/J2000" \
   -pwfs2Cosys "FK5/J2000" \
   -oiwfsCosys "FK5/J2000"
createTrackFrame "Az/El" \
   -mountCosys "" \
   -sourceACosys "" \
   -sourceBCosys "" \
   -sourceCCosys "" \
   -pwfs1Cosys "" \
   -pwfs2Cosys "" \
   -oiwfsCosys ""
