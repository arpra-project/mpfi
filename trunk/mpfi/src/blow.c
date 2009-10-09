/* blow.c -- Interval blow.

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

/* keeps the same center and multiplies the radius by (1+fact) */

int
mpfi_blow (mpfi_ptr y, mpfi_srcptr x, double fact)
/* if c = mid (x) and r = rad (x), y = [c - (1+fact)*r , c + (1+fact)*r] */
{
  mp_prec_t prec;
  mpfr_t radius, factor;
  mpfr_t centre;
  int inexact_diam, inexact_div, inexact_factor, inexact_rad, inexact_centre;
  int inexact_left, inexact_right, inexact=0;

  if ( MPFI_NAN_P (x) ) {
    mpfr_set_nan (&(y->left));
    mpfr_set_nan (&(y->right));
    MPFR_RET_NAN;
  }

  prec=mpfi_get_prec (x);
  mpfr_init2 (radius, prec);
  mpfr_init2 (factor, prec);
  mpfr_init2 (centre, prec);

  inexact_diam = mpfi_diam_abs (radius, x);
  inexact_div = mpfr_div_2exp (radius, radius, 1, GMP_RNDU);
  if (fact > 0.0)
    inexact_factor = mpfr_set_d (factor, (1.0+fact), GMP_RNDU);
  else
    inexact_factor = mpfr_set_d (factor, (1.0-fact), GMP_RNDU);
  inexact_rad = mpfr_mul (radius, radius, factor, GMP_RNDU);
  inexact_centre = mpfi_mid (centre, x);
  inexact_left = mpfr_sub (&(y->left), centre, radius, GMP_RNDD);
  inexact_right = mpfr_add (&(y->right), centre, radius, GMP_RNDU);

  mpfr_clear (radius);
  mpfr_clear (factor);
  mpfr_clear (centre);

  if ( MPFI_NAN_P (y) )
    MPFR_RET_NAN;

  if ( MPFI_LEFT_IS_INEXACT (inexact_diam)   || MPFI_LEFT_IS_INEXACT (inexact_div) ||
       MPFI_LEFT_IS_INEXACT (inexact_factor) || MPFI_LEFT_IS_INEXACT (inexact_rad) ||
       MPFI_LEFT_IS_INEXACT (inexact_centre) || inexact_left  )
    inexact += 1;
  if ( MPFI_RIGHT_IS_INEXACT (inexact_diam)   || MPFI_RIGHT_IS_INEXACT (inexact_div) ||
       MPFI_RIGHT_IS_INEXACT (inexact_factor) || MPFI_RIGHT_IS_INEXACT (inexact_rad) ||
       MPFI_RIGHT_IS_INEXACT (inexact_centre) || inexact_right  )
    inexact += 2;

  if (mpfi_revert_if_needed (y)) {
    WARNING_REVERTED_ENDPOINTS (a, "mpfi_blow");
    inexact = MPFI_REVERT_INEXACT_FLAGS (inexact);
  }

  return inexact;
}
