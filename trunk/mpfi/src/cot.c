/* cot.c -- Cotangent of an interval.

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
mpfi_cot (mpfi_ptr a, mpfi_srcptr b)
{
  int inexact_left, inexact_right, inexact=0;
  mpz_t z_left, z_right, z;

  if (MPFI_NAN_P (b)) {
    mpfr_set_nan (&(a->left));
    mpfr_set_nan (&(a->right));
    MPFR_RET_NAN;
  }

  if (MPFI_INF_P (b)) {
    /* the two endpoints are the same infinite */
    if ( mpfr_cmp (&(b->left), &(b->right)) == 0) {
      mpfr_set_nan (&(a->left));
      mpfr_set_nan (&(a->right));
      MPFR_RET_NAN;
    }
    mpfr_set_inf (&(a->left), -1);
    mpfr_set_inf (&(a->right), 1);
    return 0;
  }

  mpz_init (z_left);
  mpz_init (z_right);
  mpz_init (z);

  mpfi_quadrant (z_left, &(b->left));
  mpfi_quadrant (z_right, &(b->right));

  /* if there is at least one period in b or if b contains a k*Pi, */
  /* then a = ]-oo, +oo[ */
  mpz_sub (z, z_right, z_left);
  if ( (mpz_cmp_ui (z, 2) >= 0) ||
       (mpz_odd_p (z_left) && mpz_even_p (z_right)) ) {
    mpfr_set_inf (&(a->left), -1);
    mpfr_set_inf (&(a->right), 1);
    inexact = 0;
  }
  else { /* within one period, cot is decreasing */
    mp_prec_t prec;
    mpfr_t tmp;

    prec = mpfi_get_prec (a);
    mpfr_init2 (tmp, prec);
    inexact_left = mpfr_cot (tmp, &(b->right), GMP_RNDD);
    inexact_right = mpfr_cot (&(a->right), &(b->left), GMP_RNDU);
    inexact_left |= mpfr_set (&(a->left), tmp, GMP_RNDN);
    mpfr_clear (tmp);

    if (inexact_left) inexact += 1;
    if (inexact_right) inexact += 2;
  }

  mpz_clear (z_left);
  mpz_clear (z_right);
  mpz_clear (z);

  return inexact;
}
