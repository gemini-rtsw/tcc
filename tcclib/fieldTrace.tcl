#+
#  fieldTrace
#
#  This is a variable trace procedure that tracks changes to the current
#  field name and updates the elements in the Config array that specify
#  the namespace and list in which targets are stored, creating them if
#  necessary.
#
#  Arguments:
#             The usual trace proc arguments.
#
#  D Terrett 28 February 2002
#
#  Copyright CCLRC
#-

proc fieldTrace {n1 n2 op} {

# Set the namespace and listname config array elements for all the target
# types.
   if { $n2 == "tcs.field,value" } {
      if { $::Config(tcs.field,value) == "" } {
         set fieldname $::Config(tcs.field,anon)
      } else {
         set fieldname \
               $::Config(tcs.field,namespace)::$::Config(tcs.field,value)
      }
      set namespace [$fieldname cget -namespace]
      set listname [$fieldname cget -targetlist]
      foreach target {science pwfs1 pwfs2 oiwfs} {
         set ::Config(tcs.field.${target}target,namespace) $namespace
         set ::Config(tcs.field.${target}target,list) $listname
      }
   }
}
