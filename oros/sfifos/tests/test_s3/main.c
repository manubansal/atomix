/**
Atomix project, ./test_s3/main.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include <c6x.h>
#include <ti/csl/csl_tsc.h>

//#include "fifos.h"	//fifos for this test module
#include <ipc/ipc_cpy.h>

#include <osl/inc/timing.h>

//#include "blockX.h"
//#include "blockY.h"
//#include "blockT.h"

#include "__atoms__.h"
#include "__fifos__.h"

//extern FIFO_Handle ff;
//extern FIFO_Handle f2;

//extern FIFO_State ff_state;
//extern FIFO_State f2_state;

//the following pragma's ensure that all cores see the same copy of these variables,
//not their own copies in respective memories, so that they can communicate using
//these variables

//this placement doesn't work because shared memory is not consistent with multiple 
//core caches, so a flag written by one core doesn't get propagated to another core.
//#pragma DATA_SECTION(core1InitDone, ".shared_mem");
//#pragma DATA_SECTION(core2InitDone, ".shared_mem");
//#pragma DATA_SECTION(core1Done, ".shared_mem");
//#pragma DATA_SECTION(core2Done, ".shared_mem");

//this placement works becaues it uses unique copies of the flags that bypass cache
//layer for all but core 0, and core 0 is consistent with this copy anyway.
#pragma DATA_SECTION(core0InitDone, ".common_flags");
#pragma DATA_SECTION(core1InitDone, ".common_flags");
#pragma DATA_SECTION(core2InitDone, ".common_flags");
#pragma DATA_SECTION(core3InitDone, ".common_flags");

#pragma DATA_SECTION(core0Done, ".common_flags");
#pragma DATA_SECTION(core1Done, ".common_flags");
#pragma DATA_SECTION(core2Done, ".common_flags");
#pragma DATA_SECTION(core3Done, ".common_flags");

volatile int core0InitDone = 0;
volatile int core1InitDone = 0;
volatile int core2InitDone = 0;
volatile int core3InitDone = 0;

volatile int core0Done = 0;
volatile int core1Done = 0;
volatile int core2Done = 0;
volatile int core3Done = 0;

#define N_RW_ITERATIONS		10
//#pragma DATA_SECTION(sumX, ".common_flags");
//#pragma DATA_SECTION(sumY, ".common_flags");
//Uint32 sumX[N_RW_ITERATIONS];
//Uint32 sumY[N_RW_ITERATIONS];


//-----------------------------
void doCore1() {
//-----------------------------
  int i;
  printf("doCore1\n");

  //core1: A
  for (i = 0; i < N_RW_ITERATIONS; i++) {
    printf("doCore1: iteration %d\n", i);

    BlockX_do(x1);

    BlockX_do(x2);

    //BlockTD_do(t);
    BlockTR_do(t);

    BlockY_do(y1);
  }
}

//-----------------------------
void doCore2() {
//-----------------------------
  int i;
  printf("doCore2\n");

  //core2: B
  for (i = 0; i < N_RW_ITERATIONS; i++) {
    printf("doCore2: iteration %d\n", i);

    BlockY_do(y2);
  }
}

void doCore3() {
  //enabled in s1c
}

//void verify_test() {
//  int i;
//  for (i = 0; i < N_RW_ITERATIONS; i++) {
//    if (sumX[i] != sumY[i]) {
//      printf("TEST FAIL on iteration %d, sumX = %d, sumY = %d\n", i, sumX[i], sumY[i]);
//      return;
//    }
//    else {
//      printf("TEST PASS on iteration %d, sumX = %d, sumY = %d\n", i, sumX[i], sumY[i]);
//    }
//  }
//  //printf("TEST PASS.");
//}

void wait(Uint64 nCycles) {
  volatile CSL_Uint64 t;
  t = CSL_tscRead();
  while (CSL_tscRead() < t + nCycles);
}

//void init() {
////  setupFifos();
//  setupAtoms();
//  switch (DNUM) {
//    case 1:
//      core1InitDone = 1;
//      break;
//    case 2:
//      core2InitDone = 1;
//      break;
//    default:
//  }
//}

void initCore0() {
  setupFifoPtrs();

  setupFifoStates();
  setupFifoLinks();

  setupAtomPtrs();
  setupAtomWiring();
  setupAtomParams();
}

void initCore1() {
  setupFifoPtrs();
  setupAtomPtrs();
}

void initCore2() {
  setupFifoPtrs();
  setupAtomPtrs();
}

void initCore3() {
  setupFifoPtrs();
  setupAtomPtrs();
}


int main() {
  //on all cores
  CSL_tscEnable();
  IPC_init();
  

  //init only on core 0
  if (DNUM == 0) {
    //setupFifos();
    //setupAtoms();
    //wait(10000);
    initCore0();
    core0InitDone = 1;

    core0Done = 1;
    while (!(core0Done && core1Done && core2Done && core3Done));
    printf("All slave cores done, exiting on master core\n");
    wait(10000);
  }
  else if (DNUM == 1) {
    initCore1();
    while (!core0InitDone);
    core1InitDone = 1;

    while (!(core0InitDone && core1InitDone && core2InitDone && core3InitDone));
    doCore1();
    //verify_test();

    core1Done = 1;
    while (!(core0Done && core1Done && core2Done && core3Done));
    wait(10000);
  }
  else if (DNUM == 2) {
    initCore2();
    while (!core0InitDone);
    core2InitDone = 1;

    while (!(core0InitDone && core1InitDone && core2InitDone && core3InitDone));
    doCore2();
    //verify_test();

    core2Done = 1;
    while (!(core0Done && core1Done && core2Done && core3Done));
    wait(10000);
  }
  else if (DNUM == 3) {
    initCore3();
    while (!core0InitDone);
    core3InitDone = 1;

    while (!(core0InitDone && core1InitDone && core2InitDone && core3InitDone));
    doCore3();
    //verify_test();

    core3Done = 1;
    while (!(core0Done && core1Done && core2Done && core3Done));
    wait(10000);
  }

  IPC_close();
  //printf("core %d, no init\n", DNUM);
}
