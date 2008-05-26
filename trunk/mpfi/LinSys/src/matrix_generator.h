#ifndef _matrix_generator_h
#define _matrix_generator_h

#include "mpfr_mat.h"
#include "mpfi_mat.h"

void pascal_mat_fr   (mpfr_mat_srcptr, int, int);
void pascal_mat_fi   (mpfi_mat_srcptr, int, int);
void hilbert_mat_fr  (mpfr_mat_srcptr, int,
		      int, mp_rnd_t);
void hilbert_mat_fi  (mpfi_mat_srcptr, int, int);
void ones_mat_fr     (mpfr_mat_srcptr, int, int);
void ones_mat_fi     (mpfi_mat_srcptr, int, int);
void zeros_mat_fr    (mpfr_mat_srcptr, int, int);
void zeros_mat_fi    (mpfi_mat_srcptr, int, int);
void eyes_mat_fr     (mpfr_mat_srcptr, int, int);
void eyes_mat_fi     (mpfi_mat_srcptr, int, int);
void toeplitz_mat_fr (mpfr_mat_srcptr,
		      int, int,
		      double, double, double,
		      mp_rnd_t);
void toeplitz_mat_fi (mpfi_mat_srcptr,
		      int, int,
		      double, double, double);

#endif
