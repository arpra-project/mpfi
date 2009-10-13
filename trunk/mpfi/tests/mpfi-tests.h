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

#include <stdlib.h>

#include "mpfi.h"
#include "mpfi-impl.h"
#include "mpfi_io.h"

typedef int (*mpfi_f) (mpfi_t, mpfi_srcptr, mpfi_srcptr);
typedef int (*mpfr_f) (mpfr_t, mpfr_srcptr, mpfr_srcptr, mp_rnd_t);

typedef struct {
  mpfi_f func;      /* MPFI function */
  mpfr_f mpfr_func; /* associated MPFR function */
} mpfi_function;

#ifdef __cplusplus
extern "C" {
#endif

void check_data     (mpfi_function, const char *);
void check_random   (mpfi_function, mp_prec_t, mp_prec_t, int);

extern gmp_randstate_t  rands;
extern char             rands_initialized;

void test_start     (void);
void test_end       (void);

#ifdef __cplusplus
}
#endif
