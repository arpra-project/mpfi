/* read_data.c -- Tests from data file.

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

#include <string.h>
#include "mpfi-tests.h"

char *pathname;
unsigned long line_number; /* file name with complete path and currently read
                              line; kept globally to simplify parameter
                              passing */
int nextchar; /* character appearing next in the file, may be EOF */

/* comparisons: return true when arguments have the same value (even if both
   are NaN) */
int
same_mpfr_value (mpfr_ptr got, mpfr_ptr ref)
{
   if (mpfr_nan_p (got))
      return mpfr_nan_p (ref);
   if (mpfr_inf_p (got))
      return mpfr_inf_p (ref) && mpfr_signbit (got) == mpfr_signbit (ref);
   if (mpfr_zero_p (got))
      return mpfr_zero_p (ref) && mpfr_signbit (got) == mpfr_signbit (ref);
   return mpfr_cmp (got, ref) == 0;
}

int
same_value (mpfi_ptr a, mpfi_ptr b)
{
  return same_mpfr_value (&(a->left), &(b->left))
    && same_mpfr_value (&(a->right), &(b->right));
}


/* read primitives */

/* skips characters until reaching '\n' or EOF;
   '\n' is skipped as well  */
static void
skip_line (FILE *f)
{
  while (nextchar != EOF && nextchar != '\n')
    nextchar = getc (f);
  if (nextchar != EOF) {
    line_number++;
    nextchar = getc (f);
  }
}

/* skips over whitespace if any until reaching EOF or non-whitespace  */
static void
skip_whitespace (FILE *f)
{
  while (isspace (nextchar)) {
    if (nextchar == '\n')
      line_number++;
    nextchar = getc (f);
  }
}

/* skips over all whitespace and comments, if any */
void
skip_whitespace_comments (FILE *f)
{
  skip_whitespace (f);
  while (nextchar == '#') {
    skip_line (f);
    if (nextchar != EOF)
      skip_whitespace (f);
  }
}

static void
read_exactness (FILE *f, int *exactness)
{
  if (!isdigit (nextchar) || nextchar < '0' || nextchar > '4') {
    printf ("Error: unexpected exactness flag '%c' in file '%s' line %lu\n",
              nextchar, pathname, line_number);
      exit (1);
    }

  *exactness = nextchar - '0';
  nextchar = getc (f);
  if (!isspace(nextchar)) {
    printf ("Error: unexpected character '%c' after exactness flag"
            " in file '%s' line %lu\n", nextchar, pathname, line_number);
      exit (1);
    }

  skip_whitespace_comments (f);
}

static mpfr_prec_t
read_prec (FILE *f)
{
  unsigned long prec;
  int n;

  if (nextchar == EOF) {
    printf ("Error: Unexpected EOF when reading mpfr precision "
            "in file '%s' line %lu\n",
            pathname, line_number);
    exit (1);
  }
  ungetc (nextchar, f);
  n = fscanf (f, "%lu", &prec);
  if (ferror (f)) /* then also n == EOF */
    perror ("Error when reading precision");
  if (n == 0 || n == EOF || prec < MPFR_PREC_MIN || prec > MPFR_PREC_MAX) {
    printf ("Error: Impossible precision in file '%s' line %lu\n",
            pathname, line_number);
    exit (1);
  }
  nextchar = getc (f);
  skip_whitespace_comments (f);
  return (mpfr_prec_t) prec;
}

static void
read_mpq (FILE *f, mpq_ptr x)
{
   if (nextchar == EOF) {
      printf ("Error: Unexpected EOF when reading mpq number"
              "in file '%s' line %lu\n",
              pathname, line_number);
      exit (1);
   }
   ungetc (nextchar, f);
   if (mpq_inp_str (x, f, 0) == 0) {
      printf ("Error: Impossible to read mpq number "
              "in file '%s' line %lu\n",
              pathname, line_number);
      exit (1);
   }

   nextchar = getc (f);
   skip_whitespace_comments (f);
}

static void
read_mpfr_number (FILE *f, mpfr_ptr x)
{
   if (nextchar == EOF) {
      printf ("Error: Unexpected EOF when reading mpfr number"
              "in file '%s' line %lu\n",
              pathname, line_number);
      exit (1);
   }
   ungetc (nextchar, f);
   if (mpfr_inp_str (x, f, 0, GMP_RNDN) == 0) {
      printf ("Error: Impossible to read mpfr number "
              "in file '%s' line %lu\n",
              pathname, line_number);
      exit (1);
   }

   nextchar = getc (f);
   skip_whitespace_comments (f);
}

static void
read_mpfr (FILE *f, mpfr_ptr x)
{
   mpfr_set_prec (x, read_prec (f));
   read_mpfr_number (f, x);
}

