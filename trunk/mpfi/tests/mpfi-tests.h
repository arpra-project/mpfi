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
typedef int (*R_fun)   (mpfr_t, mp_rnd_t);
typedef int (*RR_fun)  (mpfr_t, mpfr_srcptr, mp_rnd_t);
typedef int (*RRR_fun) (mpfr_t, mpfr_srcptr, mpfr_srcptr, mp_rnd_t);

typedef union
{
  I_fun   I;       /* output: mpfi_t, no input */
  II_fun  II;      /* output: mpfi_t, input: mpfi_t */
  III_fun III;     /* output: mpfi_t, inputs: mpfi_t, mpfi_t */
} func_ptr;

typedef union
{
  R_fun   I;       /* output: mpfr_t, no input */
  RR_fun  II;      /* output: mpfr_t, input: mpfr_t */
  RRR_fun III;     /* output: mpfr_t, inputs: mpfr_t, mpfr_t */
} mpfr_func_ptr;

typedef enum
  {
    I,     /* no input */
    II,    /* one input */
    III,   /* two inputs */
  } func_type;

typedef struct
{
  func_type      type;
  char *         name;
  func_ptr       func;
  mpfr_func_ptr  mpfr_func; /* associated MPFR function */
} mpfi_function;

/* helper macro to abstract (to mask) mpfi_function type */

#define MPFI_GET_TYPE(_mpfi_function)            (_mpfi_function).type
#define MPFI_GET_FUNCTION_NAME(_mpfi_function)   (_mpfi_function).name
#define MPFI_GET_FUNCTION(_mpfi_function, _type) (_mpfi_function).func._type
#define MPFI_GET_MPFR_FUNCTION(_mpfi_function, _type)   \
  (_mpfi_function).mpfr_func._type

#define MPFI_SET_FUNCTION(_mpfi_function, _type, _func, _mpfr_func)     \
  do {                                                                  \
    (_mpfi_function).type = (_type);                                    \
    (_mpfi_function).name = QUOTE (_func);                              \
    (_mpfi_function).func._type = (_func);                              \
    (_mpfi_function).mpfr_func._type = (_mpfr_func);                    \
  } while (0)


/* Helper functions */

#ifdef __cplusplus
extern "C" {
#endif

void check_data     (mpfi_function, const char *);
void check_random   (mpfi_function, mp_prec_t, mp_prec_t, int);
void check_const    (mpfi_function, mp_prec_t, mp_prec_t);

extern gmp_randstate_t  rands;
extern char             rands_initialized;

void test_start     (void);
void test_end       (void);
void random_interval (mpfi_ptr);

#ifdef __cplusplus
}
#endif

#endif /*_mpfi_tests_h */
