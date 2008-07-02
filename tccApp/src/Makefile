All: tcccmn.so tccext.so tsdext.so tcctime.so slaext.so tcctelsim.so

TCL_VERS = 8.4

CC = gcc

include ${HLPG_INSTALL_BASE}/conf/gemini_env

CMNOBJ = tccDecode.o tccUtil.o tccAst.o
TCCOBJ = tccext.o autorepeat.o plosc.o 
TSDOBJ = tsdext.o tsdDomeView.o tsdWrapView.o tsdElView.o
TIMEOBJ = tccTime.o
SLAOBJ = slaext.o
TELSIMOBJ = tccTelSim.o

CFLAGS = -fPIC -Wall -G -DUSE_TCL_STUBS -DUSE_TK_STUBS \
         -I${HLPG_EXT_RUNTIME_BASE}/tcl$(TCL_VERS)/include \
         -I${EPICS}/modules/slalib/include \
         -I${EPICS}/modules/timelib/include \
         -I${EPICS}/modules/astlib/include

LDFLAGS = -L${EPICS}/modules/slalib/lib/${EPICS_HOST_ARCH} \
          -L${EPICS}/modules/timelib/lib/${EPICS_HOST_ARCH} \
          -L${EPICS}/modules/astlib/lib/${EPICS_HOST_ARCH} \
          -L${HLPG_EXT_RUNTIME_BASE}/tcl$(TCL_VERS)/lib \
          -ltclstub$(TCL_VERS) -ltkstub$(TCL_VERS) -last -ltime -lsla #-lposix4

tcccmn.so : $(CMNOBJ)
	ld -G -z text -o $@ $(CMNOBJ) $(LDFLAGS) 

tccext.so : $(TCCOBJ) 
	ld -G -z text -o $@ $(TCCOBJ) $(LDFLAGS) tcccmn.so \
              -R `pwd`/../lib/solaris

tsdext.so : $(TSDOBJ)
	ld -G -z text -o $@ $(TSDOBJ) $(CMNOBJ) $(LDFLAGS) tcccmn.so \
              -R `pwd`/../lib/solaris

tcctime.so : $(TIMEOBJ)
	ld -G -z text -o $@ $(TIMEOBJ) $(LDFLAGS) 

slaext.so : $(SLAOBJ)
	ld -G -z text -o $@ $(SLAOBJ) $(LDFLAGS) tcccmn.so \
              -R `pwd`/../lib/solaris

tcctelsim.so : $(TELSIMOBJ)
	ld -G -z text -o $@ $(TELSIMOBJ) $(LDFLAGS)

tsdext.o : tccAst.h
tccext.o : tccAst.h autorepeat.h plosc.h
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
	-rm tcccmn.so tccext.so tsdext.so tcctime.so slaext.so tcctelsim.so

install: All
	mkdir -p ../tcclib/${EPICS_HOST_ARCH}
	cp tccext.so ../tcclib/${EPICS_HOST_ARCH}
	cp tcctelsim.so ../tcclib/${EPICS_HOST_ARCH}
	mkdir -p ../tsdlib/${EPICS_HOST_ARCH}
	cp tsdext.so ../tsdlib/${EPICS_HOST_ARCH}
	mkdir -p ../lib/${EPICS_HOST_ARCH}
	cp tcctime.so ../lib/${EPICS_HOST_ARCH}
	cp slaext.so ../lib/${EPICS_HOST_ARCH}
	cp tcccmn.so ../lib/${EPICS_HOST_ARCH}
