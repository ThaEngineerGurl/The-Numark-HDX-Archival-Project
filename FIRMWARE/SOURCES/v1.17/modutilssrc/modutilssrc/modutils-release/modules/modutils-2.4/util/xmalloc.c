/* Misc utility functions.
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

#ident "$Id: xmalloc.c 1.3 Mon, 26 Feb 2001 15:16:48 +1100 kaos $"

#include <stdlib.h>
#include "util.h"


/*======================================================================*/

void *
xmalloc(size_t size)
{
  void *ptr = malloc(size);
  if (!ptr)
    {
      error("Out of memory");
      exit(1);
    }
  return ptr;
}
