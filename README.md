# tcc

Gemini Telescope Control Console — Tcl/Tk operator GUIs for the TCS
(`tcc`, `tsd`, `pointtest`, `probecal`). App usage: see
[tccApp/README.TXT](tccApp/README.TXT).

## Build & CI

Built as an RPM by the shared [gemini-rtsw-ci](https://github.com/gemini-rtsw/gemini-rtsw-ci)
pipeline (submodule). Every push to `main` builds the RPM, publishes it to the
shared `rpm-repo` on GHCR, and pushes a dev image to
`ghcr.io/gemini-rtsw/tcc:el9-latest-devel`.

**tcc is EL9-only.** That is defined solely by the matrix in
[.github/workflows/ci.yml](.github/workflows/ci.yml) (`el: ['9']`); add `'8'`
there if an EL8 build is ever needed. The shared CI scripts default to EL8, so
**always pass `--el 9` locally**:

```bash
./gemini-rtsw-ci/build_rpm.sh --el 9          # build RPM -> rpms/
./gemini-rtsw-ci/build_docker.sh --el 9       # build dev image
./gemini-rtsw-ci/dev_environment.sh --el 9    # start dev container
```

(Requires Docker and a GHCR login — see the gemini-rtsw-ci README.)

## custom-repo-setup.sh

Runs automatically before dependency resolution in CI builds. tcc's GUI stack
depends on grandfathered i686 binaries (ocswish, itk, itcl, …) built against
the Gemini **Tcl/Tk 8.5** packages in rpm-repo; without this hook dnf prefers
Rocky/EPEL's Tcl/Tk 8.6 and resolution fails. Details in the script header —
read it before changing dependencies.
