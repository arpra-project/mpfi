/* q_sub.c -- Subtract an interval by an rational number.

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
the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
MA 02110-1301, USA. */


#include <stdio.h>
#include "mpfi.h"
#include "mpfi-impl.h"

int
mpfi_q_sub (mpfi_ptr a, mpq_srcptr b, mpfi_srcptr c)
{
  mpfi_t tmp;
  int inexact_set, inexact_sub, inexact=0;

  mpfi_init2 (tmp, mpfi_get_prec (a));
  inexact_set = mpfi_set_q (tmp, b);
  inexact_sub = mpfi_sub (a, tmp, c);
  MPFI_CLEAR (tmp);

  if (MPFI_NAN_P (a))
    MPFR_RET_NAN;

  if ( mpfr_inf_p (&(a->left)) ) {
    if  (MPFI_LEFT_IS_INEXACT (inexact_sub)) /* overflow */
      inexact += 1;
  }
  else if (MPFI_LEFT_IS_INEXACT (inexact_set) || MPFI_LEFT_IS_INEXACT (inexact_sub))
    inexact += 1;
  if ( mpfr_inf_p (&(a->right)) ) {
    if (MPFI_RIGHT_IS_INEXACT (inexact_sub) )  /* overflow */
      inexact += 2;
  }
  else if (MPFI_RIGHT_IS_INEXACT (inexact_set) || MPFI_RIGHT_IS_INEXACT (inexact_sub))
    inexact += 2;

  if (mpfi_revert_if_needed (a)) {
    WARNING_REVERTED_ENDPOINTS (a, "mpfi_q_sub");
    inexact = MPFI_REVERT_INEXACT_FLAGS (inexact);
  }

  return inexact;
}
