#include "mpfr_mat.h"

#define MME(a, ind_i, ind_j) a->mems[ind_i]+ind_j

static int mpfr_mat_error=0;

/* Flags of mpfr_mat */
#define MPFR_MAT_FLAGS_UNDERFLOW 1
#define MPFR_MAT_FLAGS_OVERFLOW 2
#define MPFR_MAT_FLAGS_NAN 4
#define MPFR_MAT_FLAGS_INEXACT 8
#define MPFR_MAT_FLAGS_ERANGE 16
#define MPFR_MAT_FLAGS_ALL 31


void mpfr_mat_out_str(FILE *f, int base,
			size_t n_digits,
		      mpfr_mat_srcptr a,
		      mp_rnd_t rnd)
{
  int i,j;
  for (i = 0; i < a->rows; i++){
    fprintf(f, "[ ");
    for(j = 0; j < a->cols; j++){
      mpfr_out_str(f, base, n_digits, MME(a,i,j), rnd);
      fprintf(f, " ");
    }
    fprintf(f, " ]\n");
  }
}

#define MPFR_MAT_GET_FLAGS(underf,overf,nanf,inexf,erangef)		\
  underf = mpfr_underflow_p(); overf = mpfr_overflow_p();		\
  nanf = mpfr_nanflag_p(); inexf = mpfr_inexflag_p();			\
  erangef = mpfr_erangeflag_p();

#define MPFR_MAT_SET_FLAGS(underf,overf,nanf,inexf,erangef)	\
  if (underf) mpfr_set_underflow();				\
  if (overf) mpfr_set_overflow();				\
  if (nanf) mpfr_set_nanflag();					\
  if (inexf) mpfr_set_inexflag();				\
  if (erangef) mpfr_set_erangeflag();

#define MPFR_MAT_FUNC_INIT()						\
  unsigned int __old_flag1, __old_flag2, __old_flag3;			\
  unsigned int __old_flag4, __old_flag5;				\
  MPFR_MAT_GET_FLAGS(__old_flag1, __old_flag2,				\
		     __old_flag3, __old_flag4, __old_flag5);		\
  mpfr_clear_flags();

#define MPFR_MAT_FUNC_RET(ret)					\
  MPFR_MAT_SET_FLAGS(__old_flag1, __old_flag2,				\
		     __old_flag3, __old_flag4, __old_flag5);		\
  return ret;

#define MPFR_MAT_TEST_FLAGS()			\
  (mpfr_underflow_p() || mpfr_nanflag_p())

#define MPFR_MAT_STOP_IF_NAN_W(x, warning)			\
  {								\
    if(mpfr_nan_p(x)) {						\
      fprintf(stdout, "*** WARNING %s \n", warning);		\
      return MPFR_MAT_FLAGS_NAN;				\
    }								\
  }

#define MPFR_MAT_STOP_IF_NAN(x)					\
  {								\
    if(mpfr_nan_p(x)) {						\
      return MPFR_MAT_FLAGS_NAN;				\
    }								\
  }

#define MPFR_MAT_ERROR(s)						\
  {									\
    if(!mpfr_mat_error) mpfr_mat_error=1;fprintf(stderr,"\n%s\n",s);	\
  }

void mpfr_mat_set_error(const int i)
{
  mpfr_mat_error=i;
}

void mpfr_mat_reset_error() 
{
  mpfr_mat_error=0;
}

int mpfr_mat_is_error()
{
  return(mpfr_mat_error==1);
}


/************************************************/
/* Rounding                                     */
/************************************************/

int mpfr_mat_round_prec(mpfr_mat_ptr x, mp_prec_t prec,
			mp_rnd_t rnd)
{
  int inexact = 0;
  int i,j;
  for (i = 0; i < x->rows; i++){
    for (j = 0; j < x->cols; j++){
      inexact |= mpfr_round_prec (MME(x,i,j),
				  rnd, prec);
    }
  }

  return inexact;
}

/************************************************/
/* Initialization, destruction and assignment   */
/************************************************/

void mpfr_mat_init (mpfr_mat_ptr x,int r, int c)
{
  x->rows = r;
  x->cols = c;
  int i,j;
  x->mems = (mpfr_ptr*)malloc(r * sizeof(mpfr_ptr));
  for (i = 0; i < r; i++){
    x->mems[i] = (mpfr_ptr)malloc(c * sizeof(mpfr_t));
    for (j = 0; j < c; j++){
      mpfr_init(MME(x,i,j));
    }
  }
  x->prec = mpfr_get_prec(MME(x,0,0));
}

void mpfr_mat_init2 (mpfr_mat_ptr x,int r, int c,
		     mp_prec_t p)
{
  x->rows = r;
  x->cols = c;
  x->prec = p;
  int i,j;
  x->mems = (mpfr_ptr*)malloc(r * sizeof(mpfr_ptr));
  for (i = 0; i < r; i++){
    x->mems[i] = (mpfr_ptr)malloc(c * sizeof(mpfr_t));
    for (j = 0; j < x->cols; j++){
      mpfr_init2(MME(x,i,j),p);
    }
  }
}

void mpfr_mat_clear(mpfr_mat_ptr x)
{
  int i,j;
  for (i = 0; i < x->rows; i++){
    for (j = 0; j < x->cols; j++){
      mpfr_clear(MME(x,i,j));
    }
    free(x->mems[i]);
  }
  free(x->mems);
}

mp_prec_t mpfr_mat_get_prec(mpfr_mat_srcptr x)
{
  return x->prec;
}

void mpfr_mat_set_prec(mpfr_mat_ptr x,mp_prec_t prec)
{
  int i,j;
  for (i = 0; i < x->rows; i++){
    for (j = 0; j < x->cols; j++){
      mpfr_set_prec(MME(x,i,j),prec);
    }
  }
  x->prec = prec;
}

/************************************************/
/* Assignment functions                         */
/************************************************/

int mpfr_mat_set(mpfr_mat_ptr a, int i, int j,
		 mpfr_srcptr b, mp_rnd_t rnd)
{
  int inexact=0;
  if( i >= a->rows || j >= a->cols)
    return -1;
  else
    inexact |= mpfr_set(MME(a,i,j), b,rnd);

  return inexact;
}

int mpfr_mat_set_si(mpfr_mat_ptr a, int i, int j,
		    const long b, mp_rnd_t rnd)
{
  int inexact=0;
  if( i >= a->rows || j >= a->cols)
    return -1;
  else
    inexact = mpfr_set_si(MME(a,i,j), b,rnd);

  return inexact;
}

int mpfr_mat_set_ui(mpfr_mat_ptr a, int i, int j,
		    const unsigned long b, mp_rnd_t rnd)
{
  int inexact=0;
  if( i >= a->rows || j >= a->cols)
    return -1;
  else
    inexact = mpfr_set_ui(MME(a,i,j), b,rnd);

  return inexact;
}

int mpfr_mat_set_d(mpfr_mat_ptr a, int i, int j,
		   const double b, mp_rnd_t rnd)
{
 
  int inexact=0;
  if( i >= a->rows || j >= a->cols)
    return -1;
  else
    inexact = mpfr_set_d(MME(a,i,j), b,rnd);

  return inexact;
}

