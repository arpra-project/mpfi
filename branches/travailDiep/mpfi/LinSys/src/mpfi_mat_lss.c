
#include "mpfi_mat.h"
#include "mpfr_mat.h"
#include <time.h>

#define VERSION "0.0.1"

#define DEBUG

#define RESIDU_PREC_FUNCTION(p) p+32
#define GAUSS_ITERATIONS 5
#define MAX_TRY_INVERT_A 2

#define MME(a, ind_i, ind_j) a->mems[ind_i]+ind_j

#define debug(str)				\
  fprintf(stdout,"*** debug: %s",str);
#define debug1(str, param1)				\
  fprintf(stdout,"*** debug: ");			\
  fprintf(stdout,str,param1);				\
  fprintf(stdout, "\n");
#define debug2(str, param1, param2)	\
  fprintf(stdout,"*** debug: ");	\
  fprintf(stdout,str,param1, param2);	\
  fprintf(stdout, "\n");

int __divsect(mpfi_ptr r, mpfi_srcptr a,
	      mpfi_srcptr b, mpfi_srcptr x){
  int inexact = 0;
  mpfr_t inf, sup;
  int cmp_al, cmp_ar, cmp_bl, cmp_br;

  if ( mpfi_nan_p(a) || mpfi_nan_p(b)){
    return 0;
  }

  cmp_al = mpfr_cmp_ui(&(a->left),0);
  cmp_ar = mpfr_cmp_ui(&(a->right),0);

  if ((cmp_al > 0) || (cmp_ar < 0)){
    mpfi_t tmp;
    mpfi_init2(tmp, mpfi_get_prec(r));
    inexact |= mpfi_div(tmp, b, a);
    inexact |= mpfi_intersect(r, tmp, x);
  }
  else{
    cmp_bl = mpfr_cmp_ui(&(b->left),0);
    cmp_br = mpfr_cmp_ui(&(b->right),0);
    if((cmp_bl <= 0) && (cmp_br >= 0)){
      if (r == x){
	return 0;
      }
      else {
	mpfi_set(r, x);
	return 0;
      }
    }
    else{
      mpfr_init2(inf, mpfi_get_prec(r));
      mpfr_init2(sup, mpfi_get_prec(r));
      // b > 0
      if (cmp_bl > 0){
	inexact |= mpfr_div(sup, &(b->left),
			    &(a->left), GMP_RNDU);
	inexact |= mpfr_div(inf, &(b->left),
			    &(a->right), GMP_RNDD);
      }
      // b < 0
      else{
	inexact |= mpfr_div(sup, &(b->right),
			    &(a->right), GMP_RNDU);
	inexact |= mpfr_div(inf, &(b->right),
			    &(a->left), GMP_RNDD);
      }

      cmp_al = mpfr_cmp(&(x->left),sup);
      cmp_ar = mpfr_cmp(&(x->right),inf);
      if((cmp_al <= 0) && (cmp_ar >= 0)){
	if (r == x){
	  return 0;
	}
	else {
	  inexact |= mpfi_set(r, x);
	  return inexact;	
	}
      }
      else if ((cmp_al > 0) && (cmp_ar < 0)) {
	mpfr_set_nan(&(r->left));
	mpfr_set_nan(&(r->right));
	return 3;
      }
      else{
	if (cmp_al > 0){
	  inexact |= mpfr_set(&(r->left), inf, GMP_RNDD);
	  inexact |= mpfr_set(&(r->right), &(x->right),
			      GMP_RNDU);
	}
	else{
	  inexact |= mpfr_set(&(r->right), sup, GMP_RNDD);
	  inexact |= mpfr_set(&(r->left), &(x->left),
			      GMP_RNDU);
	}
	return inexact;
      }
    }
  }
}

