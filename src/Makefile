All: tccext.so tsdext.so tcctime.so slaext.so

TCL_VERS = 8.2

CMNOBJ = tccDecode.o tccUtil.o tccAst.o
TCCOBJ = tccext.o tccPreview.o autorepeat.o
TSDOBJ = tsdext.o tsdDomeView.o tsdWrapView.o tsdElView.o
TIMEOBJ = tccTime.o
SLAOBJ = slaext.o

CC = /opt/SUNWspro/bin/cc 
CFLAGS = -K pic -Xc -G -DUSE_TCL_STUBS -DUSE_TK_STUBS -DUSE_OLD_CANVAS \
         -I${HLPG_INSTALL_BASE}/../packages/tcl$(TCL_VERS)/include \
         -I/gemini/slalib/include \
         -I/gemini/timelib/include \
         -I/gemini/astlib/include

#CC = gcc
#CFLAGS =  -fPIC -G -DUSE_TCL_STUBS -DUSE_TK_STUBS -DUSE_OLD_CANVAS \
#         -I/gemini/packages/tcl$(TCL_VERS)/include \
#         -I$(HOME)/gemini/slalib/rel/include \
#         -I$(HOME)/gemini/timelib/rel/include \
#         -I$(HOME)/gemini/astlib/rel/include

LDFLAGS = -L/gemini/slalib/lib/solaris \
          -L/gemini/timelib/lib/solaris \
          -L/gemini/astlib/lib/solaris \
          -L${HLPG_INSTALL_BASE}/../packages/tcl$(TCL_VERS)/lib \
          -ltclstub$(TCL_VERS) -ltkstub$(TCL_VERS) -last -ltime -lsla -lposix4

tccext.so : $(TCCOBJ) $(CMNOBJ)
	ld -G -z text -o $@ $(TCCOBJ) $(CMNOBJ) $(LDFLAGS) 

tsdext.so : $(TSDOBJ) $(CMNOBJ)
	ld -G -z text -o $@ $(TSDOBJ) $(CMNOBJ) $(LDFLAGS) 

tcctime.so : $(TIMEOBJ)
	ld -G -z text -o $@ $(TIMEOBJ) $(LDFLAGS) 

slaext.so : $(SLAOBJ)
	ld -G -z text -o $@ $(SLAOBJ) $(LDFLAGS) 

tsdext.o : tccAst.h
tccext.o : tccAst.h tccPreview.h autorepeat.h
tccPreview.o : tccPreview.h tccConstants.h tccUtil.h tccDecode.h
tccUtil.o : tccConstants.h tccUtil.h
tccTime.o : tccConstants.h
tccAst.o : tccConstants.h tccUtil.h tccAst.h
tsdDomeview.o : tccConstants.h tccDecode.h tccUtil.h
tsdWrapview.o : tccConstants.h tccDecode.h tccUtil.h
tsdElview.o : tccConstants.h tccDecode.h tccUtil.h

clean:
	-rm $(TCCOBJ) $(TSDOBJ) $(CMNOBJ) $(SLAOBJ)
	-rm tccext.so tsdext.so tcctime.so slaext.so

install: All
	mkdir -p ../tcclib/solaris
	cp tccext.so ../tcclib/solaris
	mkdir -p ../tsdlib/solaris
	cp tsdext.so ../tsdlib/solaris
	mkdir -p ../lib/solaris
	cp tcctime.so ../lib/solaris
	cp slaext.so ../lib/solaris