int mpfr_mat_set_z(mpfr_mat_ptr a, int i, int j,
		   mpz_srcptr b, mp_rnd_t rnd)
{
  int inexact=0;
  if( i >= a->rows || j >= a->cols)
    return -1;
  else
    inexact = mpfr_set_z(MME(a,i,j), b,rnd);

  return inexact;
}

int mpfr_mat_set_q(mpfr_mat_ptr a, int i, int j,
		   mpq_srcptr b, mp_rnd_t rnd)
{
  int inexact=0;
  if( i >= a->rows || j >= a->cols)
    return -1;
  else
    inexact = mpfr_set_q(MME(a,i,j), b,rnd);

  return inexact;
}

void mpfr_mat_swap (mpfr_mat_ptr a, mpfr_mat_ptr b)
{
  int tmp;
  tmp = a->rows; a->rows = b->rows; b->rows = tmp;
  tmp = a->cols; a->cols = b->cols; b->cols = tmp;
  mpfr_ptr *ptr;
  ptr = a->mems; a->mems = b->mems; b->mems = ptr;
}

/******************************************/
/* Arithmetic operation                   */
/******************************************/

int mpfr_mat_is_nan(mpfr_mat_ptr a){
  int ret = 0;
  int i,j;
  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      if(mpfr_nan_p(MME(a,i,j))){
	return 1;
      }
    }
  }
  return 0;
}

int mpfr_mat_is_inf(mpfr_mat_ptr a){
  int ret = 0;
  int i,j;
  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      if(mpfr_inf_p(MME(a,i,j))){
	return 1;
      }
    }
  }
  return 0;
}

int mpfr_mat_add (mpfr_mat_ptr r, mpfr_mat_srcptr a,
		  mpfr_mat_srcptr b, mp_rnd_t rnd)
{
  if (a->rows != b-> rows || a->cols != b->cols)
    return -1;
  if (a->rows != r-> rows || a->cols != r->cols)
    return -1;

  MPFR_MAT_FUNC_INIT();

  int inexact = 0;
  int i,j;
  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      inexact |= mpfr_add(MME(r,i,j), MME(a,i,j),
	       MME(b,i,j), rnd);

      if(MPFR_MAT_TEST_FLAGS())
	MPFR_MAT_FUNC_RET(inexact);
    }
  }

  MPFR_MAT_FUNC_RET(inexact);
}

int mpfr_mat_sub (mpfr_mat_ptr r, mpfr_mat_srcptr a,
		  mpfr_mat_srcptr b, mp_rnd_t rnd)
{
  if (a->rows != b-> rows || a->cols != b->cols)
    return -1;
  if (a->rows != r-> rows || a->cols != r->cols)
    return -1;

  MPFR_MAT_FUNC_INIT();

  int inexact = 0;
  int i,j;
  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      inexact |= mpfr_sub(MME(r,i,j), MME(a,i,j),
	       MME(b,i,j), rnd);
      if(MPFR_MAT_TEST_FLAGS())
	MPFR_MAT_FUNC_RET(inexact);
    }
  }
  MPFR_MAT_FUNC_RET(inexact);
}


int mpfr_mat_mul (mpfr_mat_ptr r, mpfr_mat_srcptr a,
		  mpfr_mat_srcptr b, mp_rnd_t rnd)
{
  if (a->rows != r-> rows || b->cols != r->cols)
    return -1;
  if (a->cols != b-> rows)
    return -1;

  MPFR_MAT_FUNC_INIT();

  int inexact = 0;
  int i,j, k;
  mpfr_t tmp,tmpmul;

  mpfr_init2(tmp, mpfr_mat_get_prec(r));
  mpfr_init2(tmpmul, mpfr_mat_get_prec(r));

  for (i = 0; i < a->rows; i++){
    for (j = 0; j < b->cols; j++){
      inexact |= mpfr_mul(tmp, MME(a,i,0),
			  MME(b,0,j), rnd);
      if(MPFR_MAT_TEST_FLAGS()){
	mpfr_clear(tmp);
	mpfr_clear(tmpmul);
	MPFR_MAT_FUNC_RET(inexact);
      }
	
      for (k = 1; k < a->cols; k++){
	inexact |= mpfr_mul(tmpmul, MME(a,i,k), MME(b,k,j), rnd);
	inexact |= mpfr_add(tmp, tmp, tmpmul, rnd);

	if(MPFR_MAT_TEST_FLAGS()){
	  mpfr_clear(tmp);
	  mpfr_clear(tmpmul);
	  MPFR_MAT_FUNC_RET(inexact);
	}
      }
      inexact |= mpfr_set(MME(r,i,j), tmp, rnd);
    }
  }

  mpfr_clear(tmp);
  mpfr_clear(tmpmul);

  MPFR_MAT_FUNC_RET(inexact);
}


/*
  La fonction de faire la decomposition LU
  @param 1 : matrice L de sortie
  @param 2 : matrice U de sortie
  @param 3 : matrice A d'entree
  @param 4 : mode d'arrondi
 */
int mpfr_mat_lu (mpfr_mat_ptr l, mpfr_mat_ptr u,
		 mpfr_mat_srcptr a, mp_rnd_t rnd)
{
  if (a->rows != a->cols)
    return -1;
  if ((l->rows != l->cols) || (u->rows != u->cols)
      || (l->rows != a->cols) || (u->rows != a->cols))
    return -1;

  MPFR_MAT_FUNC_INIT();

  int inexact = 0;
  int i, j, k, r = a->rows;
  mpfr_ptr tmpl;
  mpfr_t tmpmul, tmpu;
  int max_prec;

  max_prec = mpfr_mat_get_prec(l);
  if (mpfr_mat_get_prec(u) > max_prec)
    max_prec = mpfr_mat_get_prec(u);

  tmpl = (mpfr_ptr)malloc(r * sizeof(mpfr_t));

  for (i = 0; i < r; i++)
    mpfr_init2(tmpl + i, max_prec);

  mpfr_init2(tmpmul, max_prec);
  mpfr_init2(tmpu, mpfr_mat_get_prec(u));

  if (l != u){
    for (i = 0; i < r; i++){
      for (j = i + 1; j < r; j++){
	inexact |= mpfr_mat_set_ui(u, j, i, 0, rnd);
	inexact |= mpfr_mat_set_ui(l, i, j, 0, rnd);
      }
    }
  }

  for (k = 0; k < r; k++){
    for (i = k; i < r; i++){
      mpfr_set(tmpl + i, MME(a,i,k), rnd);
      for (j = 0; j < k; j++){
	inexact |= mpfr_mul(tmpmul, MME(l,i,j), MME(u,j,k), rnd);
	inexact |= mpfr_sub(tmpl + i, tmpl + i, tmpmul, rnd);
	if(MPFR_MAT_TEST_FLAGS()){
	  mpfr_clear(tmpmul);
	  mpfr_clear(tmpu);
	  for (i = 0; i < r; i++)
	    mpfr_clear(tmpl + i);
	  free(tmpl);
	  MPFR_MAT_FUNC_RET(inexact);
	}
      }
    }

    /*
      calculer les elements de la colonne k de L
      on ne calcule que les elements dessous le pivot
      Si on utilise une seule matrice pour toutes les deux L et U
      on ne stocque pas le pivot de L (qui vaut 1)
     */
    if (l != u)
      inexact |= mpfr_mat_set_ui(l, k, k, 1, rnd);

    for (i = k + 1; i < r; i++){
      inexact |= mpfr_div(MME(l,i,k), tmpl + i, tmpl + k, rnd);

      if(MPFR_MAT_TEST_FLAGS()){
	mpfr_clear(tmpmul);
	mpfr_clear(tmpu);
	for (i = 0; i < r; i++)
	  mpfr_clear(tmpl + i);
	free(tmpl);
	MPFR_MAT_FUNC_RET(inexact);
      }
    }

    /*
      Calculer les elements de la ligne k de U
     */
    inexact |= mpfr_mat_set(u, k, k, tmpl + k, rnd);

    for (i = k + 1; i < r; i++){
      mpfr_set(tmpu, MME(a,k,i), rnd);
      for (j = 0; j < k; j++){
	inexact |= mpfr_mul(tmpmul, MME(l,k,j), MME(u,j,i), rnd);
	inexact |= mpfr_sub(tmpu, tmpu, tmpmul, rnd);

	if(MPFR_MAT_TEST_FLAGS()){
	  mpfr_clear(tmpmul);
	  mpfr_clear(tmpu);
	  for (i = 0; i < r; i++)
	    mpfr_clear(tmpl + i);
	  free(tmpl);
	  MPFR_MAT_FUNC_RET(inexact);
	}
      }
      inexact |= mpfr_set(MME(u,k,i), tmpu, rnd);
    }
  }

  mpfr_clear(tmpmul);
  mpfr_clear(tmpu);
  for (i = 0; i < r; i++)
    mpfr_clear(tmpl + i);
  free(tmpl);

  MPFR_MAT_FUNC_RET(inexact);
}


