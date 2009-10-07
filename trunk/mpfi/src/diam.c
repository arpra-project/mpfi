/* diam.c -- Various diameter functions.

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

/* Absolute diameter                            */
int
mpfi_diam_abs (mpfr_ptr diam, mpfi_srcptr interv)
{
  return (mpfr_sub (diam, &(interv->right), &(interv->left), GMP_RNDU));
}

/* Relative diameter                                     */
/* Always computes an overestimation                     */
/* return value: 0 if the result is exact, > 0 otherwise */
int
mpfi_diam_rel (mpfr_ptr diam, mpfi_srcptr interv)
{
  mpfr_t centre, tmp_diam;
  int inexact_sub, inexact_mid, inexact_neg, inexact_sub2=0, inexact=0;

  mpfr_init2 (tmp_diam, mpfr_get_prec (diam));

  inexact_sub = mpfr_sub (tmp_diam, &(interv->right), &(interv->left), GMP_RNDU);

  if (mpfi_bounded_p (interv)) {
    mpfr_init2 (centre, mpfr_get_prec (diam));
    inexact_mid = mpfi_mid (centre, interv);

    if (mpfr_cmp_ui (centre, 0) <0) {
      inexact_neg = mpfr_neg (centre, centre, GMP_RNDD);
      if ( (!inexact_neg) || (inexact_mid<0) )
        mpfr_sub_one_ulp (centre, GMP_RNDD);
    }

    if (mpfr_cmp_ui (centre,0))
      inexact = mpfr_div (diam, tmp_diam, centre, GMP_RNDU);

    mpfr_clear (centre);
  } /* if interv is bounded, then a relative diameter can be computed */

  mpfr_clear (tmp_diam);

  if ( mpfr_nan_p (diam) )
    MPFR_RET_NAN;

  if ( inexact || inexact_sub || inexact_sub2 )
    return 1;
  else
    return 0;
}

/* Diameter: relative if the interval does not contain 0 */
/* absolute otherwise                                    */
int
mpfi_diam (mpfr_ptr diam, mpfi_srcptr interv)
{

  if (!mpfi_has_zero (interv)) {
    return (mpfi_diam_rel (diam, interv));
  }
  else {
    return (mpfi_diam_abs (diam, interv));
  }
}
