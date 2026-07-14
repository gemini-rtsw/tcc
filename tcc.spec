%define debug_package %{nil}
%define _build_id_links none

# Define version and release
%define name tcc
%define version 1.0
%define release 3
# Short git hash of the built commit; exported by gemini-rtsw-ci/build_rpm.sh
# into the build container (same pattern as tcslib/gemUtil).
%define checkout %(if [ -n "$GIT_HASH" ]; then echo "$GIT_HASH"; else git rev-parse --short HEAD 2>/dev/null || echo nogit; fi)
%define gemopt opt
%define repository gemini
%define _prefix /gemsoft

# This hack is needed for our build system that creates 
# a file with the literal name containing %{pkg_name}
%define pkg_name %{name}
%define pkg_version %{version}

Summary: TCC Package
Name: %{name}
Version: %{version}
Release: %{release}.git.%{checkout}%{?dist}.%{repository}
License: GPL
## Source:%{name}-%{auto_version}.tar.gz
Group: Gemini
Source0: %{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-buildroot
BuildArch: x86_64
Prefix: %{_prefix}

## You may specify dependencies here
# The C extensions are built 32-bit (-m32, they load into ocswish i686), so
# the build needs the 32-bit toolchain. custom-repo-setup.sh anchors the
# older-than-best i686 providers before dependency resolution.
BuildRequires: gcc gcc-c++ make
BuildRequires: glibc-devel(x86-32)
BuildRequires: tcl-devel(x86-32) tk-devel(x86-32) itcl-devel(x86-32)
BuildRequires: epics_module-astlib(x86-32) epics_module-slalib(x86-32) epics_module-timelib(x86-32)
BuildRequires: tcl-devel tk-devel itcl-devel
BuildRequires: epics_module-astlib-devel epics_module-slalib-devel epics_module-timelib-devel
# src/Makefile.linux includes ${HLPG_INSTALL_BASE}/conf/gemini_env, shipped
# by ocswish.
BuildRequires: ocswish
Requires: tcl tk itcl itk iwidgets
Requires: ocswish seqexec
Requires: tcldom-libxml2
Requires: epics_module-timelib epics_module-slalib epics_module-astlib


## Switch dependency checking off
AutoReqProv: no

%description
This is a default description for the %{name} package

## If you want to have a devel-package to be generated uncomment the following:
%package devel
Summary: %{name}-devel Package
Group: Development/Gemini
Requires: %{name}
# Toolchain for building the tcc C extensions (tcctime.so etc.) in the dev
# container. They are 32-bit (-m32) because they load into ocswish (i686):
#   cd tccApp && EPICS_HOST_ARCH=linux-x86 make -f Makefile.linux install
# Only the dev image installs tcc-devel — ops never sees these.
Requires: gcc gcc-c++ make
Requires: glibc-devel(x86-32)
Requires: tcl-devel(x86-32) tk-devel(x86-32) itcl-devel(x86-32)
Requires: epics_module-astlib-devel epics_module-slalib-devel epics_module-timelib-devel
Requires: epics_module-astlib(x86-32) epics_module-slalib(x86-32) epics_module-timelib(x86-32)
%description devel
This is a default description for the %{name}-devel package

## Of course, you also can create additional packages, e.g for "doc". Just
## follow the same way as I did with "%package devel".

%prep
## Do some preparation stuff, e.g. unpacking the source with
%setup -q -n %{name}-%{version}


%build
# Build the six Tcl C extensions (tcctime.so etc.) exactly as the dev
# container does. Production build hosts have this environment implicitly;
# here it must be explicit: EPICS_HOST_ARCH selects the 32-bit EPICS libs,
# HOST_ARCH names the install subdir (lib/Linux) that the tcc launcher
# loads from. A build failure MUST fail the RPM -- a silent '|| true' here
# shipped hollow RPMs for years.
# rpmbuild runs a non-login shell, so the env the profile.d scripts provide
# on hosts/dev containers must be set explicitly here.
cd tccApp
export EPICS=/gemsoft/opt/epics
export EPICS_HOST_ARCH=linux-x86
export HOST_ARCH=Linux
export HLPG_INSTALL_BASE=/gemsoft/opt/ocswish
make -f Makefile.linux

%install
## Write install instructions here, e.g
rm -rf $RPM_BUILD_ROOT
mkdir -p  $RPM_BUILD_ROOT/%{_prefix}/bin
mkdir -p  $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/

# Create missing directories if they don't exist
for dir in ca_config ca_config.tc1 lib plugins pointcats pointlib scriptlib scripts seqlib series96 tcclib tsdlib; do
    mkdir -p $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/$dir
done

# Create linux-bin directory if it doesn't exist
mkdir -p tccApp/linux-bin

# Copy files from tccApp directory
[ -d tccApp/ca_config ] && cp -a tccApp/ca_config/* $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/ca_config/ || :
[ -d tccApp/ca_config.tc1 ] && cp -a tccApp/ca_config.tc1/* $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/ca_config.tc1/ || :
# NOTE: not `[ -f glob ] && cp` -- test -f is false whenever the glob matches
# more than one file, which silently dropped options.MK/options.CP and the
# default* files from the package.
cp -a tccApp/calparams* $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/ 2>/dev/null || :
cp -a tccApp/options* $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/ 2>/dev/null || :
cp -a tccApp/default* $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/ 2>/dev/null || :
[ -d tccApp/lib ] && cp -a tccApp/lib/* $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/lib/ || :
[ -d tccApp/plugins ] && cp -a tccApp/plugins/* $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/plugins/ || :
[ -d tccApp/pointcats ] && cp -a tccApp/pointcats/* $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/pointcats/ || :
[ -d tccApp/pointlib ] && cp -a tccApp/pointlib/* $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/pointlib/ || :
[ -d tccApp/scriptlib ] && cp -a tccApp/scriptlib/* $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/scriptlib/ || :
[ -d tccApp/scripts ] && cp -a tccApp/scripts/* $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/scripts/ || :
[ -d tccApp/seqlib ] && cp -a tccApp/seqlib/* $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/seqlib/ || :
[ -d tccApp/series96 ] && cp -a tccApp/series96/* $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/series96/ || :
[ -d tccApp/tcclib ] && cp -a tccApp/tcclib/* $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/tcclib/ || :
[ -d tccApp/tsdlib ] && cp -a tccApp/tsdlib/* $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/tsdlib/ || :

# Copy executable scripts to tcc directory
[ -f tccApp/tcc ] && cp -a tccApp/tcc $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/ || :
[ -f tccApp/tsd ] && cp -a tccApp/tsd $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/ || :
[ -f tccApp/tccscript ] && cp -a tccApp/tccscript $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/ || :
[ -f tccApp/tccSkycat ] && cp -a tccApp/tccSkycat $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/ || :
[ -f tccApp/pointtest ] && cp -a tccApp/pointtest $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/ || :
[ -f tccApp/probecal ] && cp -a tccApp/probecal $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/ || :

# Seed the runtime configuration (/gemsoft/etc/tcc) from the committed
# gnconfig snapshot (test-config/, see its README). Packaged in the DEVEL
# subpackage only (%config(noreplace)): dev containers get a working config,
# while the regular tcc package never touches /gemsoft/etc — that tree is
# site-managed (gnconfig) in operations.
mkdir -p $RPM_BUILD_ROOT/%{_prefix}/etc/tcc
cp -a test-config/defaults test-config/calparams test-config/pointtests $RPM_BUILD_ROOT/%{_prefix}/etc/tcc/

# Copy binaries to bin directory if linux-bin exists and has files
if [ -d tccApp/linux-bin ] && [ "$(ls -A tccApp/linux-bin 2>/dev/null)" ]; then
    cp -a tccApp/linux-bin/* $RPM_BUILD_ROOT/%{_prefix}/bin/ || :
    chmod 755 $RPM_BUILD_ROOT/%{_prefix}/bin/* || :
fi

# Install each launcher wrapper under its bare name as well (tcc, tsd, ...).
# The old spec touch'd EMPTY placeholder files here "to satisfy package
# requirements", shipping a /gemsoft/bin/tcc that did nothing.
for bin in tcc tsd tccscript tccSkycat pointtest probecal; do
    if [ -f tccApp/linux-bin/$bin.sh ]; then
        install -m 755 tccApp/linux-bin/$bin.sh $RPM_BUILD_ROOT/%{_prefix}/bin/$bin
    fi
done

## if you want to do something after installation uncomment the following
## and list the actions to perform:
%post
for i in tcc tc1; do
	LOGDIR=/usr/tmp/$i
	[ -d $LOGDIR  ] || mkdir -p $LOGDIR
	# Commented out user/group assignment that might fail in some environments
	# chown -R software $LOGDIR
	# chgrp -R gemini $LOGDIR
	chmod -R 775 $LOGDIR
done
## actions, e.g. /sbin/ldconfig

## If you want to have a devel-package to be generated and do some
## %post-stuff regarding it uncomment the following:
%post devel
# GEMINI_SITE comes from the file /gemsoft/etc/site (login shells run
# `export GEMINI_SITE=$(cat ...)`), which gemini-setup seeds with NOSITE.
# Default dev containers to MK so the tcc launcher finds options.MK without
# manual steps; only ever touch the placeholder value, never a real site.
if [ ! -r /gemsoft/etc/site ] || grep -qx NOSITE /gemsoft/etc/site 2>/dev/null; then
    echo MK > /gemsoft/etc/site
fi

## if you want to do something after uninstallation uncomment the following
## and list the actions to perform. But be aware of e.g. deleting directories,
## see the example below how to do it:
# %postun
## if [ "$1" = "0" ]; then
##	rm -rf %{_prefix}/zzz
## fi

## If you want to have a devel-package to be generated and do some
## %postun-stuff regarding it uncomment the following:
%postun devel

## Its similar for %pre, %preun, %pre devel, %preun devel.

%clean
## Usually you won't do much more here than
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
## list files that are installed here, e.g
## %{_prefix}/zzz/zzz
%{_prefix}/%{gemopt}/tcc/
%{_prefix}/bin/*

## If you want to have a devel-package to be generated uncomment the following
%files devel
%defattr(-,root,root)
# Seed runtime config for dev containers only; ops manages the real
# /gemsoft/etc/tcc via gnconfig and never installs tcc-devel.
%config(noreplace) %{_prefix}/etc/tcc/


%changelog
* Thu Jul 09 2026 Hawi Stecher <hawi.stecher@noirlab.edu> 1.0-3
- Install real launcher wrappers as /gemsoft/bin/{tcc,tsd,...} (were empty
  placeholder files)

* Wed Jul 08 2026 Hawi Stecher <hawi.stecher@noirlab.edu> 1.0-2
- REL-4962: log target field and observation ID on Set/Slew

## Write changes here, e.g.
# * Thu Dec 6 2007 John Doe <jdoe@gemini.edu> VERSION-RELEASE
# - change made
# - other change made
