/* mpfi_tests.c -- Test constant functions
                   and test non-constant functions with random values.

Copyright (C) 2001, 2002, 2009
                     Spaces project, Inria Lorraine
                     Arenaire project, Inria Rhone-Alpes, France
                     and Lab. ANO, USTL (Univ. of Lille),  France


This file is part of the MPFI Library, based on the MPFR Library.

The MPFI Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Library General Public License as published by
the Free Software Foundation; either version 2 of the License, or (at your
option) any later version.

The MPFI Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
License for more details.

You should have received a copy of the GNU Library General Public License
along with the MPFR Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
MA 02110-1301, USA. */

#include "mpfi-tests.h"

void
check_random (mpfi_function function, mp_prec_t prec_min, mp_prec_t prec_max,
              int nb_tests)
{
  mp_prec_t prec;
  int i, dummy;
  mpfi_t x, y, z;
  mpfr_t e, f, g;

  if (MPFI_GET_TYPE (function) != II && MPFI_GET_TYPE (function) != III) {
    printf ("Wrong function type error.\n");
    printf ("There is a bug in the test suite itself,"
	    " you should not see this message.\n");
    exit (1);
  }

  if (MPFI_GET_MPFR_FUNCTION (function, II) == NULL)
    return;

  if (!rands_initialized) {
    printf ("Put test_start at the beginning of your test function.\n");
    exit (1);
  }

  mpfi_init2 (x, prec_max);
  mpfi_init2 (z, prec_max);

  mpfr_init2 (e, prec_max);
  mpfr_init2 (f, prec_max);

  if (MPFI_GET_TYPE (function) == III) {
    mpfi_init2 (y, prec_max);
    mpfr_init2 (g, prec_max);
  }

  for (prec = prec_min; prec <= prec_max; ++prec) {
    mpfi_set_prec (x, prec);
    mpfi_set_prec (z, prec);

    mpfr_set_prec (e, prec);
    mpfr_set_prec (f, prec);

    if (MPFI_GET_TYPE (function) == III) {
      mpfi_set_prec (y, prec);
      mpfr_set_prec (g, prec);
    }

    for (i = 0; i < nb_tests; ++i) {
      mpfr_urandomb (e, rands);
      mpfr_ui_div (e, 1, e, MPFI_RNDD);
      mpfr_urandomb (f, rands);
      mpfi_set_fr (x, f);
      mpfr_urandomb (f, rands);
      mpfi_div_fr (x, x, f);
      mpfi_increase (x, e);

      if (MPFI_GET_TYPE (function) == II) {
        mpfi_alea (e, x); /* FIXME use random seed */

        dummy = (MPFI_GET_FUNCTION (function, II)) (z, x);
        dummy = (MPFI_GET_MPFR_FUNCTION (function, II)) (f, e, GMP_RNDN);

        if (mpfr_cmp (f, &(z->left)) < 0 || mpfr_cmp (f, &(z->right)) > 0){
          printf ("Error in op:\nthe result z of op(x) does not contain "
                  "f = op(e) with e in x.\nx = ");
          mpfi_out_str (stdout, 10, 0, x);
          printf ("\nz = ");
          mpfi_out_str (stdout, 10, 0, z);
          printf ("\ne = ");
          mpfr_out_str (stdout, 10, 0, e, MPFI_RNDU);
          printf ("\nf = ");
          mpfr_out_str (stdout, 10, 0, f, MPFI_RNDU);
          putchar ('\n');

          exit (1);
        }
      }
      else {
        mpfr_urandomb (e, rands);
        mpfr_ui_div (e, 1, e, MPFI_RNDD);
        mpfr_urandomb (f, rands);
        mpfi_set_fr (y, f);
        mpfr_urandomb (f, rands);
        mpfi_div_fr (y, y, f);
        if (i % 2) mpfi_neg (y, y);
        mpfi_increase (y, e);

        mpfi_alea (e, x); /* FIXME use random seed */
        mpfi_alea (f, y); /* FIXME */

        dummy = (MPFI_GET_FUNCTION (function, III)) (z, x, y);
        dummy = (MPFI_GET_MPFR_FUNCTION (function, III)) (g, e, f, GMP_RNDN);

        if (mpfr_cmp (g, &(z->left)) < 0 || mpfr_cmp (g, &(z->right)) > 0){
          printf ("Error in op:\nthe result z of x op y does not contain "
                  "g = e op f with e in x and f in y.\nx = ");
          mpfi_out_str (stdout, 10, 0, x);
          printf ("\ny = ");
          mpfi_out_str (stdout, 10, 0, y);
          printf ("\nz = ");
          mpfi_out_str (stdout, 10, 0, z);
          printf ("\ne = ");
          mpfr_out_str (stdout, 10, 0, e, MPFI_RNDU);
          printf ("\nf = ");
          mpfr_out_str (stdout, 10, 0, f, MPFI_RNDU);
          printf ("\ng = ");
          mpfr_out_str (stdout, 10, 0, g, MPFI_RNDU);
          putchar ('\n');

          exit (1);
        }
      }
    }
  }

  mpfi_clear(x);
  mpfi_clear(z);

  mpfr_clear(e);
  mpfr_clear(f);

  if (MPFI_GET_TYPE (function) == III) {
    mpfi_clear(y);
    mpfr_clear(g);
  }
}

