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

#ident "$Id: sys_oim.c 1.1 Fri, 05 Jan 2001 12:45:19 +1100 kaos $"

#include <stdlib.h>
#include <errno.h>

#include "module.h"

/* Kernel headers before 2.1.mumble need this on the Alpha to get
   _syscall* defined.  */
#define __LIBRARY__

#include <asm/unistd.h>


/*======================================================================*/

#define __NR_old_sys_init_module  __NR_init_module
_syscall5(int, old_sys_init_module, const char *, name, char *, code,
	  unsigned, codesize, struct old_mod_routines *, routines,
	  struct old_symbol_table *, symtab)
