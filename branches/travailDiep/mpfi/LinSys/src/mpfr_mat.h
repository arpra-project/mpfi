 
#ifndef _mpfr_mat_h
#define _mpfr_mat_h

/* Machine independent implementations */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "gmp.h"
#include "mpfr.h"

typedef struct {
  int init;
  int rows;
  int cols;
  mp_prec_t prec;
  __mpfr_struct **mems;
}__mpfr_mat_struct;

typedef __mpfr_mat_struct mpfr_mat_t[1];
typedef __mpfr_mat_struct *mpfr_mat_ptr;
typedef __mpfr_mat_struct *mpfr_mat_srcptr;

/* Rounding                                     */
int mpfr_mat_round_prec(mpfr_mat_ptr,mp_prec_t prec,
			mp_rnd_t rnd);

/* initializations */
void   mpfr_mat_init       (mpfr_mat_ptr,int,int);
void   mpfr_mat_init2      (mpfr_mat_ptr,int,
			    int,mp_prec_t);
void   mpfr_mat_clear      (mpfr_mat_ptr);

mp_prec_t mpfr_mat_get_prec(mpfr_mat_ptr);
void mpfr_mat_set_prec(mpfr_mat_ptr,mp_prec_t);


/* assignment functions                         */
int   mpfr_mat_set        (mpfr_mat_ptr, int, int,
			   mpfr_srcptr, mp_rnd_t);
int   mpfr_mat_set_si     (mpfr_mat_ptr, int, int,
			   const long, mp_rnd_t);
int   mpfr_mat_set_ui     (mpfr_mat_ptr, int, int,
			   const unsigned long, mp_rnd_t);
int   mpfr_mat_set_d      (mpfr_mat_ptr, int, int,
			   const double, mp_rnd_t);
int   mpfr_mat_set_z      (mpfr_mat_ptr, int, int,
			   mpz_srcptr, mp_rnd_t);
int   mpfr_mat_set_q      (mpfr_mat_ptr, int, int,
			   mpq_srcptr, mp_rnd_t);
int   mpfr_mat_set_fr     (mpfr_mat_ptr, int, int,
			   mpfr_srcptr, mp_rnd_t);
int   mpfr_mat_set_str    (mpfr_mat_ptr, int, int,
			   char *, int, mp_rnd_t);

/* swapping two intervals */
void mpfr_mat_swap (mpfr_mat_ptr, mpfr_mat_ptr);
void mpfr_mat_swap_rows(mpfr_mat_ptr, int, int);
void mpfr_mat_swap_cols(mpfr_mat_ptr, int, int);


/* arithmetic operations between two interval operands */
int  mpfr_mat_is_nan      (mpfr_mat_ptr);
int  mpfr_mat_is_inf      (mpfr_mat_ptr);
int   mpfr_mat_add        (mpfr_mat_ptr, mpfr_mat_srcptr,
			   mpfr_mat_srcptr, mp_rnd_t);
int   mpfr_mat_sub        (mpfr_mat_ptr, mpfr_mat_srcptr,
			   mpfr_mat_srcptr, mp_rnd_t);
int   mpfr_mat_mul        (mpfr_mat_ptr, mpfr_mat_srcptr,
			   mpfr_mat_srcptr, mp_rnd_t);
int mpfr_mat_inv_uptriangle(mpfr_mat_ptr,
			    mpfr_mat_srcptr, mp_rnd_t);
int mpfr_mat_inv_lowtriangle(mpfr_mat_ptr,
			    mpfr_mat_srcptr, mp_rnd_t);

int   mpfr_mat_inv_lu_method_D (mpfr_mat_ptr, mpfr_mat_srcptr,
				int, mp_rnd_t);
int   mpfr_mat_inv_lu_method_B (mpfr_mat_ptr,
				mpfr_mat_srcptr,
				int,
				mp_rnd_t);
int   mpfr_mat_inv_lu_method_C (mpfr_mat_ptr, mpfr_mat_srcptr,
				int, mp_rnd_t);

#define mpfr_mat_inv_lu(r,a,pivot,rnd) mpfr_mat_inv_lu_method_B(r,a,pivot,rnd)

/* arithmetic operations between a matrix and a double
   prec. floating-point */
int   mpfr_mat_add_d      (mpfr_mat_ptr, mpfr_mat_srcptr,
			   const double, mp_rnd_t);
int   mpfr_mat_sub_d      (mpfr_mat_ptr, mpfr_mat_srcptr,
			   const double, mp_rnd_t);
int   mpfr_mat_d_sub      (mpfr_mat_ptr, const double,
			   mpfr_mat_srcptr, mp_rnd_t);
int   mpfr_mat_mul_d      (mpfr_mat_ptr, mpfr_mat_srcptr,
			   const double, mp_rnd_t);
