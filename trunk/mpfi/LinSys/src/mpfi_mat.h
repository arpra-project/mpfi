 
#ifndef _mpfi_mat_h
#define _mpfi_mat_h

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "gmp.h"
#include "mpfr.h"
#include "mpfr_mat.h"
#include "mpfi.h"

typedef struct {
  int rows;
  int cols;
  mp_prec_t prec;
  __mpfi_struct **mems;
}__mpfi_mat_struct;

typedef __mpfi_mat_struct mpfi_mat_t[1];
typedef __mpfi_mat_struct *mpfi_mat_ptr;
typedef __mpfi_mat_struct *mpfi_mat_srcptr;

#define mp_mat_ele(a, ind_i, ind_j) a->mems[ind_i]+ind_j

/* Rounding                                     */
int mpfi_mat_round_prec(mpfi_mat_ptr,mp_prec_t prec);

/* initializations */
void   mpfi_mat_init       (mpfi_mat_ptr,int,int);
void   mpfi_mat_init2      (mpfi_mat_ptr,int,
			    int,mp_prec_t);
void   mpfi_mat_clear      (mpfi_mat_ptr);

mp_prec_t mpfi_mat_get_prec(mpfi_mat_ptr);
void mpfi_mat_set_prec(mpfi_mat_ptr,mp_prec_t);


/* assignment functions                         */
int   mpfi_mat_set        (mpfi_mat_ptr, int, int,
			   mpfi_srcptr);
int   mpfi_mat_set_si     (mpfi_mat_ptr, int, int,
			   const long);
int   mpfi_mat_set_ui     (mpfi_mat_ptr, int, int,
			   const unsigned long);
int   mpfi_mat_set_d      (mpfi_mat_ptr, int, int,
			   const double);
int   mpfi_mat_set_z      (mpfi_mat_ptr, int, int,
			   mpz_srcptr);
int   mpfi_mat_set_q      (mpfi_mat_ptr, int, int,
			   mpq_srcptr);
int   mpfi_mat_set_fr     (mpfi_mat_ptr, int, int,
			   mpfr_srcptr);
int   mpfi_mat_set_str    (mpfi_mat_ptr, int, int,
			   char *, int);

/* swapping two intervals */
void mpfi_mat_swap (mpfi_mat_ptr, mpfi_mat_ptr);


/* arithmetic operations between two interval matrix */
int   mpfi_mat_add        (mpfi_mat_ptr, mpfi_mat_srcptr,
			   mpfi_mat_srcptr);
int   mpfi_mat_sub        (mpfi_mat_ptr, mpfi_mat_srcptr,
			   mpfi_mat_srcptr);
int   mpfi_mat_mul        (mpfi_mat_ptr, mpfi_mat_srcptr,
			   mpfi_mat_srcptr);
int mpfi_mat_inv_uptriangle(mpfi_mat_ptr,
			    mpfi_mat_srcptr);
int mpfi_mat_inv_lowtriangle(mpfi_mat_ptr,
			    mpfi_mat_srcptr);
int   mpfi_mat_inv        (mpfi_mat_ptr, mpfi_mat_srcptr);
int   mpfi_mat_neg        (mpfi_mat_ptr, mpfi_mat_srcptr);
int   mpfi_mat_abs        (mpfi_mat_ptr, mpfi_mat_srcptr);
int   mpfi_mat_diam       (mpfr_mat_ptr, mpfi_mat_srcptr);
int   mpfi_mat_mid        (mpfr_mat_ptr, mpfi_mat_srcptr);
int   mpfi_mat_mag        (mpfr_mat_ptr, mpfi_mat_srcptr);
int   mpfi_mat_mig        (mpfr_mat_ptr, mpfi_mat_srcptr);
int   mpfi_mat_compmat    (mpfr_mat_ptr, mpfi_mat_srcptr);


/* arithmetic operations between interval and an mpfr matrix */
int   mpfir_mat_add        (mpfi_mat_ptr, mpfi_mat_srcptr,
			    mpfr_mat_srcptr);
int   mpfir_mat_sub        (mpfi_mat_ptr, mpfi_mat_srcptr,
			    mpfr_mat_srcptr);
int   mpfir_mat_mul        (mpfi_mat_ptr, mpfi_mat_srcptr,
			    mpfr_mat_srcptr);

int   mpfri_mat_sub        (mpfi_mat_ptr, mpfr_mat_srcptr,
			    mpfi_mat_srcptr);
int   mpfri_mat_mul        (mpfi_mat_ptr, mpfr_mat_srcptr,
			    mpfi_mat_srcptr);
int   mpfrri_mat_mul       (mpfi_mat_ptr,mpfr_mat_srcptr,
			    mpfr_mat_srcptr);

/* arithmetic operations between a matrix and a double
   prec. floating-point */
