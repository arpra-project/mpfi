/* tbisect.c -- Test mpfi_bisect.

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

static void
check (mpfi_ptr left, mpfi_ptr right, mpfi_srcptr interval,
       mpfi_srcptr expected_left, mpfi_srcptr expected_right,
       int expected_retval)
{
  int ret;

  ret = mpfi_bisect (left, right, interval);
  if (ret != expected_retval)
    {
      printf ("Error: mpfi_bisect does not return expected value."
              "\ninterval: ");
      mpfi_out_str (stdout, 16, 0, interval);
      printf ("\n     got: %d\nexpected: %d\n", ret, expected_retval);
      exit (1);
    }
  if (!same_value (left, expected_left)
      || !same_value (right, expected_right))
    {
      printf ("Error: mpfi_bisect does not return expected subintervals.\n"
              "interval:");
      mpfi_out_str (stdout, 16, 0, interval);
      printf ("\n    left:");
      mpfi_out_str (stdout, 16, 0, left);
      printf ("\n   right:");
      mpfi_out_str (stdout, 16, 0, right);
      printf ("\n expected left:");
      mpfi_out_str (stdout, 16, 0, expected_left);
      printf ("\nexpected right:");
      mpfi_out_str (stdout, 16, 0, expected_right);
      printf ("\n");
      exit (1);
    }


  /* reuse variable tests */

  if (mpfi_get_prec (interval) == mpfi_get_prec (expected_left))
    {
      mpfi_set (left, interval);
      ret = mpfi_bisect (left, right, left);
      if (ret != expected_retval)
        {
          printf ("Error: mpfi_bisect does not return expected value when "
                  "reusing the first variable.\ninterval: ");
          mpfi_out_str (stdout, 16, 0, interval);
          printf ("\n     got: %d\nexpected: %d\n", ret, expected_retval);
          exit (1);
        }
      if (!same_value (left, expected_left)
          || !same_value (right, expected_right))
        {
          printf ("Error: mpfi_bisect does not return expected interval when "
                  "reusing the first variable.\ninitial interval:");
          mpfi_out_str (stdout, 16, 0, interval);
          printf ("\n            left:");
          mpfi_out_str (stdout, 16, 0, left);
          printf ("\n           right:");
          mpfi_out_str (stdout, 16, 0, right);
          printf ("\n   expected left:");
          mpfi_out_str (stdout, 16, 0, expected_left);
          printf ("\n  expected right:");
          mpfi_out_str (stdout, 16, 0, expected_right);
          printf ("\n");
          exit (1);
        }
    }

  if (mpfi_get_prec (interval) == mpfi_get_prec (expected_right))
    {
      mpfi_set (right, interval);
      ret = mpfi_bisect (left, right, right);
      if (ret != expected_retval)
        {
          printf ("Error: mpfi_bisect does not return expected value when "
                  "reusing the second variable.\ninterval: ");
          mpfi_out_str (stdout, 16, 0, interval);
          printf ("\n     got: %d\nexpected: %d\n", ret, expected_retval);
          exit (1);
        }
      if (!same_value (left, expected_left)
          || !same_value (right, expected_right))
        {
          printf ("Error: mpfi_bisect does not return expected interval when "
                  "reusing the second variable.\ninitial interval:");
          mpfi_out_str (stdout, 16, 0, interval);
          printf ("\n            left:");
          mpfi_out_str (stdout, 16, 0, left);
          printf ("\n           right:");
          mpfi_out_str (stdout, 16, 0, right);
          printf ("\n   expected left:");
          mpfi_out_str (stdout, 16, 0, expected_left);
          printf ("\n  expected right:");
          mpfi_out_str (stdout, 16, 0, expected_right);
          printf ("\n");
          exit (1);
        }
    }
}

