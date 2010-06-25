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
the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
MA 02110-1301, USA. */

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
#define MPFI_IS_ZERO(a)  (MPFI_NAN_P(a) ? 0 : ((mpfr_sgn(&(a->right))==0) && (mpfr_sgn(&(a->left))==0)))

#define MPFI_CLEAR(m) {mpfr_clear(&(m->right)); mpfr_clear(&(m->left));}
*/

#define MPFI_IS_POS(x) ((mpfr_sgn((&(x->left)))>=0) && (mpfr_sgn((&(x->right)))>0))
#define MPFI_IS_STRICTLY_POS(x) ((mpfr_sgn((&(x->left)))>0) && (mpfr_sgn((&(x->right)))>0))
#define MPFI_IS_NONNEG(x) ((mpfr_sgn((&(x->left)))>=0) && (mpfr_sgn((&(x->right)))>=0))
#define MPFI_IS_NEG(x) ((mpfr_sgn((&(x->left)))<0) && (mpfr_sgn((&(x->right)))<=0))
#define MPFI_IS_STRICTLY_NEG(x) ((mpfr_sgn((&(x->left)))<0) && (mpfr_sgn((&(x->right)))<0))
#define MPFI_IS_NONPOS(x) ((mpfr_sgn((&(x->left)))<=0) && (mpfr_sgn((&(x->right)))<=0))
#define MPFI_IS_NULL(x) ((mpfr_sgn((&(x->left)))==0) && (mpfr_sgn((&(x->right)))==0))
#define MPFI_HAS_ZERO(x) ((mpfr_sgn((&(x->left)))<0) && (mpfr_sgn((&(x->right)))>0))
#define MPFI_HAS_ZERO_NONSTRICT(x) ((mpfr_sgn((&(x->left)))<=0) && (mpfr_sgn((&(x->right)))>=0))


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

/* Internal functions */

#if defined (__cplusplus)
extern "C" {
#endif

/* WARNING: this is an MPFR internal function which may be removed in future
   MPFR versions. Should be replaced by mpfr_printf. */
void mpfr_print_binary _MPFR_PROTO ((mpfr_srcptr));


mp_prec_t mpfi_quadrant (mpz_ptr, mpfr_srcptr);
int     mpfi_cmp_sym_pi (mpz_srcptr, mpfr_srcptr, mpfr_srcptr, mp_prec_t);

/* default comparison functions */
int    	mpfi_cmp_default    (mpfi_srcptr, mpfi_srcptr);
int    	mpfi_cmp_d_default  (mpfi_srcptr, const double);
int    	mpfi_cmp_ui_default (mpfi_srcptr, const unsigned long);
int    	mpfi_cmp_si_default (mpfi_srcptr, const long);
int    	mpfi_cmp_z_default  (mpfi_srcptr, mpz_srcptr);
int    	mpfi_cmp_q_default  (mpfi_srcptr, mpq_srcptr);
int    	mpfi_cmp_fr_default (mpfi_srcptr, mpfr_srcptr);

int    	mpfi_is_pos_default          (mpfi_srcptr);
int    	mpfi_is_nonneg_default       (mpfi_srcptr);
int    	mpfi_is_neg_default          (mpfi_srcptr);
int    	mpfi_is_nonpos_default       (mpfi_srcptr);
int    	mpfi_is_zero_default         (mpfi_srcptr);
int    	mpfi_is_strictly_neg_default (mpfi_srcptr);
int    	mpfi_is_strictly_pos_default (mpfi_srcptr);

#if defined (__cplusplus)
}
#endif

/* Debug macros */
#ifdef WARN_IF_REVERTED_ENDPOINTS
/* include stdio.h when using this macro */
# define WARNING_REVERTED_ENDPOINTS(a, fname)                           \
  do {									\
  fprintf (stdout, "Pb endpoints in reverse order in %s\n", fname);     \
  mpfi_out_str (stdout, 10, 0, a);                                      \
  fprintf (stdout, "\n");						\
  } while (0)
#else
# define WARNING_REVERTED_ENDPOINTS(a, fname)
#endif

#endif
