/* init.c -- Initialize/clear function arguments before/after tests.

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
extern unsigned long line_number;

void
clear_ii (mpfi_function_ptr this)
{
  /* [0] auxiliary variable (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 0, mpfi));
  /* [1] return value (int), needs no deallocation */
  /* [2] expected value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 2, mpfi));
  /* [3] operand (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 3, mpfi));

  free (MPFI_FUN_ARGS (*this));
  MPFI_FUN_ARGS (*this) = NULL;
}

void
clear_iii (mpfi_function_ptr this)
{
  /* [0] auxiliary variable (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 0, mpfi));
  /* [1] return value (int), needs no deallocation */
  /* [2] expected value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 2, mpfi));
  /* [3] first operand (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 3, mpfi));
  /* [4] second operand (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 4, mpfi));

  free (MPFI_FUN_ARGS (*this));
  MPFI_FUN_ARGS (*this) = NULL;
}

void
clear_iu (mpfi_function_ptr this)
{
  /* [0] initial value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 0, mpfi));
  /* [1] return value (int), needs no deallocation */
  /* [2] expected value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 2, mpfi));
  /* [3] operand (unsigned long), needs no deallocation */

  free (MPFI_FUN_ARGS (*this));
  MPFI_FUN_ARGS (*this) = NULL;
}

void
clear_is (mpfi_function_ptr this)
{
  /* [0] initial value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 0, mpfi));
  /* [1] return value (int), needs no deallocation */
  /* [2] expected value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 2, mpfi));
  /* [3] operand (signed long), needs no deallocation */

  free (MPFI_FUN_ARGS (*this));
  MPFI_FUN_ARGS (*this) = NULL;
}

void
clear_id (mpfi_function_ptr this)
{
  /* [0] initial value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 0, mpfi));
  /* [1] return value (int), needs no deallocation */
  /* [2] expected value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 2, mpfi));
  /* [3] operand (double), needs no deallocation */

  free (MPFI_FUN_ARGS (*this));
  MPFI_FUN_ARGS (*this) = NULL;
}

void
clear_iz (mpfi_function_ptr this)
{
  /* [0] initial value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 0, mpfi));
  /* [1] return value (int), needs no deallocation */
  /* [2] expected value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 2, mpfi));
  /* [3] operand (mpz_t) */
  mpz_clear (MPFI_FUN_ARG (*this, 3, mpz));

  free (MPFI_FUN_ARGS (*this));
  MPFI_FUN_ARGS (*this) = NULL;
}

void
clear_iq (mpfi_function_ptr this)
{
  /* [0] initial value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 0, mpfi));
  /* [1] return value (int), needs no deallocation */
  /* [2] expected value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 2, mpfi));
  /* [3] operand (mpz_t) */
  mpq_clear (MPFI_FUN_ARG (*this, 3, mpq));

  free (MPFI_FUN_ARGS (*this));
  MPFI_FUN_ARGS (*this) = NULL;
}

void
clear_ir (mpfi_function_ptr this)
{
  /* [0] initial value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 0, mpfi));
  /* [1] return value (int), needs no deallocation */
  /* [2] expected value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 2, mpfi));
  /* [3] operand (mpfr_t) */
  mpfr_clear (MPFI_FUN_ARG (*this, 3, mpfr));

  free (MPFI_FUN_ARGS (*this));
  MPFI_FUN_ARGS (*this) = NULL;
}

/* signed/unsigned long as second parameter */
void
clear_iil (mpfi_function_ptr this)
{
  /* [0] initial value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 0, mpfi));
  /* [1] return value (int), needs no deallocation */
  /* [2] expected value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 2, mpfi));
  /* [3] operand (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 3, mpfi));
  /* [4] operand (signed/unsigned long), needs no deallocation */

  free (MPFI_FUN_ARGS (*this));
  MPFI_FUN_ARGS (*this) = NULL;
}

/* double as second parameter */
void
clear_iid (mpfi_function_ptr this)
{
  /* [0] initial value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 0, mpfi));
  /* [1] return value (int), needs no deallocation */
  /* [2] expected value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 2, mpfi));
  /* [3] operand (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 3, mpfi));
  /* [4] operand (double), needs no deallocation */

  free (MPFI_FUN_ARGS (*this));
  MPFI_FUN_ARGS (*this) = NULL;
}

/* Initializations, global function for all prototypes
   In operands array, variables are in the same order as for data in
   '.dat' files plus, when needed, one additional variable before them. */
