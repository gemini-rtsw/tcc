# Sourced by gemini-rtsw-ci/dev_environment.sh on the HOST; the exported
# variables are passed into the dev container.
#
# GEMINI_SITE selects the site config (options.MK / options.CP etc.) that the
# tcc launcher reads. In operations a site setup provides it; the dev
# container has none, so default to MK. Change to CP for Cerro Pachon work.
export GEMINI_SITE=MK
