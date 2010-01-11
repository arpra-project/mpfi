/* check_line.c -- Check actual result against data from a test file.

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

extern char *pathname;
extern unsigned long test_line_number;

/* when one (at least) endpoint of the result is exact, it can be
   used to check the function at a different precision. */
void
check_with_different_prec (mpfi_function_ptr function, mp_prec_t prec)
{
  mpfr_t x;

  /* rename operands for better readability */
  int type_II   = MPFI_FUN_TYPE (*function) == II;
  II_fun  f_II  = MPFI_FUN_GET (*function, II);
  III_fun f_III = MPFI_FUN_GET (*function, III);
  mpfi_ptr got      = MPFI_FUN_ARG (*function, 0, mpfi);
  int expected_inex = MPFI_FUN_ARG (*function, 1, i);
  mpfi_ptr expected = MPFI_FUN_ARG (*function, 2, mpfi);
  mpfi_ptr op1      = MPFI_FUN_ARG (*function, 3, mpfi);
  mpfi_ptr op2      = type_II ? NULL : MPFI_FUN_ARG (*function, 4, mpfi);


  mpfr_init2 (x, prec);
  mpfi_set_prec (got, prec);

  if (type_II)
    f_II (got, op1);
  else
    f_III (got, op1, op2);

  if (!MPFI_LEFT_IS_INEXACT (expected_inex)) {
    mpfr_set (x, &(expected->left), MPFI_RNDD);
    if (!same_mpfr_value (x, &(got->left))) {
      printf ("Error at precision = %lu (line %lu).\n",
              (unsigned long)prec, test_line_number);
      if (type_II) {
        printf ("op = ");
        mpfi_out_str (stdout, 16, 0, op1);
      }
      else {
        printf ("op1 = ");
        mpfi_out_str (stdout, 16, 0, op1);
        printf ("\nop2 = ");
        mpfi_out_str (stdout, 16, 0, op2);
      }
      printf ("\nleft endpoint: got = ");
      mpfr_out_str (stdout,  2, 0, &(got->left), MPFI_RNDD);
      printf ("\n          expected = ");
      mpfr_out_str (stdout,  2, 0, x, MPFI_RNDD);
      putchar ('\n');

      exit (1);
    }
  }

  if (!MPFI_RIGHT_IS_INEXACT (expected_inex)) {
    mpfr_set (x, &(expected->right), MPFI_RNDU);
    if (!same_mpfr_value (x, &(got->right))) {
      printf ("Error at precision = %lu (line %lu).\n",
              (unsigned long)prec, test_line_number);
      if (type_II) {
        printf ("op = ");
        mpfi_out_str (stdout, 16, 0, op1);
      }
      else {
        printf ("op1 = ");
        mpfi_out_str (stdout, 16, 0, op1);
        printf ("\nop2 = ");
        mpfi_out_str (stdout, 16, 0, op2);
      }
      printf ("\nright endpoint: got = ");
      mpfr_out_str (stdout,  2, 0, &(got->right), MPFI_RNDU);
      printf ("\n           expected = ");
      mpfr_out_str (stdout,  2, 0, x, MPFI_RNDU);
      putchar ('\n');

      exit (1);
    }
  }

  mpfr_clear (x);
}


/* check function against data at different precisions and test if an
   input variable can be reused as output */

