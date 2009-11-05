/* mpfi_csc.c -- Cosecant of an interval.

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
mpfi_csc (mpfi_ptr a, mpfi_srcptr b)
{
  int inexact_left, inexact_right, inexact;
  mp_prec_t prec, prec_left, prec_right;
  mpfr_t tmp;
  mpz_t z_left, z_right;
  mpz_t z, zmod4;
  int ql_mod4, qr_mod4;

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

  prec_left = mpfi_quadrant (z_left, &(b->left));
  prec_right = mpfi_quadrant (z_right, &(b->right));

  /* if there is at least one period in b or if b contains a k*Pi, */
  /* then a = ]-oo, +oo[ */
  mpz_sub (z, z_right, z_left);
  if ( (mpz_cmp_ui (z, 2) >= 0) ||
       (mpz_odd_p (z_left) && mpz_even_p (z_right)) ) {
    mpfr_set_inf (&(a->left), -1);
    mpfr_set_inf (&(a->right), 1);
    inexact = 0;
  }
  else {
    /* computing precision = maximal precision required to determine the          */
    /* relative position of the endpoints of b and of integer multiples of Pi / 2 */
    prec = mpfi_get_prec (a);
    mpfr_init2 (tmp, prec);

    if (prec_left > prec) prec = prec_left;
    if (prec_right > prec) prec = prec_right;

    mpz_init (zmod4);

    /* qr_mod4 gives the quadrant where the right endpoint of b is */
    /* qr_mod4 = floor (2 b->right / pi) mod 4 */
    mpz_mod_ui (zmod4, z_right, 4);
    qr_mod4 = mpz_get_ui (zmod4);

    /* z_left gives the quadrant where the left endpoint of b is */
    /* z_left = floor (2 b->left / pi) mod 4 */
    mpz_mod_ui (zmod4, z_left, 4);
    ql_mod4 = mpz_get_ui (zmod4);


    if (qr_mod4 == ql_mod4) {
      if (qr_mod4 == 1 || qr_mod4 == 2) {
	/* csc is increasing on b */
	inexact_left = mpfr_csc (&(a->left), &(b->left), GMP_RNDD);
	inexact_right = mpfr_csc (&(a->right), &(b->right), GMP_RNDU);
      }
      else {
	/* csc is decreasing on b */
	inexact_left = mpfr_csc (tmp, &(b->right), GMP_RNDD);
	inexact_right = mpfr_csc (&(a->right), &(b->left), GMP_RNDU);
	mpfr_set (&(a->left), tmp, GMP_RNDN);
      }
    }
    else if (ql_mod4 == 2) {
      mpz_add (z, z_left, z_right);
      mpz_add_ui (z, z, 1);
      if (mpfi_cmp_sym_pi (z, &(b->right), &(b->left), prec) >= 0)
	inexact_left = mpfr_csc (&(a->left), &(b->left), GMP_RNDD);
      else
	inexact_left = mpfr_csc (&(a->left), &(b->right), GMP_RNDD);
      inexact_right = mpfr_set_si (&(a->right), -1, GMP_RNDU);
    }
    else {
      mpz_add (z, z_left, z_right);
      mpz_add_ui (z, z, 1);
      if (mpfi_cmp_sym_pi (z, &(b->right), &(b->left), prec) >= 0)
	inexact_right = mpfr_csc (&(a->right), &(b->left), GMP_RNDU);
      else
	inexact_right = mpfr_csc (&(a->right), &(b->right), GMP_RNDU);
      inexact_left = mpfr_set_ui (&(a->left), +1, GMP_RNDD);
    }

    if (inexact_left) inexact = 1;
    if (inexact_right) inexact += 2;

    mpz_clear (zmod4);
    mpfr_clear (tmp);
  }

  mpz_clear (z_left);
  mpz_clear (z_right);

  return inexact;
}
