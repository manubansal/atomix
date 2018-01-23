
include $(top_srcdir)/Makefile.am.common


#CFLAGS+=-I"$(srcdir)/inc"
#CFLAGS+=-I"$(top_srcdir)/osl/inc"
CFLAGS+=-I"`_cygpath -m $(srcdir)/inc`"


#osl library
noinst_LIBRARIES = lib.osl.a	#GNU naming convention
lib_osl_a_SOURCES = \
   util.c  \
   memSetup.c \
   OSL_debug.c \
   LOG_logger.c

# amem4bitcpy.c 

dist_doc_DATA = README

#osl vitdec library
noinst_LIBRARIES += lib.osl.vitdec.a	#GNU naming convention
lib_osl_vitdec_a_SOURCES = \
   vitdec/src/vcpdriver/debug.c \
   vitdec/src/vcpdriver/edmaConfig.c \
   vitdec/src/vcpdriver/vcpConfig.c \
   vitdec/src/vcpdriver/vcpdriver.c \
   vitdec/src/vitdec/vitdec_profile.c

#osl ethernet library
if OSLETH
noinst_LIBRARIES += lib.osl.eth.a	#GNU naming convention
lib_osl_eth_a_SOURCES = \
	eth/cppi_device.c \
	eth/cpsw_mgmt.c \
	eth/pa_mgmt.c \
	eth/singlecore_osal.c \
	eth/cppi_qmss_mgmt.c \
	eth/eth.c \
	eth/qmss_device.c \
	eth/view_ale_table.c \
	eth/pam_bin.c \
	eth/classify1_bin.c \
	eth/classify2_bin.c \
	uhd/uhd.c
endif 

#osl platform library
noinst_LIBRARIES += lib.osl.platform.a #GNU naming convention
lib_osl_platform_a_SOURCES = \
	platform/singlecore_osal.c
