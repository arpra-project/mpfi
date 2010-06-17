/* urandom.c -- Random element in the interval.

Copyright 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2010
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

/* Picks randomly a point m in y */
void
mpfi_urandom (mpfr_ptr m, mpfi_srcptr y, gmp_randstate_t state)
{
  mp_prec_t prec;
  mpfr_t diam, fact;
  prec = mpfr_get_prec (m);

  if ( MPFI_NAN_P(y) ) {
    mpfr_set_nan (m);
  }

  mpfr_init2 (diam, prec);
  mpfr_init2 (fact, prec);

  mpfi_diam_abs (diam, y);
  mpfr_urandomb (fact, state);

  /* fact lies between 0 and 1, the picked point lies at a relative
     distance "fact" of the left endpoint:  m = inf + (sup - inf)*fact  */
  mpfr_mul (diam, diam, fact, MPFI_RNDD);

  mpfr_add (m, &(y->left), diam, MPFI_RNDD);
  if (!mpfr_nan_p (m)) {  /* Ensure that m belongs to y */
    if (mpfr_cmp (m, &(y->left)) < 0)
      mpfr_set (m, &(y->left), MPFI_RNDU);

    if (mpfr_cmp (&(y->right), m) < 0)
      mpfr_set (m, &(y->right), MPFI_RNDD);
  }

  mpfr_clear (diam);
  mpfr_clear (fact);
}