void mpfr_mat_swap_rows(mpfr_mat_ptr a, int r1, int r2)
{
  if (r1 >= a->rows || r2 >= a->rows)
    return;
  if (r1 == r2)
    return;

  int i;
  mpfr_ptr tmp;
  tmp = a->mems[r1];
  a->mems[r1] = a->mems[r2];
  a->mems[r2] = tmp;
  
}

void mpfr_mat_swap_rows_partly(mpfr_mat_ptr a, int r1,
			      int r2, int start, int end)
{
  if (r1 >= a->rows || r2 >= a->rows)
    return;

  if (start < 0)
    start = 0;

  if (end > a->rows - 1)
    end = a->rows -1;

  if (start > end)
    return;

  if (r1 == r2)
    return;

  int i;
  for (i = start; i < end + 1; i++){
    mpfr_swap(MME(a,r1,i), MME(a,r2,i));
  }
}

void mpfr_mat_swap_cols(mpfr_mat_ptr a, int c1, int c2)
{
  if (c1 >= a->cols || c2 >= a->cols)
    return;
  if (c1 == c2)
    return;

  int i;
  for (i = 0; i < a->rows; i++){
    mpfr_swap(MME(a,i,c1), MME(a,i,c2));
  }
}

void mpfr_mat_swap_cols_partly(mpfr_mat_ptr a, int c1,
			      int c2, int start, int end)
{
  if (c1 >= a->cols || c2 >= a->cols)
    return;
  if (c1 == c2)
    return;
  if (start < 0)
    start = 0;
  if (end > a->cols - 1)
    end = a->cols -1;
  if (start > end)
    return;

  int i;
  for (i = start; i < end + 1; i++){
    mpfr_swap(MME(a,i,c1), MME(a,i,c2));
  }
}

int mpfr_mat_plu (int *p, mpfr_mat_ptr l,
		  mpfr_mat_ptr u,
		  mpfr_mat_srcptr a,
		  mp_rnd_t rnd)
{
  if (a->rows != a->cols)
    return -1;
  if ((l->rows != l->cols) || (u->rows != u->cols)
      || (l->rows != a->cols) || (u->rows != a->cols))
    return -1;
  if (p == NULL)
    return -1;

  MPFR_MAT_FUNC_INIT();

  int inexact = 0;
  int i, j, k, r = a->rows;
  mpfr_ptr tmpl;
  mpfr_t tmpmul, tmpu;
  int max_prec, imax, cmp;
  int *q;

  q = (int*) malloc(r * sizeof(int));

  for (i = 0; i < r; i++){
    p[i] = i;
  }

  max_prec = mpfr_mat_get_prec(l);
  if (mpfr_mat_get_prec(u) > max_prec)
    max_prec = mpfr_mat_get_prec(u);

  tmpl = (mpfr_ptr)malloc(r * sizeof(mpfr_t));

  for (i = 0; i < r; i++)
    mpfr_init2(tmpl + i, max_prec);

  mpfr_init2(tmpmul, max_prec);
  mpfr_init2(tmpu, mpfr_mat_get_prec(u));

  if (l != u){
    for (i = 0; i < r; i++){
      for (j = i + 1; j < r; j++){
	inexact |= mpfr_mat_set_ui(u, j, i, 0, rnd);
	inexact |= mpfr_mat_set_ui(l, i, j, 0, rnd);
      }
    }
  }

  for (k = 0; k < r; k++){
    for (i = k; i < r; i++){
      mpfr_set(tmpl + i, MME(a,i,k), rnd);
      for (j = 0; j < k; j++){
	inexact |= mpfr_mul(tmpmul, MME(l,i,j), MME(u,j,k), rnd);
	inexact |= mpfr_sub(tmpl + i, tmpl + i, tmpmul, rnd);

	if(MPFR_MAT_TEST_FLAGS()){
	  mpfr_clear(tmpmul);
	  mpfr_clear(tmpu);
	  for (i = 0; i < r; i++)
	    mpfr_clear(tmpl + i);
	  free(tmpl);
	  free(q);
	  MPFR_MAT_FUNC_RET(inexact);
	}
      }
    }

    /* -----> PIVOTTAGE
      trouver la maximale valeur
      et effectuer le pivottage
     */
    imax = k;
    for (i = k+1; i < r; i++){
      cmp = mpfr_cmp(tmpl + i, tmpl + imax);
      if (cmp > 0){
	imax = i;
      }
    }
 
    if (imax != k){
      mpfr_mat_swap_rows(a, imax, k);
      mpfr_mat_swap_rows_partly(l, imax, k, 0, k );
      mpfr_swap(tmpl + k, tmpl + imax);
      i = p[k]; p[k] = p[imax]; p[imax] = i;
    }
    
    // <--- FIN DU PIVOTTAGE

    if (l != u)
      inexact |= mpfr_mat_set_ui(l, k, k, 1, rnd);

    for (i = k + 1; i < r; i++){
      cmp = mpfr_cmp_ui(tmpl + i, 0);
      if (cmp == 0)
	mpfr_set_ui(MME(l,i,k), 0, rnd);
      else
	inexact |= mpfr_div(MME(l,i,k), tmpl + i, tmpl + k, rnd);

      if(MPFR_MAT_TEST_FLAGS()){
	mpfr_clear(tmpmul);
	mpfr_clear(tmpu);
	for (i = 0; i < r; i++)
	  mpfr_clear(tmpl + i);
	free(tmpl);
	free(q);
	MPFR_MAT_FUNC_RET(inexact);
      }
    }

    mpfr_mat_set(u, k, k, tmpl + k, rnd);
    for (i = k + 1; i < r; i++){
      inexact |= mpfr_set(tmpu, MME(a,k,i), rnd);
      for (j = 0; j < k; j++){
	inexact |= mpfr_mul(tmpmul, MME(l,k,j), MME(u,j,i), rnd);
	inexact |= mpfr_sub(tmpu, tmpu, tmpmul, rnd);

	if(MPFR_MAT_TEST_FLAGS()){
	  mpfr_clear(tmpmul);
	  mpfr_clear(tmpu);
	  for (i = 0; i < r; i++)
	    mpfr_clear(tmpl + i);
	  free(tmpl);
	  free(q);
	  MPFR_MAT_FUNC_RET(inexact);
	}
      }
      inexact |= mpfr_set(MME(u,k,i), tmpu, rnd);
    }
  }

  /*
    rendre des lignes de la matrice d'entree A a l'ordre original
   */
  for (i = 0; i < r; i++){
    q[i] = p[i];
  }
  for (i = 0; i < r; i++){
    if (q[i] != i){
      // chercher la position de la ligne i
      for (j = i + 1; j < r; j++){
	if (q[j] == i){
	  mpfr_mat_swap_rows(a, i, j);
	  q[j] = q[i];
	  q[i] = i;
	  break;
	}
      }
    }
  }
  // <---- FIN DU RENDU

  mpfr_clear(tmpmul);
  mpfr_clear(tmpu);
  for (i = 0; i < r; i++)
    mpfr_clear(tmpl + i);
  free(tmpl);
  free(q);

  MPFR_MAT_FUNC_RET(inexact);
}