void
init (mpfi_function_ptr this)
{
  switch (MPFI_FUN_TYPE (*this))
    {
    case II:
      /* init operands */
      MPFI_FUN_ARGS (*this) =
        (mpfi_fun_operand_t*) malloc (4 * sizeof (mpfi_fun_operand_t));

      /* [0] auxiliary variable (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG (*this, 0, mpfi), 1024);
      /* [1] return value (int), needs no initialization */
      /* [2] expected value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG (*this, 2, mpfi), 1024);
      /* [3] operand (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG (*this, 3, mpfi), 1024);

      /* init methods */
      this->read_line  = read_line_ii;
      this->check_line = check_line_i;
      this->clear      = clear_ii;
      break;

    case III:
      MPFI_FUN_ARGS (*this) =
        (mpfi_fun_operand_t*) malloc (5 * sizeof (mpfi_fun_operand_t));

      /* [0] auxiliary variable (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG (*this, 0, mpfi), 1024);
      /* [1] return value (int), needs no initialization */
      /* [2] expected value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG (*this, 2, mpfi), 1024);
      /* [3] first operand (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG (*this, 3, mpfi), 1024);
      /* [4] second operand (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG (*this, 4, mpfi), 1024);

      /* init methods */
      this->read_line  = read_line_iii;
      this->check_line = check_line_i;
      this->clear      = clear_iii;
      break;

    case IU:
      MPFI_FUN_ARGS (*this) =
        (mpfi_fun_operand_t*) malloc (4 * sizeof (mpfi_fun_operand_t));

      /* [0] initial value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG (*this, 0, mpfi), 1024);
      /* [1] return value (int), needs no initialization */
      /* [2] expected value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG (*this, 2, mpfi), 1024);
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
      mpfi_init2 (MPFI_FUN_ARG (*this, 0, mpfi), 1024);
      /* [1] return value (int), needs no initialization */
      /* [2] expected value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG (*this, 2, mpfi), 1024);
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
      mpfi_init2 (MPFI_FUN_ARG (*this, 0, mpfi), 1024);
      /* [1] return value (int), needs no initialization */
      /* [2] expected value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG (*this, 2, mpfi), 1024);
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
      mpfi_init2 (MPFI_FUN_ARG (*this, 0, mpfi), 1024);
      /* [1] return value (int), needs no initialization */
      /* [2] expected value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG (*this, 2, mpfi), 1024);
      /* [3] operand (mpz_t) */
      mpz_init (MPFI_FUN_ARG (*this, 3, mpz));

      /* init methods */
      this->read_line  = read_line_iz;
      this->check_line = check_line_iz;
      this->clear      = clear_iz;
      break;

    case IQ:
      MPFI_FUN_ARGS (*this) =
        (mpfi_fun_operand_t*) malloc (4 * sizeof (mpfi_fun_operand_t));

      /* [0] initial value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG (*this, 0, mpfi), 1024);
      /* [1] return value (int), needs no initialization */
      /* [2] expected value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG (*this, 2, mpfi), 1024);
      /* [3] operand (mpq_t) */
      mpq_init (MPFI_FUN_ARG (*this, 3, mpq));

      /* init methods */
      this->read_line  = read_line_iq;
      this->check_line = check_line_iq;
      this->clear      = clear_iq;
      break;

    case IR:
      MPFI_FUN_ARGS (*this) =
        (mpfi_fun_operand_t*) malloc (4 * sizeof (mpfi_fun_operand_t));

      /* [0] initial value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG (*this, 0, mpfi), 1024);
      /* [1] return value (int), needs no initialization */
      /* [2] expected value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG (*this, 2, mpfi), 1024);
      /* [3] operand (mpq_t) */
      mpfr_init2 (MPFI_FUN_ARG (*this, 3, mpfr), 1024);

      /* init methods */
      this->read_line  = read_line_ir;
      this->check_line = check_line_ir;
      this->clear      = clear_ir;
      break;

    case IIU:
      MPFI_FUN_ARGS (*this) =
        (mpfi_fun_operand_t*) malloc (5 * sizeof (mpfi_fun_operand_t));

      /* [0] initial value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG (*this, 0, mpfi), 1024);
      /* [1] return value (int), needs no initialization */
      /* [2] expected value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG (*this, 2, mpfi), 1024);
      /* [3] first operand (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG (*this, 3, mpfi), 1024);
      /* [4] second operand (unsigned long), needs no initialization */

      /* init methods */
      this->read_line  = read_line_iiu;
      this->check_line = check_line_iiu;
      this->clear      = clear_iil;
      break;

    case IIS:
      MPFI_FUN_ARGS (*this) =
        (mpfi_fun_operand_t*) malloc (5 * sizeof (mpfi_fun_operand_t));

      /* [0] initial value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG (*this, 0, mpfi), 1024);
      /* [1] return value (int), needs no initialization */
      /* [2] expected value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG (*this, 2, mpfi), 1024);
      /* [3] first operand (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG (*this, 3, mpfi), 1024);
      /* [4] second operand (signed long), needs no initialization */

      /* init methods */
      this->read_line  = read_line_iis;
      this->check_line = check_line_iis;
      this->clear      = clear_iil;
      break;

    case IID:
      MPFI_FUN_ARGS (*this) =
        (mpfi_fun_operand_t*) malloc (5 * sizeof (mpfi_fun_operand_t));

      /* [0] initial value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG (*this, 0, mpfi), 1024);
      /* [1] return value (int), needs no initialization */
      /* [2] expected value (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG (*this, 2, mpfi), 1024);
      /* [3] first operand (mpfi_t) */
      mpfi_init2 (MPFI_FUN_ARG (*this, 3, mpfi), 1024);
      /* [4] second operand (double), needs no initialization */

      /* init methods */
      this->read_line  = read_line_iid;
      this->check_line = check_line_iid;
      this->clear      = clear_iid;
      break;

    default:
      printf ("init not implemented (yet)!\n");
      exit (1);
    }
}
