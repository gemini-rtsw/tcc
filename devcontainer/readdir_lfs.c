/*
 * readdir_lfs.c -- DEV CONTAINER ONLY. Never installed by the tcc package.
 *
 * The grandfathered i686 Tcl 8.5 stack (tcl, tk, itcl, itk, ocswish) is built
 * without _FILE_OFFSET_BITS=64, so it calls the 32-bit readdir(). glibc has to
 * squeeze the kernel's 64-bit d_off cookie into a 32-bit long, and fails with
 * EOVERFLOW whenever it does not fit. Tcl's glob treats that as "empty
 * directory", so the auto_path scan finds no pkgIndex.tcl and every
 * `package require` in tccApp/tcc dies with "can't find package Itcl".
 *
 * Whether it fits depends on the filesystem, not the OS:
 *
 *   - Ops Rocky 9 consoles are XFS. Its cookies are small, readdir() is happy,
 *     and the tcc has always worked there. Nothing in this file applies.
 *   - The dev container is overlayfs over ext4. ext4's htree hands back hashed
 *     cookies >= 2^31 for some directories, /usr/lib/tcl8.5 among them.
 *
 * Forwarding readdir() to readdir64() and truncating the cookie removes the
 * overflow. Truncation is safe for this stack: Tcl only ever scans a directory
 * forward and copies d_name immediately; it never telldir()/seekdir(), which
 * is the only thing d_off is good for.
 *
 * Built for both arches and preloaded via the loader's $LIB expansion, so a
 * 64-bit process picks up lib64/ and a 32-bit one lib/. Nothing needs
 * overriding on 64-bit -- struct dirent is already the large one -- so that
 * build is a deliberately empty stub, there purely so $LIB always resolves.
 */

#if defined(__i386__)

#define _GNU_SOURCE
#include <dirent.h>
#include <stddef.h>
#include <string.h>

struct dirent *readdir(DIR *dirp)
{
    /* Thread-local, mirroring glibc's per-stream buffer closely enough for
       callers that copy d_name before the next readdir() on the same DIR. */
    static __thread struct dirent out;
    struct dirent64 *e = readdir64(dirp);
    size_t len;

    if (e == NULL)
        return NULL;            /* end of stream, or errno already set */

    len = strlen(e->d_name);
    if (len >= sizeof out.d_name)
        len = sizeof out.d_name - 1;

    out.d_ino = (unsigned long) e->d_ino;   /* truncating: see above */
    out.d_off = (long) e->d_off;
    out.d_type = e->d_type;
    out.d_reclen = offsetof(struct dirent, d_name) + len + 1;
    memcpy(out.d_name, e->d_name, len);
    out.d_name[len] = '\0';

    return &out;
}

#endif /* __i386__ */
