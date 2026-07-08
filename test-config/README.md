# test-config

Snapshot of the tcc runtime configuration (`/gemsoft/etc/tcc`) for dev
containers, taken from gnconfig (gemini-north-config) commit 079e841,
2026-07-07: `defaults/`, `calparams/`, `pointtests/`.

The RPM installs these under /gemsoft/etc/tcc as %config(noreplace): a fresh
dev container gets a working configuration, while machines that already have
real (site-managed) config keep theirs untouched on install/upgrade.

Operations does NOT use this copy — production config is managed in gnconfig.
To refresh: pull gnconfig and re-copy the three directories.
