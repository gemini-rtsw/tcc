all: tclIndex

clean:
	@rm -f tclIndex

tclIndex : *.*tcl *.*tk
	@for f in $?; do echo "modified: $$f"; done
	@echo "package require Itcl;auto_mkindex . *.*tcl *.*tk;exit" | ${HLPG_INSTALL_BASE}/bin/solaris/ocssh
	@echo "build done: tclIndex"
