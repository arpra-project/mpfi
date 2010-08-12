/* intersect.c -- Intervals intersection.

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
mpfi_intersect (mpfi_ptr a, mpfi_srcptr b, mpfi_srcptr c)
{
/* If intersection = empty set, a will have its left bound > right one */
  int inexact_left, inexact_right, inexact=0;

  if ( MPFI_NAN_P (b) || MPFI_NAN_P (c) ) {
    mpfr_set_nan (&(a->left));
    mpfr_set_nan (&(a->right));
    MPFR_RET_NAN;
  }

  if (mpfr_cmp (&(b->left), &(c->left)) <= 0)
    inexact_left = mpfr_set (&(a->left), &(c->left), MPFI_RNDD);
  else
    inexact_left = mpfr_set (&(a->left), &(b->left), MPFI_RNDD);

  if (mpfr_cmp (&(c->right), &(b->right)) <= 0)
    inexact_right = mpfr_set (&(a->right), &(c->right), MPFI_RNDU);
  else
    inexact_right = mpfr_set (&(a->right), &(b->right), MPFI_RNDU);

  if (inexact_left)
    inexact += 1;
  if (inexact_right)
    inexact += 2;
  if (mpfr_cmp (&(a->left), &(a->right)) > 0) /* a is the empty set */
    inexact = MPFI_FLAGS_BOTH_ENDPOINTS_INEXACT;

  return inexact;
}