int iterate_Gauss_Seidel(mpfi_mat_ptr r,
			 mpfi_mat_srcptr a,
			 mpfi_mat_srcptr b,
			 mpfi_mat_srcptr x)
{
  if (r->rows != x->rows || r->cols != x->cols){
    return -1;
  }
  if (b->rows != x->rows || b->cols != x->cols){
    return -1;
  }
  if (a->cols != x->rows || a->rows != a->cols){
    return -1;
  }

  int dim = a->rows;
  int nbsys = b->cols;
  int i,j,k;
  int inexact = 0;

  mpfi_t tmp, tmp2;

  mpfi_init2(tmp, mpfi_mat_get_prec(r));
  mpfi_init2(tmp2, mpfi_mat_get_prec(r));

  for (k = 0; k < nbsys; k++){
    for (i = 0; i < dim; i++){
      mpfi_set(tmp, b->mems[i]+k);
      for (j = 0; j < i; j++){
	inexact |= mpfi_mul(tmp2, MME(a,i,j),
			    MME(r,j,k));
	inexact |= mpfi_sub(tmp, tmp, tmp2);
      }
      for (j = i+1; j < dim; j++){
	inexact |= mpfi_mul(tmp2, MME(a,i,j),
			    MME(x,j,k));
	inexact |= mpfi_sub(tmp, tmp, tmp2);
      }
      inexact |= __divsect(MME(r,i,k), MME(a,i,i),
			   tmp, MME(x,i,k));
    }
  }
  mpfi_clear(tmp);
  mpfi_clear(tmp2);

  return inexact;
}


/*
  resoudre un syteme lineare
  @param 1: vecteur interval du resultat
  @param 2: matrice mpfr de coefficients
  @param 3: matrice mpfr droite
  @param 4: nombre de passages de Gauss-Seidel
  @param 5: utiliser le pivotage ou non
 */
