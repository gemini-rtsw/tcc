default_target:
	@ if test -n "${site}"; then :;\
	   else echo "Usage: make site=<site code>"; exit 1; fi
	@ if test -r calparams.${site}; then :;\
	   else echo "No configuration files for site \"${site}\""; exit 1; fi
	cp calparams.${site} calparams.dat
	cp options.${site} options
	cd src; make install
	cd lib; make
	cd pointlib; make
	cd scriptlib; make
	cd tcclib; make
	cd tsdlib; make

clean:
	cd src; make clean
	cd lib; make clean
	cd pointlib; make clean
	cd scriptlib; make clean
	cd tcclib; make clean
	cd tsdlib; make clean

