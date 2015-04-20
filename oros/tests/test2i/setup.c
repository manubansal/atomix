/**
Atomix project, setup.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include <ti/csl/csl_cacheAux.h>
#include <osl/inc/memSetup.h>

void test2i_setup() {
//  CACHE_invAllL1d(CACHE_WAIT);
//  CACHE_invAllL1p(CACHE_WAIT);
  CACHE_setL1DSize((CACHE_L1Size)CACHE_L1_16KCACHE);
  if (CACHE_getL1DSize() != CACHE_L1_16KCACHE) {
    printf("ERROR: L1D cache size: %d\n", CACHE_getL1DSize());
  }
  else {
	  DEBUG(
	  printf("SUCCESS: L1D cache size: %d\n", CACHE_getL1DSize());
	  )
  }
  CACHE_setL1PSize((CACHE_L1Size)CACHE_L1_16KCACHE);
  if (CACHE_getL1PSize() != CACHE_L1_16KCACHE) {
    printf("ERROR: L1P cache size: %d\n", CACHE_getL1PSize());
  }
  else {
	  DEBUG(
	  printf("SUCCESS: L1P cache size: %d\n", CACHE_getL1PSize());
	  )
  }



//	MEMSETUP_remapAndTurnCachingOff (
//	unsigned int priority, 
//	unsigned long virtual, 
//	unsigned long physical, 
//	unsigned long size_kbytes
//	){

	MEMSETUP_remapAndTurnCachingOff (
	3, 
	0xE0000000, 
	0x0C000000, 
	0x400	
	);
}

void test2i_setup_once() {
}
