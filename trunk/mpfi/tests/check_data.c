/* check_data.c -- Tests from data file.

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

#include <float.h>
#include <string.h>
#include "mpfi-tests.h"

extern char *pathname;
extern unsigned long line_number;
extern int nextchar;


/*
 * read expected result and operands from file
 */

static void
read_line_ii (mpfi_function_ptr this, FILE* fp)
{
  /* [1] return value */
  read_exactness (fp, &(MPFI_FUN_ARG(*this, 1, i)));
  /* [2] expected value */
  read_mpfi (fp, MPFI_FUN_ARG(*this, 2, mpfi));
  /* [3] operand */
  read_mpfi (fp, MPFI_FUN_ARG(*this, 3, mpfi));
}

static void
read_line_iii (mpfi_function_ptr this, FILE* fp)
{
  /* [1] return value */
  read_exactness (fp, &(MPFI_FUN_ARG(*this, 1, i)));
  /* [2] expected value */
  read_mpfi (fp, MPFI_FUN_ARG(*this, 2, mpfi));
  /* [3] first operand */
  read_mpfi (fp, MPFI_FUN_ARG(*this, 3, mpfi));
  /* [4] second operand */
  read_mpfi (fp, MPFI_FUN_ARG(*this, 4, mpfi));
}

static void
read_line_iu (mpfi_function_ptr this, FILE* fp)
{
  /* [0] initial value */
  read_mpfi (fp, MPFI_FUN_ARG(*this, 0, mpfi));
  /* [1] return value */
  read_exactness (fp, &(MPFI_FUN_ARG(*this, 1, i)));
  /* [2] expected value */
  read_mpfi (fp, MPFI_FUN_ARG(*this, 2, mpfi));
  /* [3] unsigned integral operand */
  read_ui (fp, &(MPFI_FUN_ARG(*this, 3, ui)));

  /* data validation */
  if (mpfi_get_prec (MPFI_FUN_ARG(*this, 0, mpfi)) != mpfi_get_prec (MPFI_FUN_ARG(*this, 2, mpfi))) {
    printf ("Error in data file %s line %lu\nThe precisions of interval "
            "are different.\n", pathname, line_number - 1);
    exit (1);
  }
}

static void
read_line_is (mpfi_function_ptr this, FILE* fp)
{
  /* [0] initial value */
  read_mpfi (fp, MPFI_FUN_ARG(*this, 0, mpfi));
  /* [1] return value */
  read_exactness (fp, &(MPFI_FUN_ARG(*this, 1, i)));
  /* [2] expected value */
  read_mpfi (fp, MPFI_FUN_ARG(*this, 2, mpfi));
  /* [3] unsigned integral operand */
  read_si (fp, &(MPFI_FUN_ARG(*this, 3, si)));

  /* data validation */
  if (mpfi_get_prec (MPFI_FUN_ARG(*this, 0, mpfi)) != mpfi_get_prec (MPFI_FUN_ARG(*this, 2, mpfi))) {
    printf ("Error in data file %s line %lu\nThe precisions of interval "
            "are different.\n", pathname, line_number - 1);
    exit (1);
  }
}

static void
read_line_id (mpfi_function_ptr this, FILE* fp)
{
  /* [0] initial value */
  read_mpfi (fp, MPFI_FUN_ARG(*this, 0, mpfi));
  /* [1] return value */
  read_exactness (fp, &(MPFI_FUN_ARG(*this, 1, i)));
  /* [2] expected value */
  read_mpfi (fp, MPFI_FUN_ARG(*this, 2, mpfi));
  /* [3] double operand */
  read_double (fp, &(MPFI_FUN_ARG(*this, 3, d)));

  /* data validation */
  if (mpfi_get_prec (MPFI_FUN_ARG(*this, 0, mpfi)) != mpfi_get_prec (MPFI_FUN_ARG(*this, 2, mpfi))) {
    printf ("Error in data file %s line %lu\nThe precisions of interval "
            "are different.\n", pathname, line_number - 1);
    exit (1);
  }
}

