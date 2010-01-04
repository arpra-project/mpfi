/* read_line.c -- Read expected result and operands from file.

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
extern unsigned long line_number; /* current line read in file */ 
unsigned long test_line_number;   /* start line of a test      */

void
read_line_ii (mpfi_function_ptr this, FILE* fp)
{
  test_line_number = line_number;
  /* [1] return value */
  read_exactness (fp, &(MPFI_FUN_ARG (*this, 1, i)));
  /* [2] expected value */
  read_mpfi (fp, MPFI_FUN_ARG (*this, 2, mpfi));
  /* [3] operand */
  read_mpfi (fp, MPFI_FUN_ARG (*this, 3, mpfi));
}

void
read_line_iii (mpfi_function_ptr this, FILE* fp)
{
  test_line_number = line_number;
  /* [1] return value */
  read_exactness (fp, &(MPFI_FUN_ARG (*this, 1, i)));
  /* [2] expected value */
  read_mpfi (fp, MPFI_FUN_ARG (*this, 2, mpfi));
  /* [3] first operand */
  read_mpfi (fp, MPFI_FUN_ARG (*this, 3, mpfi));
  /* [4] second operand */
  read_mpfi (fp, MPFI_FUN_ARG (*this, 4, mpfi));
}

void
read_line_iu (mpfi_function_ptr this, FILE* fp)
{
  test_line_number = line_number;
  /* [0] initial value */
  read_mpfi (fp, MPFI_FUN_ARG (*this, 0, mpfi));
  /* [1] return value */
  read_exactness (fp, &(MPFI_FUN_ARG (*this, 1, i)));
  /* [2] expected value */
  read_mpfi (fp, MPFI_FUN_ARG (*this, 2, mpfi));
  /* [3] unsigned integral operand */
  read_ui (fp, &(MPFI_FUN_ARG (*this, 3, ui)));

  /* data validation */
  if (mpfi_get_prec (MPFI_FUN_ARG (*this, 0, mpfi)) != mpfi_get_prec (MPFI_FUN_ARG (*this, 2, mpfi))) {
    printf ("Error in data file %s line %lu\nThe precisions of interval "
            "are different.\n", pathname, test_line_number);
    exit (1);
  }
}

void
read_line_is (mpfi_function_ptr this, FILE* fp)
{
  test_line_number = line_number;
  /* [0] initial value */
  read_mpfi (fp, MPFI_FUN_ARG (*this, 0, mpfi));
  /* [1] return value */
  read_exactness (fp, &(MPFI_FUN_ARG (*this, 1, i)));
  /* [2] expected value */
  read_mpfi (fp, MPFI_FUN_ARG (*this, 2, mpfi));
  /* [3] unsigned integral operand */
  read_si (fp, &(MPFI_FUN_ARG (*this, 3, si)));

  /* data validation */
  if (mpfi_get_prec (MPFI_FUN_ARG (*this, 0, mpfi)) != mpfi_get_prec (MPFI_FUN_ARG (*this, 2, mpfi))) {
    printf ("Error in data file %s line %lu\nThe precisions of interval "
            "are different.\n", pathname, test_line_number);
    exit (1);
  }
}

void
read_line_id (mpfi_function_ptr this, FILE* fp)
{
  test_line_number = line_number;
  /* [0] initial value */
  read_mpfi (fp, MPFI_FUN_ARG (*this, 0, mpfi));
  /* [1] return value */
  read_exactness (fp, &(MPFI_FUN_ARG (*this, 1, i)));
  /* [2] expected value */
  read_mpfi (fp, MPFI_FUN_ARG (*this, 2, mpfi));
  /* [3] double operand */
  read_double (fp, &(MPFI_FUN_ARG (*this, 3, d)));

  /* data validation */
  if (mpfi_get_prec (MPFI_FUN_ARG (*this, 0, mpfi)) != mpfi_get_prec (MPFI_FUN_ARG (*this, 2, mpfi))) {
    printf ("Error in data file %s line %lu\nThe precisions of interval "
            "are different.\n", pathname, test_line_number);
    exit (1);
  }
}