/*
  Calculer l'inversion d'une matrice triangulaire haute
  @param 1: la matrice inverse de sortie
  @param 2: la matrice d'entree
  @param 3: mode d'arrondi
  @param 4: parametre controller l'extension de la precision

  - - - - - - -
  0 - - - - - -
  0 0 - - - - -
  0 0 0 - <---|  ^
  0 0 0 0 + + +  |
  0 0 0 0 0 + +  |
  0 0 0 0 0 0 +  -

 */
int mpfr_mat_inv_uptriangle(mpfr_mat_ptr r,
			    mpfr_mat_srcptr a,
			    mp_rnd_t rnd)
{
  if (a->rows != a->cols)
    return -1;
  if ((r->rows != r->cols) || (r->rows != a->rows))
    return -1;

  MPFR_MAT_FUNC_INIT();

  int i,j,k, rows = a->rows;
  int cmp;
  int inexact = 0;
  
  mpfr_t tmp, tmpmul;

  mpfr_init2(tmpmul, mpfr_mat_get_prec(r));
  mpfr_init2(tmp, mpfr_mat_get_prec(r));

  for (i = rows-1; i > -1; i--){
    cmp = mpfr_cmp_ui(MME(a,i,i),0);
    if (cmp == 0){
      mpfr_clear(tmpmul);
      mpfr_clear(tmp);
      return -1;
    }

    if (r != a){
      for (j = i+1; j < rows; j++){
	inexact |= mpfr_mat_set_ui(r, j, i, 0, rnd);
      }
    }

    for (j = rows - 1; j > i; j--){
      mpfr_set_ui(tmp, 0, rnd);
      for (k = j; k > i; k--){
	inexact |= mpfr_mul(tmpmul, MME(a,i,k), MME(r,k,j), rnd);
	inexact |= mpfr_sub(tmp, tmp, tmpmul, rnd);

	if(MPFR_MAT_TEST_FLAGS()){
	  mpfr_clear(tmpmul);
	  mpfr_clear(tmp);	
	  MPFR_MAT_FUNC_RET(inexact);
	}
      }
      inexact |= mpfr_div(MME(r,i,j), tmp,
			  MME(a,i,i), rnd);

      if(MPFR_MAT_TEST_FLAGS()){
	mpfr_clear(tmpmul);
	mpfr_clear(tmp);	
	MPFR_MAT_FUNC_RET(inexact);
      }
    }

    mpfr_ui_div(MME(r,i,i), 1, MME(a,i,i), rnd);

    if(MPFR_MAT_TEST_FLAGS()){
      mpfr_clear(tmpmul);
      mpfr_clear(tmp);	
      MPFR_MAT_FUNC_RET(inexact);
    }
  }
  mpfr_clear(tmpmul);
  mpfr_clear(tmp);

  MPFR_MAT_FUNC_RET(inexact);
}

/*
  Calculer l'inversion d'une matrice triangulaire basse
  @param 1: la matrice inverse de sortie
  @param 2: la matrice d'entree
  @param 3: mode d'arrondi
  @param 4: parametre controller l'extension de la precision

  + 0 0 0 0 0 0  _
  + + 0 0 0 0 0  |
  + + + 0 0 0 0  |
  |---> - 0 0 0  V
  - - - - - 0 0
  - - - - - - 0
  - - - - - - -

 */

int __inv_lowtriangle_ext(mpfr_mat_ptr r,
			  mpfr_mat_srcptr a,
			  mp_rnd_t rnd,
			  int pivot_unit)
{
  if (a->rows != a->cols)
    return -1;
  if ((r->rows != r->cols) || (r->rows != a->rows))
    return -1;

  MPFR_MAT_FUNC_INIT();

  int i,j,k, rows = a->rows;
  int cmp;
  int inexact = 0;
  
  mpfr_t tmpmul, tmp;

  mpfr_init2(tmpmul, mpfr_mat_get_prec(r));
  mpfr_init2(tmp, mpfr_mat_get_prec(r));

  for (i = 0; i < rows; i++){
    if (pivot_unit != 1){
      cmp = mpfr_cmp_ui(MME(a,i,i),0);
      if (cmp == 0){
	mpfr_clear(tmpmul);
	mpfr_clear(tmp);
	return -1;
      }
    }

    if (r != a){
      for (j = 0; j < i; j++){
	inexact |= mpfr_mat_set_ui(r, j, i, 0, rnd);
      }
    }

    for (j = 0; j < i; j++){
      if (pivot_unit == 1){
	inexact |= mpfr_set(tmp, MME(a,i,j), rnd);
      }
      else{
	inexact |= mpfr_mul(tmp, MME(a,i,j), MME(r,j,j), rnd);
      }
      inexact |= mpfr_neg(tmp, tmp, rnd);

      for (k = j+1; k < i; k++){
	inexact |= mpfr_mul(tmpmul, MME(a,i,k), MME(r,k,j),rnd);
	inexact |= mpfr_sub(tmp, tmp, tmpmul, rnd);

	if(MPFR_MAT_TEST_FLAGS()){
	  mpfr_clear(tmpmul);
	  mpfr_clear(tmp);
	  MPFR_MAT_FUNC_RET(inexact);
	}
      }
      if (pivot_unit != 1)
	inexact |= mpfr_div(MME(r,i,j), tmp,
			    MME(a,i,i), rnd);
      else
	inexact |= mpfr_set(MME(r,i,j), tmp, rnd);

      if(MPFR_MAT_TEST_FLAGS()){
	mpfr_clear(tmpmul);
	mpfr_clear(tmp);
	MPFR_MAT_FUNC_RET(inexact);
      }
    }

    /*
      pivot_unit = 1 veut dire les elements sur le pivot valent tout
      1 et on ne doit pas calculer les elements sur le pivot
      de la matrice resultat
     */
    if (pivot_unit != 1)
      inexact |= mpfr_ui_div(MME(r,i,i), 1,MME(a,i,i),rnd);
  }

  mpfr_clear(tmpmul);
  mpfr_clear(tmp);
  MPFR_MAT_FUNC_RET(inexact);
}


