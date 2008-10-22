#!/bin/sh

cd ${GEMINI_TOP}/opt/tcc

for i in $*; do
        case $1 in
		-init) shift; export TCC_INITPATH=$1; shift;;
		*) ARGS="$ARGS $1"; shift;;
	esac
done 
if [ "$TCC_XML_PATH" = "" ]; then
	export TCC_INITPATH=${GEMINI_TOP}/opt/tcc/var/local-config/${GEMINI_SITE}
fi
echo "Using TCC_INITPATH=$TCC_INITPATH"
echo "ARGS: $ARGS"
./tcc $ARGS -init $TCC_INITPATH