static void
check_special (void)
{
  mpfi_t left, right, interval;
  mpfi_t expected_left, expected_right;

  mpfi_init2 (interval, 53);
  mpfi_init2 (left, 53);
  mpfi_init2 (right, 53);
  mpfi_init2 (expected_left, 53);
  mpfi_init2 (expected_right, 53);


  /* NaNs */

  /* During initialization, intervals are set to [NaN, NaN] */
  mpfi_set_ui (left, 0);
  mpfi_set_ui (right, 0);
  check (left, right, interval, expected_left, expected_right, 0);

  mpfr_set_nan (&(interval->left));
  mpfr_set_ui (&(interval->right), 0, MPFI_RNDD);
  mpfi_set_ui (left, 0);
  mpfi_set_ui (right, 0);
  check (left, right, interval, expected_left, expected_right, 0);
  
  mpfr_set_ui (&(interval->left), 0, MPFI_RNDD);
  mpfr_set_nan (&(interval->right));
  mpfi_set_ui (left, 0);
  mpfi_set_ui (right, 0);
  check (left, right, interval, expected_left, expected_right, 0);

  
  /* Infinities */

  mpfi_set_ui (left, 0);
  mpfi_set_ui (right, 0);
  mpfr_set_inf (&(interval->left), +1);
  mpfr_set_inf (&(interval->right), +1);
  mpfi_set (expected_left, interval);
  check (left, right, interval, expected_left, expected_right, 0);

  mpfi_set_ui (left, 0);
  mpfi_set_ui (right, 0);
  mpfr_set_inf (&(interval->left), +1);
  mpfr_set_ui (&(interval->right), 0, MPFI_RNDD);
  mpfi_set (expected_left, interval);
  check (left, right, interval, expected_left, expected_right, 0);
  
  mpfi_set_ui (left, 0);
  mpfi_set_ui (right, 0);
  mpfr_set_inf (&(interval->left), +1);
  mpfr_set_inf (&(interval->right), -1);
  mpfi_set (expected_left, interval);
  check (left, right, interval, expected_left, expected_right, 0);
  
  mpfi_set_ui (left, 0);
  mpfi_set_ui (right, 0);
  mpfr_set_ui (&(interval->left), 0, MPFI_RNDD);
  mpfr_set_inf (&(interval->right), -1);
  mpfi_set (expected_left, interval);
  check (left, right, interval, expected_left, expected_right, 0);
  

  /* Signed zeros */

  mpfr_set_nan (&(left->left));
  mpfr_set_nan (&(left->right));
  mpfi_set (right, left);
  /* [+0, 12345678] -> [+0, 6172839] u [6172839, 12345678] exactly */
  mpfr_set_ui (&(interval->left), 0, MPFI_RNDD);
  mpfr_set_ui (&(interval->right), 12345678, MPFI_RNDU);
  mpfr_set_ui (&(expected_left->left), 0, MPFI_RNDD);
  mpfr_set_ui (&(expected_left->right), 6172839, MPFI_RNDU);
  mpfr_set_ui (&(expected_right->left), 6172839, MPFI_RNDD);
  mpfr_set_ui (&(expected_right->right), 12345678, MPFI_RNDU);
  check (left, right, interval, expected_left, expected_right, 0);

  mpfr_set_nan (&(left->left));
  mpfr_set_nan (&(left->right));
  mpfi_set (right, left);
  /* [+0, -0] -> [+0, -0] u [+0, -0] */
  mpfr_set_ui (&(interval->left), 0, MPFI_RNDU);
  mpfr_set_ui (&(interval->right), 0, MPFI_RNDU);
  mpfr_neg (&(interval->right), &(interval->right), MPFI_RNDD);
  mpfi_set (expected_left, interval);
  mpfi_set (expected_right, interval);
  check (left, right, interval, expected_left, expected_right, 0);

  mpfr_set_nan (&(left->left));
  mpfr_set_nan (&(left->right));
  mpfi_set (right, left);
  /* [-12345678, -0] -> [-12345678, -6172839] u [-6172839, -0] exactly */
  mpfr_set_si (&(interval->left), -12345678, MPFI_RNDD);
  mpfr_set_ui (&(interval->right), 0, MPFI_RNDU);
  mpfr_neg (&(interval->right), &(interval->right), MPFI_RNDD);
  mpfr_set_si (&(expected_left->left), -12345678, MPFI_RNDD);
  mpfr_set_si (&(expected_left->right), -6172839, MPFI_RNDU);
  mpfr_set_si (&(expected_right->left), -6172839, MPFI_RNDD);
  mpfr_set_ui (&(expected_right->right), 0, MPFI_RNDU);
  mpfr_neg (&(expected_right->right), &(expected_right->right), MPFI_RNDD);
  check (left, right, interval, expected_left, expected_right, 0);

  mpfr_set_nan (&(left->left));
  mpfr_set_nan (&(left->right));
  mpfi_set (right, left);
  /* [-123456, 123456] -> [-123456, -0] u [+0, 123456] */
  mpfr_set_si (&(interval->left), -123456, MPFI_RNDD);
  mpfr_set_ui (&(interval->right), 123456, MPFI_RNDU);
  mpfr_set_si (&(expected_left->left), -123456, MPFI_RNDD);
  mpfr_set_ui (&(expected_left->right), 0, MPFI_RNDU);
  mpfr_neg (&(expected_left->right), &(expected_left->right), MPFI_RNDD);
  mpfr_set_ui (&(expected_right->left), 0, MPFI_RNDU);
  mpfr_set_ui (&(expected_right->right), 123456, MPFI_RNDU);
  check (left, right, interval, expected_left, expected_right, 0);

  mpfi_set_prec (left, 4);
  mpfi_set_prec (right, 4);
  mpfi_set_prec (expected_left, 4);
  mpfi_set_prec (expected_right, 4);
  mpfr_set_nan (&(left->left));
  mpfr_set_nan (&(left->right));
  mpfi_set (right, left);
  /* [+0, 12345678] -> [+0, 6291456] u [5767168, 12582912] */
  mpfr_set_ui (&(interval->left), 0, MPFI_RNDU);
  mpfr_set_ui (&(interval->right), 12345678, MPFI_RNDU);
  mpfr_set_ui (&(expected_left->left), 0, MPFI_RNDU);
  mpfr_set_ui (&(expected_left->right), 0x6p+20, MPFI_RNDU);
  mpfr_set_ui (&(expected_right->left), 0x58p+16, MPFI_RNDD);
  mpfr_set_ui (&(expected_right->right), 0xcp+20, MPFI_RNDU);
  check (left, right, interval, expected_left, expected_right, 0);

  mpfi_clear (interval);
  mpfi_clear (left);
  mpfi_clear (right);
  mpfi_clear (expected_left);
  mpfi_clear (expected_right);
}

