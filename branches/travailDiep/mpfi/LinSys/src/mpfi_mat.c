#include "mpfi.h"
#include "mpfi_io.h"
#include "mpfi_mat.h"

#define MME(a, ind_i, ind_j) a->mems[ind_i]+ind_j

/******************************************/
/* Arithmetic operation                   */
/******************************************/
int mpfi_mat_add (mpfi_mat_ptr r, mpfi_mat_srcptr a,
		  mpfi_mat_srcptr b)
{
  if (a->rows != b-> rows || a->cols != b->cols)
    return -1;
  if (a->rows != r-> rows || a->cols != r->cols)
    return -1;

  int inexact = 0;
  int i,j;
  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      inexact |= mpfi_add(MME(r,i,j),
			  MME(a,i,j),
			  MME(b,i,j));
    }
  }
  return inexact;
}

int mpfi_mat_sub (mpfi_mat_ptr r, mpfi_mat_srcptr a,
		  mpfi_mat_srcptr b)
{
  if (a->rows != b-> rows || a->cols != b->cols)
    return -1;
  if (a->rows != r-> rows || a->cols != r->cols)
    return -1;

  int inexact = 0;
  int i,j;
  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      inexact |= mpfi_sub(MME(r,i,j),
			  MME(a,i,j),
			  MME(b,i,j));
    }
  }
  return inexact;
}

int mpfi_mat_mul (mpfi_mat_ptr r, mpfi_mat_srcptr a,
		  mpfi_mat_srcptr b)
{
  if (a->rows != r-> rows || b->cols != r->cols)
    return -1;
  if (a->cols != b-> rows)
    return -1;

  int inexact = 0;
  int i,j, k;
  mpfi_t tmp;
  mpfi_init2(tmp, mpfi_mat_get_prec(r));
  for (i = 0; i < a->rows; i++){
    for (j = 0; j < b->cols; j++){
      inexact |= mpfi_mul(r->mems[i] + j, a->mems[i],
		 b->mems[0] + j);
      for (k = 1; k < a->cols; k++){
	inexact |= mpfi_mul(tmp, MME(a,i,k),
			    MME(b,k,j));
	inexact |= mpfi_add(MME(r,i,j),
			    MME(r,i,j), tmp);
      }
    }
  }
  mpfi_clear(tmp);
  return inexact;
}

int mpfi_mat_neg (mpfi_mat_ptr r, mpfi_mat_srcptr a)
{
  if ((r->rows != a->rows) || (r->cols != a->cols))
    return -1;

  int inexact = 0;
  int i, j, k;

  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      inexact |= mpfi_neg(MME(r,i,j), MME(a,i,j));
    }
  }

  return inexact;
}

void mpfi_mat_trans (mpfi_mat_ptr r, mpfi_mat_srcptr a)
{
  if (a->rows != r->cols || a->cols != r->rows)
    return;

  int i,j;
  if (r != a){
    for (i = 0; i < a->rows; i++){
      for (j = 0; j < a->cols; j++){
	mpfi_set(MME(r,j,i), MME(a,i,j));
      }
    }
  }
  else {
    if (a->rows == a->cols){
      for (i = 0; i < a->rows; i++){
	for (j = i + 1; j < a->cols; j++){
	  mpfi_swap(MME(a,j,i), MME(a,i,j));
	}
      }
    }
    else{
      mpfi_mat_ptr tmp;
      mpfi_mat_init(tmp, a->cols, a->rows);
      for (i = 0; i < a->rows; i++){
	for (j = 0; j < a->cols; j++){
	  mpfi_swap(MME(tmp,j,i), MME(a,i,j));
	}
      }
      mpfi_mat_swap(tmp,a);
      mpfi_mat_clear(tmp);
    }
  }
}


int mpfi_mat_abs (mpfi_mat_ptr r, mpfi_mat_srcptr a)
{
  if ((r->rows != a->rows) || (r->cols != a->cols))
    return -1;

  int inexact = 0;
  int i, j, k;

  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      inexact |= mpfi_abs(MME(r,i,j), MME(a,i,j));
    }
  }

  return inexact;
}

int mpfi_mat_diam (mpfr_mat_ptr r, mpfi_mat_srcptr a)
{
  if ((r->rows != a->rows) || (r->cols != a->cols))
    return -1;

  int inexact = 0;
  int i, j, k;

  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      inexact |= mpfi_diam(MME(r,i,j), MME(a,i,j));
    }
  }

  return inexact;
}

