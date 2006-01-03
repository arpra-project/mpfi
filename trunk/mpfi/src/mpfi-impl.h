/* mpfi-impl.h -- Include file for mpfi.

Copyright 1999, 2000, 2001, 2002, 2003, 2004, 2005, 
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
the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA 02111-1307, USA. */

#ifndef __MPFI_IMPL_H__
#define __MPFI_IMPL_H__

#include <mpfi_config.h>

#define MPFI_RNDD GMP_RNDD
#define MPFI_RNDU GMP_RNDU

/* Return values for MPFI:
   case where the two endpoints are exact, where only the left one is exact,
   where only the right one is exact, where both are inexact.
   Not very proud of that: it depends explicitely on the representation by endpoints...
*/

/* move in mpfi.h
#define MPFI_FLAGS_BOTH_ENDPOINTS_EXACT       0
#define MPFI_FLAGS_LEFT_ENDPOINT_INEXACT      1
#define MPFI_FLAGS_RIGHT_ENDPOINT_INEXACT     2
#define MPFI_FLAGS_BOTH_ENDPOINTS_INEXACT     3

#define MPFI_BOTH_ARE_EXACT(x) ( (int)(x) == 0 )
#define MPFI_LEFT_IS_INEXACT(x) ( (int)(x)%2 )
#define MPFI_RIGHT_IS_INEXACT(x) ( (int)(x)/2 )
#define MPFI_BOTH_ARE_INEXACT(x) ( (int)(x) == 3 )

#define MPFI_REVERT_INEXACT_FLAGS(x) \
  ( ((x)==1) ? 2 : ((x)==2) ? 1 : x )

#define MPFI_NAN_P(a) ( MPFR_IS_NAN(&(a->left)) || MPFR_IS_NAN (&(a->right)) )
#define MPFI_INF_P(a) ( MPFR_IS_INF(&(a->left)) || MPFR_IS_INF (&(a->right)) )
#define MPFI_IS_ZERO(a)  (MPFI_NAN_P(a) ? 0 : ((MPFR_SIGN(&(a->right))==0) && (MPFR_SIGN(&(a->left))==0)))

#define MPFI_CLEAR(m) {mpfr_clear(&(m->right)); mpfr_clear(&(m->left));}
*/
#if defined(GMP_NUMB_BITS) /* GMP 4.1.2 or above */
# define BITS_PER_MP_LIMB  (GMP_NUMB_BITS+GMP_NAIL_BITS)
#elif defined (__GMP_BITS_PER_MP_LIMB) /* Older versions 4.x.x */
#define BITS_PER_MP_LIMB  __GMP_BITS_PER_MP_LIMB
#else
# error "Could not detect BITS_PER_MP_LIMB. Get GMP 4.0.0 at least."
#endif

/* Try to detect MPFR 2.0.1 or above */
#ifndef mpfr_round_prec
# define MPFR_RET_NAN return (__mpfr_flags |= MPFR_FLAGS_NAN), 0
#elif MPFR_VERSION >= MPFR_VERSION_NUM(2,2,0)
# define MPFR_RET_NAN do {mpfr_set_nanflag(); return 0; }while (0)
#else
# define MPFR_RET_NAN return (__gmpfr_flags |= MPFR_FLAGS_NAN), 0
#endif

#define MPFR_IS_NAN(x)  (mpfr_nan_p(x))
#define MPFR_IS_ZERO(x) (mpfr_sgn(x) == 0)
#define MPFR_IS_INF(x)  (mpfr_inf_p(x))

#ifdef mp_get_memory_functions

#undef __gmp_allocate_func
#undef __gmp_reallocate_func
#undef __gmp_free_func
#define MPFR_GET_MEMFUNC mp_get_memory_functions(&mpfi_allocate_func, &mpfi_reallocate_func, &mpfi_free_func)
#define __gmp_allocate_func   (MPFR_GET_MEMFUNC, mpfi_allocate_func)
#define __gmp_reallocate_func (MPFR_GET_MEMFUNC, mpfi_reallocate_func)
#define __gmp_free_func       (MPFR_GET_MEMFUNC, mpfi_free_func)

extern void * (*mpfi_allocate_func)   (size_t);
extern void * (*mpfi_reallocate_func) (void *, size_t, size_t);
extern void   (*mpfi_free_func)       (void *, size_t);

#endif

#endif
