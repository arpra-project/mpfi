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

/* when one (at least) endpoint of the result is exact, it can be
   used to check the function at a different precision. */

static void
check_with_different_prec (mpfi_function_t function, mpfi_srcptr expected,
                           mpfi_srcptr op1, mpfi_srcptr op2,
                           int expected_inex, mp_prec_t prec)
{
  mpfi_t got;
  mpfr_t x;
  int inex;

  mpfr_init2 (x, prec);
  mpfi_init2 (got, prec);

  if (MPFI_FUN_TYPE (function) == II) {
    inex = (MPFI_FUN_GET (function, II)) (got, op1);
  }
  else {
    inex = (MPFI_FUN_GET (function, III)) (got, op1, op2);
  }

  if (!MPFI_LEFT_IS_INEXACT (expected_inex)) {
    mpfr_set (x, &(expected->left), MPFI_RNDD);
    if (!same_mpfr_value (x, &(got->left))) {
      printf ("Error at precision = %lu (line %lu).\nop1 = ",
              (unsigned long)prec, line_number - 1);
      mpfi_out_str (stdout, 16, 0, op1);
      if (MPFI_FUN_TYPE (function) == II) {
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
      printf ("Error at precision = %lu (line %lu).\nop1 = ",
              (unsigned long)prec, line_number - 1);
      mpfi_out_str (stdout, 16, 0, op1);
      if (MPFI_FUN_TYPE (function) == II) {
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
  mpfi_clear (got);
}

/* check_data_integer:
   read  data in the given file and compare them with
   result of the given function. */
static void
check_data_integer (mpfi_function_t function, FILE *file)
{
  int expected_inex, inex;
  mpfi_t expected, got;
  mpfi_tests_integer op;

  mpfi_init (expected);
  mpfi_init (got);

  line_number = 1;
  nextchar = getc (file);
  skip_whitespace_comments (file);

  while (nextchar != EOF) {
    read_mpfi (file, got);
    read_exactness (file, &expected_inex);
    read_mpfi (file, expected);
    read_integer (file, &op, (MPFI_FUN_TYPE (function) == IS));

    /* data validation */
    if (mpfi_get_prec (got) != mpfi_get_prec (expected)) {
      printf ("Error in data file %s line %lu\nThe precisions of interval "
              "are different.\n", pathname, line_number - 1);
      exit (1);
    }
    if (MPFI_FUN_TYPE (function) == IS)
      inex = (MPFI_FUN_GET (function, IS)) (got, op.si);
    else
      inex = (MPFI_FUN_GET (function, IU)) (got, op.ui);

    if (inex != expected_inex || !same_value (got, expected)) {
      printf ("Failed line %lu.\nop = ", line_number - 1);
      if (MPFI_FUN_TYPE (function) == IS)
        printf ("%ld", op.si);
      else
        printf ("%lu", op.ui);
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
  mpfi_clear (expected);
  mpfi_clear (got);
}

/* check_data_id:
   read  data in the given file and compare them with
   result of the given function. */
static void
check_data_id (mpfi_function_t function, FILE *file)
{
  int expected_inex, inex;
  mpfi_t expected, got;
  double op;


  mpfi_init (expected);
  mpfi_init (got);

  line_number = 1;
  nextchar = getc (file);
  skip_whitespace_comments (file);

  while (nextchar != EOF) {
    read_mpfi (file, got);
    read_exactness (file, &expected_inex);
    read_mpfi (file, expected);
    if (read_double (file, &op)) {
      /* double cannot be read */
      continue;
    }

    /* data validation */
    if (mpfi_get_prec (got) != mpfi_get_prec (expected)) {
      printf ("Error in data file %s line %lu\nThe precisions of interval "
              "are different.\n", pathname, line_number - 1);
      exit (1);
    }

    inex = (MPFI_FUN_GET (function, ID)) (got, op);

    if (inex != expected_inex || !same_value (got, expected)) {
      printf ("Failed line %lu.\nop = %a", line_number - 1, op);
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
  mpfi_clear (expected);
  mpfi_clear (got);
}

/* check_data_iz:
   read  data in the given file and compare them with
   result of the given function. */
static void
check_data_iz (mpfi_function_t function, FILE *file)
{
  int expected_inex, inex;
  mpfi_t expected, got;
  mpz_t op;


  mpfi_init (expected);
  mpfi_init (got);
  mpz_init (op);

  line_number = 1;
  nextchar = getc (file);
  skip_whitespace_comments (file);

  while (nextchar != EOF) {
    read_mpfi (file, got);
    read_exactness (file, &expected_inex);
    read_mpfi (file, expected);
    read_mpz (file, op);

    /* data validation */
    if (mpfi_get_prec (got) != mpfi_get_prec (expected)) {
      printf ("Error in data file %s line %lu\nThe precisions of interval "
              "are different.\n", pathname, line_number - 1);
      exit (1);
    }

    inex = (MPFI_FUN_GET (function, IZ)) (got, op);

    if (inex != expected_inex || !same_value (got, expected)) {
      printf ("Failed line %lu.\nop = ", line_number - 1);
      mpz_out_str (stdout, 16, op);
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
  mpfi_clear (expected);
  mpfi_clear (got);
  mpz_clear (op);
}

/* check_data_iq:
   read  data in the given file and compare them with
   result of the given function. */
static void
check_data_iq (mpfi_function_t function, FILE *file)
{
  int expected_inex, inex;
  mpfi_t expected, got;
  mpq_t op;


  mpfi_init (expected);
  mpfi_init (got);
  mpq_init (op);

  line_number = 1;
  nextchar = getc (file);
  skip_whitespace_comments (file);

  while (nextchar != EOF) {
    read_mpfi (file, got);
    read_exactness (file, &expected_inex);
    read_mpfi (file, expected);
    read_mpq (file, op);

    /* data validation */
    if (mpfi_get_prec (got) != mpfi_get_prec (expected)) {
      printf ("Error in data file %s line %lu\nThe precisions of interval "
              "are different.\n", pathname, line_number - 1);
      exit (1);
    }

    inex = (MPFI_FUN_GET (function, IQ)) (got, op);

    if (inex != expected_inex || !same_value (got, expected)) {
      printf ("Failed line %lu.\nop = ", line_number - 1);
      mpq_out_str (stdout, 16, op);
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
  mpfi_clear (expected);
  mpfi_clear (got);
  mpq_clear (op);
}

/* check_data_ir:
   read  data in the given file and compare them with
   result of the given function. */
static void
check_data_ir (mpfi_function_t function, FILE *file)
{
  int expected_inex, inex;
  mpfi_t expected, got;
  mpfr_t op;


  mpfi_init (expected);
  mpfi_init (got);
  mpfr_init (op);

  line_number = 1;
  nextchar = getc (file);
  skip_whitespace_comments (file);

  while (nextchar != EOF) {
    read_mpfi (file, got);
    read_exactness (file, &expected_inex);
    read_mpfi (file, expected);
    read_mpfr (file, op);

    /* data validation */
    if (mpfi_get_prec (got) != mpfi_get_prec (expected)) {
      printf ("Error in data file %s line %lu\nThe precisions of interval "
              "are different.\n", pathname, line_number - 1);
      exit (1);
    }

    inex = (MPFI_FUN_GET (function, IR)) (got, op);

    if (inex != expected_inex || !same_value (got, expected)) {
      printf ("Failed line %lu.\nop = ", line_number - 1);
      mpfr_out_str (stdout, 16, 0, op, MPFI_RNDD);
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
  mpfi_clear (expected);
  mpfi_clear (got);
  mpfr_clear (op);
}

/* check_data_i:
   read data in the given file and compare them with result of the
   given function.
   handle functions with only intervals as input. */

static void
check_data_i (mpfi_function_t function, FILE *file)
{
  int expected_inex, inex;
  mpfi_t expected, got;
  mpfi_t op1, op2;

  mpfi_init (expected);
  mpfi_init (got);
  mpfi_init (op1);

  /* WARNING: when function type is II, op2 is unused but needed for the
     check_with_different_prec call below. */
  if (MPFI_FUN_TYPE (function) == III) {
    mpfi_init (op2);
  }

  line_number = 1;
  nextchar = getc (file);
  skip_whitespace_comments (file);

  while (nextchar != EOF) {
    read_exactness (file, &expected_inex);
    read_mpfi (file, expected);
    read_mpfi (file, op1);
    if (MPFI_FUN_TYPE (function) == III) {
      read_mpfi (file, op2);
    }

    /* check with given precision */
    mpfi_set_prec (got, mpfi_get_prec (expected));

    if (MPFI_FUN_TYPE (function) == III) {
      inex = (MPFI_FUN_GET (function, III)) (got, op1, op2);
    }
    else {
      inex = (MPFI_FUN_GET (function, II)) (got, op1);
    }

    if (inex != expected_inex || !same_value (got, expected)) {
      printf ("Failed line %lu.\nop1 = ", line_number - 1);
      mpfi_out_str (stdout, 16, 0, op1);
      if (MPFI_FUN_TYPE (function) == III) {
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
    if (!MPFI_BOTH_ARE_INEXACT (expected_inex)) {
      check_with_different_prec (function, expected, op1, op2, expected_inex,
                                 2);
      check_with_different_prec (function, expected, op1, op2, expected_inex,
                                 2 * mpfi_get_prec (expected));
    }

    /* reuse input variable as output (when they have the same precision) */
    if (mpfi_get_prec (got) == mpfi_get_prec (op1)) {
      mpfi_set (got, op1);

      if (MPFI_FUN_TYPE (function) == III) {
        inex = (MPFI_FUN_GET (function, III)) (got, got, op2);
      }
      else {
        inex = (MPFI_FUN_GET (function, II)) (got, got);
      }

      if (inex != expected_inex || !same_value (got, expected)) {
        printf ("Error when reusing%s input argument as output (line %lu)."
                "\nop1 = ", (MPFI_FUN_TYPE (function) == II ? " first" : ""),
                line_number - 1);
        mpfi_out_str (stdout, 16, 0, op1);
        if (MPFI_FUN_TYPE (function) == III) {
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

    if (MPFI_FUN_TYPE (function) == III
        && mpfi_get_prec (got) == mpfi_get_prec (op2)) {
      mpfi_set (got, op2);
      inex = (MPFI_FUN_GET (function, III)) (got, op1, got);

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

  mpfi_clear (expected);
  mpfi_clear (got);
  mpfi_clear (op1);
  if (MPFI_FUN_TYPE (function) == III) {
    mpfi_clear (op2);
  }
}

/* main function */

void
check_data (mpfi_function_t function, const char *file_name)
{
  FILE *fp;
  char *src_dir;
  char default_srcdir[] = ".";

  src_dir = getenv ("srcdir");
  if (src_dir == NULL)
    src_dir = default_srcdir;

  pathname = (char *) malloc ((strlen (src_dir)) + strlen (file_name) + 2);
  if (pathname == NULL) {
    printf ("Cannot allocate memory\n");
    exit (1);
  }
  sprintf (pathname, "%s/%s", src_dir, file_name);
  fp = fopen (pathname, "r");
  if (fp == NULL) {
    fprintf (stderr, "Unable to open %s\n", pathname);
    exit (1);
  }

  switch (MPFI_FUN_TYPE (function)) {
  case II:
  case III:
    check_data_i (function, fp);
    break;
  case IS:
  case IU:
    check_data_integer (function, fp);
    break;
  case ID:
    check_data_id (function, fp);
    break;
  case IZ:
    check_data_iz (function, fp);
    break;
  case IQ:
    check_data_iq (function, fp);
    break;
  case IR:
    check_data_ir (function, fp);
    break;
  }

  free (pathname);
  fclose (fp);
}
