
include $(top_srcdir)/Makefile.am.common


#bin_PROGRAMS = hello
#hello_SOURCES = main.c

noinst_LIBRARIES = lib.oros.sfifos.ipc.a	#GNU naming convention
lib_oros_sfifos_ipc_a_SOURCES = \
  ipc_cpy.c


CFLAGS+=-I"`_cygpath -m $(srcdir)/../`"

dist_doc_DATA = README
