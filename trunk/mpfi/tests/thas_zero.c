/* thas_zero.c -- Test mpfi_has_zero.

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

#include "mpfi-tests.h"

static void
error_message (mpfi_srcptr i)
{
  printf ("Error: mpfi_has_zero(I) returns wrong value.\nI = ");
  mpfi_out_str (stdout, 10, 0, i);
  printf ("\n%d", mpfi_has_zero (i));
  exit (1);
}

static void
test_special ()
{
  mpfi_t i;

  mpfi_init2 (i, 53);

  /* [nan, nan] */
  mpfr_set_nan (&(i->left));
  mpfr_set_nan (&(i->right));
  if (mpfi_has_zero (i))
    error_message (i);

  /* [-17, nan] */
  mpfr_set_si (&(i->left), -17, MPFI_RNDD);
  if (mpfi_has_zero (i))
    error_message (i);

  /* [-inf, -inf] */
  mpfr_set_inf (&(i->left), -1);
  mpfr_set_inf (&(i->right), -1);
  if (mpfi_has_zero (i))
    error_message (i);

  /* [-inf, 17] */
  mpfr_set_ui (&(i->right), 17, MPFI_RNDD);
  if (!mpfi_has_zero (i))
    error_message (i);

  /* [+0, +inf] */
  mpfr_set_ui (&(i->left), 0, MPFI_RNDD);
  mpfr_set_inf (&(i->right), +1);
  if (!mpfi_has_zero (i))
    error_message (i);

  /* [+inf, +inf] */
  mpfr_set_inf (&(i->left), +1);
  if (mpfi_has_zero (i))
    error_message (i);

  /* [-inf, +inf] */
  mpfr_set_inf (&(i->left), -1);
  if (!mpfi_has_zero (i))
    error_message (i);

  /* [+0, -0] */
  mpfi_set_ui (i, 0);
  if (!mpfi_has_zero (i))
    error_message (i);

  mpfi_clear (i);
}

static void
test_regular ()
{
  mpfi_t i;
  mpfr_t x;

  mpfr_init2 (x, 1024);
  mpfi_init2 (i, 64);

  mpfr_set_si_2exp (x, -0x7FFFFFFF, +654, MPFI_RNDD);
  mpfi_set_fr (i, x);
  if (mpfi_has_zero (i))
    error_message (i);

  mpfr_ui_div (x, 1, x, MPFI_RNDD);
  mpfi_put_fr (i, x);
  if (mpfi_has_zero (i))
    error_message (i);

  mpfr_neg (x, x, MPFI_RNDU);
  mpfi_put_fr (i, x);
  if (!mpfi_has_zero (i))
    error_message (i);

  mpfi_neg (i, i);
  if (!mpfi_has_zero (i))
    error_message (i);

  mpfi_add_ui (i, i, 1);
  if (mpfi_has_zero (i))
    error_message (i);

  mpfr_clear (x);
  mpfi_clear (i);
}

int
main (int argc, char **argv)
{
  test_start ();
  test_special ();
  test_regular ();
  test_end ();

  return 0;
}
