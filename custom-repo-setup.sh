#!/bin/bash
set -e
#
# WHY THIS FILE EXISTS (read before deleting!)
#
# All of tcc's dependencies ARE in the shared rpm-repo — this script is not
# about missing packages. It exists because the CI container also has Rocky's
# repos (baseos/appstream/CRB) and EPEL enabled, and dnf always picks the
# HIGHEST version of a package name across all repos:
#
#   - Rocky ships Tcl/Tk 8.6 (tcl in baseos, tk in appstream, i686 pair in
#     CRB). Our grandfathered itcl/itk/skycat/python2.7 i686 binaries hard-
#     require the 8.5 ABI, served by rpm-repo as the Gemini tcl/tk 8.5 builds.
#     Without the excludes, dnf installs 8.6 and the install fails
#     ("nothing provides tcl(abi) = 8.5" / file conflicts).
#   - EPEL's tcllib and CRB's tix are built against 8.6; the Gemini builds
#     (tcllib-1.14, tix-8.4.3-12) in rpm-repo are the 8.5-compatible ones.
#
# Production hosts hit the same collision (dnf --resolve on hbftelops-ld3
# fails with "tcl-8.5.13 does not belong to a distupgrade repository"); they
# work because the stack is already installed there. This script encodes the
# same "Gemini 8.5 stack wins" rule for the throwaway CI containers only.
#
# It is run automatically by gemini-rtsw-ci (build_rpm.sh and the Dockerfile)
# before dependency resolution, if present in the repo root.
#
dnf config-manager --save \
    --setopt=excludepkgs="tcl-8.6* tcl-1:8.6* tcl-devel-8.6* tcl-devel-1:8.6* tk-8.6* tk-1:8.6* tk-devel-8.6* tk-devel-1:8.6*" \
    --setopt=epel.exclude="tcllib" \
    --setopt=crb.exclude="tix"
dnf install -y tcl-1:8.5.13-8.el9.i686 tk-1:8.5.13-6.el9.i686
