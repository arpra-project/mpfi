/* tz_div.c -- Test mpfi_z_div.

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
mpfr_z_div (mpfr_ptr x, mpz_srcptr z, mpfr_srcptr y, mp_rnd_t rnd)
{
  int ret;
  mpfr_t zz;

  mpfr_init2 (zz, mpz_sizeinbase (z, 2));
  mpfr_set_z (zz, z, MPFI_RNDD); /* exact */
  ret = mpfr_div (x, zz, y, rnd);
  mpfr_clear (zz);

  return ret;
}

int
main (int argc, char **argv)
{
  struct mpfi_function_t i_z_div;

  mpfi_fun_init_IZI (&i_z_div, mpfi_z_div, mpfr_z_div);
  test_start ();

/*   check_data (&i_z_div, "z_div.dat"); */
  check_random (&i_z_div, 2, 1000, 10);

  test_end ();
  mpfi_fun_clear (&i_z_div);

  return 0;
}
