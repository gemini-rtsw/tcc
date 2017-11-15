#+
# class TaggedTarget
#
# This class associates a target with a tag. The tag indicates that this target
# is a cadidate target for a specific WFS, or is the science target
#+

package require Itcl

itcl::class TaggedTarget {
	public {
		variable tagPrefix ""
		variable tagSuffix ""
		variable target ""

		constructor {tag _target} {}
		method getFullTag {{separator -}} {}
	}
}

itcl::body TaggedTarget::constructor {tag _target} {
	if { [string equal $tag ""] } {
		set tagPrefix ""
		set tagSuffix ""
	} else {
		#Try to decompose the tag
		set separatorIndex [string last - $tag]
		if { $separatorIndex == -1 } {
			if {[string match "User*" $tag]} {
				set tagPrefix User
				set tagSuffix [string range $tag 4 end]
			} else {
				set tagPrefix $tag
				set tagSuffix ""
			}
		} else {
			if { $separatorIndex > 0 } {
				set tagPrefix [string range $tag 0 [expr "$separatorIndex - 1"] ]
			} else {
				set tagPrefix ""
			}
			if { $separatorIndex < [expr "[string length $tag]-1"] } {
				set tagSuffix [string range $tag [expr "$separatorIndex + 1"] end]
			} else {
				set tagSuffix ""
			}
		}
	}
	set target $_target
}

itcl::body TaggedTarget::getFullTag {{separator -}} {
   if { ![ string equal [$target cget -tag] "" ] } {
      set prefix [$target cget -tag]
   } else {
      set prefix $tagPrefix
   }
   if { [string equal $tagSuffix ""] } {
		return $prefix
	} else {
		return $prefix$separator$tagSuffix
	}
}