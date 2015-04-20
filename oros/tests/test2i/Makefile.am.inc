#------------------------
# test2i
#------------------------

bin_PROGRAMS += oros_test2i.out

oros_test2i_out_SOURCES = \
   oros/tests/test2i/main.c \
   oros/tests/test2i/setup.c

#GENERATED_BLOCK_FILES=`ls oros/tests/test2i/*_j_*.c`

nodist_oros_test2i_out_SOURCES = \
   oros/tests/test2i/__fifos__.h \
   oros/tests/test2i/__fifos__.c \
   oros/tests/test2i/__atoms__.h \
   oros/tests/test2i/__atoms__.c \
   oros/tests/test2i/__actions__.h \
   oros/tests/test2i/__actions__.c \
   oros/tests/test2i/__states__.h \
   oros/tests/test2i/__states__.c 

#   $(wildcard oros/tests/test2i/__*_j_*.c)

#   $(GENERATED_BLOCK_FILES)


oros_test2i_out_CLEANFILES = \
   __fifos__.h \
   __fifos__.c \
   __atoms__.h \
   __atoms__.c \
   __actions__.h \
   __actions__.c \
   __states__.h \
   __states__.c

EXTRA_DIST += \
   oros/tests/test2i/fifos.txt \
   oros/tests/test2i/atoms.txt \
   oros/tests/test2i/states.txt

oros_test2i_out_LDADD = \
   osl/lib.osl.a \
   oros/sfifos/lib/edmadriver/lib.oros.sfifos.edmadriver.a \
   oros/sfifos/lib/ipc/lib.oros.sfifos.ipc.a \
   oros/sfifos/ilib/lib.oros.sfifos.ilib.a \
   oros/sfifos/lib.oros.sfifos.a \
   oros/sysilib/lib.oros.sysilib.a \
   exilib/lib.exilib.a
#   lib.oros.sfifos.test_blocks.a
#   oros/common/src/sfifos/lib.oros.common.sfifos.test_blocks.a
#   orlib/orlib_blocks/lib.orlib.blocks.a

oros_test2i_out_CFLAGS=-DENABLE_TIMING

oros_test2i_out_CFLAGS += -I"$(top_srcdir)/oros/tests/test2i"
oros_test2i_out_CFLAGS += -Ioros/tests/test2i
#oros_test2i_out_CFLAGS += -I$(top_srcdir)/exilib
#oros_test2i_out_CFLAGS += -I$(top_srcdir)/oros/sfifos/ilib
#oros_test2i_out_CFLAGS += -I$(top_srcdir)/oros/sysilib

ILIB_INCLUDE_PATH  =
ILIB_INCLUDE_PATH += -I$(top_srcdir)/exilib
ILIB_INCLUDE_PATH += -I$(top_srcdir)/oros/sfifos/ilib
ILIB_INCLUDE_PATH += -I$(top_srcdir)/oros/sysilib

oros_test2i_out_CFLAGS += ${ILIB_INCLUDE_PATH}

oros_test2i_out_LDFLAGS=\
--LINKER_SCRIPT_FILE "`_cygpath -w \"$(top_srcdir)/memorymaps/$(MEMORYMAP_FILE)\"`"


#version 1: generates code everytime
#BUILT_SOURCES += af
#
#af:
#	_create_afs_i$(EXEEXT) \
#		$(top_srcdir)/oros/tests/test2i/fifos.txt \
#		$(top_srcdir)/oros/tests/test2i/atoms.txt \
#		$(top_srcdir)/oros/tests/test2i/states.txt \
#		oros/tests/test2i/__fifos__.h \
#		oros/tests/test2i/__fifos__.c \
#		oros/tests/test2i/__atoms__.h \
#	        oros/tests/test2i/__atoms__.c \
#	        oros/tests/test2i/__actions__.h \
#	        oros/tests/test2i/__actions__.c \
#	        oros/tests/test2i/__states__.h \
#	        oros/tests/test2i/__states__.c \
#		${ILIB_INCLUDE_PATH}


#version 2
#overriding rule to avoid regenerating source code when no update occurs to source file
OROS_TEST1I_OUT_GENERATED_FILES = fifos atoms actions states
OROS_TEST1I_OUT_SRCDIR = $(top_srcdir)/oros/tests/test2i
OROS_TEST1I_OUT_BLDDIR = oros/tests/test2i

oros_test2i.out$(EXEEXT): $(OROS_TEST1I_OUT_GENERATED_FILES) $(oros_test2i_out_OBJECTS) $(oros_test2i_out_DEPENDENCIES) $(EXTRA_oros_test2i_out_DEPENDENCIES) 
	@rm -f oros_test2i.out$(EXEEXT)
	$(oros_test2i_out_LINK) $(oros_test2i_out_OBJECTS) $(oros_test2i_out_LDADD) $(LIBS) $(wildcard oros/tests/test2i/__*_j_*.c)


$(OROS_TEST1I_OUT_GENERATED_FILES): %: __%__.c

__%__.c: $(OROS_TEST1I_OUT_SRCDIR)/fifos.txt $(OROS_TEST1I_OUT_SRCDIR)/atoms.txt $(OROS_TEST1I_OUT_SRCDIR)/states.txt
	_create_afs_i$(EXEEXT) \
		$(OROS_TEST1I_OUT_SRCDIR)/fifos.txt \
		$(OROS_TEST1I_OUT_SRCDIR)/atoms.txt \
		$(OROS_TEST1I_OUT_SRCDIR)/states.txt \
		$(OROS_TEST1I_OUT_BLDDIR)/__fifos__.h \
		$(OROS_TEST1I_OUT_BLDDIR)/__fifos__.c \
		$(OROS_TEST1I_OUT_BLDDIR)/__atoms__.h \
	        $(OROS_TEST1I_OUT_BLDDIR)/__atoms__.c \
	        $(OROS_TEST1I_OUT_BLDDIR)/__actions__.h \
	        $(OROS_TEST1I_OUT_BLDDIR)/__actions__.c \
	        $(OROS_TEST1I_OUT_BLDDIR)/__states__.h \
	        $(OROS_TEST1I_OUT_BLDDIR)/__states__.c \
		${ILIB_INCLUDE_PATH}
	