static void
read_line_iz (mpfi_function_ptr this, FILE* fp)
{
  /* [0] initial value */
  read_mpfi (fp, MPFI_FUN_ARG(*this, 0, mpfi));
  /* [1] return value */
  read_exactness (fp, &(MPFI_FUN_ARG(*this, 1, i)));
  /* [2] expected value */
  read_mpfi (fp, MPFI_FUN_ARG(*this, 2, mpfi));
  /* [3] mpz operand */
  read_mpz (fp, MPFI_FUN_ARG(*this, 3, mpz));

  /* data validation */
  if (mpfi_get_prec (MPFI_FUN_ARG(*this, 0, mpfi)) != mpfi_get_prec (MPFI_FUN_ARG(*this, 2, mpfi))) {
    printf ("Error in data file %s line %lu\nThe precisions of interval "
            "are different.\n", pathname, line_number - 1);
    exit (1);
  }
}

static void
read_line_iq (mpfi_function_ptr this, FILE* fp)
{
  /* [0] initial value */
  read_mpfi (fp, MPFI_FUN_ARG(*this, 0, mpfi));
  /* [1] return value */
  read_exactness (fp, &(MPFI_FUN_ARG(*this, 1, i)));
  /* [2] expected value */
  read_mpfi (fp, MPFI_FUN_ARG(*this, 2, mpfi));
  /* [3] mpq operand */
  read_mpq (fp, MPFI_FUN_ARG(*this, 3, mpq));

  /* data validation */
  if (mpfi_get_prec (MPFI_FUN_ARG(*this, 0, mpfi)) != mpfi_get_prec (MPFI_FUN_ARG(*this, 2, mpfi))) {
    printf ("Error in data file %s line %lu\nThe precisions of interval "
            "are different.\n", pathname, line_number - 1);
    exit (1);
  }
}

static void
read_line_ir (mpfi_function_ptr this, FILE* fp)
{
  /* [0] initial value */
  read_mpfi (fp, MPFI_FUN_ARG(*this, 0, mpfi));
  /* [1] return value */
  read_exactness (fp, &(MPFI_FUN_ARG(*this, 1, i)));
  /* [2] expected value */
  read_mpfi (fp, MPFI_FUN_ARG(*this, 2, mpfi));
  /* [3] mpfr operand */
  read_mpfr (fp, MPFI_FUN_ARG(*this, 3, mpfr));

  /* data validation */
  if (mpfi_get_prec (MPFI_FUN_ARG(*this, 0, mpfi)) != mpfi_get_prec (MPFI_FUN_ARG(*this, 2, mpfi))) {
    printf ("Error in data file %s line %lu\nThe precisions of interval "
            "are different.\n", pathname, line_number - 1);
    exit (1);
  }
}


/*
 * check function call result against test data
 */

/* when one (at least) endpoint of the result is exact, it can be
   used to check the function at a different precision. */
static void
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
              (unsigned long)prec, line_number - 1);
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
              (unsigned long)prec, line_number - 1);
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

static void
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
    printf ("Failed line %lu.\n", line_number - 1);
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
              line_number - 1);
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
              "\nop1 = ", line_number - 1);
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

static void
check_line_iu (mpfi_function_ptr this)
{
  int inex;

  inex = (MPFI_FUN_GET (*this, IU)) (MPFI_FUN_ARG(*this, 0, mpfi), MPFI_FUN_ARG(*this, 3, ui));
  if (inex != MPFI_FUN_ARG(*this, 1, i)
      || !same_value (MPFI_FUN_ARG(*this, 0, mpfi), MPFI_FUN_ARG(*this, 2, mpfi))) {
    printf ("Failed line %lu.\nop = %lu", line_number - 1, MPFI_FUN_ARG(*this, 3, ui));
    printf ("\ngot      = ");
    mpfi_out_str (stdout, 16, 0, MPFI_FUN_ARG(*this, 0, mpfi));
    printf ("\nexpected = ");
    mpfi_out_str (stdout, 16, 0, MPFI_FUN_ARG(*this, 2, mpfi));
    putchar ('\n');
    if (inex != MPFI_FUN_ARG(*this, 1, i))
      printf ("inexact flag: got = %u, expected = %u\n",
              inex, MPFI_FUN_ARG(*this, 1, i));

    exit (1);
  }
}

