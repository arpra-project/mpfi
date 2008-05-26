/* test_mpfi.c -- Test file for mpfi.

Copyright (C) 2001, 2002 Spaces project, Inria Lorraine
                     Arenaire project, Inria Rhone-Alpes, France
                     and Lab. ANO, USTL (Univ. of Lille),  France


This file is part of the MPFI Library, based on the MPFR Library.

The MPFI Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Library General Public License as published by
the Free Software Foundation; either version 2 of the License, or (at your
option) any later version.

The MPFI Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
License for more details.

You should have received a copy of the GNU Library General Public License
along with the MPFR Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA 02111-1307, USA. */


#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "mpfi_io.h"
#include "mpfr_mat.h"

#define TWOPI (6.2831853071795864769252867665590057683943387987502) /* 2 * pi */
#define RAND (rand())/((double) RAND_MAX)
#define RANDN (sqrt(-2.0*log(RAND))*cos(TWOPI*RAND))

void perturbe(mpfr_mat_ptr r, mpfr_mat_srcptr a){
  int i,j, p;

  mpfr_t tmp;
  p = mpfr_mat_get_prec(a);
  p = p/2;

  mpfr_init2(tmp, 2 * p);

  for (i = 0; i < a->rows; i++){
    for (j = 0; j < a->cols; j++){
/*       mpfr_set_ui(tmp, rand(), GMP_RNDN); */
/*       mpfr_div_ui(tmp, tmp, RAND_MAX, GMP_RNDN); */
/*       mpfr_mul_2ui(tmp, tmp, 1, GMP_RNDN); */
      mpfr_set_d(tmp, RANDN, GMP_RNDN);
      mpfr_sub_ui(tmp, tmp, 1, GMP_RNDN);
      mpfr_div_2ui(tmp, tmp, p, GMP_RNDN);

      mpfr_add_ui(tmp, tmp, 1, GMP_RNDN);

      mpfr_mul(r->mems[i] + j, a->mems[i] + j, tmp, GMP_RNDN);
    }
  }

  mpfr_clear(tmp);
}

/*
  Calculer exactement m * a + n * b * c
 */
void fma_exact(mpfr_mat_ptr r, mpfr_mat_srcptr a,
	       mpfr_mat_srcptr b, mpfr_mat_srcptr c,
	       int m, int n, mp_rnd_t rnd){
  int i,j,k, exp;
  mpfr_t tmpmul;
  mpfr_t tmpsum;
  int prec;
  int emax, emin;

  mpfr_init2(tmpmul, mpfr_mat_get_prec(b) +
	     mpfr_mat_get_prec(c) + logb(abs(n)) + 1);
  mpfr_init(tmpsum);

  for(i = 0; i < b->rows; i++){
    for (j = 0; j < c->cols; j++){

      if(a != NULL && (mpfr_cmp_ui(a->mems[i]+j, 0) != 0)){
	emax = mpfr_get_exp(a->mems[i] + j) + logb(abs(m)) + 1;
	emin = mpfr_get_exp(a->mems[i] + j) + logb(abs(m));
      }
      else{
	emax = mpfr_get_emin();
	emin = mpfr_get_emax();
      }

      for (k = 0; k < b->cols; k++){
	if ((mpfr_cmp_ui(b->mems[i]+k, 0) != 0) && 
	    (mpfr_cmp_ui(c->mems[k]+j, 0) != 0)){
	  exp = mpfr_get_exp(b->mems[i] + k) +
	    mpfr_get_exp(c->mems[k] + j) + logb(abs(n));
	  
	  if (emax < exp + 1) emax = exp + 1;
	  if (emin > exp) emin = exp;
	}
      }

      if (a != NULL)
	prec = mpfr_mat_get_prec(a) + logb(abs(m)) + 1;
      else prec = 0;

      if (prec < mpfr_get_prec(tmpmul))
	prec = mpfr_get_prec(tmpmul);

      if (emax > emin)
	prec += (emax - emin) + logb(b->cols) + 1;

      mpfr_set_prec(tmpsum, prec);

      if (a != NULL){
	mpfr_set(tmpsum, a->mems[i] + j, rnd);
	mpfr_mul_si(tmpsum, tmpsum, m, rnd);
      }
      else{
	mpfr_set_ui(tmpsum, 0, rnd);
      }
      
      for (k = 0; k < b->cols; k++){
	mpfr_mul(tmpmul, b->mems[i] + k, c->mems[k] + j, rnd);
	mpfr_mul_si(tmpmul, tmpmul, n, rnd);
	mpfr_add(tmpsum, tmpsum, tmpmul, rnd);
      }

      mpfr_set(r->mems[i] + j, tmpsum, rnd);
    }
  }

  mpfr_clear(tmpmul);
  mpfr_clear(tmpsum);
}

