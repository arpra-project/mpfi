/* si_div.c -- Divide a signed long int by an interval.

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


#include "mpfi.h"
#include "mpfi-impl.h"

int
mpfi_si_div (mpfi_ptr a, const long b, mpfi_srcptr c)
{
  mpfi_t tmp;
  int inexact_div, inexact = 0;
  mp_prec_t tmpprec;

  tmpprec = mpfi_get_prec (a);
  if (sizeof(b) * 8 > tmpprec) tmpprec = sizeof(b) * 8;
  mpfi_init2 (tmp, tmpprec);
  mpfi_set_si (tmp, b); /* Exact */
  inexact_div = mpfi_div (a, tmp, c);
  MPFI_CLEAR (tmp);

  if (MPFI_NAN_P (a))
    MPFR_RET_NAN;

  if (mpfr_inf_p (&(a->left))  && MPFI_LEFT_IS_INEXACT (inexact_div))   /* overflow */
    inexact += 1;
  if (mpfr_inf_p (&(a->right)) && MPFI_RIGHT_IS_INEXACT (inexact_div))  /* overflow */
    inexact += 2;
  if (mpfi_bounded_p (a)) {
    inexact = inexact_div;
  }

  if (mpfi_revert_if_needed (a)) {
    /*
      fprintf(stderr, "Pb endpoints in reverse order in mpfi_si_div: ");
      mpfi_out_str(stderr, 10, 0, a);
      fprintf(stderr, "\n");
    */
    inexact = MPFI_REVERT_INEXACT_FLAGS (inexact);
  }

  return inexact;
}
