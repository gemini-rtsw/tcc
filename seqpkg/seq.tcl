#+
#  seq.tcl
#
#  Implements a package for managing sequences of commands sent to an
#  EPICS principle system. The package defines two classes; 
#  seq::PrincipleSystem objects monitor a principle system for command
#  completion and seq::Action objects represent the individual commands
#  that make up the sequence.
#
#  D Terrett 13 June 2000
#
#  Copyright CCLRC
#+

package require Itcl
package require Ocspkg
package provide Seq 1.0
set dirname [file dirname [info script]]
lappend auto_path [file join $dirname src]
