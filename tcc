#!/bin/sh
# the next line restarts using ocswish \
exec $HLPG_INSTALL_BASE/bin/solaris/ocswish "$0" "$@"

#+
#  tcc
#
#  This script starts the Gemini telescope control console. It contains all
#  the messy stuff involved in finding the tcl packages and libraries that
#  the tcc uses. It is probably installation dependent and will need to be
#  hacked to get it to work.
#
#  D Terrett 17 September 1999
#
#  Copyright CCLRC
#-

# Define the default directory for writing files.
global SAVE_FILE_DIR
set SAVE_FILE_DIR /gemini/files

# Define the root directory for locating libraries and applications.
global ROOT
set ROOT [file dirname [info script]]
if { [string compare [file pathtype $ROOT] relative] == 0 } {
   set ROOT [pwd]/$ROOT
}

# The tcc library files live in the lib subdirectory.
lappend auto_path $ROOT/lib
lappend auto_path $ROOT/tcclib

# Load the time and tcc extensions to tcl.
load $ROOT/lib/solaris/tcctime.so
load $ROOT/lib/solaris/slaext.so
load $ROOT/lib/solaris/bitmaps.so
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
