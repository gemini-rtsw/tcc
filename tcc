#!/bin/sh
# the next line restarts using ocswish \
exec $GEMINI_BASE/ocs/bin/solaris/ocswish8 "$0" "$@"

#+
#  tcc
#
#  This script starts the Gemini telescope control console. It contains all
#  the messy stuff involved in finding the tcl packages and libraries that
#  the tcc uses. It is probably installation dependent and will need to be
#  hacked to get it to work.
#
#  D Terrett 26 April 1999
#
#  Copyright CCLRC
#-

# Define the root directory for locating libraries and applications.
global ROOT
set ROOT [file dirname [info script]]

# The tcc library files live in the lib subdirectory.
lappend auto_path $ROOT/lib
lappend auto_path $ROOT/tcclib

# Load the time and tcc extensions to tcl.
load $ROOT/lib/solaris/tcctime.so
load $ROOT/tcclib/solaris/tccext.so

# Load all the packages required by the tcc.
package require Itcl
namespace import itcl::*
package require Itk
package require Iwidgets
package require Ocspkg

# Set the name of the epics service configuration file.
if { [info exists env(TCC_TCSNAME)] } {
   puts "Using $env(TCC_TCSNAME)"
   set ext .$env(TCC_TCSNAME)
} else {
   set ext ""
}
appData add configPath $ROOT/ca_config$ext
appData add configFile tcc.ca

#appData add debuglevel information

# Read the options configuration file.
option readfile $ROOT/options userDefault

# Start the tcc (eval is used to expand $argv into a list of arguments).
eval tccMain $argv
