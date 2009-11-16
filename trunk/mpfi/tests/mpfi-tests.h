/* mpfi-tests.h -- Include file for mpfi's tests.

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

#ifndef _mpfi_tests_h
#define _mpfi_tests_h

#include <stdlib.h>

#include "mpfi.h"
#include "mpfi-impl.h"
#include "mpfi_io.h"

#define STR(a) # a
#define QUOTE(a) STR(a)


/** GENERIC TESTS **/

typedef int (*I_fun)   (mpfi_t);
typedef int (*II_fun)  (mpfi_t, mpfi_srcptr);
typedef int (*III_fun) (mpfi_t, mpfi_srcptr, mpfi_srcptr);
typedef int (*IS_fun)  (mpfi_t, long);
typedef int (*IU_fun)  (mpfi_t, unsigned long);
typedef int (*ID_fun)  (mpfi_t, double);
typedef int (*IZ_fun)  (mpfi_t, mpz_srcptr);
typedef int (*IQ_fun)  (mpfi_t, mpq_srcptr);
typedef int (*IR_fun)  (mpfi_t, mpfr_srcptr);
typedef int (*R_fun)   (mpfr_t, mp_rnd_t);
typedef int (*RR_fun)  (mpfr_t, mpfr_srcptr, mp_rnd_t);
typedef int (*RRR_fun) (mpfr_t, mpfr_srcptr, mpfr_srcptr, mp_rnd_t);
typedef void *NULL_fun;

typedef union
{
  I_fun   I;       /* output: mpfi_t, no input */
  II_fun  II;      /* output: mpfi_t, input: mpfi_t */
  III_fun III;     /* output: mpfi_t, inputs: mpfi_t, mpfi_t */
  IS_fun  IS;      /* output: mpfi_t, input: long */
  IU_fun  IU;      /* output: mpfi_t, input: unsigned long */
  ID_fun  ID;      /* output: mpfi_t, input: double */
  IZ_fun  IZ;      /* output: mpfi_t, input: mpz_t */
  IQ_fun  IQ;      /* output: mpfi_t, input: mpq_t */
  IR_fun  IR;      /* output: mpfi_t, input: mpfr_t */
} mpfi_fun_ptr;

typedef union
{
  R_fun    I;      /* output: mpfr_t, no input */
  RR_fun   II;     /* output: mpfr_t, input: mpfr_t */
  RRR_fun  III;    /* output: mpfr_t, inputs: mpfr_t, mpfr_t */
  NULL_fun IS;     /* dummy, no corresponding mpfr function */
  NULL_fun IU;     /* dummy, no corresponding mpfr function */
  NULL_fun ID;     /* dummy, no corresponding mpfr function */
  NULL_fun IZ;     /* dummy, no corresponding mpfr function */
  NULL_fun IQ;     /* dummy, no corresponding mpfr function */
  NULL_fun IR;     /* dummy, no corresponding mpfr function */
} mpfi_fun_mpfr_ptr;

typedef enum
  {
    I,     /* no input */
    II,    /* one input: interval */
    III,   /* two inputs */
    IS,    /* one input: long */
    IU,    /* one input: unsigned long */
    ID,    /* one input: double */
    IZ,    /* one input: mpz_t */
    IQ,    /* one input: mpq_t */
    IR,    /* one input: mpfr_t */
  } mpfi_fun_type;

typedef union {
  int           i;
  unsigned long ui;
  signed long   si;
  double        d;
  mpz_t         mpz;
  mpq_t         mpq;
  mpfr_t        mpfr;
  mpfi_t        mpfi;
} mpfi_fun_operand_t;

struct mpfi_funtion_t;
typedef struct mpfi_function_t* mpfi_function_ptr;

struct mpfi_function_t
{
  mpfi_fun_type       type;
  char *              name;
  mpfi_fun_ptr        func;
  mpfi_fun_mpfr_ptr   mpfr_func; /* associated MPFR function */
  mpfi_fun_operand_t* operands;

  void (*read_line)  (mpfi_function_ptr, FILE *);
  void (*check_line) (mpfi_function_ptr);
  void (*clear)      (mpfi_function_ptr);
};


/* helper macro to abstract (to mask) mpfi_function_t type */

#define MPFI_FUN_TYPE(_mpfi_function)       (_mpfi_function).type
#define MPFI_FUN_NAME(_mpfi_function)       (_mpfi_function).name
#define MPFI_FUN_GET(_mpfi_function, _type) (_mpfi_function).func._type
#define MPFI_FUN_MPFR_FUNCTION(_mpfi_function, _type)   \
  (_mpfi_function).mpfr_func._type
#define MPFI_FUN_ARGS(_mpfi_function)           \
  ((_mpfi_function).operands)
#define MPFI_FUN_ARG(_mpfi_function, _arg_no, _arg_type)        \
  ((_mpfi_function).operands[(_arg_no)]._arg_type)

#define MPFI_FUN_SET(_mpfi_function, _type, _func, _mpfr_func)          \
  do {                                                                  \
    (_mpfi_function).type            = (_type);                         \
    (_mpfi_function).name            = QUOTE (_func);                   \
    (_mpfi_function).func._type      = (_func);                         \
    (_mpfi_function).mpfr_func._type = (_mpfr_func);                    \
    (_mpfi_function).operands        = NULL;                            \
    (_mpfi_function).read_line       = NULL;                            \
    (_mpfi_function).check_line      = NULL;                            \
    (_mpfi_function).clear           = NULL;                            \
  } while (0)


/* Helper functions */

#ifdef __cplusplus
extern "C" {
#endif

  void test_start      (void);
  void test_end        (void);
  void check_data      (mpfi_function_ptr, const char *);
  void check_random    (mpfi_function_ptr, mp_prec_t, mp_prec_t, int);
  void check_const     (mpfi_function_ptr, mp_prec_t, mp_prec_t);

  extern gmp_randstate_t  rands;
  extern char             rands_initialized;
  void random_interval (mpfi_ptr);

  int  same_mpfr_value (mpfr_ptr, mpfr_ptr);
  int  same_value      (mpfi_ptr, mpfi_ptr); 

  FILE* open_file      (const char *);
  void init_reading    (FILE*);
  void close_file      (FILE*);
  void skip_whitespace_comments (FILE*);
  void read_exactness  (FILE*, int*);
  void read_ui         (FILE*, unsigned long*);
  void read_si         (FILE*, long*);
  int  read_double     (FILE*, double*);
  void read_mpz        (FILE*, mpz_ptr);
  void read_mpq        (FILE*, mpq_ptr);
  void read_mpfr       (FILE*, mpfr_ptr);
  void read_mpfi       (FILE*, mpfi_ptr);

#ifdef __cplusplus
}
#endif

#endif /*_mpfi_tests_h */