int mpfr_mat_inv_lowtriangle(mpfr_mat_ptr r,
			      mpfr_mat_srcptr a,
			      mp_rnd_t rnd)
{
  return __inv_lowtriangle_ext(r, a, rnd, 0);
}

/*
  Methode D d'apres Higham
 */
int mpfr_mat_inv_lu_method_D(mpfr_mat_ptr r, mpfr_mat_srcptr a,
		    int pivot,
		    mp_rnd_t rnd)
{
  if (a->rows != a->cols)
    return -1;
  if ((r->rows != r->cols) || (r->rows != a->rows))
    return -1;

  MPFR_MAT_FUNC_INIT();

  int inexact = 0;
  int i, j, k, rows = a->rows;

  int *p = NULL;
  mpfr_t tmp_add, tmp_mul;

  if (pivot == 1)
    p = (int*)malloc(rows * sizeof(int));

  if (pivot == 1)
    inexact |= mpfr_mat_plu(p, r, r, a, rnd);
  else
    inexact |= mpfr_mat_lu(r, r, a, rnd);

  if(MPFR_MAT_TEST_FLAGS()){
    if (pivot == 1)
      free(p);
    MPFR_MAT_FUNC_RET(inexact);
  }

  inexact |= mpfr_mat_inv_uptriangle(r, r, rnd); 
  if(MPFR_MAT_TEST_FLAGS()){
    if (pivot == 1)
      free(p);
    MPFR_MAT_FUNC_RET(inexact);
  }

  inexact |= __inv_lowtriangle_ext(r, r, rnd, 1);
  if(MPFR_MAT_TEST_FLAGS()){
    if (pivot == 1)
      free(p);
    MPFR_MAT_FUNC_RET(inexact);
  }

  /*
    inv(a) = (upper(r) + diag(r)) * (lower(r) + I)
   */
  mpfr_init2(tmp_mul, mpfr_mat_get_prec(r));
  mpfr_init2(tmp_add, mpfr_mat_get_prec(r));

  for (i = 0; i < rows; i++){
    for (j = 0; j < i; j++){
      mpfr_set_ui(tmp_add, 0, rnd);
      for (k = i; k < rows; k++){
	inexact |= mpfr_mul(tmp_mul, MME(r,i,k), MME(r,k,j), rnd);
	inexact |= mpfr_add(tmp_add, tmp_add, tmp_mul, rnd);
	if(MPFR_MAT_TEST_FLAGS()){
	  if (pivot == 1)
	    free(p);
	  mpfr_clear(tmp_add);
	  mpfr_clear(tmp_mul);
	  MPFR_MAT_FUNC_RET(inexact);
	}
      }
      inexact |= mpfr_set(MME(r,i,j), tmp_add, rnd);
    }

    for (j = i; j < rows; j++){
      inexact |= mpfr_set(tmp_add, MME(r,i,j), rnd);
      for (k = j + 1; k < rows; k++){
	inexact |= mpfr_mul(tmp_mul, MME(r,i,k), MME(r,k,j), rnd);
	inexact |= mpfr_add(tmp_add, tmp_add, tmp_mul, rnd);

	if(MPFR_MAT_TEST_FLAGS()){
	  if (pivot == 1)
	    free(p);
	  mpfr_clear(tmp_add);
	  mpfr_clear(tmp_mul);
	  MPFR_MAT_FUNC_RET(inexact);
	}
      }
      inexact |= mpfr_set(MME(r,i,j), tmp_add, rnd);
    }
  }

  mpfr_clear(tmp_add);
  mpfr_clear(tmp_mul);

  if (pivot == 1){
    // pivottage
    for (i = 0; i < rows; i++){
      if (p[i] != i){
	j = i + 1;
	while (j < rows && p[j] != i)
	  j++;
	mpfr_mat_swap_cols(r, i, j);
	p[j] = p[i];
	p[i] = i;
      }
    }
    free(p);
  }

  MPFR_MAT_FUNC_RET(inexact);
}


/*
  method B
 */
int mpfr_mat_inv_lu_method_B(mpfr_mat_ptr r,
			     mpfr_mat_srcptr a,
			     int pivot,
			     mp_rnd_t rnd)
{
  if (a->rows != a->cols)
    return -1;
  if ((r->rows != r->cols) || (r->rows != a->rows))
    return -1;

  MPFR_MAT_FUNC_INIT();

  int inexact = 0;
  int i, j, k, rows = a->rows;

  int *p = NULL;
  mpfr_t tmp_add, tmp_mul;
  mpfr_ptr tmp;

  if (pivot == 1)
    p = (int*)malloc(rows * sizeof(int));

  if (pivot == 1)
    inexact |= mpfr_mat_plu(p, r, r, a, rnd);
  else
    inexact |= mpfr_mat_lu(r, r, a, rnd);

  if(MPFR_MAT_TEST_FLAGS()){
    if(pivot == 1)
      free(p);
    MPFR_MAT_FUNC_RET(inexact);
  }

  // Calculer inv(U)
  inexact |= mpfr_mat_inv_uptriangle(r, r, rnd); 
  if(MPFR_MAT_TEST_FLAGS()){
    if(pivot == 1)
      free(p);
    MPFR_MAT_FUNC_RET(inexact);
  }

  mpfr_init2(tmp_mul, mpfr_mat_get_prec(r));
  mpfr_init2(tmp_add, mpfr_mat_get_prec(r));
  tmp = (mpfr_ptr)malloc(rows * sizeof(mpfr_t));
  for(i = 0; i < rows; i++){
    mpfr_init2(tmp + i, mpfr_mat_get_prec(r));
  }

  // Calculer X tel que: X L = inv(U)
  for(k = rows - 1; k >= 0; k--){
    for(i = 0; i < k + 1 ; i++){
      for (j = k + 1; j < rows; j++){
	inexact |= mpfr_mul(tmp_mul, MME(r,i,j),
			    MME(r,j,k), rnd);
	inexact |= mpfr_sub(MME(r,i,k), MME(r,i,k),
			    tmp_mul, rnd);

	if(MPFR_MAT_TEST_FLAGS()){
	  if(pivot == 1)
	    free(p);
	  mpfr_clear(tmp_add);
	  mpfr_clear(tmp_mul);
	  for(i = 0; i < rows; i++){
	    mpfr_clear(tmp + i);
	  }
	  free(tmp);
	  MPFR_MAT_FUNC_RET(inexact);
	}
      }
    }

    for (; i < rows; i++){
      mpfr_set_ui(tmp + i, 0, GMP_RNDN);
      for (j = k + 1; j < rows; j++){
	inexact |= mpfr_mul(tmp_mul, MME(r,i,j),
			    MME(r,j,k), rnd);
	inexact |= mpfr_sub(tmp + i, tmp + i, tmp_mul, rnd);

	if(MPFR_MAT_TEST_FLAGS()){
	  if(pivot == 1)
	    free(p);
	  mpfr_clear(tmp_add);
	  mpfr_clear(tmp_mul);
	  for(i = 0; i < rows; i++){
	    mpfr_clear(tmp + i);
	  }
	  free(tmp);
	  MPFR_MAT_FUNC_RET(inexact);
	}
      }
    }

    for (i = k + 1; i < rows; i++){
      mpfr_swap(MME(r,i,k), tmp + i);
    }
  }
  

  mpfr_clear(tmp_add);
  mpfr_clear(tmp_mul);
  for(i = 0; i < rows; i++){
    mpfr_clear(tmp + i);
  }
  free(tmp);

  if (pivot == 1){
    // pivottage
    for (i = 0; i < rows; i++){
      if (p[i] != i){
	j = i + 1;
	while (j < rows && p[j] != i)
	  j++;
	mpfr_mat_swap_cols(r, i, j);
	p[j] = p[i];
	p[i] = i;
      }
    }
    free(p);
  }

  MPFR_MAT_FUNC_RET(inexact);
}

