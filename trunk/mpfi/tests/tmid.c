/* tmid.c -- Test mpfi_mid.

Copyright 2010,
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

void
check_underflow (void)
{
  mpfi_t i;
  mpfr_t m;
  int inex;

  mpfr_init2 (m, 128);
  mpfi_init2 (i, 128);
  mpfi_set_ui (i, 0);
  mpfr_nextabove (&(i->right));

  inex = mpfi_mid (m, i);

  if (!mpfr_zero_p (m) || inex >= 0) {
    printf ("Error: mpfi_mid(I) does not correct value.\nI =");
    mpfi_out_str (stdout, 10, 0, I);
    printf ("  result: ");
    mpfr_out_str (stdout, 10, 0, m, MPFI_RNDU);
    if (inex >= 0) {
      printf ("return value: %d\n   expected: -1\n", inex);
    }
    else {
      printf ("\nexpected: 0\n");
    }
    exit (1);
  }

  mpfr_clear (m);
  mpfi_clear (i);
}

int
main (int argc, char **argv)
{
  struct mpfi_function_t i_mid;

  mpfi_fun_init_RI (&i_mid, mpfi_mid, NULL);

  check_data (&i_mid, "mid.dat");
  check_underflow ();

  mpfi_fun_clear (&i_mid);

  return 0;
}
