# Sourced by gemini-rtsw-ci/dev_environment.sh on the HOST; the exported
# variables are passed into the dev container.
#
# GEMINI_SITE selects the site config (options.MK / options.CP etc.) that the
# tcc launcher reads. In operations a site setup provides it; the dev
# container has none, so default to MK. Change to CP for Cerro Pachon work.
export GEMINI_SITE=MK

# Dev-container-only workaround; see devcontainer/readdir_lfs.c for the full
# story. Short version: the i686 Tcl 8.5 stack calls the non-LFS readdir(),
# which returns EOVERFLOW on the container's overlayfs-over-ext4, so Tcl's
# glob reports every directory empty and the tcc dies on "can't find package
# Itcl". Ops Rocky 9 consoles are XFS and never hit this.
#
# The shim ships in tcc-devel only (never in the tcc package, never referenced
# by tcc.sh or tccApp/tcc), and this file is never packaged and never read at
# runtime, so an operational console cannot pick either of them up.
#
# $LIB is expanded by the dynamic loader, NOT the shell -- keep it single
# quoted. It resolves to lib for i686 processes and lib64 for x86_64 ones, so
# both halves of the ocssh -> ocssh.exe chain preload a matching object.
export LD_PRELOAD='/gemsoft/opt/tcc-devel/$LIB/readdir_lfs.so'
