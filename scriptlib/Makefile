all: tclIndex

clean:
	@rm -f tclIndex

tclIndex : *.*tcl
	@for f in $?; do echo "modified: $$f"; done
	@echo "package require Itcl;auto_mkindex . *.*tcl;exit" | /gemini/ocs/ocs/bin/solaris/ocswish
	@echo "build done: tclIndex"
