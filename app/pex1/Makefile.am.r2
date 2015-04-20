include $(top_srcdir)/Makefile.am.common

#bin_PROGRAMS = app_pex1
EXTRA_PROGRAMS = app_pex1


app_pex1_SOURCES = \
   main.c \
   setup.c \
   dstmacaddr.c


app_pex1_CFLAGS=-DENABLE_TIMING
TRACE_DIR=${APP_PEX1_OUT_BLDDIR}/trace
app_pex1_CFLAGS+=-DTRACE_DIR_MIXED=\\\"${TRACE_DIR}\\\"

#include $(srcdir)/Makefile.am.traceconf

DSTMACADDR_ARRAY_NAME=trace_mac_addr
DSTMACADDR_N_SAMPLES_PER_SYMBOL=8
DSTMACADDR_N_SAMPLES=8
DSTMACADDR_N_WRAPAROUNDS=1
app_pex1_CFLAGS+=-DDSTMACADDR_ARRAY_NAME=${DSTMACADDR_ARRAY_NAME}
app_pex1_CFLAGS+=-DDSTMACADDR_N_SAMPLES_PER_SYMBOL=${DSTMACADDR_N_SAMPLES_PER_SYMBOL}
app_pex1_CFLAGS+=-DDSTMACADDR_N_SAMPLES=${DSTMACADDR_N_SAMPLES}
app_pex1_CFLAGS+=-DDSTMACADDR_N_WRAPAROUNDS=${DSTMACADDR_N_WRAPAROUNDS}



nodist_app_pex1_SOURCES = \
   __fifos__.h \
   __fifos__.c \
   __atoms__.h \
   __atoms__.c \
   __actions__.h \
   __actions__.c \
   __states__.h \
   __states__.c

app_pex1_CLEANFILES = \
   __fifos__.h \
   __fifos__.c \
   __atoms__.h \
   __atoms__.c \
   __actions__.h \
   __actions__.c \
   __states__.h \
   __states__.c

EXTRA_DIST = \
   fifos.or \
   atoms.or \
   states.or

app_pex1_LDADD = \
   $(top_builddir)/osl/lib.osl.a \
   $(top_builddir)/oros/sfifos/lib/edmadriver/lib.oros.sfifos.edmadriver.a \
   $(top_builddir)/oros/sfifos/lib/ipc/lib.oros.sfifos.ipc.a \
   $(top_builddir)/oros/sfifos/ilib/lib.oros.sfifos.ilib.a \
   $(top_builddir)/oros/sfifos/lib.oros.sfifos.a \
   $(top_builddir)/oros/sysilib/lib.oros.sysilib.a \
   $(top_builddir)/exilib/lib.exilib.a \
   ${ORILIB_INSTALL_DIR}/lib.orilib.a \
   $(top_builddir)/osl/lib.osl.vitdec.a \
   $(top_builddir)/osl/lib.osl.platform.a 
if !DEVICE_IS_6638K2K
   app_pex1_LDADD += $(top_builddir)/osl/lib.osl.eth.a 
endif

#   lib.oros.sfifos.test_blocks.a
#   oros/common/src/sfifos/lib.oros.common.sfifos.test_blocks.a
#   orlib/orlib_blocks/lib.orlib.blocks.a


app_pex1_CFLAGS += -I"`_cygpath -m $(top_srcdir)/app/pex1`"
#app_pex1_CFLAGS += -Iapp/pex1
app_pex1_CFLAGS += -I.
#app_pex1_CFLAGS += -I$(top_srcdir)/exilib
#app_pex1_CFLAGS += -I$(top_srcdir)/oros/sfifos/ilib
#app_pex1_CFLAGS += -I$(top_srcdir)/oros/sysilib

ILIB_INCLUDE_PATH  =
ILIB_INCLUDE_PATH += -I"`_cygpath -m $(top_srcdir)/exilib`"
ILIB_INCLUDE_PATH += -I"`_cygpath -m $(top_srcdir)/oros/sfifos/ilib`"
ILIB_INCLUDE_PATH += -I"`_cygpath -m $(top_srcdir)/oros/sysilib`"
ILIB_INCLUDE_PATH += -I"`_cygpath -m ${ORILIB_INC_DIR}`"

app_pex1_CFLAGS += ${ILIB_INCLUDE_PATH}

#app_pex1_LDFLAGS=\
#--LINKER_SCRIPT_FILE "`_cygpath -w \"$(top_srcdir)/memorymaps/$(MEMORYMAP_FILE)\"`" \
#-I"${CSL_ROOT}/ti/csl/lib"  \
#-I"${DSPLIB_ROOT}/lib" 

app_pex1_LDFLAGS=\
  -I"${CSL_ROOT}/ti/csl/lib"  \
  -I"${DSPLIB_ROOT}/packages/ti/dsplib/lib"

