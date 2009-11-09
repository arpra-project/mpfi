LDADD = libmpfi-tests.la $(top_builddir)/src/.libs/libmpfi.la
AM_CPPFLAGS = -I$(top_srcdir)/src

check_LTLIBRARIES = libmpfi-tests.la
libmpfi_tests_la_SOURCES = mpfi-tests.h read_data.c random.c	\
    mpfi-tests.c

check_PROGRAMS = tabs tacos tacosh tadd tasin tasinh tatan	\
    tatan2 tatanh tcbrt tconst_catalan tconst_euler		\
    tconst_log2 tconst_pi tcos tcosh tcsc tcsch tcot tcoth	\
    tdiv texp texp2 texpm1 tintersect tinv tlog tlog2 tlog10	\
    tlog1p tmul tneg tput_fr tput_q tput_z tsec tsech tset	\
    tset_fr tset_q tset_str tset_z tsin tsinh tsqr tsqrt	\
    tsub ttan ttanh tunion

EXTRA_DIST = abs.dat add.dat cbrt.dat div.dat intersect.dat	\
    inv.dat log.dat mul.dat neg.dat put_fr.dat put_q.dat	\
    put_z.dat set_fr.dat set_q.dat set_z.dat sqr.dat		\
    sqrt.dat sub.dat union.dat

TESTS = $(check_PROGRAMS)
