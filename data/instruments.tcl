createInstrument "NIRI" \
   -iaa "0.0" \
   -issFace "port 1" \
   -centralBaffle "closed" \
   -deployableBaffle "near IR" \
   -focusOffset "0.2" \
   -airmassLimit 1.8

createInstrument "NIRS" \
   -iaa "0.0" \
   -issFace "port 3" \
   -centralBaffle "closed" \
   -deployableBaffle "near IR" \
   -focusOffset "0.1" \
   -airmassLimit 1.8

createInstrument "GMOS (IR)" \
   -iaa "0.0" \
   -issFace "port 5" \
   -centralBaffle "closed" \
   -deployableBaffle "near IR" \
   -focusOffset "-0.3" \
   -airmassLimit 1.8

createInstrument "GMOS (optical)" \
   -iaa "0.0" \
   -issFace "port 5" \
   -centralBaffle "closed" \
   -deployableBaffle "optical" \
   -focusOffset "-0.3" \
   -airmassLimit 2.0

createInstrument "MIRI" \
   -iaa "0.0" \
   -issFace "port 1" \
   -centralBaffle "open" \
   -deployableBaffle "thermal IR" \
   -focusOffset "0.2" \
   -airmassLimit 1.5

createInstrument "Michelle" \
   -iaa "0.0" \
   -issFace "port 3" \
   -centralBaffle "open" \
   -deployableBaffle "thermal IR" \
   -focusOffset "-0.1" \
   -airmassLimit 1.5

createInstrument "Phoenix" \
   -iaa "0.0" \
   -issFace "port 5" \
   -centralBaffle "closed" \
   -deployableBaffle "near IR" \
   -focusOffset "0.0" \
   -airmassLimit 1.8
