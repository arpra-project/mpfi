/* td_div.c -- Test mpfi_d_div.

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

#include <float.h>
#include "mpfi-tests.h"

#ifdef DBL_MAX_EXP
void
check_overflow (void)
{
  mpfi_t interval;
  mpfi_t got;
  double d = 2.0;
  int inex;

  if (DBL_MAX_EXP >= MPFR_EMAX_DEFAULT)
    return;

  mpfi_init2 (interval, 53);
  mpfi_init2 (got, 53);

  mpfi_set_ui (interval, 0);
  mpfr_nextabove (&(interval->right));
  mpfr_nextabove (&(interval->left)); /* tiny left endpoint x0 */
  mpfr_nextabove (&(interval->right)); /* interval = [x0, x0 + 1 ulp] */

  inex = mpfi_d_div (got, d, interval);
  if (!MPFI_BOTH_ARE_INEXACT (inex) || mpfi_bounded_p (got)) {
    printf ("Error: mpfi_d_div (rop, %g, op) does not correctly handle "
            "overflow.\nop = ", d);
    mpfi_out_str (stdout, 10, 0, interval);
    printf ("\nrop = ");
    mpfi_out_str (stdout, 10, 0, got);
    printf ("\nreturn value = %d\n", inex);
    exit (1);
  }

  mpfi_clear (interval);
  mpfi_clear (got);
}
#endif

int
main (int argc, char **argv)
{
  struct mpfi_function_t i_d_div;

  mpfi_fun_init_IDI (&i_d_div, mpfi_d_div, mpfr_d_div);
  test_start ();

  check_data (&i_d_div, "d_div.dat");
  check_random (&i_d_div, 2, 1000, 10);

#ifdef DBL_MAX_EXP
  check_overflow ();
#endif

  test_end ();
  mpfi_fun_clear (&i_d_div);

  return 0;
}