void
check_line_i (mpfi_function_ptr function)
{
  int inex;

  /* rename operands for better readability
     [0]: value set by function
     [1]: return value (inexact flag)
     [2]: expected value
     [3]: first operand
     [4]: second operand (for III functions) */
  /* rename operands for better readability */
  int type_II   = MPFI_FUN_TYPE (*function) == II;
  II_fun  f_II  = MPFI_FUN_GET (*function, II);
  III_fun f_III = MPFI_FUN_GET (*function, III);
  mpfi_ptr got      = MPFI_FUN_ARG (*function, 0, mpfi);
  int expected_inex = MPFI_FUN_ARG (*function, 1, i);
  mpfi_ptr expected = MPFI_FUN_ARG (*function, 2, mpfi);
  mpfi_ptr op1      = MPFI_FUN_ARG (*function, 3, mpfi);
  mpfi_ptr op2      = type_II ? NULL : MPFI_FUN_ARG (*function, 4, mpfi);

  mpfi_set_prec (got, mpfi_get_prec (expected));

  if (type_II)
    inex = f_II (got, op1);
  else
    inex = f_III (got, op1, op2);

  if (inex != expected_inex || !same_value (got, expected)) {
    printf ("Failed line %lu.\n", test_line_number);
    if (type_II) {
      printf ("op = ");
      mpfi_out_str (stdout, 16, 0, op1);
    }
    else {
      printf ("op1 = ");
      mpfi_out_str (stdout, 16, 0, op1);
      printf ("\nop2 = ");
      mpfi_out_str (stdout, 16, 0, op2);
    }
    printf ("\ngot      = ");
    mpfi_out_str (stdout, 16, 0, got);
    printf ("\nexpected = ");
    mpfi_out_str (stdout, 16, 0, expected);
    putchar ('\n');
    if (inex != expected_inex)
      printf ("inexact flag: got = %u, expected = %u\n",
              inex, expected_inex);
    exit (1);
  }

  /* when one endpoint is exact, compute function at lower and higher
     precision */
  if (!MPFI_NAN_P (expected) && !MPFI_BOTH_ARE_INEXACT (inex)) {
    check_with_different_prec (function, 2);
    check_with_different_prec (function, 2 * mpfi_get_prec (expected));
    mpfi_set_prec (got, mpfi_get_prec (expected));
  }

  /* reuse input variable as output (when they have the same precision) */
  if (mpfi_get_prec (got) == mpfi_get_prec (op1)) {
    mpfi_set (got, op1);

    if (type_II)
      inex = f_II (got, got);
    else
      inex = f_III (got, got, op2);

    if (inex != expected_inex || !same_value (got, expected)) {
      printf ("Error when reusing%s input argument as output (line %lu)."
              "\nop1 = ", (type_II ? " first" : ""),
              test_line_number);
      mpfi_out_str (stdout, 16, 0, op1);
      if (!type_II) {
        printf ("\nop2 = ");
        mpfi_out_str (stdout, 16, 0, op2);
      }
      printf ("\ngot      = ");
      mpfi_out_str (stdout, 16, 0, got);
      printf ("\nexpected = ");
      mpfi_out_str (stdout, 16, 0, expected);
      putchar ('\n');
      if (inex != expected_inex)
        printf ("inexact flag: got = %u, expected = %u\n",
                inex, expected_inex);

      exit (1);
    }
  }

  if (!type_II && mpfi_get_prec (got) == mpfi_get_prec (op2)) {
    mpfi_set (got, op2);
    inex = f_III (got, op1, got);

    if (inex != expected_inex || !same_value (got, expected)) {
      printf ("Error when reusing second argument as output (line %lu)."
              "\nop1 = ", test_line_number);
      mpfi_out_str (stdout, 16, 0, op1);
      printf ("\nop2 = ");
      mpfi_out_str (stdout, 16, 0, op2);
      printf ("\ngot      = ");
      mpfi_out_str (stdout, 16, 0, got);
      printf ("\nexpected = ");
      mpfi_out_str (stdout, 16, 0, expected);
      putchar ('\n');
      if (inex != expected_inex)
        printf ("inexact flag: got = %u, expected = %u\n",
                inex, expected_inex);

      exit (1);
    }
  }
}