int mpfi_mat_mid (mpfr_mat_ptr r, mpfi_mat_srcptr a)
{
  if ((r->rows != a->rows) || (r->cols != a->cols))
    return -1;

  int inexact = 0;
  int i, j, k;

  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      inexact |= mpfi_mid(MME(r,i,j), MME(a,i,j));
    }
  }

  return inexact;
}

int mpfi_mat_mag (mpfr_mat_ptr r, mpfi_mat_srcptr a)
{
  if ((r->rows != a->rows) || (r->cols != a->cols))
    return -1;

  int inexact = 0;
  int i, j, k;

  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      inexact |= mpfi_mag(MME(r,i,j), MME(a,i,j));
    }
  }

  return inexact;
}

int mpfi_mat_mig (mpfr_mat_ptr r, mpfi_mat_srcptr a)
{
  if ((r->rows != a->rows) || (r->cols != a->cols))
    return -1;

  int inexact = 0;
  int i, j, k;

  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      inexact |= mpfi_mig(MME(r,i,j), MME(a,i,j));
    }
  }

  return inexact;
}

int mpfi_mat_compmat (mpfr_mat_ptr r, mpfi_mat_srcptr a)
{
  if ((r->rows != a->rows) || (r->cols != a->cols))
    return -1;

  int inexact = 0;
  int i, j, k;

  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      if (i == j)
	inexact |= mpfi_mig(MME(r,i,j), MME(a,i,j));
      else{
	inexact |= mpfi_mag(MME(r,i,j), MME(a,i,j));
	inexact |= mpfr_neg(MME(r,i,j), MME(r,i,j),
			    GMP_RNDD);
      }
    }
  }

  return inexact;
}

int mpfir_mat_add(mpfi_mat_ptr r, mpfi_mat_srcptr a,
		  mpfr_mat_srcptr b)
{
  if ((b->rows != a->rows) || (b->cols != a->cols))
    return -1;
  if ((r->rows != a->rows) || (r->cols != a->cols))
    return -1;

  int inexact;

  int i,j,k;
  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      inexact |= mpfi_add_fr(MME(r,i,j), MME(a,i,j),
			     MME(b,i,j));
    }
  }
  return inexact;
}

int mpfir_mat_sub(mpfi_mat_ptr r, mpfi_mat_srcptr a,
		  mpfr_mat_srcptr b)
{
  if ((b->rows != a->rows) || (b->cols != a->cols))
    return -1;
  if ((r->rows != a->rows) || (r->cols != a->cols))
    return -1;

  int inexact;

  int i,j,k;
  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      inexact |= mpfi_sub_fr(MME(r,i,j), MME(a,i,j),
			     MME(b,i,j));
    }
  }
  return inexact;
}


int mpfri_mat_sub(mpfi_mat_ptr r, mpfr_mat_srcptr a,
		  mpfi_mat_srcptr b)
{
  if ((b->rows != a->rows) || (b->cols != a->cols))
    return -1;
  if ((r->rows != a->rows) || (r->cols != a->cols))
    return -1;

  int inexact;

  int i,j,k;
  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      inexact |= mpfi_fr_sub(MME(r,i,j), MME(a,i,j),
			     MME(b,i,j));
    }
  }
  return inexact;
}


int mpfir_mat_mul(mpfi_mat_ptr r, mpfi_mat_srcptr a,
		  mpfr_mat_srcptr b)
{
  if (b->rows != a->cols)
    return -1;
  if ((r->rows != a->rows) || (r->cols != b->cols))
    return -1;

  int inexact;
  int i,j,k;
  mpfi_t tmp_mul, tmp_sum;
  
  mpfi_init2(tmp_mul, mpfi_mat_get_prec(r));
  mpfi_init2(tmp_sum, mpfi_mat_get_prec(r));

  for (i = 0; i < a->rows; i++){
    for (j = 0; j < b->cols; j++){
      inexact |= mpfi_mul_fr(tmp_sum, MME(a,i,0),
			     MME(b,0,j));
      for (k = 1; k < a->cols; k++){
	inexact |= mpfi_mul_fr(tmp_mul, MME(a,i,k),
			       MME(b,k,j));
	inexact |= mpfi_add(tmp_sum,tmp_sum,tmp_mul);
      }
      mpfi_set(MME(r,i,j), tmp_sum);
    }
  }

  mpfi_clear(tmp_mul);
  mpfi_clear(tmp_sum);

  return inexact;
}

