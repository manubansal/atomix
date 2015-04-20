
include $(top_srcdir)/Makefile.am.common

noinst_LIBRARIES = lib.oros.sfifos.a	#GNU naming convention

lib_oros_sfifos_a_SOURCES = \
  fifoFactory.c \
  fifoManagerUtil.c

#  fifoManagerReadOpt.c \
#  fifoManagerWriteOpt.c \
#

#  fifoManager.c \
#  python/src/fifoTypedConstructors.c
#  lib/ipc/ipc_cpy.c


CFLAGS+=-I"`_cygpath -m $(srcdir)/lib`"

dist_doc_DATA = README

CFLAGS+=-DENABLE_TIMING -DNDEBUG
