LDADD = libmpfi-tests.la $(top_builddir)/src/.libs/libmpfi.la
AM_CPPFLAGS = -I$(top_srcdir)/src

check_LTLIBRARIES = libmpfi-tests.la
libmpfi_tests_la_SOURCES = mpfi-tests.h read_data.c random.c	\
    type_i.c type_ii.c type_iii.c type_iu.c type_is.c		\
    type_id.c type_iz.c type_iq.c type_ir.c type_iiu.c		\
    type_iis.c type_iid.c type_iiz.c type_iiq.c type_iir.c	\
    type_iui.c type_isi.c type_idi.c type_izi.c type_iqi.c	\
    type_iri.c check_data.c mpfi-tests.c

check_PROGRAMS = tabs tacos tacosh tadd tadd_d tadd_fr		\
    tadd_q tadd_si tadd_ui tadd_z tasin tasinh tatan tatan2	\
    tatanh tbisect tcbrt tconst_catalan tconst_euler		\
    tconst_log2 tconst_pi tcos tcosh tcot tcoth tcsc tcsch	\
    td_div td_sub tdiv tdiv_2exp tdiv_2si tdiv_2ui tdiv_d	\
    tdiv_fr tdiv_q tdiv_ui tdiv_si tdiv_z terror texp texp2	\
    texpm1 tfr_div tfr_sub thypot tinit_set tintersect		\
    tinterv_d tinterv_fr tinterv_q tinterv_si tinterv_ui	\
    tinterv_z tinv tget_d tis_empty tget_endpoints tget_fr	\
    thas_zero tlog tlog10 tlog1p tlog2 tmag tmig tmul		\
    tmul_2exp tmul_2si tmul_2ui tmul_d tmul_fr tmul_q		\
    tmul_ui tmul_si tmul_z tneg tput_d tput_fr tput_q		\
    tput_si tput_ui tput_z tq_div tq_sub tround_prec tsec	\
    tsech tset tset_d tset_fr tset_q tset_si tset_str		\
    tset_ui tset_z tsi_div tsi_sub tsin tsinh tsqr tsqrt	\
    tsub tsub_d tsub_fr tsub_q tsub_ui tsub_si tsub_z tswap	\
    ttan ttanh tunion tui_div tui_sub tz_div tz_sub		\
    tget_version

EXTRA_DIST = abs.dat acos.dat acosh.dat add.dat add_d.dat	\
    add_fr.dat add_q.dat add_si.dat add_ui.dat add_z.dat	\
    asin.dat asinh.dat atan.dat atan2.dat cbrt.dat		\
    const_catalan.dat const_euler.dat const_log2.dat		\
    const_pi.dat div.dat intersect.dat inv.dat log.dat		\
    mul.dat mul_d.dat neg.dat put_d.dat put_fr.dat put_q.dat	\
    put_si.dat put_ui.dat put_z.dat set_d.dat set_fr.dat	\
    set_q.dat set_si.dat set_ui.dat set_z.dat sqr.dat		\
    sqrt.dat sub.dat union.dat

TESTS = $(check_PROGRAMS)