if DEVICE_IS_6638K2K
  app_pex1_LDFLAGS+=\
  -I"${CSL_ROOT}/ti/drv/qmss/lib/c66" \
  -I"${CSL_ROOT}/ti/drv/cppi/lib/c66" \
  -I"${CSL_ROOT}/ti/drv/pa/lib/c66" \
  -I"${CSL_ROOT}/ti/platform/evmk2h/platform_lib/lib/debug"
else
  app_pex1_LDFLAGS+=\
  -I"${CSL_ROOT}/ti/drv/qmss/lib" \
  -I"${CSL_ROOT}/ti/drv/cppi/lib" \
  -I"${CSL_ROOT}/ti/drv/pa/lib" \
  -I"${CSL_ROOT}/ti/platform/evmc6670l/platform_lib/lib/debug"
endif

#overriding rule to avoid regenerating source code when no update occurs to source file
APP_PEX1_OUT_GENERATED_FILES = fifos atoms actions states
APP_PEX1_OUT_SRCDIR = $(top_srcdir)/app/pex1
APP_PEX1_OUT_BLDDIR = .


app_pex1$(EXEEXT): ATOMS_TXT_PREPROCESS $(APP_PEX1_OUT_GENERATED_FILES) $(app_pex1_OBJECTS) $(app_pex1_DEPENDENCIES) $(EXTRA_app_pex1_DEPENDENCIES) 
	@rm -f app_pex1$(EXEEXT)
	$(app_pex1_LINK) $(app_pex1_LDFLAGS) $(app_pex1_OBJECTS) $(app_pex1_LDADD) $(LIBS) $(wildcard __*_j_*.c)


#atoms.ethreader.or
#atoms.tracereader.or
ATOMS_TXT_PREPROCESS:
#	cat \
#		$(APP_PEX1_OUT_SRCDIR)/atoms.${SAMPLE_SOURCE}reader.or \
#		$(APP_PEX1_OUT_SRCDIR)/atoms.or \
#		> $(APP_PEX1_OUT_BLDDIR)/__atoms__.or

$(APP_PEX1_OUT_GENERATED_FILES): %: __%__.c


#	$(APP_PEX1_OUT_SRCDIR)/fifos.or \
#	$(APP_PEX1_OUT_SRCDIR)/atoms.or \
#	$(APP_PEX1_OUT_SRCDIR)/states.or \
#	$(APP_PEX1_OUT_SRCDIR)/debug.or
#
__%__.c: \
	$(APP_PEX1_OUT_BLDDIR)/fifos.or \
	$(APP_PEX1_OUT_BLDDIR)/atoms.or \
	$(APP_PEX1_OUT_BLDDIR)/states.or \
	$(APP_PEX1_OUT_SRCDIR)/debug.or
	_create_afs_i \
		$(APP_PEX1_OUT_BLDDIR)/fifos.or \
		$(APP_PEX1_OUT_BLDDIR)/atoms.or \
		$(APP_PEX1_OUT_BLDDIR)/states.or \
		$(APP_PEX1_OUT_BLDDIR)/__fifos__.h \
		$(APP_PEX1_OUT_BLDDIR)/__fifos__.c \
		$(APP_PEX1_OUT_BLDDIR)/__atoms__.h \
	        $(APP_PEX1_OUT_BLDDIR)/__atoms__.c \
	        $(APP_PEX1_OUT_BLDDIR)/__actions__.h \
	        $(APP_PEX1_OUT_BLDDIR)/__actions__.c \
	        $(APP_PEX1_OUT_BLDDIR)/__states__.h \
	        $(APP_PEX1_OUT_BLDDIR)/__states__.c \
		$(APP_PEX1_OUT_SRCDIR)/code_placement.or \
		$(APP_PEX1_OUT_SRCDIR)/debug.or \
		$(top_srcdir)/memorymaps/$(MEMORYMAP_FILE) \
		$(APP_PEX1_OUT_BLDDIR)/__memorymap__.cmd \
		${ILIB_INCLUDE_PATH} ${CFLAGS} ${app_pex1_CFLAGS}
	if [[ ! -d ${TRACE_DIR} ]]; then mkdir -p ${TRACE_DIR}; fi



app_pex1_cptrace:
#if INPTRACE_LC
#	cp -v ${INP_TRACE_C} ${OUT_TRACE_C}
#endif
#if INPTRACE_S
#	cp -v ${INP_TRACE_C} ${OUT_TRACE_C}
#
#endif


r2: app_pex1_cptrace app_pex1$(EXEEXT)
#

###########################################
#common to all binary builds
###########################################
if DEVICE_IS_6638K2K
  LDFLAGS+=--LINKER_FLAGS_FILE "`_cygpath -w \"$(top_srcdir)/linkerflags_k2k.cmd\"`"
else
  LDFLAGS+=--LINKER_FLAGS_FILE "`_cygpath -w \"$(top_srcdir)/linkerflags.cmd\"`"
endif
app_pex1_LDFLAGS+=\
--LINKER_SCRIPT_FILE "`_cygpath -w \"__memorymap__.cmd\"`"