int mpfi_solve_mpfr(mpfi_mat_ptr x,
		    mpfr_mat_srcptr A,
		    mpfr_mat_srcptr b,
		    int MAX_ITERATION,
		    int pivot)
{
  if (x->rows != b->rows || x->cols != b->cols){
    return -1;
  }

  if (A->cols != x->rows || A->rows != A->cols){
    return -1;
  }

  int dim = A->rows;
  int nbsys = b->cols;
  int i,j,k, cmp;
  int inexact = 0;
  int imin;
  int inv_prec, A_condition_prec;
  int residu_prec, b_condition_prec;
  int try_invert_A, try_precondition_A;

  time_t start, start_inv, start_precond;
  time_t end,   end_inv,   end_precond;
  time_t start_iter, start_test;
  time_t end_iter,   end_test;
  mpfr_mat_t midA, midb, approx, R, u, v;
  mpfi_mat_t residu, RA, Z;

  start = clock();

  inv_prec =  mpfi_mat_get_prec(x);
  A_condition_prec = inv_prec;
  
  mpfr_mat_init2(midA, dim, dim, inv_prec);
  mpfr_mat_init2(midb, dim, nbsys, inv_prec);
  mpfr_mat_init2(R, dim, dim, inv_prec);
  mpfr_mat_init2(approx, dim, nbsys, inv_prec);
  mpfr_mat_init2(u, dim, 1, inv_prec);
  mpfr_mat_init2(v, dim, 1, inv_prec);
  mpfi_mat_init2(RA, dim, dim, A_condition_prec);

  /*
    Calculer une approximation flottante du resultat
    en utilisant la matrice inverse de A
    Si la verification echoue, on essai d'augmenter
    la precision de la matrice inverse
   */
  try_invert_A = 0;
  while (try_invert_A < MAX_TRY_INVERT_A){
    try_invert_A++;
    if(try_invert_A > 1){
      inv_prec *= 2;
      A_condition_prec = inv_prec;
      mpfr_mat_set_prec(R, inv_prec);
      //mpfi_mat_set_prec(RA, A_condition_prec);
    }
    /*
      Calculer la matrice inverse de A avec 
      la precision: inv_prec
     */

#ifdef DEBUG
    debug1("try to invert A with precision: %d",
	   inv_prec);
    start_inv = clock();
#endif // of DEBUG

    mpfr_mat_inv_lu(R, A, pivot, GMP_RNDN);

#ifdef DEBUG
    end_inv = clock();
    debug1(" ---> Time to calculate the inverse : %g sec",
	   (double)(end_inv - start_inv)/CLOCKS_PER_SEC);

    debug("Estimated Conditionnement of matrix A:  \n");

    mpfr_t ca, ca1;
    mpfr_init2(ca, mpfi_mat_get_prec(x));
    mpfr_init2(ca1, mpfi_mat_get_prec(x));
    mpfr_mat_norm_1(ca, A);
    mpfr_mat_norm_1(ca1, R);
    mpfr_mul(ca, ca, ca1, GMP_RNDU);
    mpfr_log2(ca1, ca, GMP_RNDU);

    debug("     K1 = ");
    mpfr_out_str(stdout, 10, 5, ca, GMP_RNDU);
    fprintf(stdout,"     log2(K1) = ");
    mpfr_out_str(stdout, 10, 5, ca1, GMP_RNDU);
    fprintf(stdout,"\n");
    mpfr_clear(ca);
    mpfr_clear(ca1);
#endif // of DEBUG

    /*
      transformer le systeme original dans un autre systeme
      pour borner l'erreur
                   A  * x = b
     --> (R [*] A) * x = (R [*] b)
     [o] singnifie les operations par interval
     Si la verification echoue on essai d'augmenter 
     la precision de la matrice conditionee.
    */

    int prec_prec;
#ifdef DEBUG
    debug1("precondition matrix A with precision : %d bits",
	   mpfi_mat_get_prec(RA));
    start_precond = clock();
#endif // of DEBUG

    mpfrri_mat_mul(RA, R, A);

#ifdef DEBUG
    end_precond = clock();
    debug1(" ---> Time to precondition the left matrix : %g sec",
	   (double)(end_precond - start_precond)/CLOCKS_PER_SEC);
#endif // of DEBUG

#ifdef DEBUG
    debug(" testing if RA is an H-matrix \n");
    start_test = clock();
#endif // of DEBUG

    mpfi_mat_compmat(midA, RA);
    cmp = mpfr_mat_is_H_mat(midA, u, v);

#ifdef DEBUG
    end_test = clock();
    debug1(" ---> Time to test the conditioned matrix : %g sec",
	   (double)(end_test - start_test)/CLOCKS_PER_SEC);
#endif // of DEBUG


    if (cmp > 0){
      try_invert_A = MAX_TRY_INVERT_A;
    }
    else{
#ifdef DEBUG
      debug("!!! RA is not an H-matrix !!!\n");
#endif // of DEBUG
    }
  }


#ifdef DEBUG
  /* detecter la norme de la matrice I - RA */
  mpfr_t norm;
  mpfr_init2(norm, mpfi_mat_get_prec(RA));

  debug("The 1st-order norm of I - RA is : ");
  mpfi_mat_norm_1_ext_ui(norm, RA, 1);
  mpfr_out_str(stdout, 10, 8, norm, GMP_RNDU);
  fprintf(stdout, "\n");

  debug("The infinite  norm of I - RA is : ");
  mpfi_mat_norm_8_ext_ui(norm, RA, 1);
  mpfr_out_str(stdout, 10, 8, norm, GMP_RNDU);
  fprintf(stdout, "\n");
#endif

  /*
   *  Calculer une approximation flottante du resultat
   */
  mpfr_mat_mul(approx, R, b, GMP_RNDN);

  /*
   * Si RA n'est pas une H-matrice, le systeme n'est pas 
   * certifie par ce programme
   */
  if (cmp <= 0){

#ifdef DEBUG
    debug("!!! The solution is not certified !!! \n");
#endif

    for (k = 0; k < nbsys; k++){
      for (i = 0; i < dim; i++)
	mpfi_set_fr(MME(x,i,k), MME(approx,i,k));
    }

    mpfr_mat_clear(midA);
    mpfr_mat_clear(midb);
    mpfr_mat_clear(approx);
    mpfr_mat_clear(R);
    mpfi_mat_clear(RA);
    return 8;
  }

#ifdef DEBUG
  debug("The solution is certified. \n");
#endif


  /*
   * Essayer d'appliquer la methode iterative
   */

  int gauss_passage;
  mpfi_mat_t tmpresidu;
  mpfr_mat_t diameter, new_diameter, diff;
  mpfr_t max_diff;
  mpfr_t diam, mag;
  mpfr_init2(diam, mpfi_mat_get_prec(x));
  mpfr_init2(mag, mpfi_mat_get_prec(x));
  int nb_bits = mpfi_mat_get_prec(x);

  double epsilon = 0.00001;
  mpfi_mat_init(tmpresidu, dim, nbsys);

  mpfr_mat_init2(diameter, dim, nbsys, mpfi_mat_get_prec(x));
  mpfr_mat_init2(new_diameter, dim, nbsys, mpfi_mat_get_prec(x));
  mpfr_mat_init2(diff, dim, nbsys, mpfi_mat_get_prec(x));
  mpfr_init2(max_diff, mpfi_mat_get_prec(x));

  mpfi_mat_init2(residu, dim, nbsys, mpfi_mat_get_prec(x));

  residu_prec = RESIDU_PREC_FUNCTION(mpfi_mat_get_prec(x));
  mpfi_mat_set_prec(tmpresidu, residu_prec);

  for (gauss_passage = 0; gauss_passage < MAX_ITERATION; gauss_passage++){
    /*
     *  Calculer le residu
     */
#ifdef DEBUG
    debug1("Calculate the residual of the system with precision: %d",
	   residu_prec);
#endif

    mpfrri_mat_mul(tmpresidu, A, approx);
    mpfri_mat_sub(residu, b, tmpresidu);


    /*
     * Preconditionner la matrice droite
     */
    b_condition_prec = mpfi_mat_get_prec(x);
    mpfi_mat_init2(Z, dim, nbsys, b_condition_prec);

#ifdef DEBUG
    debug1("precondition matrix b with precision : %d bits", b_condition_prec);
    start_precond = clock();
#endif // of DEBUG

    mpfri_mat_mul(Z, R, residu);

    /*
     * Pour la premiere fois, il faut estimer une
     * solution initiale pour le systeme residu
     */
    if (gauss_passage == 0){
      // Calculer ||Z||_v
      mpfi_mat_mag(midb, Z);  

      mpfr_mat_scaled_max_norm_colwise(midb, v, midb);

      // Calculer l'estimation initiale de l'erreur
      // e0 = ||Z||_v * [-u, u]
      for (k = 0; k < nbsys; k++){
	for (i = 0; i < dim; i++){
	  mpfr_mul(&((MME(x,i,k))->right), MME(u,k,0),
		   MME(midb,0,k), GMP_RNDU);
	  mpfr_neg(&((MME(x,i,k))->left), 
		   &((MME(x,i,k))->right),
		   GMP_RNDD);
	}
      }
      mpfi_mat_diam(diameter, x);
    }
    /*
     * Pour les prochaines fois, il faut seulement
     * mettre a jour l'approximation et l'erreur
     */
    else{
      mpfi_mat_diam(new_diameter, x);
      mpfr_mat_sub(diff, diameter, new_diameter, GMP_RNDU);
      mpfr_mat_dotdiv(diff, diff, diameter, GMP_RNDU);
      mpfr_mat_max(max_diff, diff);
      // if there's no change in the diameter, stop
      if (mpfr_cmp_d(max_diff, epsilon) < 0){
#ifdef DEBUG
	debug("Cannot get more bits exact \n");
#endif
	break;
      }
      mpfr_mat_swap(diameter, new_diameter);

      mpfir_mat_sub(x, x, approx);
    }

#ifdef DEBUG
    debug1("%d. Solve the residual problem using Gauss-Seidel", gauss_passage);
    start_iter = clock();
    debug("Number of bits exact of erreur: \n");
    debug("       0 ");
#endif 

    for (i = 0; i < GAUSS_ITERATIONS; i++){
      iterate_Gauss_Seidel(x, RA, Z, x);
#ifdef DEBUG
      nb_bits = mpfi_mat_get_prec(x);
      for (j = 0; j < dim; j++){
	if (mpfi_is_zero(MME(x,j,0))){
	  mpfi_mag(diam, MME(x,j,0));
	  if (nb_bits > (0 - mpfr_get_exp(diam)))
	    nb_bits = 0 - mpfr_get_exp(diam);
	}
	else{
	  mpfi_diam(diam, MME(x,j,0));
	  mpfi_mag(mag, MME(x,j,0));
	  if (mpfr_get_exp(mag) - mpfr_get_exp(diam) < nb_bits){
	    nb_bits = mpfr_get_exp(mag) - mpfr_get_exp(diam);
	  }
	}
      }
      if (nb_bits < 0) nb_bits = 0;
      fprintf(stdout, "--> %d ", nb_bits);
#endif
    }

#ifdef DEBUG
    fprintf(stdout,"\n");
#endif

    mpfir_mat_add(x, x, approx);

    nb_bits = mpfi_mat_get_prec(x);
    for (j = 0; j < dim; j++){
      if (mpfi_is_zero(MME(x,j,0))){
	mpfi_mag(diam, MME(x,j,0));
	if (nb_bits > (0 - mpfr_get_exp(diam)))
	  nb_bits = 0 - mpfr_get_exp(diam);
      }
      else{
	mpfi_diam(diam, MME(x,j,0));
	if (mpfr_cmp_ui(diam, 0) > 0){
	  mpfi_mag(mag, MME(x,j,0));

	  if (mpfr_get_exp(mag) - mpfr_get_exp(diam) < nb_bits){
	    nb_bits = mpfr_get_exp(mag) - mpfr_get_exp(diam);
	  }
	}
      }
    }
    if (nb_bits < 0) nb_bits = 0;

#ifdef DEBUG
    debug1("Number of bits exact of result : %d ", nb_bits);
#endif

    if (nb_bits >= mpfi_mat_get_prec(x)){
#ifdef DEBUG
      debug("Reach the maximum number of bits exact ! Stop \n");
#endif
     
      break;
    }

    mpfi_mat_mid(approx, x);
  }

#ifdef DEBUG
  end_iter = clock();
  debug1(" ---> Time to verify the solution : %g sec",
	 (double)(end_iter - start_iter)/CLOCKS_PER_SEC);
#endif // of DEBUG

  mpfi_mat_clear(tmpresidu);
  mpfr_mat_clear(midA);
  mpfr_mat_clear(midb);
  mpfr_mat_clear(approx);
  mpfr_mat_clear(R);
  mpfi_mat_clear(residu);
  mpfi_mat_clear(RA);
  mpfi_mat_clear(Z);

  mpfr_mat_clear(diameter);
  mpfr_mat_clear(new_diameter);
  mpfr_mat_clear(diff);
  mpfr_clear(max_diff);
  mpfr_clear(diam);
  mpfr_clear(mag);

  return inexact;
}

