default_target:
	@ if test -n "${site}"; then :;\
	   else echo "Usage: make site=<site code>"; exit 1; fi
	@ if test -r defaultparams.${site}.`uname`; then :;\
	   else echo "No configuration files for site \"${site}\""; exit 1; fi
	cp defaultparams.${site}.`uname` defaultparams.dat
	cp options.${site} options
	cd src; make install
	cd lib; make
	cd pointlib; make
	cd scriptlib; make
	cd tcclib; make
	cd tsdlib; make
