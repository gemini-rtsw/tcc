default_target:
	@echo "Please specify either MK or CP as the make target"

MK: tclexts
	cp calparams.MK calparams.dat
	cp options.MK options

CP: tclexts
	cp calparams.CP calparams.dat
	cp options.CP options

tclexts:
	cd src; make install
	cd lib; make
	cd pointlib; make
	cd scriptlib; make
	cd tcclib; make
	cd tsdlib; make
