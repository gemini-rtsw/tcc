All: tccext.so tsdext.so tcctime.so slaext.so

CMNOBJ = tccDecode.o tccUtil.o
TCCOBJ = tccext.o tccPreview.o autorepeat.o
TSDOBJ = tsdext.o tsdDomeView.o tsdWrapView.o tsdElView.o tccAst.o
TIMEOBJ = tccTime.o
SLAOBJ = slaext.o

CC = /opt/SUNWspro/bin/cc 
CFLAGS = -K pic -Xc -G \
         -I/gemini/ocs/external/include \
         -I$(HOME)/gemini/slalib/rel/include \
         -I$(HOME)/gemini/timelib/rel/include \
         -I$(HOME)/gemini/astlib/rel/include

#CC = gcc
#CFLAGS =  -fPIC -G \
#         -I/gemini/ocs/external/include \
#         -I$(HOME)/gemini/slalib/rel/include \
#         -I$(HOME)/gemini/timelib/rel/include \
#         -I$(HOME)/gemini/astlib/rel/include

LDFLAGS = -L$(HOME)/gemini/slalib/rel/lib/solaris \
          -L$(HOME)/gemini/timelib/rel/lib/solaris \
          -L$(HOME)/gemini/astlib/rel/lib/solaris \
          -last -ltime -lsla -lposix4

tccext.so : $(TCCOBJ) $(CMNOBJ)
	ld -G -z text -o $@ $(TCCOBJ) $(CMNOBJ) $(LDFLAGS) 

tsdext.so : $(TSDOBJ) $(CMNOBJ)
	ld -G -z text -o $@ $(TSDOBJ) $(CMNOBJ) $(LDFLAGS) 

tcctime.so : $(TIMEOBJ)
	ld -G -z text -o $@ $(TIMEOBJ) $(LDFLAGS) 

slaext.so : $(SLAOBJ)
	ld -G -z text -o $@ $(SLAOBJ) $(LDFLAGS) 

tsdext.o : tccTime.h tccAst.h
tccext.o : tccPreview.h tccTime.h autorepeat.h
tccext.o : edit.xbm delete.xbm
tccPreview.o : tccConstants.h tccUtil.h tccDecode.h
tccUtil.o : tccConstants.h tccUtil.h
tccTime.o : tccConstants.h
tccAst.o : tccConstants.h tccUtil.h
tsdDomeview.o : tccConstants.h tccDecode.h tccUtil.h
tsdWrapview.o : tccConstants.h tccDecode.h tccUtil.h
tsdElview.o : tccConstants.h tccDecode.h tccUtil.h

clean:
	-rm $(TCCOBJ) $(TSDOBJ) $(CMNOBJ) $(SLAOBJ)
	-rm tccext.so

install: All
	cp tccext.so ../tcclib/solaris
	cp tsdext.so ../tsdlib/solaris
	cp tcctime.so ../lib/solaris
	cp slaext.so ../lib/solaris
