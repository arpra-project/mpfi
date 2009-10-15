/* mpfi_tests.c -- Test with random values.

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

  if (function.mpfr_func == NULL)
    return;

  if (!rands_initialized) {
    printf ("Put test_start at the beginning of your test function.\n");
    exit (1);
  }

  mpfi_init2 (x, prec_max);
  mpfi_init2 (y, prec_max);
  mpfi_init2 (z, prec_max);

  mpfr_init2 (e, prec_max);
  mpfr_init2 (f, prec_max);
  mpfr_init2 (g, prec_max);

  for (prec = prec_min; prec <= prec_max; ++prec) {
    mpfi_set_prec (x, prec);
    mpfi_set_prec (y, prec);
    mpfi_set_prec (z, prec);

    mpfr_set_prec (e, prec);
    mpfr_set_prec (f, prec);
    mpfr_set_prec (g, prec);

    for (i = 0; i < nb_tests; ++i) {

      mpfr_urandomb (e, rands);
      mpfr_ui_div (e, 1, e, MPFI_RNDD);
      mpfr_urandomb (f, rands);
      mpfi_set_fr (x, f);
      mpfr_urandomb (f, rands);
      mpfi_div_fr (x, x, f);
      mpfi_increase (x, e);

      mpfr_urandomb (e, rands);
      mpfr_ui_div (e, 1, e, MPFI_RNDD);
      mpfr_urandomb (f, rands);
      mpfi_set_fr (y, f);
      mpfr_urandomb (f, rands);
      mpfi_div_fr (y, y, f);
      if (i % 2) mpfi_neg (y, y);
      mpfi_increase (y, e);

      dummy = function.func (z, x, y);

      mpfi_alea (e, x); /* FIXME use random seed */
      mpfi_alea (f, y); /* FIXME */

      dummy = function.mpfr_func (g, e, f, GMP_RNDN);
      
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

  mpfi_clear(x);
  mpfi_clear(y);
  mpfi_clear(z);

  mpfr_clear(e);
  mpfr_clear(f);
  mpfr_clear(g);
}
