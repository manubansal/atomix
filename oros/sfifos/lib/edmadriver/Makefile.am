
include $(top_srcdir)/Makefile.am.common


#bin_PROGRAMS = hello
#hello_SOURCES = main.c

noinst_LIBRARIES = lib.oros.sfifos.edmadriver.a	#GNU naming convention
lib_oros_sfifos_edmadriver_a_SOURCES = \
  edmaDriver.c \
  edmaLinkDB.c \
  edmaIntDispatcher.c \
  timestamp.c


CFLAGS+=-I"$(srcdir)"

dist_doc_DATA = README

