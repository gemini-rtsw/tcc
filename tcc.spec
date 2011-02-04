%define _prefix __auto__
%define gemopt opt
%define name tcc
%define version __auto__
%define release __auto__
%define repository gemini

%define debug_package %{nil}

Summary: %{name} Package
Name: %{name}
Version: %{version}
Release: %{release}.%{dist}.%{repository}
License: GPL
## Source:%{name}-%{version}.tar.gz
Group: Gemini
Source0: %{name}-%{version}.tar.gz
BuildRoot: /var/tmp/%{name}-%{version}-root
BuildArch: %{arch}
Prefix: %{_prefix}
## You may specify dependencies here
# BuildRequires:
Requires: epics_module-timelib epics_module-slalib epics_module-astlib
Requires: ocswish seqexec tcldom-libxml2 skycalc
#
# FIXME: Need 2.5.1 b/c the api's changed for 3.x.
#
Requires: tcldom-libxml2 = 2.5.1
## Switch dependency checking off
# AutoReqProv: no

%description
This is a default description for the %{name} package

## If you want to have a devel-package to be generated uncomment the following:
# %package devel
# Summary: %{name}-devel Package
# Group: Development/Gemini
# Requires: %{name}
# %description devel
# This is a default description for the %{name}-devel package

## Of course, you also can create additional packages, e.g for "doc". Just
## follow the same way as I did with "%package devel".

%prep
## Do some preparation stuff, e.g. unpacking the source with
%setup -n %{name}


%build
## Write build instructions here, e.g
# sh configure
cd tccApp
make -f Makefile.linux

%install
## Write install instructions here, e.g
mkdir -p  $RPM_BUILD_ROOT/%{_prefix}/bin
mkdir -p  $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/
cp -a tccApp/ca_config* $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/
cp -a tccApp/calparams* $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/
cp -a tccApp/options* $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/
cp -a tccApp/default* $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/
cp -a tccApp/lib $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/
cp -a tccApp/plugins $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/
cp -a tccApp/pointcats $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/
cp -a tccApp/pointlib $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/
cp -a tccApp/scriptlib $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/
cp -a tccApp/scripts $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/
cp -a tccApp/seqlib $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/
cp -a tccApp/series96 $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/
cp -a tccApp/tcclib $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/
cp -a tccApp/tsdlib $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/
cp -a tccApp/tcc $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/
cp -a tccApp/tsd $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/
cp -a tccApp/tccscript $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/
cp -a tccApp/tccSkycat $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/
cp -a tccApp/pointtest $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/
cp -a tccApp/probecal $RPM_BUILD_ROOT/%{_prefix}/%{gemopt}/tcc/
cp -a tccApp/linux-bin/* $RPM_BUILD_ROOT/%{_prefix}/bin/
chmod 755 $RPM_BUILD_ROOT/%{_prefix}/bin/*

## if you want to do something after installation uncomment the following
## and list the actions to perform:
%post
for i in tcc tc1; do
	LOGDIR=/usr/tmp/$i
	[ -d $LOGDIR  ] || mkdir $LOGDIR
	chown -R software $LOGDIR
	chgrp -R gemini $LOGDIR
	chmod -R 775 $LOGDIR
done
## actions, e.g. /sbin/ldconfig

## If you want to have a devel-package to be generated and do some
## %post-stuff regarding it uncomment the following:
# %post devel

## if you want to do something after uninstallation uncomment the following
## and list the actions to perform. But be aware of e.g. deleting directories,
## see the example below how to do it:
# %postun
## if [ "$1" = "0" ]; then
##	rm -rf %{_prefix}/zzz
## fi

## If you want to have a devel-package to be generated and do some
## %postun-stuff regarding it uncomment the following:
# %postun devel

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
# %files devel
# %defattr(-,root,root)
## list files that are installed by the devel package here, e.g
## %{_prefix}/zzz/zzz


%changelog
## Write changes here, e.g.
# * Thu Dec 6 2007 John Doe <jdoe@gemini.edu> VERSION-RELEASE
# - change made
# - other change made