void
check_line_iu (mpfi_function_ptr this)
{
  int inex;

  inex = (MPFI_FUN_GET (*this, IU)) (MPFI_FUN_ARG (*this, 0, mpfi), MPFI_FUN_ARG (*this, 3, ui));
  if (inex != MPFI_FUN_ARG (*this, 1, i)
      || !same_value (MPFI_FUN_ARG (*this, 0, mpfi), MPFI_FUN_ARG (*this, 2, mpfi))) {
    printf ("Failed line %lu.\nop = %lu", test_line_number, MPFI_FUN_ARG (*this, 3, ui));
    printf ("\ngot      = ");
    mpfi_out_str (stdout, 16, 0, MPFI_FUN_ARG (*this, 0, mpfi));
    printf ("\nexpected = ");
    mpfi_out_str (stdout, 16, 0, MPFI_FUN_ARG (*this, 2, mpfi));
    putchar ('\n');
    if (inex != MPFI_FUN_ARG (*this, 1, i))
      printf ("inexact flag: got = %u, expected = %u\n",
              inex, MPFI_FUN_ARG (*this, 1, i));

    exit (1);
  }
}

void
check_line_is (mpfi_function_ptr this)
{
  int inex;
  inex = (MPFI_FUN_GET (*this, IS)) (MPFI_FUN_ARG (*this, 0, mpfi), MPFI_FUN_ARG (*this, 3, si));
  if (inex != MPFI_FUN_ARG (*this, 1, i)
      || !same_value (MPFI_FUN_ARG (*this, 0, mpfi), MPFI_FUN_ARG (*this, 2, mpfi))) {
    printf ("Failed line %lu.\nop = %ld", test_line_number, MPFI_FUN_ARG (*this, 3, si));
    printf ("\ngot      = ");
    mpfi_out_str (stdout, 16, 0, MPFI_FUN_ARG (*this, 0, mpfi));
    printf ("\nexpected = ");
    mpfi_out_str (stdout, 16, 0, MPFI_FUN_ARG (*this, 2, mpfi));
    putchar ('\n');
    if (inex != MPFI_FUN_ARG (*this, 1, i))
      printf ("inexact flag: got = %u, expected = %u\n",
              inex, MPFI_FUN_ARG (*this, 1, i));

    exit (1);
  }
}

void
check_line_id (mpfi_function_ptr this)
{
  int inex;

  inex = (MPFI_FUN_GET (*this, ID)) (MPFI_FUN_ARG (*this, 0, mpfi), MPFI_FUN_ARG (*this, 3, d));
  if (inex != MPFI_FUN_ARG (*this, 1, i)
      || !same_value (MPFI_FUN_ARG (*this, 0, mpfi), MPFI_FUN_ARG (*this, 2, mpfi))) {
    printf ("Failed line %lu.\nop = %a", test_line_number, MPFI_FUN_ARG (*this, 3, d));
    printf ("\ngot      = ");
    mpfi_out_str (stdout, 16, 0, MPFI_FUN_ARG (*this, 0, mpfi));
    printf ("\nexpected = ");
    mpfi_out_str (stdout, 16, 0, MPFI_FUN_ARG (*this, 2, mpfi));
    putchar ('\n');
    if (inex != MPFI_FUN_ARG (*this, 1, i))
      printf ("inexact flag: got = %u, expected = %u\n",
              inex, MPFI_FUN_ARG (*this, 1, i));

    exit (1);
  }
}

void
check_line_iz (mpfi_function_ptr this)
{
  int inex;

  inex = (MPFI_FUN_GET (*this, IZ)) (MPFI_FUN_ARG (*this, 0, mpfi), MPFI_FUN_ARG (*this, 3, mpz));
  if (inex != MPFI_FUN_ARG (*this, 1, i)
      || !same_value (MPFI_FUN_ARG (*this, 0, mpfi), MPFI_FUN_ARG (*this, 2, mpfi))) {
    printf ("Failed line %lu.\nop = ", test_line_number);
    mpz_out_str (stdout, 16, MPFI_FUN_ARG (*this, 3, mpz));
    printf ("\ngot      = ");
    mpfi_out_str (stdout, 16, 0, MPFI_FUN_ARG (*this, 0, mpfi));
    printf ("\nexpected = ");
    mpfi_out_str (stdout, 16, 0, MPFI_FUN_ARG (*this, 2, mpfi));
    putchar ('\n');
    if (inex != MPFI_FUN_ARG (*this, 1, i))
      printf ("inexact flag: got = %u, expected = %u\n",
              inex, MPFI_FUN_ARG (*this, 1, i));

    exit (1);
  }
}

