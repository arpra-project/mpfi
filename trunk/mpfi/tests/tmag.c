/* tmag.c -- Test mpfi_mag.

Copyright 2010
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

#include <float.h>
#include "mpfi-tests.h"

static void
error_message (mpfi_srcptr i, mpfr_ptr got, mpfr_ptr expected)
{
  printf ("Error: mpfi_mag(I) returns wrong value.\nI = ");
  mpfi_out_str (stdout, 10, 0, i);
  printf ("\ngot     : ");
  mpfr_out_str (stdout, 10, 0, got, MPFI_RNDD);
  printf ("\nexpected: ");
  mpfr_out_str (stdout, 10, 0, expected, MPFI_RNDD);
  printf ("\n");
  exit (1);
}

static void
test_special ()
{
  mpfr_t expected, got;
  mpfi_t i;

  mpfr_init2 (expected, 53);
  mpfr_init2 (got, 53);
  mpfi_init2 (i, 53);

  /* [nan, nan] */
  mpfr_set_nan (expected);
  mpfi_set_fr (i, expected);
  mpfi_mag (got, i);
  if (!same_mpfr_value (expected, got))
    error_message (i, got, expected);

  /* [-17, nan] */
  mpfr_set_si (&(i->left), -17, MPFI_RNDD);
  mpfi_mag (got, i);
  if (!same_mpfr_value (expected, got))
    error_message (i, got, expected);

  /* [-inf, -inf] */
  mpfr_set_inf (expected, -1);
  mpfi_set_fr (i, expected);
  mpfr_abs (expected, expected, MPFI_RNDD);
  mpfi_mag (got, i);
  if (!same_mpfr_value (expected, got))
    error_message (i, got, expected);

  /* [-inf, 17] */
  mpfr_set_ui (&(i->right), 17, MPFI_RNDU);
  mpfi_mag (got, i);
  if (!same_mpfr_value (expected, got))
    error_message (i, got, expected);

  /* [+0, +inf] */
  mpfr_set_ui (&(i->left), 0, MPFI_RNDD);
  mpfr_set_inf (expected, +1);
  mpfr_set (&(i->right), expected, MPFI_RNDU);
  mpfi_mag (got, i);
  if (!same_mpfr_value (expected, got))
    error_message (i, got, expected);

  /* [-inf, +inf] */
  mpfr_set_inf (&(i->left), -1);
  mpfr_set_inf (&(i->right), +1);
  mpfi_mag (got, i);
  if (!same_mpfr_value (expected, got))
    error_message (i, got, expected);

  /* [+inf, +inf] */
  mpfi_set_fr (i, expected);
  mpfi_mag (got, i);
  if (!same_mpfr_value (expected, got))
    error_message (i, got, expected);

  /* [+0, -0] */
  mpfr_set_ui (expected, 0, MPFI_RNDU);
  mpfi_set_fr (i, expected);
  mpfi_mag (got, i);
  if (!same_mpfr_value (expected, got))
    error_message (i, got, expected);

  mpfr_clear (expected);
  mpfr_clear (got);
  mpfi_clear (i);
}

static void
test_random (mp_prec_t prec_min, mp_prec_t prec_max)
{
  mpfr_t x;
  mpfi_t i;
  mp_prec_t prec;
  int dl, dr, d0;
  int ret;

  mpfr_init2 (x, prec_max);
  mpfi_init2 (i, prec_max);

  for (prec = prec_min; prec < prec_max; prec++) {
    mpfi_set_prec (i, prec);
    mpfr_set_prec (x, prec);
    random_interval (i);
    ret = mpfi_mag (x, i);
    dl = mpfr_cmp_abs (x, &(i->left));
    dr = mpfr_cmp_abs (x, &(i->right));
    if (dl < 0 || dr < 0) {
      printf ("Error: mpfi_mag(x, I) returns a value x less than some "
              "elements in I.\nI = ");
      mpfi_out_str (stdout, 10, 0, i);
      printf ("\nx = ");
      mpfr_out_str (stdout, 10, 0, x, MPFI_RNDD);
      printf ("\n");
      exit (1);
    }
    d0 = mpfr_cmp_ui (x, 0);
    if (d0 < 0) {
      printf ("Error: mpfi_mag(x, I) returns a negative value.\nI = ");
      mpfi_out_str (stdout, 10, 0, i);
      printf ("\nx = ");
      mpfr_out_str (stdout, 10, 0, x, MPFI_RNDD);
      printf ("\n");
      exit (1);
    }
    if (dl != 0 && dr != 0 && d0 != 0) {
      printf ("Error: mpfi_mag(x, I) returns a value x that is not an "
              "endpoint of abs(I).\nI = ");
      mpfi_out_str (stdout, 10, 0, i);
      printf ("\nx = ");
      mpfr_out_str (stdout, 10, 0, x, MPFI_RNDD);
      printf ("\n");
      exit (1);
    }
    if (ret != 0) {
      printf ("Error: mpfi_mag(x, I) returns a nonexact value x while the "
              "precisions of x and I are equal.\nprecision(I) = %lu, I = ",
              mpfi_get_prec (i));
      mpfi_out_str (stdout, 10, 0, i);
      printf ("\nprecision(x) = %lu, x = ", mpfr_get_prec (x));
      mpfr_out_str (stdout, 10, 0, x, MPFI_RNDD);
      printf ("\n");
      exit (1);
    }
  }

  mpfr_clear (x);
  mpfi_clear (i);
}

int
main (int argc, char **argv)
{
  test_start ();
  test_special ();
  test_random (2, 1023);
  test_end ();

  return 0;
}
