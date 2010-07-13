/* mul_q.c -- Multiply an interval and a rational number.

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
mpfi_mul_q (mpfi_ptr a, mpfi_srcptr b, mpq_srcptr c)
{
  mpfi_t tmp;
  int inexact_set, inexact_mul, inexact=0;

  mpfi_init2 (tmp, mpfi_get_prec (a));
  inexact_set = mpfi_set_q (tmp, c);
  inexact_mul = mpfi_mul (a, b, tmp);
  MPFI_CLEAR (tmp);

  if (MPFI_NAN_P (a))
    MPFR_RET_NAN;

  if (mpfr_inf_p (&(a->left))  && MPFI_LEFT_IS_INEXACT (inexact_mul))   /* overflow */
    inexact += 1;
  if (mpfr_inf_p (&(a->right)) && MPFI_RIGHT_IS_INEXACT (inexact_mul))  /* overflow */
    inexact += 2;
  if (mpfi_bounded_p (a)) {
    if (inexact_set) /* if the conversion of c into a mpfi is inexact,
                        then so are both endpoints of the result.      */
      inexact = MPFI_FLAGS_BOTH_ENDPOINTS_INEXACT;
    else
      inexact = inexact_mul;
  }

  return inexact;
}
