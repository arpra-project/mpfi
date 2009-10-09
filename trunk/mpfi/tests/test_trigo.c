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
the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
MA 02110-1301, USA. */


#include <stdlib.h>
#include <time.h>
#include "mpfi_io.h"

mpfi_t X, SinX, CosX, TanX;
mpfr_t x, y, z;
mpfr_t x2_sur_Pi, y2_sur_Pi, sinx, siny, cosx, cosy, tanx, tany;
mpfr_t Pi, Pi_sur_2, Pix2;

void mpfi_test(mpfi_ptr X)
{
  printf("Entree dans test : ");
  mpfi_out_str(stdout, 10, 10, X);
  printf("\n");
  mpfr_div(x2_sur_Pi, x, Pi_sur_2, GMP_RNDD);
  mpfr_div(y2_sur_Pi, y, Pi_sur_2, GMP_RNDU);
  printf("X * 2 / Pi = [ ");
  mpfr_out_str(stdout, 10, 0, x2_sur_Pi, GMP_RNDD);
  printf(" , ");
  mpfr_out_str(stdout, 10, 0, y2_sur_Pi, GMP_RNDU);
  printf(" ]\n");

  mpfi_sin(SinX, X);
  printf("sin X = ");
  mpfi_out_str(stdout, 10, 10, SinX);
  printf("\n");
  mpfr_sin(sinx, x, GMP_RNDD);
  mpfr_sin(siny, y, GMP_RNDU);
  printf("sin(x) = ");
  mpfr_out_str(stdout, 10, 10, sinx, GMP_RNDD);
  printf("\t sin(y) =  ");
  mpfr_out_str(stdout, 10, 10, siny, GMP_RNDU);
  printf("\n");

  mpfi_cos(CosX, X);
  printf("cos X = ");
  mpfi_out_str(stdout, 10, 10, CosX);
  printf("\n");
  mpfr_cos(cosx, x, GMP_RNDD);
  mpfr_cos(cosy, y, GMP_RNDU);
  printf("cos(x) = ");
  mpfr_out_str(stdout, 10, 10, cosx, GMP_RNDD);
  printf("\t cos(y) =  ");
  mpfr_out_str(stdout, 10, 10, cosy, GMP_RNDU);
  printf("\n");

  mpfi_tan(TanX, X);
  printf("tan X = ");
  mpfi_out_str(stdout, 10, 10, TanX);
  printf("\n");
  mpfr_tan(tanx, x, GMP_RNDD);
  mpfr_tan(tany, y, GMP_RNDU);
  printf("tan(x) = ");
  mpfr_out_str(stdout, 10, 10, tanx, GMP_RNDD);
  printf("\t tan(y) =  ");
  mpfr_out_str(stdout, 10, 10, tany, GMP_RNDU);
  printf("\n");

  mpfi_div_fr(X, X, Pi_sur_2);
  printf("X * 2 / Pi = ");
  mpfi_out_str(stdout, 10, 10, X);
  printf("\n");

  printf("\n");
}

