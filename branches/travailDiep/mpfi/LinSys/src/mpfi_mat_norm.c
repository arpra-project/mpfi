#include "mpfi.h"
#include "mpfi_mat.h"

#define MME(a, ind_i, ind_j) a->mems[ind_i]+ind_j

int mpfi_mat_norm_1(mpfr_ptr norm, mpfi_mat_ptr a){
  int rows = a->rows;
  int cols = a->cols;
  int i,j;
  
  mpfr_t tmp;
  mpfr_t tmp_n;

  mpfr_init2(tmp, mpfr_get_prec(norm));
  mpfr_init2(tmp_n, mpfr_get_prec(norm));

  mpfr_set_ui(norm, 0, GMP_RNDD);

  for (i = 0; i < rows; i++){
    mpfr_set_ui(tmp_n, 0, GMP_RNDD);
    for (j = 0; j < cols; j++){
      mpfi_mag(tmp, MME(a,i,j));
      mpfr_add(tmp_n, tmp_n, tmp, GMP_RNDU);
    }
    if(mpfr_cmp(norm, tmp_n) < 0){
      mpfr_set(norm, tmp_n, GMP_RNDU);
    }
  }

  mpfr_clear(tmp);
  mpfr_clear(tmp_n);
}

int mpfi_mat_norm_8(mpfr_ptr norm, mpfi_mat_ptr a){
  int rows = a->rows;
  int cols = a->cols;
  int i,j;
  
  mpfr_t tmp;
  mpfr_t tmp_n;

  mpfr_init2(tmp, mpfr_get_prec(norm));
  mpfr_init2(tmp_n, mpfr_get_prec(norm));

  mpfr_set_ui(norm, 0, GMP_RNDD);

  for (i = 0; i < cols; i++){
    mpfr_set_ui(tmp_n, 0, GMP_RNDD);
    for (j = 0; j < rows; j++){
      mpfi_mag(tmp, MME(a,j,i));
      mpfr_add(tmp_n, tmp_n, tmp, GMP_RNDU);
    }
    if(mpfr_cmp(norm, tmp_n) < 0){
      mpfr_set(norm, tmp_n, GMP_RNDU);
    }
  }
  
  mpfr_clear(tmp);
  mpfr_clear(tmp_n);
}

int mpfi_mat_norm_1_ext_ui(mpfr_ptr norm, mpfi_mat_ptr a, unsigned int ext){
  int rows = a->rows;
  int cols = a->cols;
  int i,j;
  
  mpfr_t tmp;
  mpfr_t tmp_n;
  mpfi_t tmpi;

  mpfr_init2(tmp, mpfr_get_prec(norm));
  mpfr_init2(tmp_n, mpfr_get_prec(norm));
  mpfi_init2(tmpi, mpfr_get_prec(norm));

  mpfr_set_ui(norm, 0, GMP_RNDD);

  for (i = 0; i < rows; i++){
    mpfr_set_ui(tmp_n, 0, GMP_RNDD);
    for (j = 0; j < cols; j++){
      if (j == i){
	mpfi_sub_ui(tmpi, MME(a,i,j), ext);
	mpfi_mag(tmp, tmpi);
      }
      else{
	mpfi_mag(tmp, MME(a,i,j));
      }
      mpfr_add(tmp_n, tmp_n, tmp, GMP_RNDU);
    }
    if(mpfr_cmp(norm, tmp_n) < 0){
      mpfr_set(norm, tmp_n, GMP_RNDU);
    }
  }

  mpfr_clear(tmp);
  mpfr_clear(tmp_n);
  mpfi_clear(tmpi);
}

int mpfi_mat_norm_8_ext_ui(mpfr_ptr norm, mpfi_mat_ptr a, unsigned int ext){
  int rows = a->rows;
  int cols = a->cols;
  int i,j;
  
  mpfr_t tmp;
  mpfr_t tmp_n;
  mpfi_t tmpi;

  mpfr_init2(tmp, mpfr_get_prec(norm));
  mpfr_init2(tmp_n, mpfr_get_prec(norm));
  mpfi_init2(tmpi, mpfr_get_prec(norm));

  mpfr_set_ui(norm, 0, GMP_RNDD);

  for (i = 0; i < cols; i++){
    mpfr_set_ui(tmp_n, 0, GMP_RNDD);
    for (j = 0; j < rows; j++){
      if (i == j){
	mpfi_sub_ui(tmpi, MME(a,i,i), ext);
	mpfi_mag(tmp, tmpi);
      }
      else{
	mpfi_mag(tmp, MME(a,j,i));
      }
      mpfr_add(tmp_n, tmp_n, tmp, GMP_RNDU);
    }
    if(mpfr_cmp(norm, tmp_n) < 0){
      mpfr_set(norm, tmp_n, GMP_RNDU);
    }
  }
  
  mpfr_clear(tmp);
  mpfr_clear(tmp_n);
  mpfi_clear(tmpi);
}
