#------------------------
# test_s5p
#------------------------

bin_PROGRAMS += sfifos_test_s5p.out

sfifos_test_s5p_out_SOURCES = \
   oros/common/src/sfifos/tests/test_s5p/main.c

nodist_sfifos_test_s5p_out_SOURCES = \
   oros/common/src/sfifos/tests/test_s5p/__fifos__.h \
   oros/common/src/sfifos/tests/test_s5p/__fifos__.c \
   oros/common/src/sfifos/tests/test_s5p/__atoms__.h \
   oros/common/src/sfifos/tests/test_s5p/__atoms__.c

sfifos_test_s5p_out_CLEANFILES = \
   oros/common/src/sfifos/tests/test_s5p/__fifos__.h \
   oros/common/src/sfifos/tests/test_s5p/__fifos__.c \
   oros/common/src/sfifos/tests/test_s5p/__atoms__.h \
   oros/common/src/sfifos/tests/test_s5p/__atoms__.c

EXTRA_DIST += \
   oros/common/src/sfifos/tests/test_s5p/fifos.or \
   oros/common/src/sfifos/tests/test_s5p/atoms.or

BUILT_SOURCES += af_s5p

af_s5p:
	_create_af$(EXEEXT) \
		$(top_srcdir)/oros/common/src/sfifos/tests/test_s5p/fifos.or \
		$(top_srcdir)/oros/common/src/sfifos/tests/test_s5p/atoms.or \
		oros/common/src/sfifos/tests/test_s5p/__fifos__.h \
		oros/common/src/sfifos/tests/test_s5p/__fifos__.c \
		oros/common/src/sfifos/tests/test_s5p/__atoms__.h \
		oros/common/src/sfifos/tests/test_s5p/__atoms__.c 

sfifos_test_s5p_out_LDADD = \
   osl/lib.osl.a \
   oros/common/src/sfifos/lib/edmadriver/lib.oros.common.sfifos.edmadriver.a \
   oros/common/src/sfifos/lib/ipc/lib.oros.common.sfifos.ipc.a \
   oros/common/src/sfifos/lib.oros.common.sfifos.a \
   lib.oros.common.sfifos.test_blocks.a

sfifos_test_s5p_out_CFLAGS = -I"$(top_srcdir)/oros/common/src/sfifos/tests/test_blocks"
sfifos_test_s5p_out_CFLAGS += -Ioros/common/src/sfifos/tests/test_s5p

sfifos_test_s5p_out_LDFLAGS=\
--LINKER_SCRIPT_FILE "`_cygpath -w \"$(top_srcdir)/memorymaps/$(MEMORYMAP_FILE)\"`"
