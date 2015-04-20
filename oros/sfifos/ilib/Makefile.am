
include $(top_srcdir)/Makefile.am.common

#-------------------------------------------------
# block library for sfifos system functionality
#-------------------------------------------------
noinst_LIBRARIES = lib.oros.sfifos.ilib.a	#GNU naming convention
lib_oros_sfifos_ilib_a_SOURCES = \
   BlockTR_i.c \
   BlockTR_i.h \
   BlockTZ_i.c \
   BlockTZ_i.h 
   #BlockTD_i.c \
   #BlockTD_i.h 

#nodist_lib_oros_sfifos_ilib_a_SOURCES = \
#  __BlockTR__.c \
#  __BlockTR__.h \
#  __BlockTD__.c \
#  __BlockTD__.h 
#
#lib_oros_sfifos_ilib_a_CLEANFILES = \
#  $(top_srcdir)/oros/sfifos/ilib/__BlockTR__.h \
#  $(top_srcdir)/oros/sfifos/ilib/__BlockTR__.c \
#  $(top_srcdir)/oros/sfifos/ilib/__BlockTD__.h \
#  $(top_srcdir)/oros/sfifos/ilib/__BlockTD__.c
#
#
##version 2: overriding automake generated rule and manually recording dependencies
#LIB_OROS_SFIFOS_ILIB_A_GENERATED_BLOCKS = BlockTR BlockTD
#LIB_OROS_SFIFOS_ILIB_A_DIR = $(top_srcdir)/oros/sfifos/ilib
#
#lib.oros.sfifos.ilib.a: $(lib_oros_sfifos_ilib_a_OBJECTS) $(lib_oros_sfifos_ilib_a_DEPENDENCIES) $(EXTRA_lib_oros_sfifos_ilib_a_DEPENDENCIES)  $(LIB_OROS_SFIFOS_ILIB_A_GENERATED_BLOCKS)
#	-rm -f lib.oros.sfifos.ilib.a
#	$(lib_oros_sfifos_ilib_a_AR) lib.oros.sfifos.ilib.a $(lib_oros_sfifos_ilib_a_OBJECTS) $(lib_oros_sfifos_ilib_a_LIBADD)
#	$(RANLIB) lib.oros.sfifos.ilib.a
#
#$(LIB_OROS_SFIFOS_ILIB_A_GENERATED_BLOCKS): %: __%__.c 
#
#__%__.c: $(LIB_OROS_SFIFOS_ILIB_A_DIR)/%_i.h
#	_create_b_i$(EXEEXT) $< $(LIB_OROS_SFIFOS_ILIB_A_DIR)/__$*__.h $(LIB_OROS_SFIFOS_ILIB_A_DIR)/__$*__.c
#


dist_doc_DATA = README
