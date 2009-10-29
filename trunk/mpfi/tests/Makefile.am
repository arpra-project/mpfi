LDADD = libmpfi-tests.la $(top_builddir)/src/.libs/libmpfi.la
AM_CPPFLAGS = -I$(top_srcdir)/src

check_LTLIBRARIES = libmpfi-tests.la
libmpfi_tests_la_SOURCES = mpfi-tests.h read_data.c random.c	\
    mpfi-tests.c

check_PROGRAMS = tabs tadd tcbrt tdiv tintersect tinv tlog	\
    tmul tneg tsqr tsqrt tsub tunion

EXTRA_DIST = abs.dat add.dat cbrt.dat div.dat intersect.dat	\
    inv.dat log.dat mul.dat neg.dat sqr.dat sqrt.dat sub.dat	\
    union.dat

TESTS = $(check_PROGRAMS)