void
check_line_iq (mpfi_function_ptr this)
{
  int inex;

  inex = (MPFI_FUN_GET (*this, IQ)) (MPFI_FUN_ARG (*this, 0, mpfi), MPFI_FUN_ARG (*this, 3, mpq));
  if (inex != MPFI_FUN_ARG (*this, 1, i)
      || !same_value (MPFI_FUN_ARG (*this, 0, mpfi), MPFI_FUN_ARG (*this, 2, mpfi))) {
    printf ("Failed line %lu.\nop = ", test_line_number);
    mpq_out_str (stdout, 16, MPFI_FUN_ARG (*this, 3, mpq));
    printf ("\ngot      = ");
    mpfi_out_str (stdout, 16, 0, MPFI_FUN_ARG (*this, 0, mpfi));
    printf ("\nexpected = ");
    mpfi_out_str (stdout, 16, 0, MPFI_FUN_ARG (*this, 2, mpfi));
    putchar ('\n');
    if (inex != MPFI_FUN_ARG (*this, 1, i))
      printf ("inexact flag: got = %u, expected = %u\n",
              inex, MPFI_FUN_ARG (*this, 1, i));

    exit (1);
  }
}

void
check_line_ir (mpfi_function_ptr this)
{
  int inex;

  inex = (MPFI_FUN_GET (*this, IR)) (MPFI_FUN_ARG (*this, 0, mpfi), MPFI_FUN_ARG (*this, 3, mpfr));
  if (inex != MPFI_FUN_ARG (*this, 1, i)
      || !same_value (MPFI_FUN_ARG (*this, 0, mpfi), MPFI_FUN_ARG (*this, 2, mpfi))) {
    printf ("Failed line %lu.\nop = ", test_line_number);
    mpfr_out_str (stdout, 16, 0, MPFI_FUN_ARG (*this, 3, mpfr), MPFI_RNDD);
    printf ("\ngot      = ");
    mpfi_out_str (stdout, 16, 0, MPFI_FUN_ARG (*this, 0, mpfi));
    printf ("\nexpected = ");
    mpfi_out_str (stdout, 16, 0, MPFI_FUN_ARG (*this, 2, mpfi));
    putchar ('\n');
    if (inex != MPFI_FUN_ARG (*this, 1, i))
      printf ("inexact flag: got = %u, expected = %u\n",
              inex, MPFI_FUN_ARG (*this, 1, i));

    exit (1);
  }
}

