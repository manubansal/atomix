#------------------------
# test_s1
#------------------------

bin_PROGRAMS += sfifos_test_s1.out

#sfifos_test_s1a.out$(EXEEXT):
#	echo jj
#all-local:
#	if [ -e fifos.or ]; then create_fifos.py; fi

##http://stackoverflow.com/questions/6284918/how-to-generate-a-source-file-when-building-using-autotools
#################################
#EXTRA_DIST = fifos.or
##bin_PROGRAMS = prog
#noinst_PROGRAMS = generator
#
## Source files for the generator
#generator_SOURCES = generator.c ...
#
## Source files for prog
##prog_SOURCES = prog.c ...
##nodist_prog_SOURCES = generated.c 
#
## generated.c is a built source that must be cleaned
#CLEANFILES = generated.c
#
## Build rule for generated.c
#generated.c: $(srcdir)/input.txt generator$(EXEEXT)
#        ./generator$(EXEEXT) $(srcdir)/input.txt
#
#################################

sfifos_test_s1_out_SOURCES = \
   oros/sfifos/tests/test_s1/main.c
#   oros/common/src/sfifos/tests/test_s1a/blocks.c
#   oros/common/src/sfifos/tests/test_s1a/blocks.h

#   oros/common/src/sfifos/tests/test_s1a/fifos.c \
#   oros/common/src/sfifos/tests/test_s1a/fifos.h 
nodist_sfifos_test_s1_out_SOURCES = \
   oros/sfifos/tests/test_s1/__fifos__.h \
   oros/sfifos/tests/test_s1/__fifos__.c \
   oros/sfifos/tests/test_s1/__atoms__.h \
   oros/sfifos/tests/test_s1/__atoms__.c

sfifos_test_s1_out_CLEANFILES = \
   oros/sfifos/tests/test_s1/__fifos__.h \
   oros/sfifos/tests/test_s1/__fifos__.c \
   oros/sfifos/tests/test_s1/__atoms__.h \
   oros/sfifos/tests/test_s1/__atoms__.c

EXTRA_DIST = \
   oros/sfifos/tests/test_s1/fifos.or \
   oros/sfifos/tests/test_s1/atoms.or

#noinst_PROGRAMS = _create_fifos

#_create_fifos_SOURCES = \
#  oros/common/src/sfifos/tests/test_s1a/__fifos__.h \
#  oros/common/src/sfifos/tests/test_s1a/__fifos__.c


#BUILT_SOURCES = \
#  oros/common/src/sfifos/tests/test_s1a/__fifos__.h \
#  oros/common/src/sfifos/tests/test_s1a/__fifos__.c \
#  oros/common/src/sfifos/tests/test_s1a/__atoms__.h \
#  oros/common/src/sfifos/tests/test_s1a/__atoms__.c
#sfifos_test_s1a_out.$(OBJEXT): af
#sfifos_test_s1a_out-main.$(OBJEXT): af
#BUILT_SOURCES = jj.c
BUILT_SOURCES = af_s1


#oros/common/src/sfifos/tests/test_s1a/__fifos__.% : 
#oros/common/src/sfifos/tests/test_s1a/__%__.% : 
#jj.c:
af_s1:
	_create_af$(EXEEXT) \
		$(top_srcdir)/oros/sfifos/tests/test_s1/fifos.or \
		$(top_srcdir)/oros/sfifos/tests/test_s1/atoms.or \
		oros/sfifos/tests/test_s1/__fifos__.h \
		oros/sfifos/tests/test_s1/__fifos__.c \
		oros/sfifos/tests/test_s1/__atoms__.h \
		oros/sfifos/tests/test_s1/__atoms__.c 

sfifos_test_s1_out_LDADD = \
   osl/lib.osl.a \
   oros/sfifos/lib/edmadriver/lib.oros.sfifos.edmadriver.a \
   oros/sfifos/lib/ipc/lib.oros.sfifos.ipc.a \
   oros/sfifos/blib/lib.oros.sfifos.blib.a \
   oros/sfifos/lib.oros.sfifos.a \
   exblib/lib.exblib.a
#   lib.oros.sfifos.test_blocks.a
#   oros/common/src/sfifos/tests/lib.oros.common.sfifos.test_blocks.a
#   orlib/orlib_blocks/lib.orlib.blocks.a

sfifos_test_s1_out_CFLAGS = -I"$(top_srcdir)/oros/sfifos/tests/test_blocks"
sfifos_test_s1_out_CFLAGS += -Ioros/sfifos/tests/test_s1
sfifos_test_s1_out_CFLAGS += -I$(top_srcdir)/exblib
sfifos_test_s1_out_CFLAGS += -I$(top_srcdir)/oros/sfifos/blib

sfifos_test_s1_out_LDFLAGS=\
--LINKER_SCRIPT_FILE "`_cygpath -w \"$(top_srcdir)/memorymaps/$(MEMORYMAP_FILE)\"`"
