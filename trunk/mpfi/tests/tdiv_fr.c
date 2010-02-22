/* tdiv_fr.c -- Test mpfi_div_fr.

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

int
main (int argc, char **argv)
{
  struct mpfi_function_t i_div_fr;

  mpfi_fun_init_IIR (&i_div_fr, mpfi_div_fr, mpfr_div);
  test_start ();

/*   check_data (&i_div_fr, "div_fr.dat"); */
  check_random (&i_div_fr, 2, 1000, 10);

  test_end ();
  mpfi_fun_clear (&i_div_fr);

  return 0;
}
