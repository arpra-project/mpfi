/* mul.c -- Multiply two intervals.

Copyright 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2010,
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

#include "mpfi-impl.h"

int
mpfi_mul (mpfi_ptr a, mpfi_srcptr u, mpfi_srcptr c)
{
  mpfr_t t1;
  mpfr_t t2;
  int inexact_left, inexact_right;
  int inexact = 0;

  /* Handling the NaN cases */
  if ( MPFI_NAN_P (u) || MPFI_NAN_P (c) )
    {
      mpfr_set_nan (&(a->left));
      mpfr_set_nan (&(a->right));
      MPFR_RET_NAN;
    }

  /* Handling the case where one operand is 0, in order */
  /* to avoid problems with 0 * an infinite interval    */
  if (MPFI_IS_ZERO (u)) {
    return (mpfi_set (a, u));
  }
  if (MPFI_IS_ZERO (c)) {
    return (mpfi_set (a, c));
  }

  if (mpfr_sgn (&(u->left)) >= 0) {
    if (mpfr_sgn (&(c->left)) >=0) {
      /* u nonnegative and c nonnegative */
      inexact_left = mpfr_mul (&(a->left), &(u->left), &(c->left), MPFI_RNDD);
      inexact_right = mpfr_mul (&(a->right), &(u->right), &(c->right), MPFI_RNDU);
    }
    else {
      mpfr_init2 (t1, mpfr_get_prec (&(a->left)));
      if (mpfr_sgn (&(c->right)) <= 0) {
	/* u nonnegative and c non-positive */
        inexact_left = mpfr_mul (t1, &(u->right), &(c->left), MPFI_RNDD);
        inexact_right = mpfr_mul (&(a->right), &(u->left), &(c->right), MPFI_RNDU);
      }
      else {
	/* u nonnegative and c overlapping 0 */
	inexact_left = mpfr_mul (t1, &(u->right), &(c->left), MPFI_RNDD);
	inexact_right = mpfr_mul (&(a->right), &(u->right), &(c->right), MPFI_RNDU);
      }
      mpfr_set (&(a->left), t1, MPFI_RNDD); /* exact */
      mpfr_clear (t1);
    }
  }
  else {
    if (mpfr_sgn (&(u->right)) <= 0) {
      /* u non-positive */
      mpfr_init2 (t1, mpfr_get_prec (&(a->left)));
      if (mpfr_sgn (&(c->left)) >= 0) {
        /* u non-positive and c nonnegative */
        inexact_left = mpfr_mul (t1, &(u->left), &(c->right), MPFI_RNDD);
        inexact_right = mpfr_mul (&(a->right), &(u->right), &(c->left), MPFI_RNDU);
      }
      else {
        if (mpfr_sgn (&(c->right)) <= 0) {
	  /* u non-positive and c non-positive */
          inexact_left = mpfr_mul (t1, &(u->right), &(c->right), MPFI_RNDD);
          inexact_right = mpfr_mul (&(a->right), &(u->left), &(c->left), MPFI_RNDU);
        }
        else {
	  /* u non-positive and c overlapping 0 */
	  inexact_left = mpfr_mul (t1, &(u->left), &(c->right), MPFI_RNDD);
	  inexact_right = mpfr_mul (&(a->right), &(u->left), &(c->left), MPFI_RNDU);
        }
      }
      mpfr_set (&(a->left), t1, MPFI_RNDD); /* exact */
      mpfr_clear (t1);
    }
    else {
      /* u contains 0 */
      if (mpfr_sgn (&(c->left)) >= 0) {
	/* u overlapping 0 and c nonnegative  */
        mpfr_init2 (t1, mpfr_get_prec (&(a->left)));

	inexact_left = mpfr_mul (t1, &(u->left), &(c->right), MPFI_RNDD);
	inexact_right = mpfr_mul (&(a->right), &(u->right), &(c->right), MPFI_RNDU);

        mpfr_set (&(a->left), t1, MPFI_RNDD);
        mpfr_clear (t1);
      }
      else {
	if (mpfr_sgn (&(c->right)) <= 0) {
	  /* u overlapping 0 and c non-positive */
          mpfr_init2 (t1, mpfr_get_prec (&(a->left)));

	  inexact_left = mpfr_mul (t1, &(u->right), &(c->left), MPFI_RNDD);
	  inexact_right = mpfr_mul (&(a->right), &(u->left), &(c->left), MPFI_RNDU);

          mpfr_set (&(a->left), t1, MPFI_RNDD);
          mpfr_clear (t1);
	}
	else {
	  /* u overlapping 0 and c overlapping 0
	     Beware the case where the result is one of the operands! */
          int inexact_tmp;

	  mpfr_init2 (t1, mpfr_get_prec (&(a->left)));
	  mpfr_init2 (t2, mpfr_get_prec (&(a->left)));
	  inexact_right = mpfr_mul (t1, &(u->left), &(c->right), MPFI_RNDD);
	  inexact_left = mpfr_mul (t2, &(u->right), &(c->left), MPFI_RNDD);
	  if (mpfr_cmp (t1, t2) < 0) {
	    mpfr_swap (t2, t1); /* same precision */
            inexact_left = inexact_right;
	  }

          mpfr_set_prec (t1, mpfr_get_prec (&(a->right)));
	  inexact_tmp = mpfr_mul (t1, &(u->left), &(c->left), MPFI_RNDU);
	  inexact_right = mpfr_mul (&(a->right), &(u->right), &(c->right), MPFI_RNDU);
	  if (mpfr_cmp (t1, &(a->right)) > 0) {
            mpfr_set (&(a->right), t1, MPFI_RNDU); /* exact */
	    inexact_right = inexact_tmp;
	  }
	  mpfr_set (&(a->left), t2, MPFI_RNDD); /* exact */
	  mpfr_clear (t1);
	  mpfr_clear (t2);
	}
      }
    }
  }

  if (inexact_left)
    inexact += 1;
  if (inexact_right)
    inexact += 2;

  return inexact;
}