void
check_line_iiu (mpfi_function_ptr this)
{
  int inex;

  /* rename operands for better readability */
  IIU_fun  f_IIU    = MPFI_FUN_GET (*this, IIU);
  mpfi_ptr got      = MPFI_FUN_ARG (*this, 0, mpfi);
  int expected_inex = MPFI_FUN_ARG (*this, 1, i);
  mpfi_ptr expected = MPFI_FUN_ARG (*this, 2, mpfi);
  mpfi_ptr op1      = MPFI_FUN_ARG (*this, 3, mpfi);
  unsigned long op2 = MPFI_FUN_ARG (*this, 4, ui);

  mpfi_set_prec (got, mpfi_get_prec (expected));

  inex = f_IIU (got, op1, op2);

  if (inex != expected_inex || !same_value (got, expected)) {
    printf ("Failed line %lu.\nop1 = ", test_line_number);
    mpfi_out_str (stdout, 16, 0, op1);
    printf ("\nop2 = %lu", op2);
    printf ("\ngot      = ");
    mpfi_out_str (stdout, 16, 0, got);
    printf ("\nexpected = ");
    mpfi_out_str (stdout, 16, 0, expected);
    putchar ('\n');
    if (inex != expected_inex)
      printf ("inexact flag: got = %u, expected = %u\n", inex, expected_inex);

    exit (1);
  }

  /* reuse input variable as output */
  mpfi_set (got, op1);

  inex = f_IIU (got, got, op2);

  if (inex != expected_inex || !same_value (got, expected)) {
    printf ("Error when reusing first input argument as output (line %lu)."
            "\nop1 = ", test_line_number);
    mpfi_out_str (stdout, 16, 0, op1);
    printf ("\nop2 = %lu", op2);
    printf ("\ngot      = ");
    mpfi_out_str (stdout, 16, 0, got);
    printf ("\nexpected = ");
    mpfi_out_str (stdout, 16, 0, expected);
    putchar ('\n');
    if (inex != expected_inex)
      printf ("inexact flag: got = %u, expected = %u\n",
              inex, expected_inex);

    exit (1);
  }
}

void
check_line_iis (mpfi_function_ptr this)
{
  int inex;

  /* rename operands for better readability */
  IIS_fun  f_IIS    = MPFI_FUN_GET (*this, IIS);
  mpfi_ptr got      = MPFI_FUN_ARG (*this, 0, mpfi);
  int expected_inex = MPFI_FUN_ARG (*this, 1, i);
  mpfi_ptr expected = MPFI_FUN_ARG (*this, 2, mpfi);
  mpfi_ptr op1      = MPFI_FUN_ARG (*this, 3, mpfi);
  long op2          = MPFI_FUN_ARG (*this, 4, si);

  mpfi_set_prec (got, mpfi_get_prec (expected));

  inex = f_IIS (got, op1, op2);

  if (inex != expected_inex || !same_value (got, expected)) {
    printf ("Failed line %lu.\nop1 = ", test_line_number);
    mpfi_out_str (stdout, 16, 0, op1);
    printf ("\nop2 = %ld", op2);
    printf ("\ngot      = ");
    mpfi_out_str (stdout, 16, 0, got);
    printf ("\nexpected = ");
    mpfi_out_str (stdout, 16, 0, expected);
    putchar ('\n');
    if (inex != expected_inex)
      printf ("inexact flag: got = %u, expected = %u\n", inex, expected_inex);

    exit (1);
  }

  /* reuse input variable as output */
  mpfi_set (got, op1);

  inex = f_IIS (got, got, op2);

  if (inex != expected_inex || !same_value (got, expected)) {
    printf ("Error when reusing first input argument as output (line %lu)."
            "\nop1 = ", test_line_number);
    mpfi_out_str (stdout, 16, 0, op1);
    printf ("\nop2 = %ld", op2);
    printf ("\ngot      = ");
    mpfi_out_str (stdout, 16, 0, got);
    printf ("\nexpected = ");
    mpfi_out_str (stdout, 16, 0, expected);
    putchar ('\n');
    if (inex != expected_inex)
      printf ("inexact flag: got = %u, expected = %u\n",
              inex, expected_inex);

    exit (1);
  }
}

