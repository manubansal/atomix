
include $(top_srcdir)/Makefile.am.common

noinst_LIBRARIES = lib.exilib.a	#GNU naming convention
lib_exilib_a_SOURCES = \
  BlockA_i.h \
  BlockA_i.c \
  BlockAS_i.h \
  BlockAS_i.c \
  BlockASI_i.c \
  BlockASI_i.h \
  BlockB_i.c \
  BlockB_i.h \
  BlockX_i.c \
  BlockX_i.h \
  BlockY_i.c \
  BlockY_i.h \
  BlockDX_i.c \
  BlockDX_i.h 

#nodist_lib_exilib_a_SOURCES = \
#  __BlockA__.h \
#  __BlockA__.c \
#  __BlockAS__.h \
#  __BlockAS__.c \
#  __BlockASI__.c \
#  __BlockASI__.h \
#  __BlockB__.c \
#  __BlockB__.h \
#  __BlockX__.c \
#  __BlockX__.h \
#  __BlockY__.c \
#  __BlockY__.h \
#  __BlockDX__.c \
#  __BlockDX__.h
#
#
#
#lib_exilib_a_CLEANFILES = \
#  $(top_srcdir)/exilib/__BlockA__.h \
#  $(top_srcdir)/exilib/__BlockA__.c \
#  $(top_srcdir)/exilib/__BlockAS__.h \
#  $(top_srcdir)/exilib/__BlockAS__.c \
#  $(top_srcdir)/exilib/__BlockASI__.c \
#  $(top_srcdir)/exilib/__BlockASI__.h \
#  $(top_srcdir)/exilib/__BlockB__.c \
#  $(top_srcdir)/exilib/__BlockB__.h \
#  $(top_srcdir)/exilib/__BlockX__.c \
#  $(top_srcdir)/exilib/__BlockX__.h \
#  $(top_srcdir)/exilib/__BlockY__.c \
#  $(top_srcdir)/exilib/__BlockY__.h \
#  $(top_srcdir)/exilib/__BlockDX__.c \
#  $(top_srcdir)/exilib/__BlockDX__.h
#
#LIB_EXILIB_A_GENERATED_BLOCKS = BlockA BlockAS BlockASI BlockB BlockX BlockY BlockDX
#LIB_EXILIB_A_DIR = $(top_srcdir)/exilib
#
#
#lib.exilib.a: $(LIB_EXILIB_A_GENERATED_BLOCKS) $(lib_exilib_a_OBJECTS) $(lib_exilib_a_DEPENDENCIES) $(EXTRA_lib_exilib_a_DEPENDENCIES)  
#	-rm -f lib.exilib.a
#	$(lib_exilib_a_AR) lib.exilib.a $(lib_exilib_a_OBJECTS) $(lib_exilib_a_LIBADD)
#	$(RANLIB) lib.exilib.a
#
#$(LIB_EXILIB_A_GENERATED_BLOCKS): %: __%__.c 
#
#__%__.c: $(LIB_EXILIB_A_DIR)/%_i.h
#	_create_b_i$(EXEEXT) $< $(LIB_EXILIB_A_DIR)/__$*__.h $(LIB_EXILIB_A_DIR)/__$*__.c

dist_doc_DATA = README
