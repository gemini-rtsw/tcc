createCosys FK5/J1997.5 -frame FK5 -equinox J1997.5
createCosys FK5/J1993.5 -frame FK5 -equinox J1993.5
createCosys FK4/B1950 -frame FK4 -equinox B1950

createScienceTarget "Vela Pulsar" \
   -objName "Vela Pulsar" \
   -cosys "FK4/B1950" \
   -radec "08:33:39.22 -45:00:10.1" \
   -epoch "1977"

createScienceTarget "NGC 224 (M31)" \
   -objName "M31" \
   -cosys "FK5/J1997.5" \
   -radec "0:42:36 41:15:18" \
   -epoch "J1997.5"

createScienceTarget "3C 273" \
   -objName "3C 273" \
   -cosys "FK5/J2000" \
   -radec "12:29:06.67 2:03:08.59" \
   -epoch "J2000"

createScienceTarget "BL Lac" \
   -objName "BL Lac" \
   -cosys "FK5/J2000" \
   -radec "22:02:43.291 42:16:39.98" \
   -epoch "J2000"

createScienceTarget "NGC 5921" \
   -cosys FK5/J1993.5 \
   -radec "15:21.62 +5:05.6" \