void invert_rump(mpfr_mat_ptr r, mpfr_mat_srcptr a,
		 int Kmax, mp_rnd_t rnd){
  int k;
  mpfr_mat_t s, c,x;

  mpfr_mat_init2(s, a->rows, a->cols, mpfr_mat_get_prec(a));
  mpfr_mat_init2(c, a->rows, a->cols, mpfr_mat_get_prec(a));
  mpfr_mat_init2(x, a->rows, a->cols, mpfr_mat_get_prec(a));

  mpfr_mat_inv_lu(r, a, 1, rnd);

  while(mpfr_mat_is_inf(r)){
    perturbe(c, a);
    mpfr_mat_inv_lu(r, c, 1, rnd);
  }
  
  for (k = 0; k < Kmax; k++){
    fma_exact(c, NULL, r, a, 0, 1, rnd);

    mpfr_mat_inv_lu(x,c,1,rnd);
    
    while(mpfr_mat_is_inf(x)){
      perturbe(x, c);
      mpfr_mat_inv_lu(x, x, 1, rnd);
    }
    mpfr_mat_set_prec(s, (k + 2) * mpfr_mat_get_prec(a));
    fma_exact(s, NULL, x, r, 0, 1, rnd);
    mpfr_mat_swap(s, r);
  }

  mpfr_mat_clear(s);
  mpfr_mat_clear(c);
  mpfr_mat_clear(x);
}

/* void invert_rump2(mpfr_mat_ptr r, mpfr_mat_srcptr a, */
/* 		 int Kmax, mp_rnd_t rnd){ */
/*   int k; */
/*   mpfr_mat_t s, c,x; */

/*   mpfr_mat_init2(s, a->rows, a->cols, mpfr_mat_get_prec(a)); */
/*   mpfr_mat_init2(c, a->rows, a->cols, mpfr_mat_get_prec(a)); */
/*   mpfr_mat_init2(x, a->rows, a->cols, mpfr_mat_get_prec(a)); */

/*   mpfr_mat_inv_lu(r, a, 1, rnd); */

/*   while(mpfr_mat_is_inf(r)){ */
/*     perturbe(c, a); */
/*     mpfr_mat_inv_lu(r, c, 1, rnd); */
/*   } */
  
/*   for (k = 0; k < Kmax; k++){ */
/*     mpfr_mat_mul_ext(c, r, a, rnd, 2); */

/*     mpfr_mat_inv_lu(x,c,1,rnd); */
    
/*     while(mpfr_mat_is_inf(x)){ */
/*       perturbe(x, c); */
/*       mpfr_mat_inv_lu(x, x, 1, rnd); */
/*     } */
/*     mpfr_mat_set_prec(s, (k + 2) * mpfr_mat_get_prec(a)); */
/*     mpfr_mat_mul_ext(s, x, r, 0); */
/*     mpfr_mat_swap(s, r); */
/*   } */

/*   mpfr_mat_clear(s); */
/*   mpfr_mat_clear(c); */
/*   mpfr_mat_clear(x); */
/* } */

