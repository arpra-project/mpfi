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
#include "mpfi_mat.h"
#include "matrix_generator.h"
#include <time.h>

void midrad(FILE *stream, int base, int prec,mpfi_t x)
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

void middia(FILE *stream, int base, int base2, int prec,mpfi_t x)
{
  mpfr_t tmp;
  mpfr_init2(tmp, mpfi_get_prec(x));
  fprintf(stream, "{ ");
  mpfi_mid(tmp, x);
  mpfr_out_str(stream, base, prec, tmp, GMP_RNDD);
  fprintf(stream, ",");
  mpfr_sub(tmp, &(x->right), &(x->left), GMP_RNDU);
  mpfr_out_str(stream, base2, prec, tmp, GMP_RNDU);
  fprintf(stream, " }");  
}

void test_mpfi_solution(int dim, int iterations,
			int input_prec, int output_prec)
{
  int i, j, cptr = 0;

  mpfi_mat_t a, b;
  mpfi_mat_t x, y;

  mpfr_t tmp1, tmp2;

  mpfi_mat_init2(a, dim, dim, input_prec);
  mpfi_mat_init2(b, dim, 1, input_prec);
  mpfi_mat_init2(x, dim, 1, output_prec);
  mpfi_mat_init2(y, dim, 1, output_prec);

  mpfr_init2(tmp1, input_prec);
  mpfr_init2(tmp2, input_prec);

  for (i = 0; i < dim; i++){
    for (j = 0; j < dim; j++){
      mpfr_set_ui(tmp1, i + j + 1, GMP_RNDD);
      mpfr_set_ui(tmp2, i + j + 1, GMP_RNDD);
      mpfr_ui_div(tmp1, 1, tmp1, GMP_RNDD);
      mpfr_ui_div(tmp2, 1, tmp2, GMP_RNDU);
      mpfi_interv_fr(a->mems[i]+j, tmp1, tmp2);
    }
  }

  for (i = 0; i < dim; i++){
    mpfi_mat_set_ui(b, i, 0, 1);
  }

  /*
    resoudre un syteme lineare
    @param 1: vecteur interval du resultat
    @param 2: matrice mpfi gauche
    @param 3: matrice mpfi droite
    @param 4: nombre d'iteration pour la Gauss-Seidel
    @param 5: pivotage ou non
  */
  cptr = mpfi_solve_mpfi(x,a,b, iterations , 1);

  if (cptr == 0){
    fprintf(stdout, "Le resultat est cetifie \n");
  }
  else {
    fprintf(stdout,"!!! Le resultat n'est pas cetifie !!!\n");
  }

  mpfi_mat_out_str_midrad(stdout,10,0,x);

  mpfi_mat_clear(a);
  mpfi_mat_clear(b);
  mpfi_mat_clear(x);
  mpfi_mat_clear(y);
  mpfr_clear(tmp1);
  mpfr_clear(tmp2);
}


void test_mpfr_solution(int dim, int iterations,
			int input_prec, int output_prec, int input_matrix)
{
  int i, j, cptr = 0;

  mpfr_mat_t a, b, tmp;
  mpfi_mat_t x,y;

  time_t start, end;

  start = clock();

  mpfr_mat_init2(a, dim, dim, input_prec);
  mpfr_mat_init2(b, dim, 1, input_prec);
  mpfr_mat_init2(tmp, dim, 1, input_prec);
  mpfi_mat_init2(x, dim, 1, output_prec);
  mpfi_mat_init2(y, dim, 1, output_prec);

  for (i = 0; i < dim; i++){
    mpfr_mat_set_ui(tmp, i, 0, i+1, GMP_RNDN);
  }

  if(input_matrix == 0){
    hilbert_mat_fr(a, dim, dim, GMP_RNDN);
  }
  else{
    pascal_mat_fr(a, dim, dim);
  }

  ones_mat_fr(tmp, dim,1);
  mpfr_mat_mul(b, a, tmp, GMP_RNDN);
  mpfr_mat_clear(tmp);

  /*
    resoudre un syteme lineare
    @param 1: vecteur interval du resultat
    @param 2: matrice mpfr gauche
    @param 3: matrice mpfr droite
    @param 4: nombre d'iteration pour la Gauss-Seidel
    @param 5: pivot ou non
  */
  cptr = mpfi_solve_mpfr(x,a,b, iterations, 1);

  //mpfi_mat_out_str_midrad(stdout,10,0,x);

  /*
    Calculer le nombre de bits exacts
  */
  mpfr_t diam;
  mpfr_init2(diam, mpfi_mat_get_prec(x));
  int nb_bits = mpfi_mat_get_prec(x);

  for (i = 0; i < dim; i++){
    mpfr_sub(diam, &(x->mems[i][0].right), &(x->mems[i][0].left), GMP_RNDU);
    if (mpfr_cmp_ui(diam, 0) > 0){
      if (mpfr_get_exp(&(x->mems[i][0].left)) - mpfr_get_exp(diam) < nb_bits){
	nb_bits = mpfr_get_exp(&(x->mems[i][0].left)) - mpfr_get_exp(diam);
      }
    }
  }

  mpfr_clear(diam);

  if (nb_bits < 0) nb_bits = 0;
  fprintf(stdout, "\n*** number of bits exact: %d bits \n", nb_bits);


  mpfr_mat_clear(a);
  mpfr_mat_clear(b);
  mpfi_mat_clear(x);
  mpfi_mat_clear(y);

  end = clock();

  fprintf(stdout, "*** Running time : %g sec \n",
	  (double)(end - start)/CLOCKS_PER_SEC);
}


