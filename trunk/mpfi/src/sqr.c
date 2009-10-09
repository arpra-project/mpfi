/* sqr.c -- Square of an interval.

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
mpfi_sqr (mpfi_ptr a, mpfi_srcptr u)
{
  mpfr_t t1;
  int inexact_left, inexact_right, inexact=0;

  if ( MPFI_NAN_P (u) ) {
    mpfr_set_nan (&(a->left));
    mpfr_set_nan (&(a->right));
    MPFR_RET_NAN;
  }
  if (mpfr_sgn (&(u->left)) >= 0) {
      /* u nonnegative */
      inexact_left = mpfr_mul (&(a->left), &(u->left), &(u->left), MPFI_RNDD);
      inexact_right = mpfr_mul (&(a->right), &(u->right), &(u->right), MPFI_RNDU);
  }
  else {
    if (mpfr_sgn (&(u->right)) <= 0) {
      /* u non-positive -> beware the case where a = u */
      mpfr_init2 (t1, mpfi_get_prec (a));
      inexact_right = mpfr_mul (t1, &(u->left), &(u->left), MPFI_RNDU);
      inexact_left = mpfr_mul (&(a->left), &(u->right), &(u->right), MPFI_RNDD);
      inexact_right |= mpfr_set (&(a->right), t1, MPFI_RNDU);
      mpfr_clear (t1);
    }
    else {
      /* inf = 0, sup = max of the squares of the endpoints of u */
      mpfr_init2 (t1, mpfi_get_prec (u));
      inexact_right = mpfr_abs (t1, &(u->left), MPFI_RNDU);
      if (mpfr_cmp (t1, &(u->right)) <= 0) {
        inexact_right = mpfr_mul (&(a->right), &(u->right), &(u->right), MPFI_RNDU);
      }
      else {
        inexact_right = mpfr_mul (&(a->right), &(u->left), &(u->left), MPFI_RNDU);
      }
      mpfr_clear (t1);
      inexact_left = mpfr_set_si (&(a->left), (long)0, GMP_RNDZ);
    }
  }

  /* The NaN case has already been handled */
  if (inexact_left)
    inexact += 1;
  if (inexact_right)
    inexact += 2;

  if (mpfi_revert_if_needed (a)) {
    WARNING_REVERTED_ENDPOINTS (a, "mpfi_sqr");
    inexact = MPFI_REVERT_INEXACT_FLAGS (inexact);
  }

  return inexact;
}
