/* Functions for the Linux module syscall interface.
   Copyright 1996, 1997 Linux International.
   Contributed by Richard Henderson <rth@tamu.edu>

   This file is part of the Linux modutils.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#ident "$Id: sys_nim.c 1.2 Sun, 25 Mar 2001 21:13:48 +1000 kaos $"

#include <stdlib.h>
#include <errno.h>

#include "module.h"

/* Kernel headers before 2.1.mumble need this on the Alpha to get
   _syscall* defined.  */
#define __LIBRARY__

#include <asm/unistd.h>


/*======================================================================*/

#ifndef CONFIG_USE_SYSCALL

extern int init_module(const char *name, const struct module *info);

int
sys_init_module(const char *name, const struct module *info)
{
  return init_module(name, info);
}

#else

#define __NR_sys_init_module  __NR_init_module
_syscall2(int, sys_init_module, const char *, name,
	  const struct module *, info)

#endif
