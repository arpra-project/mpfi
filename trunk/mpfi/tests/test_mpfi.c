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
#include "mpfi_io.h"

int mpfr_test_op(mpfi_srcptr z,mpfi_srcptr x,mpfi_srcptr y,
		 int (*op)(mpfr_ptr a, mpfr_srcptr b, mpfr_srcptr c, mp_rnd_t rnd_mode))
{
  mpfr_t e2,e1,e3;
  int dummy;
  mpfr_init(e2);
  dummy = mpfr_add(e2,&(x->left),&(x->right),GMP_RNDU);
  dummy = mpfr_div_ui(e2,e2,2,GMP_RNDU);
  mpfr_init(e1);
  dummy = mpfr_add(e1,&(y->left),&(y->right),GMP_RNDU);
  dummy = mpfr_div_ui(e1,e1,2,GMP_RNDU);
  mpfr_init(e3);
  dummy = op(e3,e2,e1,GMP_RNDU);
  if((mpfr_cmp(e3,&(z->left))>=0) && (mpfr_cmp(e3,&(z->right))<=0)){
    mpfr_clear(e1);
    mpfr_clear(e2);
    mpfr_clear(e3);
    return(1);
  }
  else {
    fprintf(stderr,"\n ERROR : ");
    mpfr_out_str(stderr,10,0,e2,GMP_RNDU);
    fprintf(stderr," * ");
    mpfr_out_str(stderr,10,0,e1,GMP_RNDU);
    fprintf(stderr," = ");
    mpfr_out_str(stderr,10,0,e3,GMP_RNDU);
    abort();
  }
  mpfr_clear(e1);
  mpfr_clear(e2);
  mpfr_clear(e3);
}

void mpfi_test(mpfi_ptr z,mpfi_srcptr x,mpfi_srcptr y)
{
  mpfi_add(z,x,y);
  mpfr_test_op(z,x,y,mpfr_add);
  if (mpfi_is_error()) 
    mpfi_reset_error();
  else {
  }
  mpfi_sub(z,x,y);
  mpfr_test_op(z,x,y,mpfr_sub);
  if (mpfi_is_error()) 
    mpfi_reset_error();
  else {
  }
  mpfi_mul(z,x,y);
  mpfr_test_op(z,x,y,mpfr_mul);
  if (mpfi_is_error()) 
    mpfi_reset_error();
  else {
  }
  mpfi_div(z,x,y);
  mpfr_test_op(z,x,y,mpfr_div);
  if (mpfi_is_error()) 
    mpfi_reset_error();
  else {
  }
}

int main(int argc, char *argv[])
{
  int prec, nb_tests;
  int i, cptr;
  mpfi_t x,y,z;
  mpfr_t e, f;
  gmp_randstate_t state;

  if (argc<2) {
    prec = 53;
    mpfr_set_default_prec(prec);
    nb_tests=100000;
  }
  else if (argc<3) {
    prec = atoi(argv[1]);
    mpfr_set_default_prec(prec);
    nb_tests=100000;
  }
  else {
    prec = atoi(argv[1]);
    mpfr_set_default_prec(prec);
    nb_tests=atoi(argv[2]);
  }

  mpfi_init(x);
  mpfi_init(y);
  mpfi_init(z);
  gmp_randinit (state, GMP_RAND_ALG_LC, 128);
  gmp_randseed_ui (state, time(NULL));

  i=0;
  mpfr_init(e);
  mpfr_init(f);

  for (cptr=0; cptr<nb_tests; cptr++) {
    mpfr_urandomb(e, state);
    mpfr_ui_div(e,1,e,GMP_RNDD);
    mpfr_urandomb(f, state);
    mpfi_set_fr(x,f);
    mpfr_urandomb(f, state);
    mpfi_div_fr(x,x,f);
    mpfi_increase(x,e);

    mpfr_urandomb(e, state);
    mpfr_ui_div(e,1,e,GMP_RNDD);
    mpfr_urandomb(f, state);
    mpfi_set_fr(y,f);
    mpfr_urandomb(f, state);
    mpfi_div_fr(y,y,f);
    if(cptr%2) mpfi_neg(y,y);
    mpfi_increase(y,e);

    mpfi_test(z,x,y);
  }
  mpfi_clear(x);
  mpfi_clear(y);
  mpfi_clear(z);
  mpfr_clear(e);
  mpfr_clear(f);
  return(0);
}