static void
check_line_is (mpfi_function_ptr this)
{
  int inex;
  inex = (MPFI_FUN_GET (*this, IS)) (MPFI_FUN_ARG(*this, 0, mpfi), MPFI_FUN_ARG(*this, 3, si));
  if (inex != MPFI_FUN_ARG(*this, 1, i)
      || !same_value (MPFI_FUN_ARG(*this, 0, mpfi), MPFI_FUN_ARG(*this, 2, mpfi))) {
    printf ("Failed line %lu.\nop = %ld", line_number - 1, MPFI_FUN_ARG(*this, 3, si));
    printf ("\ngot      = ");
    mpfi_out_str (stdout, 16, 0, MPFI_FUN_ARG(*this, 0, mpfi));
    printf ("\nexpected = ");
    mpfi_out_str (stdout, 16, 0, MPFI_FUN_ARG(*this, 2, mpfi));
    putchar ('\n');
    if (inex != MPFI_FUN_ARG(*this, 1, i))
      printf ("inexact flag: got = %u, expected = %u\n",
              inex, MPFI_FUN_ARG(*this, 1, i));

    exit (1);
  }
}

static void
check_line_id (mpfi_function_ptr this)
{
  int inex;

  inex = (MPFI_FUN_GET (*this, ID)) (MPFI_FUN_ARG(*this, 0, mpfi), MPFI_FUN_ARG(*this, 3, d));
  if (inex != MPFI_FUN_ARG(*this, 1, i)
      || !same_value (MPFI_FUN_ARG(*this, 0, mpfi), MPFI_FUN_ARG(*this, 2, mpfi))) {
    printf ("Failed line %lu.\nop = %a", line_number - 1, MPFI_FUN_ARG(*this, 3, d));
    printf ("\ngot      = ");
    mpfi_out_str (stdout, 16, 0, MPFI_FUN_ARG(*this, 0, mpfi));
    printf ("\nexpected = ");
    mpfi_out_str (stdout, 16, 0, MPFI_FUN_ARG(*this, 2, mpfi));
    putchar ('\n');
    if (inex != MPFI_FUN_ARG(*this, 1, i))
      printf ("inexact flag: got = %u, expected = %u\n",
              inex, MPFI_FUN_ARG(*this, 1, i));

    exit (1);
  }
}


static void
check_line_iz (mpfi_function_ptr this)
{
  int inex;

  inex = (MPFI_FUN_GET (*this, IZ)) (MPFI_FUN_ARG(*this, 0, mpfi), MPFI_FUN_ARG(*this, 3, mpz));
  if (inex != MPFI_FUN_ARG(*this, 1, i)
      || !same_value (MPFI_FUN_ARG(*this, 0, mpfi), MPFI_FUN_ARG(*this, 2, mpfi))) {
    printf ("Failed line %lu.\nop = ", line_number - 1);
    mpz_out_str (stdout, 16, MPFI_FUN_ARG(*this, 3, mpz));
    printf ("\ngot      = ");
    mpfi_out_str (stdout, 16, 0, MPFI_FUN_ARG(*this, 0, mpfi));
    printf ("\nexpected = ");
    mpfi_out_str (stdout, 16, 0, MPFI_FUN_ARG(*this, 2, mpfi));
    putchar ('\n');
    if (inex != MPFI_FUN_ARG(*this, 1, i))
      printf ("inexact flag: got = %u, expected = %u\n",
              inex, MPFI_FUN_ARG(*this, 1, i));

    exit (1);
  }
}

