
include $(top_srcdir)/Makefile.am.common

#-------------------------------------------------
# block library for oros system core functionality
#-------------------------------------------------
noinst_LIBRARIES = lib.oros.sysilib.a	#GNU naming convention
lib_oros_sysilib_a_SOURCES = \
   BlockKK_i.c \
   BlockCP_i.c \
   BlockJ_i.c \
   BlockJn_i.c \
   BlockDrain_i.c \
   BlockTouch_i.c \
   BlockNOP_i.c \
   BlockCP2_i.c \
   SYS_TimeStamp.c

#nodist_lib_oros_sysilib_a_SOURCES = \
#  __BlockKK__.c \
#  __BlockCP__.c \
#  __BlockJ__.c \
#  __BlockDrain__.c \
#  __BlockCP2__.c
#
#lib_oros_sysilib_a_CLEANFILES = \
#  $(top_srcdir)/oros/sysilib/__BlockKK__.h \
#  $(top_srcdir)/oros/sysilib/__BlockKK__.c \
#  $(top_srcdir)/oros/sysilib/__BlockCP__.h \
#  $(top_srcdir)/oros/sysilib/__BlockCP__.c \
#  $(top_srcdir)/oros/sysilib/__BlockJ__.h \
#  $(top_srcdir)/oros/sysilib/__BlockJ__.c \
#  $(top_srcdir)/oros/sysilib/__BlockDrain__.h \
#  $(top_srcdir)/oros/sysilib/__BlockDrain__.c \
#  $(top_srcdir)/oros/sysilib/__BlockCP2__.h \
#  $(top_srcdir)/oros/sysilib/__BlockCP2__.c 
#
#	
##version 3: overriding automake generated rule and manually recording dependencies
#LIB_OROS_SYSILIB_A_GENERATED_BLOCKS = BlockCP BlockKK BlockJ BlockDrain BlockCP2
#LIB_OROS_SYSILIB_A_DIR = $(top_srcdir)/oros/sysilib
#
#lib.oros.sysilib.a: $(LIB_OROS_SYSILIB_A_GENERATED_BLOCKS) $(lib_oros_sysilib_a_OBJECTS) $(lib_oros_sysilib_a_DEPENDENCIES) $(EXTRA_lib_oros_sysilib_a_DEPENDENCIES)  
#	-rm -f lib.oros.sysilib.a
#	$(lib_oros_sysilib_a_AR) lib.oros.sysilib.a $(lib_oros_sysilib_a_OBJECTS) $(lib_oros_sysilib_a_LIBADD)
#	$(RANLIB) lib.oros.sysilib.a
#
#$(LIB_OROS_SYSILIB_A_GENERATED_BLOCKS): %: __%__.c 
#
#__%__.c: $(LIB_OROS_SYSILIB_A_DIR)/%_i.h
#	_create_b_i$(EXEEXT) $< $(LIB_OROS_SYSILIB_A_DIR)/__$*__.h $(LIB_OROS_SYSILIB_A_DIR)/__$*__.c
#
##CFLAGS+=-I$(top_srcdir)/oros/sfifos/lib

dist_doc_DATA = README