/*
  method C
 */
int mpfr_mat_inv_lu_method_C(mpfr_mat_ptr r,
			     mpfr_mat_srcptr a,
			     int pivot,
			     mp_rnd_t rnd)
{
  if (a->rows != a->cols)
    return -1;
  if ((r->rows != r->cols) || (r->rows != a->rows))
    return -1;

  MPFR_MAT_FUNC_INIT();

  int inexact = 0;
  int i, j, k, rows = a->rows;

  int *p = NULL;
  mpfr_t tmp_add, tmp_mul;
  mpfr_ptr tmp;

  if (pivot == 1)
    p = (int*)malloc(rows * sizeof(int));

  if (pivot == 1)
    inexact |= mpfr_mat_plu(p, r, r, a, rnd);
  else
    inexact |= mpfr_mat_lu(r, r, a, rnd);

  if(MPFR_MAT_TEST_FLAGS()){
    if(pivot == 1)
      free(p);
    MPFR_MAT_FUNC_RET(inexact);
  }

  mpfr_init2(tmp_mul, mpfr_mat_get_prec(r));
  mpfr_init2(tmp_add, mpfr_mat_get_prec(r));
  tmp = (mpfr_ptr)malloc(rows * sizeof(mpfr_t));
  for(i = 0; i < rows; i++){
    mpfr_init2(tmp + i, mpfr_mat_get_prec(r));
  }

  // Calculer X tel que: U X L = I
  for(k = rows - 1; k >= 0; k--){
    for (i = k + 1; i < rows; i++){
      mpfr_set_ui(tmp + i, 0, GMP_RNDN);
      for (j = k + 1; j < rows; j++){
	inexact |= mpfr_mul(tmp_mul, MME(r,k,j),
			    MME(r,j,i), rnd);
	inexact |= mpfr_sub(tmp + i, tmp + i, tmp_mul, rnd);

	if(MPFR_MAT_TEST_FLAGS()){
	  if(pivot == 1)
	    free(p);
	  mpfr_clear(tmp_add);
	  mpfr_clear(tmp_mul);
	  for(i = 0; i < rows; i++){
	    mpfr_clear(tmp + i);
	  }
	  free(tmp);
	  MPFR_MAT_FUNC_RET(inexact);
	}
      }
    }

    for (i = k + 1; i < rows; i++){
      mpfr_div(MME(r,k,i), tmp + i, MME(r,k,k), rnd);
    }
    //-------
    for (i = k + 1; i < rows; i++){
      mpfr_set_ui(tmp + i, 0, GMP_RNDN);
      for (j = k + 1; j < rows; j++){
	inexact |= mpfr_mul(tmp_mul, MME(r,i,j),
			    MME(r,j,k), rnd);
	inexact |= mpfr_sub(tmp + i, tmp + i, tmp_mul, rnd);

	if(MPFR_MAT_TEST_FLAGS()){
	  if(pivot == 1)
	    free(p);
	  mpfr_clear(tmp_add);
	  mpfr_clear(tmp_mul);
	  for(i = 0; i < rows; i++){
	    mpfr_clear(tmp + i);
	  }
	  free(tmp);
	  MPFR_MAT_FUNC_RET(inexact);
	}
      }
    }

    //-------
    mpfr_ui_div(MME(r,k,k), 1, MME(r,k,k), rnd);

    if(MPFR_MAT_TEST_FLAGS()){
      if(pivot == 1)
	free(p);
      mpfr_clear(tmp_add);
      mpfr_clear(tmp_mul);
      for(i = 0; i < rows; i++){
	mpfr_clear(tmp + i);
      }
      free(tmp);
      MPFR_MAT_FUNC_RET(inexact);
    }

    for (j = k + 1; j < rows; j++){
      inexact |= mpfr_mul(tmp_mul, MME(r,k,j),
			  MME(r,j,k), rnd);
      inexact |= mpfr_sub(MME(r,k,k), MME(r,k,k),
			  tmp_mul, rnd);

      if(MPFR_MAT_TEST_FLAGS()){
	if(pivot == 1)
	  free(p);
	mpfr_clear(tmp_add);
	mpfr_clear(tmp_mul);
	for(i = 0; i < rows; i++){
	  mpfr_clear(tmp + i);
	}
	free(tmp);
	MPFR_MAT_FUNC_RET(inexact);
      }
    }
    
    for (i = k + 1; i < rows; i++){
      mpfr_swap(MME(r,i,k), tmp + i);
    }
  }
  

  mpfr_clear(tmp_add);
  mpfr_clear(tmp_mul);
  for(i = 0; i < rows; i++){
    mpfr_clear(tmp + i);
  }
  free(tmp);

  if (pivot == 1){
    // pivottage
    for (i = 0; i < rows; i++){
      if (p[i] != i){
	j = i + 1;
	while (j < rows && p[j] != i)
	  j++;
	mpfr_mat_swap_cols(r, i, j);
	p[j] = p[i];
	p[i] = i;
      }
    }
    free(p);
  }

  MPFR_MAT_FUNC_RET(inexact);
}

int mpfr_mat_mul_fr (mpfr_mat_ptr r, mpfr_mat_srcptr a,
		     mpfr_srcptr b, mp_rnd_t rnd){
  int i,j;
  int inexact = 0;
  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      inexact |= mpfr_mul(MME(r,i,j), MME(a,i,j),
	       b, rnd);
      MPFR_MAT_STOP_IF_NAN(MME(r,i,j));
    }
  }
  return inexact;
}