static void
check_line_iq (mpfi_function_ptr this)
{
  int inex;

  inex = (MPFI_FUN_GET (*this, IQ)) (MPFI_FUN_ARG(*this, 0, mpfi), MPFI_FUN_ARG(*this, 3, mpq));
  if (inex != MPFI_FUN_ARG(*this, 1, i)
      || !same_value (MPFI_FUN_ARG(*this, 0, mpfi), MPFI_FUN_ARG(*this, 2, mpfi))) {
    printf ("Failed line %lu.\nop = ", line_number - 1);
    mpq_out_str (stdout, 16, MPFI_FUN_ARG(*this, 3, mpq));
    printf ("\ngot      = ");
    mpfi_out_str (stdout, 16, 0, MPFI_FUN_ARG(*this, 0, mpfi));
    printf ("\nexpected = ");
    mpfi_out_str (stdout, 16, 0, MPFI_FUN_ARG(*this, 2, mpfi));
    putchar ('\n');
    if (inex != MPFI_FUN_ARG(*this, 1, i))
      printf ("inexact flag: got = %u, expected = %u\n",
              inex, MPFI_FUN_ARG(*this, 1, i));

    exit (1);
  }
}

static void
check_line_ir (mpfi_function_ptr this)
{
  int inex;

  inex = (MPFI_FUN_GET (*this, IR)) (MPFI_FUN_ARG(*this, 0, mpfi), MPFI_FUN_ARG(*this, 3, mpfr));
  if (inex != MPFI_FUN_ARG(*this, 1, i)
      || !same_value (MPFI_FUN_ARG(*this, 0, mpfi), MPFI_FUN_ARG(*this, 2, mpfi))) {
    printf ("Failed line %lu.\nop = ", line_number - 1);
    mpfr_out_str (stdout, 16, 0, MPFI_FUN_ARG(*this, 3, mpfr), MPFI_RNDD);
    printf ("\ngot      = ");
    mpfi_out_str (stdout, 16, 0, MPFI_FUN_ARG(*this, 0, mpfi));
    printf ("\nexpected = ");
    mpfi_out_str (stdout, 16, 0, MPFI_FUN_ARG(*this, 2, mpfi));
    putchar ('\n');
    if (inex != MPFI_FUN_ARG(*this, 1, i))
      printf ("inexact flag: got = %u, expected = %u\n",
              inex, MPFI_FUN_ARG(*this, 1, i));

    exit (1);
  }
}


/*
 * initialization & clearing
 */

static void
clear_ii (mpfi_function_ptr this)
{
  /* [0] auxiliary variable (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG(*this, 0, mpfi));
  /* [1] return value (int), needs no deallocation */
  /* [2] expected value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG(*this, 2, mpfi));
  /* [3] operand (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG(*this, 3, mpfi));

  free (MPFI_FUN_ARGS (*this));
  MPFI_FUN_ARGS (*this) = NULL;
}

static void
clear_iii (mpfi_function_ptr this)
{
  /* [0] auxiliary variable (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG(*this, 0, mpfi));
  /* [1] return value (int), needs no deallocation */
  /* [2] expected value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG(*this, 2, mpfi));
  /* [3] first operand (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG(*this, 3, mpfi));
  /* [4] second operand (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG(*this, 4, mpfi));

  free (MPFI_FUN_ARGS (*this));
  MPFI_FUN_ARGS (*this) = NULL;
}

static void
clear_iu (mpfi_function_ptr this)
{
  /* [0] initial value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG(*this, 0, mpfi));
  /* [1] return value (int), needs no deallocation */
  /* [2] expected value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG(*this, 2, mpfi));
  /* [3] operand (unsigned long), needs no deallocation */

  free (MPFI_FUN_ARGS (*this));
  MPFI_FUN_ARGS (*this) = NULL;
}