void
read_line_iz (mpfi_function_ptr this, FILE* fp)
{
  test_line_number = line_number;
  /* [0] initial value */
  read_mpfi (fp, MPFI_FUN_ARG (*this, 0, mpfi));
  /* [1] return value */
  read_exactness (fp, &(MPFI_FUN_ARG (*this, 1, i)));
  /* [2] expected value */
  read_mpfi (fp, MPFI_FUN_ARG (*this, 2, mpfi));
  /* [3] mpz operand */
  read_mpz (fp, MPFI_FUN_ARG (*this, 3, mpz));

  /* data validation */
  if (mpfi_get_prec (MPFI_FUN_ARG (*this, 0, mpfi)) != mpfi_get_prec (MPFI_FUN_ARG (*this, 2, mpfi))) {
    printf ("Error in data file %s line %lu\nThe precisions of interval "
            "are different.\n", pathname, test_line_number);
    exit (1);
  }
}

void
read_line_iq (mpfi_function_ptr this, FILE* fp)
{
  test_line_number = line_number;
  /* [0] initial value */
  read_mpfi (fp, MPFI_FUN_ARG (*this, 0, mpfi));
  /* [1] return value */
  read_exactness (fp, &(MPFI_FUN_ARG (*this, 1, i)));
  /* [2] expected value */
  read_mpfi (fp, MPFI_FUN_ARG (*this, 2, mpfi));
  /* [3] mpq operand */
  read_mpq (fp, MPFI_FUN_ARG (*this, 3, mpq));

  /* data validation */
  if (mpfi_get_prec (MPFI_FUN_ARG (*this, 0, mpfi)) != mpfi_get_prec (MPFI_FUN_ARG (*this, 2, mpfi))) {
    printf ("Error in data file %s line %lu\nThe precisions of interval "
            "are different.\n", pathname, test_line_number);
    exit (1);
  }
}

void
read_line_ir (mpfi_function_ptr this, FILE* fp)
{
  test_line_number = line_number;
  /* [0] initial value */
  read_mpfi (fp, MPFI_FUN_ARG (*this, 0, mpfi));
  /* [1] return value */
  read_exactness (fp, &(MPFI_FUN_ARG (*this, 1, i)));
  /* [2] expected value */
  read_mpfi (fp, MPFI_FUN_ARG (*this, 2, mpfi));
  /* [3] mpfr operand */
  read_mpfr (fp, MPFI_FUN_ARG (*this, 3, mpfr));

  /* data validation */
  if (mpfi_get_prec (MPFI_FUN_ARG (*this, 0, mpfi)) != mpfi_get_prec (MPFI_FUN_ARG (*this, 2, mpfi))) {
    printf ("Error in data file %s line %lu\nThe precisions of interval "
            "are different.\n", pathname, test_line_number);
    exit (1);
  }
}

void
read_line_iiu (mpfi_function_ptr this, FILE* fp)
{
  test_line_number = line_number;
  /* [1] return value */
  read_exactness (fp, &(MPFI_FUN_ARG (*this, 1, i)));
  /* [2] expected value */
  read_mpfi (fp, MPFI_FUN_ARG (*this, 2, mpfi));
  /* [3] mpfi_t operand */
  read_mpfi (fp, MPFI_FUN_ARG (*this, 3, mpfi));
  /* [4] unsigned integral operand */
  read_ui (fp, &(MPFI_FUN_ARG (*this, 4, ui)));
}

void
read_line_iis (mpfi_function_ptr this, FILE* fp)
{
  test_line_number = line_number;
  /* [1] return value */
  read_exactness (fp, &(MPFI_FUN_ARG (*this, 1, i)));
  /* [2] expected value */
  read_mpfi (fp, MPFI_FUN_ARG (*this, 2, mpfi));
  /* [3] mpfi_t operand */
  read_mpfi (fp, MPFI_FUN_ARG (*this, 3, mpfi));
  /* [4] signed integral operand */
  read_si (fp, &(MPFI_FUN_ARG (*this, 4, si)));
}
