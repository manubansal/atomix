
#------------------------------------------------------------------------
check_PROGRAMS+=check_orilib
check_orilib_CFLAGS = -I"`_cygpath -m $(top_srcdir)`"
check_orilib_SOURCES = \
	orilib/main.c  \
	orilib/TEST_ORILIB_CFOEstimatorLTF_i.c

TRACE_DIR=${CHECK_ORILIB_OUT_BLDDIR}
check_orilib_CFLAGS+=-DTRACE_DIR_MIXED=\\\"${TRACE_DIR}\\\"

check_orilib_LDADD = \
  $(top_builddir)/osl/lib.osl.a \
  $(top_builddir)/orilib/lib.orilib.a

check_orilib_LDFLAGS = \
-I"${CSL_ROOT}/ti/csl/lib" \
-I"${DSPLIB_ROOT}/lib" 

LDFLAGS+=--LINKER_FLAGS_FILE "`_cygpath -w \"$(top_srcdir)/linkerflags.cmd\"`"

check_orilib_LDFLAGS+=\
--LINKER_SCRIPT_FILE "`_cygpath -w \"$(srcdir)/__memorymap__.cmd\"`" 

#------------------------------------------------------------------------