static void
read_mpfi (FILE *f, mpfi_ptr a)
{
  mpfi_set_prec (a, read_prec (f));
  read_mpfr_number (f, &(a->left));
  read_mpfr_number (f, &(a->right));
  if (mpfr_cmp (&(a->left), &(a->right)) > 0)
    printf ("Warning: reverted endpoints line %lu\n", line_number - 1);
  if (mpfr_zero_p (&(a->left)) && mpfr_signbit (&(a->left)))
    printf ("Warning: -0 as lower bound line %lu\n", line_number - 1);
  if (mpfr_zero_p (&(a->right)) && !mpfr_signbit (&(a->right)))
    printf ("Warning: +0 in upper bound line %lu\n", line_number - 1);
}

/* when one (at least) endpoint of the result is exact, it can be
   used to check the function at a different precision. */

static void
check_with_different_prec (mpfi_function function, mpfi_srcptr expected,
                           mpfi_srcptr op1, mpfi_srcptr op2,
                           int expected_inex, mp_prec_t prec)
{
  mpfi_t got;
  mpfr_t x;
  int inex;

  mpfr_init2 (x, prec);
  mpfi_init2 (got, prec);

  if (MPFI_GET_TYPE (function) == II) {
    inex = (MPFI_GET_FUNCTION (function, II)) (got, op1);
  }
  else {
    inex = (MPFI_GET_FUNCTION (function, III)) (got, op1, op2);
  }

  if (!MPFI_LEFT_IS_INEXACT (expected_inex)) {
    mpfr_set (x, &(expected->left), MPFI_RNDD);
    if (!same_mpfr_value (x, &(got->left))) {
      printf ("Error at precision = %lu (line %lu).\nop1 = ",
              (unsigned long)prec, line_number - 1);
      mpfi_out_str (stdout, 16, 0, op1);
      if (MPFI_GET_TYPE (function) == II) {
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
      if (MPFI_GET_TYPE (function) == II) {
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

/* check_data_iq:
   read  data in the given file and compare them with
   result of the given function. */
static void
check_data_iq (mpfi_function function, FILE *file)
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
              "are different.\n", pathname, line_number);
      exit (1);
    }

    inex = (MPFI_GET_FUNCTION (function, IQ)) (got, op);

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
check_data_ir (mpfi_function function, FILE *file)
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
              "are different.\n", pathname, line_number);
      exit (1);
    }

    inex = (MPFI_GET_FUNCTION (function, IR)) (got, op);

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
check_data_i (mpfi_function function, FILE *file)
{
  int expected_inex, inex;
  mpfi_t expected, got;
  mpfi_t op1, op2;

  mpfi_init (expected);
  mpfi_init (got);
  mpfi_init (op1);

  /* WARNING: when function type is II, op2 is unused but needed for the
     check_with_different_prec call below. */
  if (MPFI_GET_TYPE (function) == III) {
    mpfi_init (op2);
  }

  line_number = 1;
  nextchar = getc (file);
  skip_whitespace_comments (file);

  while (nextchar != EOF) {
    read_exactness (file, &expected_inex);
    read_mpfi (file, expected);
    read_mpfi (file, op1);
    if (MPFI_GET_TYPE (function) == III) {
      read_mpfi (file, op2);
    }

    /* check with given precision */
    mpfi_set_prec (got, mpfi_get_prec (expected));

    if (MPFI_GET_TYPE (function) == III) {
      inex = (MPFI_GET_FUNCTION (function, III)) (got, op1, op2);
    }
    else {
      inex = (MPFI_GET_FUNCTION (function, II)) (got, op1);
    }

    if (inex != expected_inex || !same_value (got, expected)) {
      printf ("Failed line %lu.\nop1 = ", line_number - 1);
      mpfi_out_str (stdout, 16, 0, op1);
      if (MPFI_GET_TYPE (function) == III) {
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

      if (MPFI_GET_TYPE (function) == III) {
	inex = (MPFI_GET_FUNCTION (function, III)) (got, got, op2);
      }
      else {
	inex = (MPFI_GET_FUNCTION (function, II)) (got, got);
      }

      if (inex != expected_inex || !same_value (got, expected)) {
	printf ("Error when reusing%s input argument as output (line %lu)."
		"\nop1 = ", (MPFI_GET_TYPE (function) == II ? " first" : ""),
		line_number - 1);
	mpfi_out_str (stdout, 16, 0, op1);
	if (MPFI_GET_TYPE (function) == III) {
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

    if (MPFI_GET_TYPE (function) == III
	&& mpfi_get_prec (got) == mpfi_get_prec (op2)) {
      mpfi_set (got, op2);
      inex = (MPFI_GET_FUNCTION (function, III)) (got, op1, got);

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
  if (MPFI_GET_TYPE (function) == III) {
    mpfi_clear (op2);
  }
}

  /* main function */

  void
    check_data (mpfi_function function, const char *file_name)
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

    switch (MPFI_GET_TYPE (function)) {
    case II:
    case III:
      check_data_i (function, fp);
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
