/* tz_sub.c -- Test mpfi_z_sub.

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

/* fake non-existing function */
static int
mpfr_z_sub (mpfr_ptr x, mpz_srcptr z, mpfr_srcptr y, mp_rnd_t rnd)
{
  int ret;

  /* invert rounding mode */
  if (rnd == MPFI_RNDU)
    rnd = MPFI_RNDD;
  else if (rnd == MPFI_RNDD)
    rnd = MPFI_RNDU;
  ret = mpfr_sub_z (x, y, z, rnd);
  mpfr_neg (x, x, MPFI_RNDU);
  return -ret;
}

int
main (int argc, char **argv)
{
  struct mpfi_function_t i_z_sub;

  mpfi_fun_init_IZI (&i_z_sub, mpfi_z_sub, mpfr_z_sub);
  test_start ();

/*   check_data (&i_z_sub, "z_sub.dat"); */
  check_random (&i_z_sub, 2, 1000, 10);

  test_end ();
  mpfi_fun_clear (&i_z_sub);

  return 0;
}
