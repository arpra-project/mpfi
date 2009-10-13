LDADD = libmpfi-tests.la $(top_builddir)/src/.libs/libmpfi.la
AM_CPPFLAGS = -I$(top_srcdir)/src

check_LTLIBRARIES = libmpfi-tests.la
libmpfi_tests_la_SOURCES = mpfi-tests.h read_data.c random.c mpfi-tests.c

check_PROGRAMS = tadd tsub

EXTRA_DIST = add.dat sub.dat

TESTS = $(check_PROGRAMS)
