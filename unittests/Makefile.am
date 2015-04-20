
include $(top_srcdir)/Makefile.am.common

PYTHON=python

TESTS_ENVIRONMENT = $(PYTHON) $(srcdir)/unittest.py
#TESTS=check_ORILIB_PacketSearch_Align

check_PROGRAMS=

##------------------------------------------------------------------------
#check_PROGRAMS+=check_ORILIB_PacketSearch_Align
#check_ORILIB_PacketSearch_Align_CFLAGS = -I"`_cygpath -m $(top_srcdir)`"
#check_ORILIB_PacketSearch_Align_CFLAGS += -I"`_cygpath -m $(top_srcdir)/orilib`"
#check_ORILIB_PacketSearch_Align_SOURCES = TEST_ORILIB_PacketSearch_Align.c $(top_srcdir)/orilib/ORILIB_PacketSearch_Align_i.h $(top_srcdir)/orilib/ORILIB_PacketSearch_Align_t.h
#check_ORILIB_PacketSearch_Align_LDADD = \
#  $(top_builddir)/orilib/lib.orilib.a \
#  $(top_builddir)/osl/lib.osl.a
#
#check_ORILIB_PacketSearch_Align_LDFLAGS = \
#-I"${CSL_ROOT}/ti/csl/lib"  \
#-I"${DSPLIB_ROOT}/lib" 
#
#LDFLAGS+=--LINKER_FLAGS_FILE "`_cygpath -w \"$(top_srcdir)/linkerflags.cmd\"`"
#
#check_ORILIB_PacketSearch_Align_LDFLAGS+=\
#--LINKER_SCRIPT_FILE "`_cygpath -w \"$(srcdir)/__memorymap__.cmd\"`" 
##------------------------------------------------------------------------


#------------------------------------------------------------------------
check_PROGRAMS+=check_vitdec
check_vitdec_CFLAGS = -I"`_cygpath -m $(top_srcdir)`"
check_vitdec_SOURCES = vitdec/main.c vitdec/vcp2data.c
check_vitdec_LDADD = \
  $(top_builddir)/osl/lib.osl.vitdec.a \
  $(top_builddir)/oros/sfifos/lib/edmadriver/lib.oros.sfifos.edmadriver.a \
  $(top_builddir)/osl/lib.osl.a

check_vitdec_LDFLAGS = \
-I"${CSL_ROOT}/ti/csl/lib" \
-I"${DSPLIB_ROOT}/lib" 

LDFLAGS+=--LINKER_FLAGS_FILE "`_cygpath -w \"$(top_srcdir)/linkerflags.cmd\"`"

check_vitdec_LDFLAGS+=\
--LINKER_SCRIPT_FILE "`_cygpath -w \"$(srcdir)/__memorymap__.cmd\"`" 
#------------------------------------------------------------------------

#------------------------------------------------------------------------
check_PROGRAMS+=check_vitdecmap
check_vitdecmap_CFLAGS = -I"`_cygpath -m $(top_srcdir)`"
check_vitdecmap_SOURCES = vitdecmap/main.c 
check_vitdecmap_LDADD = \
  $(top_builddir)/osl/lib.osl.vitdec.a \
  $(top_builddir)/oros/sfifos/lib/edmadriver/lib.oros.sfifos.edmadriver.a \
  $(top_builddir)/osl/lib.osl.a

check_vitdecmap_LDFLAGS = \
-I"${CSL_ROOT}/ti/csl/lib" \
-I"${DSPLIB_ROOT}/lib" 

LDFLAGS+=--LINKER_FLAGS_FILE "`_cygpath -w \"$(top_srcdir)/linkerflags.cmd\"`"

check_vitdecmap_LDFLAGS+=\
--LINKER_SCRIPT_FILE "`_cygpath -w \"$(srcdir)/__memorymap__.cmd\"`" 
#------------------------------------------------------------------------

#------------------------------------------------------------------------
include $(srcdir)/orilib/Makefile.am
#------------------------------------------------------------------------
