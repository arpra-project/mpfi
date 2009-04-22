noinst_PROGRAMS = test_mpfi_mat test_mpfr_mat
test_mpfi_mat_SOURCES = test_mpfi_mat.c
test_mpfr_mat_SOURCES = test_mpfr_mat.c
test_mpfi_mat_LDADD = ${top_srcdir}/LinSys/src/.libs/libmpfimat.a ${top_srcdir}/src/.libs/libmpfi.a @LDADD@
test_mpfr_mat_LDADD = ${top_srcdir}/LinSys/src/.libs/libmpfimat.a ${top_srcdir}/src/.libs/libmpfi.a -lm @LDADD@

# ici on utilise la variable GMP_INC_PATH declaree par le configure

INCLUDES = -I. -I${top_srcdir}/src -I${top_srcdir}/LinSys/src @INCLUDES@
LIBS = -lm @LIBS@