/*
  resoudre un syteme lineare
  @param 1: vecteur interval du resultat
  @param 2: matrice mpfi de coefficients
  @param 3: matrice mpfi droite
  @param 4: nombre de passages de Gauss-Seidel
  @param 5: utiliser le pivotage ou non
 */
int mpfi_solve_mpfi(mpfi_mat_ptr x,
		    mpfi_mat_srcptr A,
		    mpfi_mat_srcptr b,
		    int MAX_ITERATION,
		    int pivot)
{
  if (x->rows != b->rows || x->cols != b->cols){
    return -1;
  }

  if (A->cols != x->rows || A->rows != A->cols){
    return -1;
  }

  int dim = A->rows;
  int nbsys = b->cols;
  int i,j,k, cmp;
  int inexact = 0;
  int imin;
  int inv_prec, A_condition_prec;
  int residu_prec, b_condition_prec;
  int try_invert_A, try_precondition_A;

  time_t start, start_inv, start_precond;
  time_t end,   end_inv,   end_precond;
  time_t start_iter, start_test;
  time_t end_iter,   end_test;
  mpfr_mat_t midA, midb, approx, R, u, v;
  mpfi_mat_t residu, RA, Z;

  start = clock();

  inv_prec =  mpfi_mat_get_prec(x);
  A_condition_prec = inv_prec;
  
  mpfr_mat_init2(midA, dim, dim, inv_prec);
  mpfr_mat_init2(midb, dim, nbsys, inv_prec);
  mpfr_mat_init2(R, dim, dim, inv_prec);
  mpfr_mat_init2(approx, dim, nbsys, inv_prec);
  mpfr_mat_init2(u, dim, 1, inv_prec);
  mpfr_mat_init2(v, dim, 1, inv_prec);
  mpfi_mat_init2(RA, dim, dim, A_condition_prec);

  /*
    Calculer une approximation flottante du resultat
    en utilisant la matrice inverse de A
    Si la verification echoue, on essai d'augmenter
    la precision de la matrice inverse
   */
  try_invert_A = 0;
  while (try_invert_A < MAX_TRY_INVERT_A){
    try_invert_A++;
    if(try_invert_A > 1){
      inv_prec *= 2;
      A_condition_prec = inv_prec;
      mpfr_mat_set_prec(R, inv_prec);
      //mpfi_mat_set_prec(RA, A_condition_prec);
    }
    /*
      Calculer la matrice inverse de midA avec 
      la precision: inv_prec
     */

    mpfi_mat_mid(midA, A);

#ifdef DEBUG
    debug1("try to invert midA with precision: %d",
	   inv_prec);
    start_inv = clock();
#endif // of DEBUG

    mpfr_mat_inv_lu(R, midA, pivot, GMP_RNDN);

#ifdef DEBUG
    end_inv = clock();
    debug1(" ---> Time to calculate the inverse : %g sec",
	   (double)(end_inv - start_inv)/CLOCKS_PER_SEC);

    debug("Estimated Conditionnement of matrix midA:  \n");

    mpfr_t ca, ca1;
    mpfr_init2(ca, mpfi_mat_get_prec(x));
    mpfr_init2(ca1, mpfi_mat_get_prec(x));
    mpfr_mat_norm_1(ca, midA);
    mpfr_mat_norm_1(ca1, R);
    mpfr_mul(ca, ca, ca1, GMP_RNDU);
    mpfr_log2(ca1, ca, GMP_RNDU);

    debug("     K1 = ");
    mpfr_out_str(stdout, 10, 5, ca, GMP_RNDU);
    fprintf(stdout,"     log2(K1) = ");
    mpfr_out_str(stdout, 10, 5, ca1, GMP_RNDU);
    fprintf(stdout,"\n");
    mpfr_clear(ca);
    mpfr_clear(ca1);
#endif // of DEBUG

    /*
      transformer le systeme original dans un autre systeme
      pour borner l'erreur
                   A  * x = b
     --> (R [*] A) * x = (R [*] b)
     [o] singnifie les operations par interval
     Si la verification echoue on essai d'augmenter 
     la precision de la matrice conditionee.
    */

    int prec_prec;
#ifdef DEBUG
    debug1("precondition matrix A with precision : %d bits",
	   mpfi_mat_get_prec(RA));
    start_precond = clock();
#endif // of DEBUG

    mpfri_mat_mul(RA, R, A);

#ifdef DEBUG
    end_precond = clock();
    debug1(" ---> Time to precondition the left matrix : %g sec",
	   (double)(end_precond - start_precond)/CLOCKS_PER_SEC);
#endif // of DEBUG

#ifdef DEBUG
    debug(" testing if RA is an H-matrix \n");
    start_test = clock();
#endif // of DEBUG

    mpfi_mat_compmat(midA, RA);
    cmp = mpfr_mat_is_H_mat(midA, u, v);

#ifdef DEBUG
    end_test = clock();
    debug1(" ---> Time to test the conditioned matrix : %g sec",
	   (double)(end_test - start_test)/CLOCKS_PER_SEC);
#endif // of DEBUG


    if (cmp > 0){
      try_invert_A = MAX_TRY_INVERT_A;
    }
    else{
#ifdef DEBUG
      debug("!!! RA is not an H-matrix !!!\n");
#endif // of DEBUG
    }
  }


#ifdef DEBUG
  /* detecter la norme de la matrice I - RA */
  mpfr_t norm;
  mpfr_init2(norm, mpfi_mat_get_prec(RA));

  debug("The 1st-order norm of I - RA is : ");
  mpfi_mat_norm_1_ext_ui(norm, RA, 1);
  mpfr_out_str(stdout, 10, 8, norm, GMP_RNDU);
  fprintf(stdout, "\n");

  debug("The infinite  norm of I - RA is : ");
  mpfi_mat_norm_8_ext_ui(norm, RA, 1);
  mpfr_out_str(stdout, 10, 8, norm, GMP_RNDU);
  fprintf(stdout, "\n");
#endif

  /*
   *  Calculer une approximation flottante du resultat
   */
  mpfi_mat_mid(midb, b);
  mpfr_mat_mul(approx, R, midb, GMP_RNDN);

  /*
   * Si RA n'est pas une H-matrice, le systeme n'est pas 
   * certifie par ce programme
   */
  if (cmp <= 0){

#ifdef DEBUG
    debug("!!! The solution is not certified !!! \n");
#endif

    for (k = 0; k < nbsys; k++){
      for (i = 0; i < dim; i++)
	mpfi_set_fr(MME(x,i,k), MME(approx,i,k));
    }

    mpfr_mat_clear(midA);
    mpfr_mat_clear(midb);
    mpfr_mat_clear(approx);
    mpfr_mat_clear(R);
    mpfi_mat_clear(RA);
    return 8;
  }

#ifdef DEBUG
  debug("The solution is certified. \n");
#endif


  /*
   * Essayer d'appliquer la methode iterative
   */

  int gauss_passage;
  mpfi_mat_t tmpresidu;
  mpfr_mat_t diameter, new_diameter, diff;
  mpfr_t max_diff;
  mpfr_t diam, mag;
  mpfr_init2(diam, mpfi_mat_get_prec(x));
  mpfr_init2(mag, mpfi_mat_get_prec(x));
  int nb_bits = mpfi_mat_get_prec(x);

  double epsilon = 0.00001;
  mpfi_mat_init(tmpresidu, dim, nbsys);

  mpfr_mat_init2(diameter, dim, nbsys, mpfi_mat_get_prec(x));
  mpfr_mat_init2(new_diameter, dim, nbsys, mpfi_mat_get_prec(x));
  mpfr_mat_init2(diff, dim, nbsys, mpfi_mat_get_prec(x));
  mpfr_init2(max_diff, mpfi_mat_get_prec(x));

  mpfi_mat_init2(residu, dim, nbsys, mpfi_mat_get_prec(x));

  residu_prec = RESIDU_PREC_FUNCTION(mpfi_mat_get_prec(x));
  mpfi_mat_set_prec(tmpresidu, residu_prec);

  for (gauss_passage = 0; gauss_passage < MAX_ITERATION; gauss_passage++){
    /*
     *  Calculer le residu
     */
#ifdef DEBUG
    debug1("Calculate the residual of the system with precision: %d",
	   residu_prec);
#endif

    mpfir_mat_mul(tmpresidu, A, approx);
    mpfi_mat_sub(residu, b, tmpresidu);


    /*
     * Preconditionner la matrice droite
     */
    b_condition_prec = mpfi_mat_get_prec(x);
    mpfi_mat_init2(Z, dim, nbsys, b_condition_prec);

#ifdef DEBUG
    debug1("precondition matrix b with precision : %d bits", b_condition_prec);
    start_precond = clock();
#endif // of DEBUG

    mpfri_mat_mul(Z, R, residu);

    /*
     * Pour la premiere fois, il faut estimer une
     * solution initiale pour le systeme residu
     */
    if (gauss_passage == 0){
      // Calculer ||Z||_v
      mpfi_mat_mag(midb, Z);  

      mpfr_mat_scaled_max_norm_colwise(midb, v, midb);

      // Calculer l'estimation initiale de l'erreur
      // e0 = ||Z||_v * [-u, u]
      for (k = 0; k < nbsys; k++){
	for (i = 0; i < dim; i++){
	  mpfr_mul(&((MME(x,i,k))->right), MME(u,k,0),
		   MME(midb,0,k), GMP_RNDU);
	  mpfr_neg(&((MME(x,i,k))->left),
		   &((MME(x,i,k))->right),
		   GMP_RNDD);
	}
      }
      mpfi_mat_diam(diameter, x);
    }
    /*
     * Pour les prochaines fois, il faut seulement
     * mettre a jour l'approximation et l'erreur
     */
    else{
      mpfi_mat_diam(new_diameter, x);
      mpfr_mat_sub(diff, diameter, new_diameter, GMP_RNDU);
      mpfr_mat_dotdiv(diff, diff, diameter, GMP_RNDU);
      mpfr_mat_max(max_diff, diff);
      // if there's no change in the diameter, stop
      if (mpfr_cmp_d(max_diff, epsilon) < 0){
#ifdef DEBUG
	debug("Cannot get more bits exact \n");
#endif
	break;
      }
      mpfr_mat_swap(diameter, new_diameter);

      mpfir_mat_sub(x, x, approx);
    }

#ifdef DEBUG
    debug1("%d. Solve the residual problem using Gauss-Seidel", gauss_passage);
    start_iter = clock();
    debug("Number of bits exact of erreur: \n");
    debug("       0 ");
#endif 

    for (i = 0; i < GAUSS_ITERATIONS; i++){
      iterate_Gauss_Seidel(x, RA, Z, x);
#ifdef DEBUG
      nb_bits = mpfi_mat_get_prec(x);
      for (j = 0; j < dim; j++){
	if (mpfi_is_zero(MME(x,j,0))){
	  mpfi_mag(diam, MME(x,j,0));
	  if (nb_bits > (0 - mpfr_get_exp(diam)))
	    nb_bits = 0 - mpfr_get_exp(diam);
	}
	else{
	  mpfi_diam(diam, MME(x,j,0));
	  mpfi_mag(mag, MME(x,j,0));
	  if (mpfr_get_exp(mag) - mpfr_get_exp(diam) < nb_bits){
	    nb_bits = mpfr_get_exp(mag) - mpfr_get_exp(diam);
	  }
	}
      }
      if (nb_bits < 0) nb_bits = 0;
      fprintf(stdout, "--> %d ", nb_bits);
#endif
    }

#ifdef DEBUG
    fprintf(stdout,"\n");
#endif

    mpfir_mat_add(x, x, approx);

    nb_bits = mpfi_mat_get_prec(x);
    for (j = 0; j < dim; j++){
      if (mpfi_is_zero(MME(x,j,0))){
	mpfi_mag(diam, MME(x,j,0));
	if (nb_bits > (0 - mpfr_get_exp(diam)))
	  nb_bits = 0 - mpfr_get_exp(diam);
      }
      else{
	mpfi_diam(diam, MME(x,j,0));
	if (mpfr_cmp_ui(diam, 0) > 0){
	  mpfi_mag(mag, MME(x,j,0));

	  if (mpfr_get_exp(mag) - mpfr_get_exp(diam) < nb_bits){
	    nb_bits = mpfr_get_exp(mag) - mpfr_get_exp(diam);
	  }
	}
      }
    }
    if (nb_bits < 0) nb_bits = 0;

#ifdef DEBUG
    debug1("Number of bits exact of result : %d ", nb_bits);
#endif

    if (nb_bits >= mpfi_mat_get_prec(x)){
#ifdef DEBUG
      debug("Reach the maximum number of bits exact ! Stop \n");
#endif
     
      break;
    }

    mpfi_mat_mid(approx, x);
  }

#ifdef DEBUG
  end_iter = clock();
  debug1(" ---> Time to verify the solution : %g sec",
	 (double)(end_iter - start_iter)/CLOCKS_PER_SEC);
#endif // of DEBUG

  mpfi_mat_clear(tmpresidu);
  mpfr_mat_clear(midA);
  mpfr_mat_clear(midb);
  mpfr_mat_clear(approx);
  mpfr_mat_clear(R);
  mpfi_mat_clear(residu);
  mpfi_mat_clear(RA);
  mpfi_mat_clear(Z);

  mpfr_mat_clear(diameter);
  mpfr_mat_clear(new_diameter);
  mpfr_mat_clear(diff);
  mpfr_clear(max_diff);
  mpfr_clear(diam);
  mpfr_clear(mag);

  return inexact;
}