void
check_const (mpfi_function f, mp_prec_t prec_min, mp_prec_t prec_max)
{
  mp_prec_t prec;
  mpfi_t low_prec, high_prec, tmp;
  mpfr_t fr_const;

  if (MPFI_GET_TYPE (f) != I) {
    printf ("Wrong function type error.\n");
    printf ("There is a bug in the test suite itself,"
	    " you should not see this message.\n");
    exit (1);
  }

  mpfi_init2 (low_prec, prec_max);
  mpfi_init2 (high_prec, prec_max);
  mpfr_init2 (fr_const, prec_max);

  mpfi_set_prec (high_prec, prec_min);
  MPFI_GET_FUNCTION(f, I) (high_prec);

  for (prec = prec_min + 1; prec < prec_max; ++prec) {
    tmp[0] = low_prec[0];
    low_prec[0] = high_prec[0];
    high_prec[0] = tmp[0];

    mpfi_set_prec (high_prec, prec);
    MPFI_GET_FUNCTION(f, I) (high_prec);

    if ((mpfr_cmp (&(low_prec->left), &(high_prec->left)) > 0)
	|| (mpfr_cmp (&(low_prec->right), &(high_prec->right)) < 0)) {
      printf ("Error: wrong rounded value.\nAt precision %lu, %s returns\n",
	      prec-1, MPFI_GET_FUNCTION_NAME (f));
      mpfi_out_str (stdout, 10, 0, low_prec);
      printf ("\nwhile at precision %lu, it returns\n", prec);
      mpfi_out_str (stdout, 10, 0, high_prec);
      putchar ('\n');

      exit (1);
    }
  }

  MPFI_GET_MPFR_FUNCTION(f, I)(fr_const, MPFI_RNDD);
  if ((mpfr_cmp (&(high_prec->left), fr_const) > 0)
      || (mpfr_cmp (&(high_prec->right), fr_const) <= 0)) {
    printf ("Error: wrong rounded value.\nAt precision %lu, %s returns\n",\
	    prec, MPFI_GET_FUNCTION_NAME (f));
    mpfi_out_str (stdout, 10, 0, high_prec);
    printf ("\nwhile mpfr function returns (rounding towards minus infinity)\n");
    mpfr_out_str (stdout, 10, 0, fr_const, MPFI_RNDD);
    putchar ('\n');

    exit (1);
  }

  mpfi_clear (low_prec);
  mpfi_clear (high_prec);
  mpfr_clear (fr_const);
}
