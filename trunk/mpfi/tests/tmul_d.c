/* tmul_d.c -- Test mpfi_mul_d.

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
check (mpfi_ptr got, mpfi_srcptr a, double d, mpfi_srcptr expected,
       int expected_retval)
{
  int ret;

  ret = mpfi_mul_d (got, a, d);
  if (ret != expected_retval) {
    printf ("Error: mpfi_mul does not return expected value.\ninterval:",
            ret);
    mpfi_out_str (stdout, 16, 0, a);
    printf ("\n  factor: %g\n     got: %d\nexpected: %d\n",
            d, ret, expected_retval);
    exit (1);
  }
  if (!same_value (got, expected)) {
    printf ("Error: mpfi_mul does not return expected value.\n"
            "interval:");
    mpfi_out_str (stdout, 16, 0, a);
    printf ("\n  factor: %g\n     got:", d);
    mpfi_out_str (stdout, 16, 0, got);
    printf ("\nexpected:");
    mpfi_out_str (stdout, 16, 0, expected);
    printf ("\n");
    exit (1);
  }
}

static void
check_overflow ()
{
  mpfi_t a, b, c;
  int inexact;

  mpfi_init2 (a, 53);
  mpfi_init2 (b, 53);
  mpfi_init2 (c, 53);

  mpfr_set_ui (&(a->left), 10, MPFI_RNDD);
  mpfr_set_inf (&(a->right), +1);
  mpfr_nextbelow (&(a->right));
  mpfr_set_ui (&(c->left), 15, MPFI_RNDD);
  mpfr_set_inf (&(c->right), +1);
  check (b, a, 1.5, c, MPFI_FLAGS_RIGHT_ENDPOINT_INEXACT);

  mpfr_set_inf (&(a->left), -1);
  mpfr_nextabove (&(a->left));
  mpfr_set_ui (&(a->right), 10, MPFI_RNDU);
  mpfr_set_inf (&(c->left), -1);
  mpfr_set_ui (&(c->right), 15, MPFI_RNDD);
  check (b, a, 1.5, c, MPFI_FLAGS_LEFT_ENDPOINT_INEXACT);

  mpfi_clear (a);
  mpfi_clear (b);
  mpfi_clear (c);
}

static void
check_underflow ()
{
  mpfi_t a, b, c;
  int inexact;

  mpfi_init2 (a, 53);
  mpfi_init2 (b, 53);
  mpfi_init2 (c, 53);

  mpfr_set_si (&(a->left),  0, MPFI_RNDD);
  mpfr_nextabove (&(a->left));
  mpfr_set_ui (&(a->right), 1, MPFI_RNDU);
  mpfi_interv_d (c, 0, 0.5);
  check (b, a, 0.5, c, MPFI_FLAGS_LEFT_ENDPOINT_INEXACT);

  mpfi_interv_d (c, -0.25, 0);
  check (b, a, -0.25, c, MPFI_FLAGS_RIGHT_ENDPOINT_INEXACT);

  mpfi_neg (a, a);
  mpfi_interv_d (c, -0.125, 0);
  check (b, a, 0.125, c, MPFI_FLAGS_RIGHT_ENDPOINT_INEXACT);

  mpfi_interv_d (c, 0.625, 0);
  check (b, a, -0.625, c, MPFI_FLAGS_LEFT_ENDPOINT_INEXACT);

  mpfr_set_si (&(a->left),  -1, MPFI_RNDD);
  mpfr_set_ui (&(a->right), 0, MPFI_RNDU);
  mpfr_nextabove (&(a->right));
  mpfr_set_d (&(c->left), -0.5, MPFI_RNDD);
  mpfr_set (&(c->right), &(a->right), MPFI_RNDU);
  check (b, a, 0.5, c, MPFI_FLAGS_RIGHT_ENDPOINT_INEXACT);

  mpfi_neg (c, c);
  mpfr_set_d (&(c->right), 0.25, MPFI_RNDU);
  check (b, a, -0.25, c, MPFI_FLAGS_LEFT_ENDPOINT_INEXACT);

  mpfi_neg (a, a);
  mpfr_set_d (&(c->right), 0.125, MPFI_RNDU);
  check (b, a, 0.125, c, MPFI_FLAGS_LEFT_ENDPOINT_INEXACT);

  mpfi_neg (c, c);
  mpfr_set_d (&(c->left), -0.625, MPFI_RNDU);
  check (b, a, -0.625, c, MPFI_FLAGS_RIGHT_ENDPOINT_INEXACT);

  mpfi_clear (a);
  mpfi_clear (b);
  mpfi_clear (c);
}


int
main (int argc, char **argv)
{
  struct mpfi_function_t i_mul_d;

  mpfi_fun_init_IID (&i_mul_d, mpfi_mul_d, NULL);

  check_data (&i_mul_d, "mul_d.dat");
  check_overflow ();
  check_underflow ();

  mpfi_fun_clear (&i_mul_d);

  return 0;
}