int mpfr_mat_div_fr (mpfr_mat_ptr r, mpfr_mat_srcptr a,
		     mpfr_srcptr b, mp_rnd_t rnd){
  int i,j;
  int inexact = 0;

  MPFR_MAT_FUNC_INIT();

  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      inexact |= mpfr_div(MME(r,i,j), MME(a,i,j),
	       b, rnd);

      if(MPFR_MAT_TEST_FLAGS())
	MPFR_MAT_FUNC_RET(inexact);
    }
  }
  MPFR_MAT_FUNC_RET(inexact);
}

int mpfr_mat_dotmul(mpfr_mat_ptr r, 
		    mpfr_mat_srcptr a,
		    mpfr_mat_srcptr b,
		    mp_rnd_t rnd){
  int i,j;
  int inexact = 0;

  MPFR_MAT_FUNC_INIT();

  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      inexact |= mpfr_mul(MME(r,i,j), MME(a,i,j),
	       MME(b,i,j), rnd);

      if(MPFR_MAT_TEST_FLAGS())
	MPFR_MAT_FUNC_RET(inexact);
    }
  }
  MPFR_MAT_FUNC_RET(inexact);
}

int mpfr_mat_dotdiv(mpfr_mat_ptr r, 
		    mpfr_mat_srcptr a,
		    mpfr_mat_srcptr b,
		    mp_rnd_t rnd){
  int i,j;
  int inexact = 0;

  MPFR_MAT_FUNC_INIT();

  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      inexact |= mpfr_div(MME(r,i,j), MME(a,i,j),
	       MME(b,i,j), rnd);

      if(MPFR_MAT_TEST_FLAGS())
	MPFR_MAT_FUNC_RET(inexact);
    }
  }
  MPFR_MAT_FUNC_RET(inexact);
}


int mpfr_mat_norm_1(mpfr_ptr norm, mpfr_mat_ptr a){
  int rows = a->rows;
  int cols = a->cols;
  int i,j;
  
  mpfr_t tmp;
  mpfr_t tmp_n;
  int inexact = 0;

  MPFR_MAT_FUNC_INIT();

  mpfr_init2(tmp, mpfr_get_prec(norm));
  mpfr_init2(tmp_n, mpfr_get_prec(norm));

  mpfr_set_ui(norm, 0, GMP_RNDD);

  for (i = 0; i < rows; i++){
    mpfr_set_ui(tmp_n, 0, GMP_RNDD);
    for (j = 0; j < cols; j++){
      inexact |= mpfr_abs(tmp, MME(a,i,j), GMP_RNDU);
      inexact |= mpfr_add(tmp_n, tmp_n, tmp, GMP_RNDU);

      if(MPFR_MAT_TEST_FLAGS()){
	mpfr_clear(tmp);
	mpfr_clear(tmp_n);
	MPFR_MAT_FUNC_RET(inexact);
      }
    }
    if(mpfr_cmp(norm, tmp_n) < 0){
      mpfr_set(norm, tmp_n, GMP_RNDU);
    }
  }

  mpfr_clear(tmp);
  mpfr_clear(tmp_n);

  MPFR_MAT_FUNC_RET(inexact);
}

int mpfr_mat_norm_8(mpfr_ptr norm, mpfr_mat_ptr a){
  int rows = a->rows;
  int cols = a->cols;
  int i,j;
  
  mpfr_t tmp;
  mpfr_t tmp_n;
  int inexact = 0;

  MPFR_MAT_FUNC_INIT();

  mpfr_init2(tmp, mpfr_get_prec(norm));
  mpfr_init2(tmp_n, mpfr_get_prec(norm));

  mpfr_set_ui(norm, 0, GMP_RNDD);

  for (i = 0; i < cols; i++){
    mpfr_set_ui(tmp_n, 0, GMP_RNDD);
    for (j = 0; j < rows; j++){
      inexact |= mpfr_abs(tmp, MME(a,j,i), GMP_RNDU);
      inexact |= mpfr_add(tmp_n, tmp_n, tmp, GMP_RNDU);

      if(MPFR_MAT_TEST_FLAGS()){
	mpfr_clear(tmp);
	mpfr_clear(tmp_n);
	MPFR_MAT_FUNC_RET(inexact);
      }
    }
    if(mpfr_cmp(norm, tmp_n) < 0){
      mpfr_set(norm, tmp_n, GMP_RNDU);
    }
  }
  
  mpfr_clear(tmp);
  mpfr_clear(tmp_n);

  MPFR_MAT_FUNC_RET(inexact);
}

int mpfr_mat_norm_1_ext_ui(mpfr_ptr norm, mpfr_mat_ptr a, unsigned int ext){
  int rows = a->rows;
  int cols = a->cols;
  int i,j;
  
  mpfr_t tmp;
  mpfr_t tmp_n;
  mpfr_t tmpr;
  int inexact = 0;

  MPFR_MAT_FUNC_INIT();

  mpfr_init2(tmp, mpfr_get_prec(norm));
  mpfr_init2(tmp_n, mpfr_get_prec(norm));
  mpfr_init2(tmpr, mpfr_get_prec(norm));

  mpfr_set_ui(norm, 0, GMP_RNDD);

  for (i = 0; i < rows; i++){
    mpfr_set_ui(tmp_n, 0, GMP_RNDD);
    for (j = 0; j < cols; j++){
      if (j == i){
	inexact |= mpfr_sub_ui(tmpr, MME(a,i,j), ext, GMP_RND_MAX);
	inexact |= mpfr_abs(tmp, tmpr, GMP_RNDU);

      }
      else{
	mpfr_abs(tmp, MME(a,i,j), GMP_RNDU);
      }
      inexact |= mpfr_add(tmp_n, tmp_n, tmp, GMP_RNDU);

      if(MPFR_MAT_TEST_FLAGS()){
	mpfr_clear(tmp);
	mpfr_clear(tmp_n);
	mpfr_clear(tmpr);
	MPFR_MAT_FUNC_RET(inexact);
      }
    }
    if(mpfr_cmp(norm, tmp_n) < 0){
      mpfr_set(norm, tmp_n, GMP_RNDU);
    }
  }

  mpfr_clear(tmp);
  mpfr_clear(tmp_n);
  mpfr_clear(tmpr);

  MPFR_MAT_FUNC_RET(inexact);
}

int mpfr_mat_norm_8_ext_ui(mpfr_ptr norm, mpfr_mat_ptr a, unsigned int ext){
  int rows = a->rows;
  int cols = a->cols;
  int i,j;
  
  mpfr_t tmp;
  mpfr_t tmp_n;
  mpfr_t tmpr;
  int inexact = 0;

  MPFR_MAT_FUNC_INIT();

  mpfr_init2(tmp, mpfr_get_prec(norm));
  mpfr_init2(tmp_n, mpfr_get_prec(norm));
  mpfr_init2(tmpr, mpfr_get_prec(norm));

  mpfr_set_ui(norm, 0, GMP_RNDD);

  for (i = 0; i < cols; i++){
    mpfr_set_ui(tmp_n, 0, GMP_RNDD);
    for (j = 0; j < rows; j++){
      if (i == j){
	inexact |= mpfr_sub_ui(tmpr, MME(a,i,i), ext, GMP_RND_MAX);
	inexact |= mpfr_abs(tmp, tmpr, GMP_RNDU);
      }
      else{
	mpfi_mag(tmp, MME(a,j,i));
      }
      inexact |= mpfr_add(tmp_n, tmp_n, tmp, GMP_RNDU);

      if(MPFR_MAT_TEST_FLAGS()){
	mpfr_clear(tmp);
	mpfr_clear(tmp_n);
	mpfr_clear(tmpr);
	MPFR_MAT_FUNC_RET(inexact);
      }
    }
    if(mpfr_cmp(norm, tmp_n) < 0){
      mpfr_set(norm, tmp_n, GMP_RNDU);
    }
  }
  
  mpfr_clear(tmp);
  mpfr_clear(tmp_n);
  mpfr_clear(tmpr);

  MPFR_MAT_FUNC_RET(inexact);
}