int   mpfi_mat_add_d      (mpfi_mat_ptr, mpfi_mat_srcptr,
			   const double);
int   mpfi_mat_sub_d      (mpfi_mat_ptr, mpfi_mat_srcptr,
			   const double);
int   mpfi_mat_d_sub      (mpfi_mat_ptr, const double,
			   mpfi_mat_srcptr);
int   mpfi_mat_mul_d      (mpfi_mat_ptr, mpfi_mat_srcptr,
			   const double);
int   mpfi_mat_div_d      (mpfi_mat_ptr, mpfi_mat_srcptr,
			   const double);
int   mpfi_mat_d_div      (mpfi_mat_ptr, const double,
			   mpfi_mat_srcptr);

/* arithmetic operations between a matrix and an unsigned long
   integer */
int   mpfi_mat_add_ui     (mpfi_mat_ptr, mpfi_mat_srcptr,
			   const unsigned long);
int   mpfi_mat_sub_ui     (mpfi_mat_ptr, mpfi_mat_srcptr,
			   const unsigned long);
int   mpfi_mat_ui_sub   (mpfi_mat_ptr,const unsigned long,
			 mpfi_mat_srcptr);
int   mpfi_mat_mul_ui     (mpfi_mat_ptr, mpfi_mat_srcptr,
			   const unsigned long);


/* arithmetic operations between a matrix and a 
   long integer */

int   mpfi_mat_add_si     (mpfi_mat_ptr, mpfi_mat_srcptr,
			   const long);
int   mpfi_mat_sub_si     (mpfi_mat_ptr, mpfi_mat_srcptr,
			   const long);
int   mpfi_mat_si_sub     (mpfi_mat_ptr, const long,
			   mpfi_mat_srcptr);
int   mpfi_mat_mul_si     (mpfi_mat_ptr, mpfi_mat_srcptr,
			   const long);

/* arithmetic operations between a matrix and a multiple
   prec. integer */
int   mpfi_mat_add_z     (mpfi_mat_ptr, mpfi_mat_srcptr,
			  mpz_srcptr);
int   mpfi_mat_sub_z     (mpfi_mat_ptr, mpfi_mat_srcptr,
			  mpz_srcptr);
int   mpfi_mat_z_sub     (mpfi_mat_ptr, mpz_srcptr,
			  mpfi_mat_srcptr);
int   mpfi_mat_mul_z     (mpfi_mat_ptr, mpfi_mat_srcptr,
			  mpz_srcptr);

/* arithmetic operations between a matrix and a multiple
   prec. rational */
int   mpfi_mat_add_q     (mpfi_mat_ptr, mpfi_mat_srcptr,
			  mpq_srcptr);
int   mpfi_mat_sub_q     (mpfi_mat_ptr, mpfi_mat_srcptr,
			  mpq_srcptr);
int   mpfi_mat_q_sub     (mpfi_mat_ptr, mpq_srcptr,
			  mpfi_mat_srcptr);
int   mpfi_mat_mul_q     (mpfi_mat_ptr, mpfi_mat_srcptr,
			  mpq_srcptr);

/* arithmetic operations between a matrix and a
   mult. prec. floating-pt nb */
int   mpfi_mat_add_fi  (mpfi_mat_ptr, mpfi_mat_srcptr,
			mpfi_srcptr);
int   mpfi_mat_sub_fi  (mpfi_mat_ptr, mpfi_mat_srcptr,
			mpfi_srcptr);
int   mpfi_mat_fi_sub  (mpfi_mat_ptr, mpfi_srcptr,
			mpfi_mat_srcptr);
int   mpfi_mat_mul_fi  (mpfi_mat_ptr, mpfi_mat_srcptr,
			mpfi_srcptr);

/* various operations */
void  mpfi_mat_trans   (mpfi_mat_ptr, mpfi_mat_srcptr);
int   mpfi_mat_nan_p   (mpfi_mat_ptr);


/* Solving a linear problem */
int    mpfi_solve_mpfr   (mpfi_mat_ptr,mpfr_mat_srcptr,
			  mpfr_mat_srcptr,
			  int, int);

int    mpfi_solve_mpfi(mpfi_mat_ptr,mpfi_mat_srcptr,
		       mpfi_mat_srcptr,
		       int, int);

/* norm of matrix */
int    mpfi_mat_norm_1(mpfr_ptr, mpfi_mat_ptr);
int    mpfi_mat_norm_8(mpfr_ptr, mpfi_mat_ptr);
int    mpfi_mat_norm_1_ext_ui(mpfr_ptr, mpfi_mat_ptr, unsigned int);
int    mpfi_mat_norm_8_ext_ui(mpfr_ptr, mpfi_mat_ptr, unsigned int);

#endif
