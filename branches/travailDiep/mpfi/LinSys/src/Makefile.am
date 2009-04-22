lib_LTLIBRARIES = libmpfimat.la 
include_HEADERS = matrix_generator.h  mpfi_mat.h  mpfr_mat.h
libmpfimat_la_SOURCES = matrix_generator.h  mpfi_mat.h \
	mpfr_mat.c matrix_generator.c  mpfi_mat.c \
	mpfi_mat_lss.c  mpfr_mat.h mpfi_mat_io.c mpfi_mat_norm.c

INCLUDES =-I$(top_srcdir) -I${top_srcdir}/src -I${top_srcdir}/LinSys/src @INCLUDES@
