/* add_si.c -- Add an interval and a signed long int.

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
mpfi_add_si (mpfi_ptr a, mpfi_srcptr b, const long c)
{
  mpfi_t tmp;
  int inexact_add, inexact = 0;
  mp_prec_t tmpprec;

  tmpprec = mpfi_get_prec (a);
  if (sizeof(c) * 8 > tmpprec) tmpprec = sizeof(c) * 8;
  mpfi_init2 (tmp, tmpprec);
  mpfi_set_si (tmp, c); /* Exact */
  inexact_add = mpfi_add (a, b, tmp);
  MPFI_CLEAR (tmp);

  if (MPFI_NAN_P (a))
    MPFR_RET_NAN;

  if ( mpfr_inf_p (&(a->left)) ) {
    if  (MPFI_LEFT_IS_INEXACT (inexact_add)) /* overflow */
      inexact += 1;
  }
  else if (MPFI_LEFT_IS_INEXACT (inexact_add))
    inexact += 1;
  if ( mpfr_inf_p (&(a->right)) ) {
    if (MPFI_RIGHT_IS_INEXACT (inexact_add) )  /* overflow */
      inexact += 2;
  }
  else if (MPFI_RIGHT_IS_INEXACT (inexact_add))
    inexact += 2;

  if (mpfi_revert_if_needed (a)) {
    /*
      fprintf(stderr, "Pb endpoints in reverse order in mpfi_add_si: ");
      mpfi_out_str(stderr, 10, 0, a);
      fprintf(stderr, "\n");
    */
    inexact = MPFI_REVERT_INEXACT_FLAGS (inexact);
  }

  return inexact;
}
