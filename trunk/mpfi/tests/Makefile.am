LDADD = libmpfi-tests.la $(top_builddir)/src/.libs/libmpfi.la
AM_CPPFLAGS = -I$(top_srcdir)/src

check_LTLIBRARIES = libmpfi-tests.la
libmpfi_tests_la_SOURCES = mpfi-tests.h read_data.c random.c mpfi-tests.c

check_PROGRAMS = tabs tadd tdiv tintersect tmul tneg tsqr tsub	\
    tunion

EXTRA_DIST = abs.dat add.dat div.dat intersect.dat mul.dat	\
    neg.dat sqr.dat sub.dat union.dat

TESTS = $(check_PROGRAMS)
