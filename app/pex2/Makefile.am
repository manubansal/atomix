ILIB_INCLUDE_PATH  =
ILIB_INCLUDE_PATH += -I$(ORSYS_ROOT)/exilib
ILIB_INCLUDE_PATH += -I$(ORSYS_ROOT)/oros/sfifos/ilib
ILIB_INCLUDE_PATH += -I$(ORSYS_ROOT)/oros/sysilib
ILIB_INCLUDE_PATH += -I$(ORSYS_ROOT)/orilib

USING_BLOCKS =
USING_BLOCKS += BlockX BlockY BlockDX BlockJ BlockA BlockB BlockAS


all: 
	PYTHONPATH=${PYTHONPATH}:$(abs_builddir) orpy.py ${ILIB_INCLUDE_PATH} Pex2 $(srcdir) ${USING_BLOCKS}