void
check_line_iid (mpfi_function_ptr this)
{
  int inex;

  /* rename operands for better readability */
  IID_fun  f_IID    = MPFI_FUN_GET (*this, IID);
  mpfi_ptr got      = MPFI_FUN_ARG (*this, 0, mpfi);
  int expected_inex = MPFI_FUN_ARG (*this, 1, i);
  mpfi_ptr expected = MPFI_FUN_ARG (*this, 2, mpfi);
  mpfi_ptr op1      = MPFI_FUN_ARG (*this, 3, mpfi);
  double op2        = MPFI_FUN_ARG (*this, 4, d);

  mpfi_set_prec (got, mpfi_get_prec (expected));

  inex = f_IID (got, op1, op2);

  if (inex != expected_inex || !same_value (got, expected)) {
    printf ("Failed line %lu.\nop1 = ", test_line_number);
    mpfi_out_str (stdout, 16, 0, op1);
    printf ("\nop2 = %g", op2);
    printf ("\ngot      = ");
    mpfi_out_str (stdout, 16, 0, got);
    printf ("\nexpected = ");
    mpfi_out_str (stdout, 16, 0, expected);
    putchar ('\n');
    if (inex != expected_inex)
      printf ("inexact flag: got = %u, expected = %u\n", inex, expected_inex);

    exit (1);
  }

  /* reuse input variable as output */
  mpfi_set (got, op1);

  inex = f_IID (got, got, op2);

  if (inex != expected_inex || !same_value (got, expected)) {
    printf ("Error when reusing first input argument as output (line %lu)."
            "\nop1 = ", test_line_number);
    mpfi_out_str (stdout, 16, 0, op1);
    printf ("\nop2 = %g", op2);
    printf ("\ngot      = ");
    mpfi_out_str (stdout, 16, 0, got);
    printf ("\nexpected = ");
    mpfi_out_str (stdout, 16, 0, expected);
    putchar ('\n');
    if (inex != expected_inex)
      printf ("inexact flag: got = %u, expected = %u\n",
              inex, expected_inex);

    exit (1);
  }
}

void
check_line_iiz (mpfi_function_ptr this)
{
  int inex;

  /* rename operands for better readability */
  IIZ_fun  f_IIZ    = MPFI_FUN_GET (*this, IIZ);
  mpfi_ptr got      = MPFI_FUN_ARG (*this, 0, mpfi);
  int expected_inex = MPFI_FUN_ARG (*this, 1, i);
  mpfi_ptr expected = MPFI_FUN_ARG (*this, 2, mpfi);
  mpfi_ptr op1      = MPFI_FUN_ARG (*this, 3, mpfi);
  mpz_ptr op2       = MPFI_FUN_ARG (*this, 4, mpz);

  mpfi_set_prec (got, mpfi_get_prec (expected));

  inex = f_IIZ (got, op1, op2);
  if (inex != expected_inex || !same_value (got, expected)) {
    printf ("Failed line %lu.\nop1 = ", test_line_number);
    mpfi_out_str (stdout, 16, 0, op1);
    printf ("\nop2 = ");
    mpz_out_str (stdout, 16, op2);
    printf ("\ngot      = ");
    mpfi_out_str (stdout, 16, 0, got);

    /* DEBUG: remove it */
    printf (" [prec=%lu]", mpfi_get_prec (got));
    /* DEBUG */

    printf ("\nexpected = ");
    mpfi_out_str (stdout, 16, 0, expected);
    putchar ('\n');
    if (inex != expected_inex)
      printf ("inexact flag: got = %u, expected = %u\n",
              inex, expected_inex);

    exit (1);
  }

  /* reuse input variable as output */
  mpfi_set (got, op1);

  inex = f_IIZ (got, got, op2);

  if (inex != expected_inex || !same_value (got, expected)) {
    printf ("Error when reusing first input argument as output (line %lu)."
            "\nop1 = ", test_line_number);
    mpfi_out_str (stdout, 16, 0, op1);
    printf ("\nop2 = ");
    mpz_out_str (stdout, 16, op2);
    printf ("\ngot      = ");
    mpfi_out_str (stdout, 16, 0, got);
    printf ("\nexpected = ");
    mpfi_out_str (stdout, 16, 0, expected);
    putchar ('\n');
    if (inex != expected_inex)
      printf ("inexact flag: got = %u, expected = %u\n",
              inex, expected_inex);

    exit (1);
  }
}