static void
clear_is (mpfi_function_ptr this)
{
  /* [0] initial value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG(*this, 0, mpfi));
  /* [1] return value (int), needs no deallocation */
  /* [2] expected value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG(*this, 2, mpfi));
  /* [3] operand (signed long), needs no deallocation */

  free (MPFI_FUN_ARGS (*this));
  MPFI_FUN_ARGS (*this) = NULL;
}

static void
clear_id (mpfi_function_ptr this)
{
  /* [0] initial value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG(*this, 0, mpfi));
  /* [1] return value (int), needs no deallocation */
  /* [2] expected value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG(*this, 2, mpfi));
  /* [3] operand (double), needs no deallocation */

  free (MPFI_FUN_ARGS (*this));
  MPFI_FUN_ARGS (*this) = NULL;
}

static void
clear_iz (mpfi_function_ptr this)
{
  /* [0] initial value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG(*this, 0, mpfi));
  /* [1] return value (int), needs no deallocation */
  /* [2] expected value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG(*this, 2, mpfi));
  /* [3] operand (mpz_t) */
  mpz_clear (MPFI_FUN_ARG(*this, 3, mpz));

  free (MPFI_FUN_ARGS (*this));
  MPFI_FUN_ARGS (*this) = NULL;
}

static void
clear_iq (mpfi_function_ptr this)
{
  /* [0] initial value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG(*this, 0, mpfi));
  /* [1] return value (int), needs no deallocation */
  /* [2] expected value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG(*this, 2, mpfi));
  /* [3] operand (mpz_t) */
  mpq_clear (MPFI_FUN_ARG(*this, 3, mpq));

  free (MPFI_FUN_ARGS (*this));
  MPFI_FUN_ARGS (*this) = NULL;
}

static void
clear_ir (mpfi_function_ptr this)
{
  /* [0] initial value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG(*this, 0, mpfi));
  /* [1] return value (int), needs no deallocation */
  /* [2] expected value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG(*this, 2, mpfi));
  /* [3] operand (mpfr_t) */
  mpfr_clear (MPFI_FUN_ARG(*this, 3, mpfr));

  free (MPFI_FUN_ARGS (*this));
  MPFI_FUN_ARGS (*this) = NULL;
}

/* Initializations, global function for all prototypes
   In operands array, variables are in the same order as for data in
   '.dat' files plus, when needed, one additional variable before them. */
