#+
#  tcsinit.tcl
#
#  This tcc script sets the pointing, weather and time parameters that are
#  typically set at the begining of the night. When the pointing model has
#  settled down and the weather server is operational, only the time
#  parameters will need setting.
#
#  D Terrett 18 March 1999
#
#  Copyright CCLRC
#+

tcsCommands weather pointParam timeParam

# Time parameters.
set delut ""
set ypm ""
set xpm ""
promptN "UT1-UTC" -1.0 1.0 "UTC - UT1 from the IERS bulletin" delut
promptN "xpm (arcsec)" -2.0 2.0 \
      "x value of polar motion from the IERS bulletin?" xpm
promptN "ypm (arcsec)" -2.0 2.0 \
      "y value of polar motion from the IERS bulletin?" ypm

tcsParameter timeParam name delut
tcsParameter timeParam value $delut
tcsApply
tcsParameter timeParam name xpm
tcsParameter timeParam value $xpm
tcsApply
tcsParameter timeParam name ypm
tcsParameter timeParam value $ypm
tcsApply

# Weather data.
set temp ""
set press ""
set humid ""
promptN "temperature (deg C)" -20.0 20.0 "current ambient temperature" temp
promptN "pressure (mb)" 600 650 "current atmospheric pressure" press
promptN "humidity (%RH)" 0 100 "current relative humidity" humid

tcsParameter weather source 1
tcsApply
tcsParameter weather temp $temp
tcsParameter weather press $press
tcsParameter weather humid $humid
tcsApply

# Local adjustments to CA & CE.
set ca 0
set ce 0
promptN "local CA (arcsec)" 0.0 0.0 \
      "Local adjustment to azimuth collimation"  ca
promptN "local CE (arcsec)" 0.0 0.0 \
      "Local adjustment to elevation collimation" ce

tcsParameter pointParam level LOCAL
tcsParameter pointParam name CA
tcsParameter pointParam value $ca
tcsApply
tcsParameter pointParam name CE
tcsParameter pointParam value $ce
tcsApply
