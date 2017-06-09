/*
 * Copyright 2017 Stefan Dösinger for CodeWeavers
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

/* NOTE: The guest side uses mingw's headers. The host side uses Wine's headers. */

#include <windows.h>
#include <stdio.h>

#include "windows-user-services.h"
#include "dll_list.h"
#include "msvcrt.h"

#ifndef QEMU_DLL_GUEST
#include <wine/debug.h>
WINE_DEFAULT_DEBUG_CHANNEL(qemu_msvcrt);
#else

/* FIXME: This needs DllMain support to grab the pointer from the host. */
char **MSVCRT___initenv = NULL;

#endif

struct qemu___getmainargs
{
    struct qemu_syscall super;
    uint64_t argc, argv, envp;
    uint64_t expand_wildcards;
    uint64_t new_mode;
};


#ifdef QEMU_DLL_GUEST

void CDECL __getmainargs(int *argc, char** *argv, char** *envp, int expand_wildcards,
        int *new_mode)
{
    struct qemu___getmainargs call;
    call.super.id = QEMU_SYSCALL_ID(CALL___GETMAINARGS);
    call.argc = (uint64_t)argc;
    call.argv = (uint64_t)argv;
    call.envp = (uint64_t)envp;
    call.expand_wildcards = expand_wildcards;
    call.new_mode = (uint64_t)new_mode;

    qemu_syscall(&call.super);
}

#else

void qemu___getmainargs(struct qemu_syscall *call)
{
    struct qemu___getmainargs *c = (struct qemu___getmainargs *)call;
    /* This shouldn't forward, because we need to remove qemu and its args from
     * the arguments we pass to the guest file. Linux-user/main.c has some code
     * for that that can be used as a reference. */
    WINE_FIXME("\n");
    p___getmainargs(QEMU_G2H(c->argc), QEMU_G2H(c->argv), QEMU_G2H(c->envp),
            c->expand_wildcards, QEMU_G2H(c->new_mode));
}

#endif