static void
init (mpfi_function_ptr this)
{
  switch (MPFI_FUN_TYPE (*this))
    {
    case II:
      /* init operands */
      MPFI_FUN_ARGS (*this) =
        (mpfi_fun_operand_t*) malloc (4 * sizeof (mpfi_fun_operand_t));

      /* [0] auxiliary variable (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG(*this, 0, mpfi), 1024);
      /* [1] return value (int), needs no initialization */
      /* [2] expected value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG(*this, 2, mpfi), 1024);
      /* [3] operand (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG(*this, 3, mpfi), 1024);

      /* init methods */
      this->read_line  = read_line_ii;
      this->check_line = check_line_i;
      this->clear      = clear_ii;
      break;

    case III:
      MPFI_FUN_ARGS (*this) =
        (mpfi_fun_operand_t*) malloc (5 * sizeof (mpfi_fun_operand_t));

      /* [0] auxiliary variable (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG(*this, 0, mpfi), 1024);
      /* [1] return value (int), needs no initialization */
      /* [2] expected value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG(*this, 2, mpfi), 1024);
      /* [3] first operand (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG(*this, 3, mpfi), 1024);
      /* [4] second operand (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG(*this, 4, mpfi), 1024);

      /* init methods */
      this->read_line  = read_line_iii;
      this->check_line = check_line_i;
      this->clear      = clear_iii;
      break;

    case IU:
      MPFI_FUN_ARGS (*this) =
        (mpfi_fun_operand_t*) malloc (4 * sizeof (mpfi_fun_operand_t));

      /* [0] initial value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG(*this, 0, mpfi), 1024);
      /* [1] return value (int), needs no initialization */
      /* [2] expected value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG(*this, 2, mpfi), 1024);
      /* [3] operand (unsigned long), needs no initialization */

      /* init methods */
      this->read_line  = read_line_iu;
      this->check_line = check_line_iu;
      this->clear      = clear_iu;
      break;

    case IS:
      MPFI_FUN_ARGS (*this) =
        (mpfi_fun_operand_t*) malloc (4 * sizeof (mpfi_fun_operand_t));

      /* [0] initial value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG(*this, 0, mpfi), 1024);
      /* [1] return value (int), needs no initialization */
      /* [2] expected value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG(*this, 2, mpfi), 1024);
      /* [3] operand (signed long), needs no initialization */

      /* init methods */
      this->read_line  = read_line_is;
      this->check_line = check_line_is;
      this->clear      = clear_is;
      break;

    case ID:
      MPFI_FUN_ARGS (*this) =
        (mpfi_fun_operand_t*) malloc (4 * sizeof (mpfi_fun_operand_t));

      /* [0] initial value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG(*this, 0, mpfi), 1024);
      /* [1] return value (int), needs no initialization */
      /* [2] expected value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG(*this, 2, mpfi), 1024);
      /* [3] operand (double), needs no initialization */

      /* init methods */
      this->read_line  = read_line_id;
      this->check_line = check_line_id;
      this->clear      = clear_id;
      break;

    case IZ:
      MPFI_FUN_ARGS (*this) =
        (mpfi_fun_operand_t*) malloc (4 * sizeof (mpfi_fun_operand_t));

      /* [0] initial value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG(*this, 0, mpfi), 1024);
      /* [1] return value (int), needs no initialization */
      /* [2] expected value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG(*this, 2, mpfi), 1024);
      /* [3] operand (mpz_t) */
      mpz_init (MPFI_FUN_ARG(*this, 3, mpz));

      /* init methods */
      this->read_line  = read_line_iz;
      this->check_line = check_line_iz;
      this->clear      = clear_iz;
      break;

    case IQ:
      MPFI_FUN_ARGS (*this) =
        (mpfi_fun_operand_t*) malloc (4 * sizeof (mpfi_fun_operand_t));

      /* [0] initial value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG(*this, 0, mpfi), 1024);
      /* [1] return value (int), needs no initialization */
      /* [2] expected value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG(*this, 2, mpfi), 1024);
      /* [3] operand (mpq_t) */
      mpq_init (MPFI_FUN_ARG(*this, 3, mpq));

      /* init methods */
      this->read_line  = read_line_iq;
      this->check_line = check_line_iq;
      this->clear      = clear_iq;
      break;

    case IR:
      MPFI_FUN_ARGS (*this) =
        (mpfi_fun_operand_t*) malloc (4 * sizeof (mpfi_fun_operand_t));

      /* [0] initial value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG(*this, 0, mpfi), 1024);
      /* [1] return value (int), needs no initialization */
      /* [2] expected value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG(*this, 2, mpfi), 1024);
      /* [3] operand (mpq_t) */
      mpfr_init2 (MPFI_FUN_ARG(*this, 3, mpfr), 1024);

      /* init methods */
      this->read_line  = read_line_ir;
      this->check_line = check_line_ir;
      this->clear      = clear_ir;
      break;

    default:
      printf ("init not implemented (yet)!\n");
      exit (1);
    }
}

/* public main function */

void
check_data (mpfi_function_ptr function, const char *file_name)
{
  FILE *fp;

  fp = open_file (file_name);
  init_reading (fp);
  init (function);

  while (nextchar != EOF) {
    function->read_line (function, fp);
    function->check_line (function);
  }

  function->clear (function);
  close_file (fp);
}
