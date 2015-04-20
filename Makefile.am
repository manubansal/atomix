EXTRA_DIST=
BUILT_SOURCES=

include Makefile.am.common

CFLAGS+=-I"${CSL_ROOT}/ti/csl/lib"
CFLAGS+=-I"${DSPLIB_ROOT}/packages/ti/dsplib"

SUBDIRS = \
	osl \
	oros/sfifos/lib/edmadriver \
	oros/sfifos/lib/ipc \
	oros/sfifos/ilib \
	oros/sfifos \
	exilib \
	oros/sysilib  \
	unittests
#	oros/tests/test2i \
#	app/subadd \
#

dist_doc_DATA = README.md

bin_PROGRAMS = 

###########################################
#common to all binary builds
###########################################
LDFLAGS=--LINKER_FLAGS_FILE "`_cygpath -w \"$(top_srcdir)/linkerflags.cmd\"`"


###########################################
## applications #####################
###########################################
include app/Makefile.am.app

install-data-local:
	if [ ! -e @CCS_ROOT_VAL@ ] ; then echo @CCS_ROOT_VAL@ does not exist. Are you specifying the right Code Composer Studio \
	root directory? ; exit 1 ; fi
	if [ ! -e @CCS_ROOT_VAL@/ccs_base/common/targetdb/platform ] ; then mkdir -p @CCS_ROOT_VAL@/ccs_base/common/targetdb/platform ; fi
	cp $(top_srcdir)/targetconfs/support/nyquist_tms320c66xx.xml @CCS_ROOT_VAL@/ccs_base/common/targetdb/devices/

install-vim-syntax:
	if [ ! -e ~/.vim ] ; then mkdir ~/.vim ; fi
	cp $(top_srcdir)/dotvim/filetype.vim ~/.vim/
	if [ ! -e ~/.vim/syntax ] ; then mkdir ~/.vim/syntax ; fi
	cp $(top_srcdir)/dotvim/syntax/or.vim ~/.vim/syntax/

host:
	echo host architecture: $(host)

clean-local:
	rm -rf *.map
	rm -f ~/.vim/filetype.vim
	rm -f ~/.vim/syntax/or.vim
