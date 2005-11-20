/* mpfi_io.h -- Header for mpfi_io.c.

Copyright 1999, 2000, 2001, 2002, 2003, 2004, 2005, 
                     Spaces project, Inria Lorraine
                     and Salsa project, INRIA Rocquencourt,
                     and Arenaire project, Inria Rhone-Alpes, France
                     and Lab. ANO, USTL (Univ. of Lille),  France


This file is part of the MPFI Library.

The MPFI Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The MPFI Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the MPFI Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA 02111-1307, USA. */


#ifndef _mpfi_io_h
#define _mpfi_io_h

#include "mpfi.h"
#include <stdio.h>
#include <string.h> 
#include <ctype.h> 

/*#define isblank isspace*/
#define MPFI_ISSPACE isspace

size_t mpfi_out_str(FILE *stream, int base, size_t n_digits, mpfi_srcptr op);
size_t mpfi_inp_str(mpfi_ptr x,FILE *stream,int base);

int   mpfi_set_str    (mpfi_ptr, char *s, int base);

void mpfi_print_binary(mpfi_srcptr);
#endif
