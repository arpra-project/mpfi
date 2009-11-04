/* atan2.c -- Arc tangent of two intervals.

Copyright 2009
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
mpfi_atan2 (mpfi_ptr a, mpfi_srcptr b, mpfi_srcptr c)
{
  int inexact_left, inexact_right, inexact=0;

  if ( MPFI_NAN_P (b) || MPFI_NAN_P (c) ) {
    mpfr_set_nan (&(a->left));
    mpfr_set_nan (&(a->right));
    MPFR_RET_NAN;
  }

  if (MPFI_IS_POS (b)) {
    if (MPFI_IS_POS (c)) {
      inexact_left = mpfr_atan2 (&(a->left), &(b->left), &(c->right), MPFI_RNDD);
      inexact_right = mpfr_atan2 (&(a->right), &(b->right), &(c->left), MPFI_RNDU);
    }
    else if (MPFI_IS_NEG (c)) {
      inexact_left = mpfr_atan2 (&(a->left), &(b->right), &(c->right), MPFI_RNDD);
      inexact_right = mpfr_atan2 (&(a->right), &(b->left), &(c->left), MPFI_RNDU);
    }
    else {
      inexact_left = mpfr_atan2 (&(a->left), &(b->left), &(c->right), MPFI_RNDD);
      inexact_right = mpfr_atan2 (&(a->right), &(b->left), &(c->left), MPFI_RNDU);
    }
  }
  else if (MPFI_IS_NEG (b)) {
    if (MPFI_IS_POS (c)) {
      inexact_left = mpfr_atan2 (&(a->left), &(b->left), &(c->left), MPFI_RNDD);
      inexact_right = mpfr_atan2 (&(a->right), &(b->right), &(c->right), MPFI_RNDU);
    }
    else if (MPFI_IS_NEG (c)) {
      inexact_left = mpfr_atan2 (&(a->left), &(b->right), &(c->left), MPFI_RNDD);
      inexact_right = mpfr_atan2 (&(a->right), &(b->left), &(c->right), MPFI_RNDU);
    }
    else {
      inexact_left = mpfr_atan2 (&(a->left), &(b->right), &(c->left), MPFI_RNDD);
      inexact_right = mpfr_atan2 (&(a->right), &(b->right), &(c->right), MPFI_RNDU);
    }
  }
  else {
    if (MPFI_IS_POS (c)) {
      inexact_left = mpfr_atan2 (&(a->left), &(b->left), &(c->left), MPFI_RNDD);
      inexact_right = mpfr_atan2 (&(a->right), &(b->right), &(c->left), MPFI_RNDU);
    }
    else {
      inexact_left = -mpfr_const_pi (&(a->left), MPFI_RNDU);
      mpfr_neg (&(a->left), &(a->left), MPFI_RNDD);
      inexact_right = mpfr_const_pi (&(a->right), MPFI_RNDU);
    }
  }

  if (mpfr_sgn (&(a->left)) == 0) {
    /* +0 as left endpoint */
    mpfr_setsign (&(a->left), &(a->left), 0, MPFI_RNDU);
  }
  if (mpfr_sgn (&(a->right)) == 0) {
    /* -0 as right endpoint */
    mpfr_setsign (&(a->right), &(a->right), 1, MPFI_RNDD);
  }

  if (inexact_left)
    inexact += 1;
  if (inexact_right)
    inexact += 2;

  return inexact;
}