int main(int argc, char *argv[])
{
  int prec, nb_tests;
  int i, cptr;
  mpfr_t e, f, k;
  gmp_randstate_t state;

  if (argc<2) {
    prec = 53;
    mpfr_set_default_prec(prec);
    nb_tests=100;
  }
  else if (argc<3) {
    prec = atoi(argv[1]);
    mpfr_set_default_prec(prec);
    nb_tests=100;
  }
  else {
    prec = atoi(argv[1]);
    mpfr_set_default_prec(prec);
    nb_tests=atoi(argv[2]);
  }

  mpfr_init(x); mpfr_init(y); mpfr_init(z);
  mpfr_init(x2_sur_Pi); mpfr_init(y2_sur_Pi);
  mpfr_init(sinx); mpfr_init(siny);
  mpfr_init(cosx); mpfr_init(cosy);
  mpfr_init(tanx); mpfr_init(tany);
  mpfr_init(Pi); mpfr_init(Pi_sur_2); mpfr_init(Pix2);
  mpfi_init(X);
  mpfi_init(SinX);
  mpfi_init(CosX);
  mpfi_init(TanX);

  mpfr_const_pi(Pi, GMP_RNDN);
  mpfr_div_2exp(Pi_sur_2, Pi, 1, GMP_RNDN);
  mpfr_mul_2exp(Pix2, Pi, 1, GMP_RNDN);
printf("Pi = ");
mpfr_out_str(stdout, 10, 0, Pi, GMP_RNDN);
printf("\nPi / 2 = ");
mpfr_out_str(stdout, 10, 0, Pi_sur_2, GMP_RNDN);
printf("\nPi x 2 = ");
mpfr_out_str(stdout, 10, 0, Pix2, GMP_RNDN);
printf("\n\n");

  gmp_randinit (state, GMP_RAND_ALG_LC, 128);
  gmp_randseed_ui (state, time(NULL));

  i=0;
  mpfr_init(e);
  mpfr_init(f);
  mpfr_init(k);

  /* tests avec des intervalles de longueur < Pi */
  for (cptr=0; cptr<nb_tests; cptr++) {
    mpfr_urandomb(e, state);
    mpfr_ui_div(e,1,e,GMP_RNDD);
    if (cptr%2)
      mpfr_neg(e,e,GMP_RNDD);
    mpfr_urandomb(f, state);
    mpfr_set(x,f, GMP_RNDD);
    mpfr_urandomb(f, state);
    mpfr_div(x,x,f, GMP_RNDD);
    mpfr_add(x, x,e,GMP_RNDD);

    mpfr_urandomb(e, state);
    mpfr_mul(y, e, Pi, GMP_RNDN);
    mpfr_add(y, x, y, GMP_RNDU);

    mpfi_interv_fr(X, x, y);
    mpfi_test(X);
  }

  /* tests avec des intervalles de longueur < 2 Pi */
  for (cptr=0; cptr<nb_tests; cptr++) {
    mpfr_urandomb(e, state);
    mpfr_ui_div(e,1,e,GMP_RNDD);
    if (cptr%2)
      mpfr_neg(e,e,GMP_RNDD);
    mpfr_urandomb(f, state);
    mpfr_set(x,f, GMP_RNDD);
    mpfr_urandomb(f, state);
    mpfr_div(x,x,f, GMP_RNDD);
    mpfr_add(x,x,e,GMP_RNDD);

    mpfr_urandomb(e, state);
    mpfr_mul(y, e, Pi, GMP_RNDN);
    mpfr_mul_2exp(y, y, 1, GMP_RNDN);
    mpfr_add(y, x, y, GMP_RNDU);

    mpfi_interv_fr(X, x, y);
    mpfi_test(X);
  }

  /* tests avec des intervalles de longueur Pi/2 + k*Pi */
  for (cptr=0; cptr<nb_tests; cptr++) {
    mpfr_urandomb(e, state);
    mpfr_ui_div(e,1,e,GMP_RNDD);
    if (cptr%2)
      mpfr_neg(e,e,GMP_RNDD);
    mpfr_urandomb(f, state);
    mpfr_set(x,f, GMP_RNDD);
    mpfr_urandomb(f, state);
    mpfr_div(x,x,f, GMP_RNDD);
    mpfr_add(x,x,e,GMP_RNDD);

    /* intervalle de longueur Pi/2 */
    mpfr_add(y, x, Pi_sur_2, GMP_RNDU);
    mpfi_interv_fr(X, x, y);
    mpfi_test(X);

    /* intervalle de longueur Pi/2 + Pi */
    mpfr_add(y, x, Pi_sur_2, GMP_RNDU);
    mpfr_add(y, y, Pi, GMP_RNDU);
    mpfi_interv_fr(X, x, y);
    mpfi_test(X);

    /* intervalle de longueur Pi/2 + 2*Pi */
    mpfr_add(y, x, Pi_sur_2, GMP_RNDU);
    mpfr_add(y, y, Pix2, GMP_RNDU);
    mpfi_interv_fr(X, x, y);
    mpfi_test(X);

    /* intervalle de longueur Pi/2 + k*Pi */
    mpfr_add(y, x, Pi_sur_2, GMP_RNDU);
    mpfr_urandomb(e, state);
    mpfr_ui_div(e,1,e,GMP_RNDD);
    mpfr_ceil(k, e);
    mpfr_mul(k, k, Pi, GMP_RNDU);
    mpfr_add(y, y, k, GMP_RNDU);
    mpfi_interv_fr(X, x, y);
    mpfi_test(X);
  }

  /* tests avec des intervalles de longueur > k*Pi */
  for (cptr=0; cptr<nb_tests; cptr++) {
    mpfr_urandomb(e, state);
    mpfr_ui_div(e,1,e,GMP_RNDD);
    if (cptr%2)
      mpfr_neg(e,e,GMP_RNDD);
    mpfr_urandomb(f, state);
    mpfr_set(x,f, GMP_RNDD);
    mpfr_urandomb(f, state);
    mpfr_div(x,x,f, GMP_RNDD);
    mpfr_add(x,x,e,GMP_RNDD);

    mpfr_urandomb(e, state);
    mpfr_mul(y, e, Pi, GMP_RNDN);
    mpfr_add(y,x,y,GMP_RNDU);
    mpfr_urandomb(e, state);
    mpfr_ui_div(e,1,e,GMP_RNDD);
    mpfr_ceil(k, e);
    mpfr_mul(k, k, Pi, GMP_RNDU);
    mpfr_add(y, k, y, GMP_RNDU);

    mpfi_interv_fr(X, x, y);
    mpfi_test(X);
  }

  /* tests des cas speciaux : 0, infini, NaN */
  for (cptr=0; cptr<nb_tests; cptr++) {
    mpfr_urandomb(e, state);
    mpfr_ui_div(e,1,e,GMP_RNDD);
    if (cptr%2)
      mpfr_neg(e,e,GMP_RNDD);
    mpfr_urandomb(f, state);
    mpfr_set(x,f, GMP_RNDD);
    mpfr_urandomb(f, state);
    mpfr_div(x,x,f, GMP_RNDD);
    mpfr_add(x,x,e,GMP_RNDD);

    mpfr_set_inf(y,1);
    mpfi_interv_fr(X, x, y);
    mpfi_test(X);

    mpfr_set_nan(y);
    mpfi_interv_fr(X, x, y);
    mpfi_test(X);

    if (mpfr_cmp_ui(x,0) < 0)
      mpfr_neg(x,x,GMP_RNDD);
    mpfr_set_ui(y,0, GMP_RNDU);
    mpfi_interv_fr(X, x, y);
    mpfi_test(X);

    mpfr_urandomb(e, state);
    mpfr_ui_div(e,1,e,GMP_RNDD);
    if (cptr%2)
      mpfr_neg(e,e,GMP_RNDD);
    mpfr_urandomb(f, state);
    mpfr_set(y,f, GMP_RNDU);
    mpfr_urandomb(f, state);
    mpfr_div(y,y,f, GMP_RNDD);
    mpfr_add(y,y,e,GMP_RNDD);

    mpfr_set_inf(x,-1);
    mpfi_interv_fr(X, x, y);
    mpfi_test(X);

    mpfr_set_nan(x);
    mpfi_interv_fr(X, x, y);
    mpfi_test(X);

    if (mpfr_cmp_ui(y,0) < 0)
      mpfr_neg(y,y,GMP_RNDU);
    mpfr_set_ui(x,0, GMP_RNDD);
    mpfi_interv_fr(X, x, y);
    mpfi_test(X);

    mpfr_set_inf(x, -1);
    mpfr_set_inf(y,1);
    mpfi_interv_fr(X, x, y);  /* X = ]-oo, +oo[ */
    mpfi_test(X);

    mpfr_set_inf(x, -1);
    mpfr_set_nan(y);
    mpfi_interv_fr(X, x, y); /* X = ]-oo, NaN [ */
    mpfi_test(X);

    mpfr_set_nan(x);
    mpfr_set_inf(y,1);
    mpfi_interv_fr(X, x, y); /* X = ]NaN, +oo[ */
    mpfi_test(X);

    mpfr_set_nan(x);
    mpfr_set_nan(y);
    mpfi_interv_fr(X, x, y); /* X = ]NaN, NaN [ */
    mpfi_test(X);

  }

  mpfr_clear(x);
  mpfr_clear(y);
  mpfr_clear(z);
  mpfr_clear(e);
  mpfr_clear(f);
  mpfr_clear(k);
  mpfr_clear(x2_sur_Pi); mpfr_clear(y2_sur_Pi);
  mpfr_clear(sinx); mpfr_clear(siny);
  mpfr_clear(cosx); mpfr_clear(cosy);
  mpfr_clear(tanx); mpfr_clear(tany);
  mpfr_clear(Pi); mpfr_clear(Pi_sur_2); mpfr_clear(Pix2);
  mpfi_clear(X);
  mpfi_clear(SinX);
  mpfi_clear(CosX);
  mpfi_clear(TanX);
  return(0);
}