int main(int argc, char *argv[])
{
  int prec, nb_tests;
  int i, j, cptr;
  mpfi_t x,y,z;
  mpfr_t e, f, g;
  gmp_randstate_t state;
  mp_rnd_t rnd = GMP_RNDN;

  int dim = 12;
  mpfr_mat_t a, b,c, d;

  int pivot = 0;

  time_t start, end;

  start = clock();

  if (argc > 1){
    dim = atoi(argv[1]);
  }

  mpfr_mat_init(a, dim, dim);
  mpfr_mat_init(b, dim, dim);
  mpfr_mat_init(c, dim, dim);
  mpfr_mat_init(d, dim, dim);

  for (i = 0; i < dim; i++){
    for (j = 0; j < dim; j++){
      mpfr_mat_set_d(a, i, j, 1/((double)(i+j+1)), rnd);
    }
  }

  // Inverting the matrix using rump's method
  printf("****************** \n");
  printf("Inverting using rump's method and exact mul \n");
  invert_rump(b, a, 1, rnd);

  eyes_mat_fr(c, dim, dim);

  fma_exact(d, c, b, a, 1, -1, GMP_RNDD);

  mpfr_init2(e, mpfr_mat_get_prec(a));
  mpfr_init2(f, mpfr_mat_get_prec(a));

  mpfr_mat_norm_1(e, d);
  mpfr_mat_norm_8(f, d);

  printf("1er norm of I - RA = ");
  mpfr_out_str(stdout, 10, 5, e, GMP_RNDU);
  printf("\n");

  printf("Infinite norm of I - RA = ");
  mpfr_out_str(stdout, 10, 5, f, GMP_RNDU);
  printf("\n");

  end = clock();

  printf("running time: %g sec \n",
	 (double)(end - start)/CLOCKS_PER_SEC);

/*   // Inverting the matrix using rump's method */
/*   printf("****************** \n"); */
/*   printf("Inverting using rump's method and multi-precision mul \n"); */
/*   invert_rump2(b, a, 1, rnd); */

/*   fma_exact(d, c, b, a, 1, -1, GMP_RNDD); */

/*   mpfr_mat_norm_1(e, d); */
/*   mpfr_mat_norm_8(f, d); */

/*   printf("1er norm of I - RA = "); */
/*   mpfr_out_str(stdout, 10, 5, e, GMP_RNDU); */
/*   printf("\n"); */

/*   printf("Infinite norm of I - RA = "); */
/*   mpfr_out_str(stdout, 10, 5, f, GMP_RNDU); */
/*   printf("\n"); */

/*   end = clock(); */

/*   printf("running time: %g sec \n", */
/* 	 (double)(end - start)/CLOCKS_PER_SEC); */

  // Inverting the matrix by doubling the precision
  printf("****************** \n");
  printf("Inverting by doubling the precision \n");
  start = clock();

  mpfr_mat_set_prec(b, 2 *  mpfr_mat_get_prec(a));

  mpfr_mat_inv_lu(b, a, 1, rnd);

  fma_exact(d, c, b, a, 1, -1, GMP_RNDD);

  mpfr_init2(e, mpfr_mat_get_prec(a));
  mpfr_init2(f, mpfr_mat_get_prec(a));

  mpfr_mat_norm_1(e, d);
  mpfr_mat_norm_8(f, d);

  printf("1er norm of I - RA = ");
  mpfr_out_str(stdout, 10, 5, e, GMP_RNDU);
  printf("\n");

  printf("Infinite norm of I - RA = ");
  mpfr_out_str(stdout, 10, 5, f, GMP_RNDU);
  printf("\n");

  end = clock();

  printf("running time: %g sec \n",
	 (double)(end - start)/CLOCKS_PER_SEC);

  mpfr_mat_clear(a);
  mpfr_mat_clear(b);
  mpfr_mat_clear(c);
  mpfr_mat_clear(d);
  mpfr_clear(e);
  mpfr_clear(f);
  return(0);
}
