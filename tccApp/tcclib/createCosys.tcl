#+
#  createCosys
#
#  D Terrett 5 April 2002
#
#  Copyright CCLRC
#-

proc createCosys {name args} {
   catch {CosysComponent ::CosysNames::$name CosysList}
   ::CosysNames::$name init
   eval \"::CosysNames::$name\" configure $args
}