int   mpfr_mat_div_d      (mpfr_mat_ptr, mpfr_mat_srcptr,
			   const double, mp_rnd_t);
int   mpfr_mat_d_div      (mpfr_mat_ptr, const double,
			   mpfr_mat_srcptr, mp_rnd_t);

/* arithmetic operations between a matrix and an unsigned long
   integer */
int   mpfr_mat_add_ui     (mpfr_mat_ptr, mpfr_mat_srcptr,
			   const unsigned long, mp_rnd_t);
int   mpfr_mat_sub_ui     (mpfr_mat_ptr, mpfr_mat_srcptr,
			   const unsigned long, mp_rnd_t);
int   mpfr_mat_ui_sub   (mpfr_mat_ptr,const unsigned long,
			 mpfr_mat_srcptr, mp_rnd_t);
int   mpfr_mat_mul_ui     (mpfr_mat_ptr, mpfr_mat_srcptr,
			   const unsigned long, mp_rnd_t);


/* arithmetic operations between a matrix and a 
   long integer */

int   mpfr_mat_add_si     (mpfr_mat_ptr, mpfr_mat_srcptr,
			   const long);
int   mpfr_mat_sub_si     (mpfr_mat_ptr, mpfr_mat_srcptr,
			   const long);
int   mpfr_mat_si_sub     (mpfr_mat_ptr, const long,
			   mpfr_mat_srcptr);
int   mpfr_mat_mul_si     (mpfr_mat_ptr, mpfr_mat_srcptr,
			   const long);

/* arithmetic operations between a matrix and a multiple
   prec. integer */
int   mpfr_mat_add_z     (mpfr_mat_ptr, mpfr_mat_srcptr,
			  mpz_srcptr, mp_rnd_t);
int   mpfr_mat_sub_z     (mpfr_mat_ptr, mpfr_mat_srcptr,
			  mpz_srcptr, mp_rnd_t);
int   mpfr_mat_z_sub     (mpfr_mat_ptr, mpz_srcptr,
			  mpfr_mat_srcptr, mp_rnd_t);
int   mpfr_mat_mul_z     (mpfr_mat_ptr, mpfr_mat_srcptr,
			  mpz_srcptr, mp_rnd_t);

/* arithmetic operations between a matrix and a multiple
   prec. rational */
int   mpfr_mat_add_q     (mpfr_mat_ptr, mpfr_mat_srcptr,
			  mpq_srcptr, mp_rnd_t);
int   mpfr_mat_sub_q     (mpfr_mat_ptr, mpfr_mat_srcptr,
			  mpq_srcptr, mp_rnd_t);
int   mpfr_mat_q_sub     (mpfr_mat_ptr, mpq_srcptr,
			  mpfr_mat_srcptr, mp_rnd_t);
int   mpfr_mat_mul_q     (mpfr_mat_ptr, mpfr_mat_srcptr,
			  mpq_srcptr, mp_rnd_t);

/* arithmetic operations between a matrix and a
   mult. prec. floating-pt nb */
int   mpfr_mat_add_fr  (mpfr_mat_ptr, mpfr_mat_srcptr,
			mpfr_srcptr, mp_rnd_t);
int   mpfr_mat_sub_fr  (mpfr_mat_ptr, mpfr_mat_srcptr,
			mpfr_srcptr, mp_rnd_t);
int   mpfr_mat_fr_sub  (mpfr_mat_ptr, mpfr_srcptr,
			mpfr_mat_srcptr, mp_rnd_t);
int   mpfr_mat_mul_fr  (mpfr_mat_ptr, mpfr_mat_srcptr,
			mpfr_srcptr, mp_rnd_t);

/* various operations */
void  mpfr_mat_trans   (mpfr_mat_ptr, mpfr_mat_srcptr,
			mp_rnd_t);
void  mpfr_mat_abs     (mpfr_mat_ptr, mpfr_mat_srcptr,
			mp_rnd_t);
int   mpfr_mat_nan_p   (mpfr_mat_ptr);

void  mpfr_mat_max     (mpfr_ptr, mpfr_mat_srcptr);
void  mpfr_mat_min     (mpfr_ptr, mpfr_mat_srcptr);
void  mpfr_mat_minmax  (mpfr_ptr,mpfr_ptr,mpfr_mat_srcptr);

/* Miscellaneous */

char * mpfr_mat_get_version();

/* Error handling */

void   mpfr_mat_reset_error(void);
void   mpfr_mat_set_error(const int);
int    mpfr_mat_is_error(void);

/* extended functions */
int    mpfr_mat_dotmul(mpfr_mat_ptr, mpfr_mat_srcptr,
		       mpfr_mat_srcptr, mp_rnd_t);

int    mpfr_mat_dotdiv(mpfr_mat_ptr, mpfr_mat_srcptr,
		       mpfr_mat_srcptr, mp_rnd_t);

#endif