static void
check_regular (void)
{
  mpfi_t left, right, interval;
  mpfi_t expected_left, expected_right;

  mpfi_init2 (interval, 53);
  mpfi_init2 (left, 53);
  mpfi_init2 (right, 53);
  mpfi_init2 (expected_left, 53);
  mpfi_init2 (expected_right, 53);

  mpfr_set_nan (&(left->left));
  mpfr_set_nan (&(left->right));
  mpfi_set (right, left);
  /* [2, 2^53] -> [2, 2^52+1] u [2^52+1, 2^53] */
  mpfr_set_ui (&(interval->left), 2, MPFI_RNDD);
  mpfr_set_ui_2exp (&(interval->right), 1, 53, MPFI_RNDU);
  mpfr_set_ui (&(expected_left->left), 2, MPFI_RNDD);
  mpfr_set_ui_2exp (&(expected_left->right), 1, 52, MPFI_RNDU);
  mpfr_add_ui (&(expected_left->right), &(expected_left->right), 1,
               MPFI_RNDU);
  mpfr_set (&(expected_right->left), &(expected_left->right), MPFI_RNDD);
  mpfr_set_ui_2exp (&(expected_right->right), 1, 53, MPFI_RNDU);
  check (left, right, interval, expected_left, expected_right, 0);

  /* double rounding error */
  mpfi_set_prec (left, 52);
  mpfi_set_prec (right, 52);
  mpfi_set_prec (expected_left, 52);
  mpfi_set_prec (expected_right, 52);
  mpfr_set_nan (&(left->left));
  mpfr_set_nan (&(left->right));
  mpfi_set (right, left);
  /* [1, 2^54] -> [1, 2^53] u [2^53, 2^54]
     should be  [1, 2^53+2] u [2^53, 2^54]*/
  mpfr_set_ui (&(interval->left), 1, MPFI_RNDD);
  mpfr_set_ui_2exp (&(interval->right), 1, 54, MPFI_RNDU);
  mpfr_set_ui (&(expected_left->left), 1, MPFI_RNDD);
  mpfr_set_ui_2exp (&(expected_left->right), 1, 53, MPFI_RNDU);
  mpfr_set_ui_2exp (&(expected_right->left), 1, 53, MPFI_RNDD);
  mpfr_set_ui_2exp (&(expected_right->right), 1, 54, MPFI_RNDU);
  check (left, right, interval, expected_left, expected_right, -1);

  /* double rounding error */
  mpfi_set_prec (interval, 2);
  mpfi_set_prec (left, 53);
  mpfi_set_prec (right, 2);
  mpfi_set_prec (expected_left, 53);
  mpfi_set_prec (expected_right, 2);
  mpfr_set_nan (&(left->left));
  mpfr_set_nan (&(left->right));
  mpfi_set (right, left);
  /* [1, 2^53] -> [1, 2^52] u [2^52, 2^53]
     should be  [1, 2^52+1] u [2^52, 2^53]*/
  mpfr_set_ui (&(interval->left), 1, MPFI_RNDD);
  mpfr_set_ui_2exp (&(interval->right), 1, 53, MPFI_RNDU);
  mpfr_set_ui (&(expected_left->left), 1, MPFI_RNDD);
  mpfr_set_ui_2exp (&(expected_left->right), 1, 52, MPFI_RNDU);
  mpfr_set_ui_2exp (&(expected_right->left), 1, 52, MPFI_RNDD);
  mpfr_set_ui_2exp (&(expected_right->right), 1, 53, MPFI_RNDU);
  check (left, right, interval, expected_left, expected_right, -1);

  /* double rounding error */
  mpfi_set_prec (interval, 2);
  mpfi_set_prec (left, 2);
  mpfi_set_prec (right, 3);
  mpfi_set_prec (expected_left, 2);
  mpfi_set_prec (expected_right, 3);
  mpfr_set_nan (&(left->left));
  mpfr_set_nan (&(left->right));
  mpfi_set (right, left);
  /* [-1, 2^4] -> [1, 2^3] u [2^3, 2^4]
        should be [1, 2^3] u [2^3-1, 2^4]*/
  mpfr_set_si (&(interval->left), -1, MPFI_RNDD);
  mpfr_set_ui_2exp (&(interval->right), 1, 4, MPFI_RNDU);
  mpfr_set_si (&(expected_left->left), -1, MPFI_RNDD);
  mpfr_set_ui_2exp (&(expected_left->right), 1, 3, MPFI_RNDU);
  mpfr_set_ui_2exp (&(expected_right->left), 1, 3, MPFI_RNDD);
  mpfr_set_ui_2exp (&(expected_right->right), 1, 4, MPFI_RNDU);
  check (left, right, interval, expected_left, expected_right, +1);

  mpfi_clear (interval);
  mpfi_clear (left);
  mpfi_clear (right);
  mpfi_clear (expected_left);
  mpfi_clear (expected_right);
}

int
main (int argc, char **argv)
{
  test_start ();

  check_special ();
  check_regular ();

  test_end ();

  return 0;
}
