/* put_d.c -- Convex hull of an interval and a double.

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
mpfi_put_d (mpfi_ptr a, const double b)
{
  mpfi_t tmp;
  int inexact_set = 0;
  int inexact_left = 0;
  int inexact_right = 0;
  int inexact = 0;

  if ( MPFI_NAN_P (a) )
    MPFR_RET_NAN;

  mpfi_init2 (tmp, mpfi_get_prec (a));
  inexact_set = mpfi_set_d (tmp, b);

  if ( MPFI_NAN_P (tmp) ) {
    mpfr_set_nan (&(a->left));
    mpfr_set_nan (&(a->right));
  }
  else {
    if (mpfr_cmp (&(a->left), &(tmp->left)) > 0 ) {
      inexact_left = mpfr_set (&(a->left), &(tmp->left), MPFI_RNDD);

      if ( MPFI_LEFT_IS_INEXACT (inexact_set) )
        inexact_left = 1;

      /* do not allow -0 as lower bound */
      if (mpfr_zero_p (&(a->left)) && mpfr_signbit (&(a->left))) {
        mpfr_neg (&(a->left), &(a->left), MPFI_RNDU);
      }
    }

    if (mpfr_cmp (&(a->right), &(tmp->right)) < 0 ) {
      inexact_right = mpfr_set (&(a->right), &(tmp->right), MPFI_RNDU);

      if ( MPFI_RIGHT_IS_INEXACT (inexact_set) )
        inexact_right = 1;

      /* do not allow +0 as upper bound */
      if (mpfr_zero_p (&(a->right)) && !mpfr_signbit (&(a->right))) {
        mpfr_neg (&(a->right), &(a->right), MPFI_RNDD);
      }
    }
  }
  MPFI_CLEAR (tmp);

  if ( MPFI_NAN_P (a) )
    MPFR_RET_NAN;

  if (inexact_left)
    inexact += 1;
  if (inexact_right)
    inexact += 2;

  if (mpfi_revert_if_needed (a)) {
    WARNING_REVERTED_ENDPOINTS (a, "mpfi_put_d");
    inexact = MPFI_REVERT_INEXACT_FLAGS (inexact);
  }

  return inexact;
}