int mpfr_mat_scaled_max_norm_colwise(mpfr_mat_srcptr a,
				     mpfr_mat_srcptr u,
				     mpfr_mat_ptr norm){
  int i, k, cmp;
  mpfr_t tmp, max;

  if (a->rows != u->rows)
    return -1;
  if (a->cols != norm->cols)
    return -1;
  int inexact = 0;

  MPFR_MAT_FUNC_INIT();

  mpfr_init2(tmp, mpfr_mat_get_prec(norm));
  mpfr_init2(max, mpfr_mat_get_prec(norm));

  for (k = 0; k < a->cols; k++){
    mpfr_div(max, MME(a,0,k),
	     MME(u,0,0), GMP_RNDU);

    for (i = 1; i < a->rows; i++){
      inexact |= mpfr_div(tmp, MME(a,i,k),
	       MME(u,i,0), GMP_RNDU);

      cmp = mpfr_cmp(tmp, max);
      if (cmp > 0){
	mpfr_swap(max, tmp);
      }

      if(MPFR_MAT_TEST_FLAGS()){
	mpfr_clear(tmp);
	mpfr_clear(max);
	MPFR_MAT_FUNC_RET(inexact);
      }	
    }

    mpfr_set(MME(norm,0,k), max, GMP_RNDU);
  }

  mpfr_clear(tmp);
  mpfr_clear(max);
  MPFR_MAT_FUNC_RET(inexact);
}

/*
  Tester si une matrice a est une H-matrice.
  Si oui, retourner le vecteur positif u telque a * u > 0
  Retour:
    1 : if a est une H-matrices
    0 : n'est pas certifie
 */
int mpfr_mat_is_H_mat(mpfr_mat_srcptr a, mpfr_mat_ptr u,
		      mpfr_mat_ptr prod){
  /*
    valider la taille
   */
  if(a->cols != u->rows || a->rows != prod->rows)
    return -1;
  if (u->cols != 1 || prod->cols != 1)
    return -1;

  int i;
  int cmp;
  int MAX_TRY_AMELIORATE_U = 15;
  char* complist;
  mpfr_t tmp;

  complist = (char*)malloc((size_t)(u->rows*sizeof(char)));
  mpfr_init2(tmp, mpfr_mat_get_prec(a));

  for (i = 0; i < u->rows; i++){
    mpfr_mat_set_ui(u, i,0,1, GMP_RNDD);
  }

  mpfr_mat_mul(prod, a, u, GMP_RNDD);

  cmp = 1;
  for (i = 0; i < u->rows; i++){
    complist[i] = mpfr_cmp_ui(MME(prod,i,0), 0);
    if (complist[i] <= 0)
      cmp = 0;
  }

  /*
    Tenter d'ameliorer si le produit <= 0
  */
  int try_ameliorate_u = 0;

  while (cmp <= 0 &&
	 try_ameliorate_u < MAX_TRY_AMELIORATE_U)
    {
      for (i = 0; i < u->rows; i++){
	if(complist[i] > 0){
	  mpfr_div(tmp, MME(prod,i,0), MME(a,i,i),
		   GMP_RNDD);
	  mpfr_div_ui(tmp, tmp, 2, GMP_RNDD);
	  mpfr_sub(MME(u,i,0), MME(u,i,0), tmp,
		   GMP_RNDU);
	}
      }

      // recalculer le produit
      mpfr_mat_mul(prod, a, u, GMP_RNDD);

      // retester si le produit > 0
      cmp = 1;
      for (i = 0; i < u->rows; i++){
	complist[i] = mpfr_cmp_ui(MME(prod,i,0), 0);
	if (complist[i] <= 0){
	  cmp = 0;
	}
      }
      try_ameliorate_u++;
    }

  mpfr_clear(tmp);
  free(complist);

  return cmp;
}

void mpfr_mat_max(mpfr_ptr max, mpfr_mat_srcptr a){
  int i, j;

  mpfr_set(max, MME(a,0,0), GMP_RNDU);

  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      if (mpfr_cmp(MME(a,i,j), max) > 0)
	mpfr_set(max, MME(a,i,j), GMP_RNDU);
    }
  }
}

void mpfr_mat_min(mpfr_ptr min, mpfr_mat_srcptr a){
  int i, j;

  mpfr_set(min, MME(a,0,0), GMP_RNDD);

  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      if (mpfr_cmp(MME(a,i,j), min) < 0)
	mpfr_set(min, MME(a,i,j), GMP_RNDD);
    }
  }
}

void mpfr_mat_minmax(mpfr_ptr min, mpfr_ptr max,
		     mpfr_mat_srcptr a){
  int i, j;

  mpfr_set(max, MME(a,0,0), GMP_RNDU);
  mpfr_set(min, MME(a,0,0), GMP_RNDD);

  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      if (mpfr_cmp(MME(a,i,j), min) < 0){
	mpfr_set(min, MME(a,i,j), GMP_RNDD);
      }
      else if (mpfr_cmp(MME(a,i,j), max) > 0){
	mpfr_set(max, MME(a,i,j), GMP_RNDU);
      }
    }
  }
}

void mpfr_mat_abs (mpfr_mat_ptr r, mpfr_mat_srcptr a,
		  mp_rnd_t rnd)
{
  if (a->rows != r-> rows || a->cols != r->cols)
    return;

  int i,j;
  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
      mpfr_abs(MME(r,i,j), MME(a,i,j), rnd);
    }
  }
}

void mpfr_mat_trans (mpfr_mat_ptr r, mpfr_mat_srcptr a,
		  mp_rnd_t rnd)
{
  if (a->rows != r->cols || a->cols != r->rows)
    return;

  int i,j;
  if (r != a){
    for (i = 0; i < a->rows; i++){
      for (j = 0; j < a->cols; j++){
	mpfr_set(MME(r,j,i), MME(a,i,j), rnd);
      }
    }
  }
  else {
    if (a->rows == a->cols){
      for (i = 0; i < a->rows; i++){
	for (j = i + 1; j < a->cols; j++){
	  mpfr_swap(MME(a,j,i), MME(a,i,j));
	}
      }
    }
    else{
      mpfr_mat_ptr tmp;
      mpfr_mat_init(tmp, a->cols, a->rows);
      for (i = 0; i < a->rows; i++){
	for (j = 0; j < a->cols; j++){
	  mpfr_swap(MME(tmp,j,i), MME(a,i,j));
	}
      }
      mpfr_mat_swap(tmp,a);
      mpfr_mat_clear(tmp);
    }
  }
}
