/* tcoth.c -- Test mpfi_coth.

Copyright 2009 2010
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

static int
restrict_domain (mpfi_ptr a)
{
  if (MPFI_HAS_ZERO_NONSTRICT (a)) {
    /* translate a <- a - left + 1 */
    mpfi_sub_fr (a, a, &(a->left));
    mpfi_add_ui (a, a, +1);
  }
}

int
main (int argc, char **argv)
{
  struct mpfi_function_t i_coth;

  mpfi_fun_init_II (&i_coth, mpfi_coth, mpfr_coth);
  test_start ();

/*   check_data (&i_coth, "coth.dat"); */

#if MPFR_VERSION < MPFR_VERSION_NUM(2, 4, 2)
  /* mpfr_coth is bugged: returns wrong value for +-0 */
    mpfi_restrict_random (&i_coth, restrict_domain);
#endif

  check_random (&i_coth, 2, 1000, 10);

  test_end ();
  mpfi_fun_clear (&i_coth);

  return 0;
}
