/* add_d.c -- Interval translation.

Copyright 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2010,
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
the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
MA 02110-1301, USA. */

#include "mpfi-impl.h"

int
mpfi_add_d (mpfi_ptr a, mpfi_srcptr b, const double c)
{
  mpfi_t tmp;
  int inexact_set, inexact_add, inexact = 0;

  mpfi_init2 (tmp, 64); /* 64 for IA86-FPU87 issues */
  inexact_set = mpfi_set_d (tmp, c);
  inexact_add = mpfi_add (a, b, tmp);
  MPFI_CLEAR (tmp);

  if (MPFI_NAN_P (a))
    MPFR_RET_NAN;

  if (MPFI_LEFT_IS_INEXACT (inexact_add)
      || (inexact_set && !mpfr_inf_p (&a->left))) {
    inexact += 1;
  }
  if (MPFI_RIGHT_IS_INEXACT (inexact_add)
      || (inexact_set && !mpfr_inf_p (&a->right))) {
    inexact += 2;
  }

  return inexact;
}
