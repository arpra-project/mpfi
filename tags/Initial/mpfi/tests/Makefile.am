noinst_PROGRAMS = test_mpfi test_trigo
test_mpfi_SOURCES = test_mpfi.c
test_trigo_SOURCES = test_trigo.c
test_mpfi_LDADD = ${top_srcdir}/src/libmpfi.a @LDADD@
test_trigo_LDADD = ${top_srcdir}/src/libmpfi.a @LDADD@

# ici on utilise la variable GMP_INC_PATH declaree par le configure

INCLUDES = -I. -I${top_srcdir}/src @INCLUDES@
