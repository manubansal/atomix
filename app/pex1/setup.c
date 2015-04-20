/**
Atomix project, setup.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <osl/inc/swpform.h>
#include <ti/csl/csl_cacheAux.h>
#include <osl/inc/memSetup.h>
#include "setup.h"

const Uint8 NODE_ID = 1;

#pragma DATA_SECTION(TX_BUFFER, ".ddr");
Uint8 TX_BUFFER[TX_BUFFER_SIZE_BYTES];


//#define ENABLE_L1SRAM
static void setL1DSize(CACHE_L1Size size) {
  CACHE_setL1DSize((CACHE_L1Size)size);
  if (CACHE_getL1DSize() != size) {
    DEBUG_ERROR(printf("ERROR: L1D cache size: %d\n", CACHE_getL1DSize());)
  }
  else {
    DEBUG_INFO(printf("SUCCESS: L1D cache size: %d\n", CACHE_getL1DSize());)
  }
}

static void setL1PSize(CACHE_L1Size size) {
  CACHE_setL1PSize((CACHE_L1Size)size);
  if (CACHE_getL1PSize() != size) {
    DEBUG_ERROR(printf("ERROR: L1P cache size: %d\n", CACHE_getL1PSize());)
  }
  else {
    //DEBUG(
    DEBUG_INFO(printf("SUCCESS: L1P cache size: %d\n", CACHE_getL1PSize());)
    //)
  }
}


void app_wifirx54mc_setup() {
//  CACHE_invAllL1d(CACHE_WAIT);
//  CACHE_invAllL1p(CACHE_WAIT);

  //Allocate part of L1 as SRAM instead of all cache
////#ifdef ENABLE_L1SRAM
////  setL1DSize((CACHE_L1Size)CACHE_L1_16KCACHE);	//16K L1DSRAM
////  setL1PSize((CACHE_L1Size)CACHE_L1_16KCACHE);	//16K L1PSRAM
////#elif defined(ENABLE_L1SRAM_32K)
////  setL1DSize((CACHE_L1Size)CACHE_L1_0KCACHE);	//32K L1DSRAM
////  setL1PSize((CACHE_L1Size)CACHE_L1_16KCACHE);	//16K L1PSRAM
////#elif defined(DISABLE_ALL_L1_CACHING)
////  setL1DSize((CACHE_L1Size)CACHE_L1_0KCACHE);	//32K L1DSRAM
////  setL1PSize((CACHE_L1Size)CACHE_L1_0KCACHE);	//32K L1PSRAM
////#endif

#ifdef L1D_SRAM_ZERO
  setL1DSize((CACHE_L1Size)CACHE_L1_32KCACHE);	//0K L1DSRAM
#elif defined(L1D_SRAM_16K)
  setL1DSize((CACHE_L1Size)CACHE_L1_16KCACHE);	//16K L1DSRAM
#elif defined(L1D_SRAM_24K)
  setL1DSize((CACHE_L1Size)CACHE_L1_8KCACHE);	//24K L1DSRAM
#elif defined(L1D_SRAM_28K)
  setL1DSize((CACHE_L1Size)CACHE_L1_4KCACHE);	//28K L1DSRAM
#elif defined(L1D_SRAM_32K)
  setL1DSize((CACHE_L1Size)CACHE_L1_0KCACHE);	//32K L1DSRAM
#elif defined(L1D_DISABLE)
  setL1DSize((CACHE_L1Size)CACHE_L1_0KCACHE);	//no cache
#endif

#ifdef L1P_SRAM_ZERO
  setL1PSize((CACHE_L1Size)CACHE_L1_32KCACHE);	//0K L1PSRAM
#elif defined(L1P_SRAM_16K)
  setL1PSize((CACHE_L1Size)CACHE_L1_16KCACHE);	//16K L1PSRAM
#elif defined(L1P_SRAM_32K)
  setL1PSize((CACHE_L1Size)CACHE_L1_0KCACHE);	//32K L1PSRAM
#elif defined(L1P_DISABLE)
  setL1PSize((CACHE_L1Size)CACHE_L1_0KCACHE);	//no cache
#endif


  //Turn caching off for L2 shared memory 
  MEMSETUP_remapAndTurnCachingOff (
  3, 
  0xE0000000, 
  0x0C000000, 
  0x400	
  );


  WIFILIB_genDerotTable();			
  WIFILIB_genTwiddleFactors();
  WIFILIB_genMapperLUTs();
  WIFILIB_genSoftSlicingTable64qam();
  WIFILIB_genCrc32LookupTable();


  vitdec_profile_init();
}

//initialization that needs to happen on each core but must be serialized across cores
app_wifirx54mc_setup_each_seq() {
  vcp2_initOnce();
  vcp2_initPerUse(0,0,0,0);
}

void app_wifirx54mc_setup_once() {
}
