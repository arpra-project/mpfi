#include "mpfi.h"
#include "mpfi_io.h"
#include "mpfi_mat.h"

/************************************************/
/* Error                                        */
/************************************************/

static int mpfi_mat_error=0;

#define VERSION "0.0.1"
#define MME(a, ind_i, ind_j) a->mems[ind_i]+ind_j

#define MPFI_MAT_ERROR(s) \
{\
if(!mpfi_mat_error) mpfi_mat_error=1;fprintf(stderr,"\n%s\n",s);\
}

void mpfi_mat_set_error(const int i)
{
  mpfi_mat_error=i;
}

void mpfi_mat_reset_error() 
{
  mpfi_mat_error=0;
}

int mpfi_mat_is_error()
{
  return(mpfi_mat_error==1);
}


void mpfi_mat_out_str(FILE *f, int base,
			size_t n_digits,
			mpfi_mat_srcptr a)
{
  int i,j;
  for (i = 0; i < a->rows; i++){
    fprintf(f, "[ ");
    for(j = 0; j < a->cols; j++){
      mpfi_out_str(f, base, n_digits, MME(a,i,j));
      fprintf(f, " ");
    }
    fprintf(f, " ]\n");
  }
}

void _midrad_(FILE *stream, int base, int prec,mpfi_t x)
{
  mpfr_t tmp;
  mpfr_init2(tmp, mpfi_get_prec(x));
  fprintf(stream, "< ");
  mpfi_mid(tmp, x);
  mpfr_out_str(stream, base, prec, tmp, GMP_RNDD);
  fprintf(stream, ",");
  mpfi_diam(tmp,x);
  mpfr_div_ui(tmp, tmp, 2, GMP_RNDU);
  mpfr_out_str(stream, base, prec, tmp, GMP_RNDU);
  fprintf(stream, " >");  
}

void mpfi_mat_out_str_midrad(FILE *f, int base,
			size_t n_digits,
			mpfi_mat_srcptr a)
{
  int i,j;
  for (i = 0; i < a->rows; i++){
    fprintf(f, "[ ");
    for(j = 0; j < a->cols; j++){
      _midrad_(f, base, n_digits, MME(a,i,j));
      fprintf(f, " ");
    }
    fprintf(f, " ]\n");
  }
}

/************************************************/
/* Rounding                                     */
/************************************************/

int mpfi_mat_round_prec(mpfi_mat_ptr x, mp_prec_t prec)
{
  int inexact = 0;
  int i,j;
  for (i = 0; i < x->rows; i++){
    for (j = 0; j < x->cols; j++){
      inexact += mpfi_round_prec (MME(x,i,j), prec);
    }
  }

  return inexact;
}

/************************************************/
/* Initialization, destruction and assignment   */
/************************************************/

void mpfi_mat_init (mpfi_mat_ptr x,int r, int c)
{
  x->rows = r;
  x->cols = c;
  int i,j;
  x->mems = (mpfi_ptr*)malloc(r * sizeof(mpfi_ptr));
  for (i = 0; i < r; i++){
    x->mems[i] = (mpfi_ptr)malloc(c * sizeof(mpfi_t));
    for (j = 0; j < c; j++){
      mpfi_init(MME(x,i,j));
    }
  }
  x->prec = mpfi_get_prec(MME(x,0,0));
}

void mpfi_mat_init2 (mpfi_mat_ptr x,int r, int c,
		     mp_prec_t p)
{
  x->rows = r;
  x->cols = c;
  x->prec = p;
  int i,j;
  x->mems = (mpfi_ptr*)malloc(r * sizeof(mpfi_ptr));
  for (i = 0; i < r; i++){
    x->mems[i] = (mpfi_ptr)malloc(c * sizeof(mpfi_t));
    for (j = 0; j < x->cols; j++){
      mpfi_init2(MME(x,i,j),p);
    }
  }
}

void mpfi_mat_clear(mpfi_mat_ptr x)
{
  int i,j;
  for (i = 0; i < x->rows; i++){
    for (j = 0; j < x->cols; j++){
      mpfi_clear(MME(x,i,j));
    }
    free(x->mems[i]);
  }
  free(x->mems);
}

mp_prec_t mpfi_mat_get_prec(mpfi_mat_srcptr x)
{
  return x->prec;
}

void mpfi_mat_set_prec(mpfi_mat_ptr x,mp_prec_t prec)
{
  int i,j;
  for (i = 0; i < x->rows; i++){
    for (j = 0; j < x->cols; j++){
      mpfi_set_prec(MME(x,i,j),prec);
    }
  }
  x->prec = prec;
}

/************************************************/
/* Assignment functions                         */
/************************************************/

int mpfi_mat_set(mpfi_mat_ptr a, int i, int j,
		 mpfi_srcptr b)
{
  int inexact=0;
  if( i >= a->rows || j >= a->cols)
    //MPFI_RET(-1);
    return -1;
  else
    inexact = mpfi_set(MME(a,i,j), b);

  return inexact;
}

int mpfi_mat_set_fr(mpfi_mat_ptr a, int i, int j,
		 mpfr_srcptr b)
{
  int inexact=0;
  if( i >= a->rows || j >= a->cols)
    //MPFI_RET(-1);
    return -1;
  else
    inexact = mpfi_set_fr(MME(a,i,j), b);

  return inexact;
}

int mpfi_mat_set_si(mpfi_mat_ptr a, int i, int j,
		    const long b)
{
  int inexact=0;
  if( i >= a->rows || j >= a->cols)
    //MPFI_RET(-1);
    return -1;
  else
    inexact = mpfi_set_si(MME(a,i,j), b);

  return inexact;
}

int mpfi_mat_set_ui(mpfi_mat_ptr a, int i, int j,
		    const unsigned long b)
{
  int inexact=0;
  if( i >= a->rows || j >= a->cols)
    //MPFI_RET(-1);
    return -1;
  else
    inexact = mpfi_set_ui(MME(a,i,j), b);

  return inexact;
}

int mpfi_mat_set_d(mpfi_mat_ptr a, int i, int j,
		   const double b)
{
 
  int inexact=0;
  if( i >= a->rows || j >= a->cols)
    //MPFI_RET(-1);
    return -1;
  else
    inexact = mpfi_set_d(MME(a,i,j), b);

  return inexact;
}

int mpfi_mat_set_z(mpfi_mat_ptr a, int i, int j,
		   mpz_srcptr b)
{
  int inexact=0;
  if( i >= a->rows || j >= a->cols)
    //MPFI_RET(-1);
    return -1;
  else
    inexact = mpfi_set_z(MME(a,i,j), b);

  return inexact;
}

int mpfi_mat_set_q(mpfi_mat_ptr a, int i, int j,
		   mpq_srcptr b)
{
  int inexact=0;
  if( i >= a->rows || j >= a->cols)
    //MPFI_RET(-1);
    return -1;
  else
    inexact = mpfi_set_q(MME(a,i,j), b);

  return inexact;
}

void mpfi_mat_swap (mpfi_mat_ptr a, mpfi_mat_ptr b)
{
  int tmp;
  tmp = a->rows; a->rows = b->rows; b->rows = tmp;
  tmp = a->cols; a->cols = b->cols; b->cols = tmp;
  mpfi_ptr *ptr;
  ptr = a->mems; a->mems = b->mems; b->mems = ptr;
}

char * mpfi_mat_get_version()
{
  return(VERSION);
}