int mpfri_mat_mul(mpfi_mat_ptr r,
		  mpfr_mat_srcptr a,
		  mpfi_mat_srcptr b)
{
  if (b->rows != a->cols)
    return -1;
  if ((r->rows != a->rows) || (r->cols != b->cols))
    return -1;

  int inexact;
  int i,j,k;
  mpfi_t tmp_mul, tmp_sum;

  mpfi_init2(tmp_mul, mpfi_mat_get_prec(r));
  mpfi_init2(tmp_sum, mpfi_mat_get_prec(r));

  for (i = 0; i < a->rows; i++){
    for (j = 0; j < b->cols; j++){
      inexact |= mpfi_mul_fr(tmp_sum, MME(b,0,j),
			     MME(a,i,0));
      for (k = 1; k < a->cols; k++){
	inexact |= mpfi_mul_fr(tmp_mul, MME(b,k,j),
			       MME(a,i,k));
	inexact |= mpfi_add(tmp_sum,tmp_sum,tmp_mul);
      }
      mpfi_set(r->mems[i]+j, tmp_sum);
    }
  }

  mpfi_clear(tmp_mul);
  mpfi_clear(tmp_sum);

  return inexact;
}

int mpfrri_mat_mul(mpfi_mat_ptr r,
		   mpfr_mat_srcptr a,
		   mpfr_mat_srcptr b)
{
  if (b->rows != a->cols)
    return -1;
  if ((r->rows != a->rows) || (r->cols != b->cols))
    return -1;

  int inexact;
  int i,j,k;
  int prec;
  mpfi_t tmp, tmpi;

  mpfr_t tmpr, next, tmpmul;

  mpfi_init2(tmp, mpfi_mat_get_prec(r));
  mpfi_init2(tmpi, mpfi_mat_get_prec(r));
  mpfr_init2(tmpr, mpfi_mat_get_prec(r));
  mpfr_init2(next, mpfi_mat_get_prec(r));
  mpfr_init2(tmpmul, mpfi_mat_get_prec(r));

  for (i = 0; i < a->rows; i++){
    for (j = 0; j < b->cols; j++){
      inexact = 0;

      k = 0;
      mpfi_set_ui(tmp, 0);
      while (k < a->cols){
	inexact = 0;
	inexact |= mpfr_mul(tmpr, MME(a,i,k),
			    MME(b,k,j), GMP_RNDD);
	k++;
	while (inexact == 0 && k < a->cols){
	  inexact |= mpfr_mul(tmpmul, MME(a,i,k),
			      MME(b,k,j), GMP_RNDD);
	  inexact |= mpfr_add(tmpr,tmpr,tmpmul,GMP_RNDD);
	  k++;
	}
	if(inexact == 0){
	  mpfi_interv_fr(tmpi,tmpr, tmpr);
	}
	else {
	  mpfr_set(next, tmpr, GMP_RNDD);
	  mpfr_nextabove(next);
	  mpfi_interv_fr(tmpi, tmpr, next);
	}
	mpfi_add(tmp, tmp, tmpi);
      }
      mpfi_set(MME(r,i,j), tmp);
    }
  }

  mpfi_clear(tmp);
  mpfi_clear(tmpi);
  mpfr_clear(tmpr);
  mpfr_clear(tmpmul);
  mpfr_clear(next);

  return inexact;
}


int mpfi_mat_add_fi (mpfi_mat_ptr r, mpfi_mat_srcptr a,
		     mpfi_srcptr b){
  int i,j;
  int inexact = 0;
  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      inexact |= mpfi_add(MME(r,i,j), MME(a,i,j), b);
    }
  }

  return inexact;
}

int mpfi_mat_sub_fi (mpfi_mat_ptr r, mpfi_mat_srcptr a,
		     mpfi_srcptr b){
  int i,j;
  int inexact = 0;
  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      inexact |= mpfi_sub(MME(r,i,j), MME(a,i,j), b);
    }
  }  
  return inexact;
}

int mpfi_mat_fi_sub (mpfi_mat_ptr r, mpfi_srcptr a,
		     mpfi_mat_srcptr b){
  int i,j;
  int inexact = 0;
  for (i = 0; i < b->rows; i++){
    for (j = 0; j < b->cols; j++){
      inexact |= mpfi_sub(MME(r,i,j), a, MME(b,i,j));
    }
  }
  return inexact;
}

int mpfi_mat_mul_fi (mpfi_mat_ptr r, mpfi_mat_srcptr a,
		     mpfi_srcptr b){
  int i,j;
  int inexact = 0;
  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      inexact |= mpfi_mul(MME(r,i,j), MME(a,i,j), b);
    }
  }
   return inexact;
}

int mpfi_mat_div_fi (mpfi_mat_ptr r, mpfi_mat_srcptr a,
		     mpfi_srcptr b){
  int i,j;
  int inexact = 0;
  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      inexact |= mpfi_div(MME(r,i,j), MME(a,i,j), b);
    }
  }
  return inexact;
}
