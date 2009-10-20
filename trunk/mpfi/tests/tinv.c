/* tinv.c -- Test mpfi_inv.

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

#include "mpfi-tests.h"

int
fr_inv (mpfr_ptr y, mpfr_srcptr x, mp_rnd_t rnd)
{
  return mpfr_ui_div (y, 1, x, rnd);
}

int
main (int argc, char **argv)
{
  mpfi_function inv;

  MPFI_SET_FUNCTION (inv, II, mpfi_inv, fr_inv);

  test_start ();

  check_data (inv, "inv.dat");
  check_random (inv, 2, 1000, 10);

  test_end ();

  return 0;
}
