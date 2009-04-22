#include <stdlib.h>
#include <stdio.h>
#include "mpfi.h"
#include "mpfi_io.h"
#include "matrix_generator.h"

#define MME(a, ind_i, ind_j) a->mems[ind_i]+ind_j

void pascal_mat_fr(mpfr_mat_srcptr a, int rows,
		   int cols)
{
  int i,j;
  int max, min;
  int curr, next, next_right;

  for (i = 0; i < rows; i++){
    mpfr_mat_set_ui(a, 0, i, 1, GMP_RNDD);
  }

  for (i = 0; i < cols; i++){
    mpfr_mat_set_ui(a, i, 0, 1, GMP_RNDD);
  }

  curr = 1;
  next = 2;

  max = rows; min = rows;
  if (cols > rows) max = cols;
  else min = cols;

  for (i = 1; i < max; i++){
    curr = next;
    next = (curr * 2 / (i + 1)) * (2 * i + 1);
    next_right = curr;

    if (i < min)
      mpfr_mat_set_ui(a, i, i, curr, GMP_RNDD);
    
    for (j = i + 1; j < max; j++){
      next_right = next_right / (j+1) * (i + j + 1);
      if(i < rows && j < cols)
	mpfr_mat_set_ui(a, i, j, next_right, GMP_RNDD);
      if(j < rows && i < cols)
	mpfr_mat_set_ui(a, j, i, next_right, GMP_RNDD);
    }
  }
}

void pascal_mat_fi(mpfi_mat_srcptr a, int rows,
		   int cols)
{
  int i,j;
  int max, min;
  int curr, next, next_right;

  for (i = 0; i < rows; i++){
    mpfi_mat_set_ui(a, 0, i, 1);
  }

  for (i = 0; i < cols; i++){
    mpfi_mat_set_ui(a, i, 0, 1);
  }

  curr = 1;
  next = 2;

  max = rows; min = rows;
  if (cols > rows) max = cols;
  else min = cols;

  for (i = 1; i < max; i++){
    curr = next;
    next = (curr * 2 / (i + 1)) * (2 * i + 1);
    next_right = curr;

    if (i < min)
      mpfi_mat_set_ui(a, i, i, curr);
    
    for (j = i + 1; j < max; j++){
      next_right = next_right / (j+1) * (i + j + 1);
      if(i < rows && j < cols)
	mpfi_mat_set_ui(a, i, j, next_right);
      if(j < rows && i < cols)
	mpfi_mat_set_ui(a, j, i, next_right);
    }
  }
}


void hilbert_mat_fr(mpfr_mat_srcptr a,
		    int rows, int cols,
		    mp_rnd_t rnd){

  int i,j;
  for (i = 0; i < rows; i++){
    for (j = 0; j < cols; j++){
      mpfr_set_ui(MME(a,i,j), 1, rnd);
      mpfr_div_ui(MME(a,i,j), MME(a,i,j),
		  i+j+1, rnd);
    }
  }

}

void hilbert_mat_fi(mpfi_mat_srcptr a,
		    int rows, int cols){

  int i,j;

  for (i = 0; i < rows; i++){
    for (j = 0; j < cols; j++){
      mpfi_set_ui(MME(a,i,j), 1);
      mpfi_div_ui(MME(a,i,j), MME(a,i,j),
		  i+j+1);
    }
  }

}

void ones_mat_fr(mpfr_mat_srcptr a,
		 int rows, int cols){
  int i,j;
  for (i = 0; i < rows; i++){
    for (j = 0; j < cols; j++){
      mpfr_set_ui(MME(a,i,j), 1, GMP_RNDD);
    }
  }
}

void ones_mat_fi(mpfi_mat_srcptr a,
		 int rows, int cols){
  int i,j;
  for (i = 0; i < rows; i++){
    for (j = 0; j < cols; j++){
      mpfi_set_ui(MME(a,i,j), 1);
    }
  }
}

void zeros_mat_fr(mpfr_mat_srcptr a,
		 int rows, int cols){
  int i,j;
  for (i = 0; i < rows; i++){
    for (j = 0; j < cols; j++){
      mpfr_set_ui(MME(a,i,j), 0, GMP_RNDD);
    }
  }
}

void zeros_mat_fi(mpfi_mat_srcptr a,
		 int rows, int cols){
  int i,j;
  for (i = 0; i < rows; i++){
    for (j = 0; j < cols; j++){
      mpfi_set_ui(MME(a,i,j), 0);
    }
  }
}

void eyes_mat_fr(mpfr_mat_srcptr a,
		 int rows, int cols){
  int i,j;
  for (i = 0; i < rows; i++){
    for (j = 0; j < cols; j++){
      if (i == j)
	mpfr_set_ui(MME(a,i,j), 1, GMP_RNDD);
      else
	mpfr_set_ui(MME(a,i,j), 0, GMP_RNDD);
    }
  }
}

void eyes_mat_fi(mpfi_mat_srcptr a,
		 int rows, int cols){
  int i,j;
  for (i = 0; i < rows; i++){
    for (j = 0; j < cols; j++){
      if (i == j)
	mpfi_set_ui(MME(a,i,j), 1);
      else
	mpfi_set_ui(MME(a,i,j), 0);
    }
  }
}

void toeplitz_mat_fr(mpfr_mat_srcptr a,
		 int rows, int cols,
		 double c, double d,
		 double e, mp_rnd_t rnd){
  int i,j;
  for (i = 0; i < rows; i++){
    for (j = 0; j < cols; j++){
      if (i == j)
	mpfr_set_d(MME(a,i,j), d, rnd);
      else if (i == j + 1)
	mpfr_set_d(MME(a,i,j), c, rnd);
      else if (j == i + 1)
	mpfr_set_d(MME(a,i,j), e, rnd);
      else
	mpfr_set_ui(MME(a,i,j), 0, rnd);
    }
  }
}

void toeplitz_mat_fi(mpfi_mat_ptr a,
		     int rows, int cols,
		     double c, double d,
		     double e){
  int i,j;
  for (i = 0; i < rows; i++){
    for (j = 0; j < cols; j++){
      if (i == j)
	mpfi_set_d(MME(a,i,j), d);
      else if (i == j + 1)
	mpfi_set_d(MME(a,i,j), c);
      else if (j == i + 1)
	mpfi_set_d(MME(a,i,j), e);
      else
	mpfi_set_ui(MME(a,i,j), 0);
    }
  }
}

int read_double_matrix_fr(mpfr_mat_ptr a,			  
			  int prec, int init,
			  char *filename
			  )
{
  FILE *file;
  unsigned int rows,cols;
  int i,j;

  file = fopen(filename, "r");

  if(file==NULL) {
    printf("Error: cannot open file %s.\n", filename);
    return 1;
  }

  if (fread(&rows, sizeof(int),1,file) < 1 ||
      fread(&cols, sizeof(int),1,file) < 1){
    printf("Error: cannot get matrix size. \n");
    return 1;
  }

  if(init == 1)
    mpfr_mat_init2(a, rows, cols, prec);
  else{
    if (rows != a->rows || cols != a->cols){
      printf("Incompatible size. Cannot read matrix from file \n");
      return -1;
    }
  }

  double tmp[cols];

  for (i = 0; i < rows; i++){
    if(fread(tmp, sizeof(double), cols, file) < cols)
      return 1;
    for (j = 0; j < cols; j++){
      mpfr_mat_set_d(a, i, j, tmp[j], GMP_RNDN);
    }
  }

  fclose(file);

  return 0;
}