int main(int argc, char *argv[])
{
  char* system_type;
  system_type = (char*) malloc(200 * sizeof(char));
  sprintf(system_type, "mpfr");

  int nb_tests;
  int i, j, cptr = 0;
  gmp_randstate_t state;
  mp_rnd_t rnd = GMP_RNDN;
  int prec = 53;
  int input_matrix_type = 0;

  int dim = 11;
  mpfi_mat_t a, b;
  mpfi_mat_t x;
  int iter = 1;

  mpfr_t tmp1, tmp2;

  /*
    Analyser les arguments
   */
  int nb_args = (argc - 1) / 2;

  if (argc == 1 || strcmp(argv[1], "-help") == 0 || strcmp(argv[1], "?") == 0){
    char* usage;
    usage = (char*) malloc(300 * sizeof(char));
    sprintf(usage, "Usage: %s [options]", argv[0]);
    sprintf(usage, "%s\n Options: ", usage);
    sprintf(usage, "%s\n    -prec : precision of input matrix [default 53]", usage);
    sprintf(usage, "%s\n    -dim  : dimension of system", usage);
    sprintf(usage, "%s\n    -iter : number of Gauss-Seidel iterations", usage);
    sprintf(usage, "%s\n    -sys  : type of system (mpfr or mpfi [default mpfr])", usage);
    sprintf(usage, "%s\n    -inmat: input matrix (hilbert or pascal [default hilbert])", usage);
    sprintf(usage, "%s \n", usage);

    fprintf(stdout, usage);

    free(usage);
    free(system_type);
    return 0;
  }

  for (i = 0; i < nb_args; i++){
    if (strcmp(argv[2*i+1],"-dim") == 0){
      dim = atoi(argv[2*i+2]);
    }
    else if (strcmp(argv[2*i+1],"-prec") == 0){
      prec = atoi(argv[2*i+2]);
    }
    else if (strcmp(argv[2*i+1],"-iter") == 0){
      iter = atoi(argv[2*i+2]);
    }
    else if (strcmp(argv[2*i+1],"-sys") == 0){
      sprintf(system_type, "%s", argv[2*i+2]);
    }
    else if (strcmp(argv[2*i+1],"-mtype") == 0){
      if (strcmp(argv[2*i+2], "pascal") == 0)
	input_matrix_type = 1;
    }
    else{
      fprintf(stdout, "Unrecognized argument: '%s' !! \n", argv[2*i+1]);
      free(system_type);
      return -1;
    }
  }


  /*
  if (argc > 1){
    dim = atoi(argv[1]);
  }
  if (argc > 3){
    iter = atoi(argv[3]);
  }
  if (argc > 2){
    prec = atoi(argv[2]);
  }
  */

  fprintf(stdout, "Test the resolution of linear system using Gauss-Seidel and interval arithmetic \n");
  fprintf(stdout, "   Type of input data                : %s \n", system_type);
  fprintf(stdout, "   Dimension of system               : %d \n", dim);
  fprintf(stdout, "   Number of Gauss-Seidel iterations : %d \n", iter);

  if (strcmp(system_type, "mpfr") == 0){
    test_mpfr_solution(dim, iter, prec, prec, input_matrix_type);
  }
  else if (strcmp(system_type, "mpfi") == 0){
    test_mpfi_solution(dim, iter, prec, prec);
  }
  

  //fprintf(stdout, "\n===================================== \n");

  free(system_type);
  return(0);
}
