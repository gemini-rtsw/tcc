All: tccext.so tsdext.so tcctime.so slaext.so tcctelsim.so

TCL_VERS = 8.4

CC = gcc

CMNOBJ = tccDecode.o tccUtil.o tccAst.o
TCCOBJ = tccext.o autorepeat.o plosc.o 
TSDOBJ = tsdext.o tsdDomeView.o tsdWrapView.o tsdElView.o
TIMEOBJ = tccTime.o
SLAOBJ = slaext.o
TELSIMOBJ = tccTelSim.o

CFLAGS = -fPIC -Wall -G -DUSE_TCL_STUBS -DUSE_TK_STUBS \
         -I${HLPG_EXT_RUNTIME_BASE}/tcl$(TCL_VERS)/include \
         -I/gemini/epics3.13.4/slalib/slalib/include \
         -I/gemini/epics3.13.4/timelib/timelib/include \
         -I/gemini/epics3.13.4/astlib/astlib/include

LDFLAGS = -L/gemini/epics3.13.4/slalib/slalib/lib/solaris \
          -L/gemini/epics3.13.4/timelib/timelib/lib/solaris \
          -L/gemini/epics3.13.4/astlib/astlib/lib/solaris \
          -L${HLPG_EXT_RUNTIME_BASE}/tcl$(TCL_VERS)/lib \
          -ltclstub$(TCL_VERS) -ltkstub$(TCL_VERS) -last -ltime -lsla -lposix4

tccext.so : $(TCCOBJ) $(CMNOBJ)
	ld -G -z text -o $@ $(TCCOBJ) $(CMNOBJ) $(LDFLAGS) 

tsdext.so : $(TSDOBJ) $(CMNOBJ)
	ld -G -z text -o $@ $(TSDOBJ) $(CMNOBJ) $(LDFLAGS) 

tcctime.so : $(TIMEOBJ)
	ld -G -z text -o $@ $(TIMEOBJ) $(LDFLAGS) 

slaext.so : $(SLAOBJ)
	ld -G -z text -o $@ $(SLAOBJ) $(LDFLAGS) 

tcctelsim.so : $(TELSIMOBJ)
	ld -G -z text -o $@ $(TELSIMOBJ) $(LDFLAGS)

tsdext.o : tccAst.h
tccext.o : tccAst.h autorepeat.h plosc.h
tccext.o : edit.xbm delete.xbm rotate.xbm arotate.xbm hflip.xbm vflip.xbm \
            reset.xbm
tccUtil.o : tccConstants.h tccUtil.h
tccTime.o : tccConstants.h
tccAst.o : tccConstants.h tccUtil.h tccAst.h tccDecode.h
slaext.o: tccDecode.h
tsdDomeview.o : tccConstants.h tccDecode.h tccUtil.h
tsdWrapview.o : tccConstants.h tccDecode.h tccUtil.h
tsdElview.o : tccConstants.h tccDecode.h tccUtil.h
plosc.o : plosc.h
tccTelSim.o : tccConstants.h tccDecode.h tccUtil.h

clean:
	-rm $(TCCOBJ) $(TSDOBJ) $(CMNOBJ) $(SLAOBJ) $(TELSIMOBJ) $(TIMEOBJ)
	-rm tccext.so tsdext.so tcctime.so slaext.so tcctelsim.so

install: All
	mkdir -p ../tcclib/solaris
	cp tccext.so ../tcclib/solaris
	cp tcctelsim.so ../tcclib/solaris
	mkdir -p ../tsdlib/solaris
	cp tsdext.so ../tsdlib/solaris
	mkdir -p ../lib/solaris
	cp tcctime.so ../lib/solaris
	cp slaext.so ../lib/solaris
