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
#  D Terrett 8 May 2002
#
#  Copyright CCLRC
#-

# Define the root directory for locating libraries and applications.
global ROOT
set ROOT [file dirname [info script]]
if { [string compare [file pathtype $ROOT] relative] == 0 } {
   set ROOT [pwd]/$ROOT
}

# Define the location of the star catalogue files.
global CATALOG_DIR
set CATALOG_DIR $ROOT/pointcats

# Temporary location of DOM & expat packages.
set auto_path [concat $ROOT/../templibs/lib $auto_path]

# The tcc library files live in the lib subdirectory.
lappend auto_path $ROOT/lib
lappend auto_path $ROOT/tcclib

# Load the time and tcc extensions to tcl.
load $ROOT/lib/solaris/tcctime.so
load $ROOT/tcclib/solaris/tccext.so
load $ROOT/lib/solaris/slaext.so

# Load all the packages required by the tcc.
package require Itcl
package require Itk
package require Iwidgets
package require Ocspkg
package require dom
package require Seq

# Define the default directory for writing files.
set SaveFileDirectory /gemini/files

# Set the name of the epics service configuration file.
if { [info exists env(TCC_TCSNAME)] } {
   puts "Using $env(TCC_TCSNAME)"
   set ext .$env(TCC_TCSNAME)
} else {
   set ext ""
   set env(TCC_TCSNAME) tcs
}
appData add configPath $ROOT/ca_config$ext
appData add configFile tcc.ca

#appData add debuglevel information

# Read the options configuration file.
option readfile $ROOT/options userDefault

# Start the tcc (eval is used to expand $argv into a list of arguments).
eval tccMain $argv
