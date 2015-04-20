/**
Atomix project, main.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include <ti/csl/csl_tsc.h>
#include <ti/csl/csl_cacheAux.h>
#include <c6x.h>

//#include "fifos.h"	//fifos for this test module
//#include <ipc/ipc_cpy.h>

#include <osl/inc/timing.h>

//#include "blockX.h"
//#include "blockY.h"
//#include "blockT.h"

#include "__atoms__.h"
#include "__fifos__.h"
#include "__states__.h"
//#include "__stateTable__.h"

//extern FIFO_Handle ff;
//extern FIFO_Handle f2;

//extern FIFO_State ff_state;
//extern FIFO_State f2_state;

//the following pragma's ensure that all cores see the same copy of these variables,
//not their own copies in respective memories, so that they can communicate using
//these variables

//this placement works becaues it uses unique copies of the flags that bypass cache
//layer for all but core 0, and core 0 is consistent with this copy anyway.
#pragma DATA_SECTION(core0Init0Done, ".common_flags");
#pragma DATA_SECTION(core1Init0Done, ".common_flags");
#pragma DATA_SECTION(core2Init0Done, ".common_flags");
#pragma DATA_SECTION(core3Init0Done, ".common_flags");

#pragma DATA_SECTION(core0Init1Done, ".common_flags");
#pragma DATA_SECTION(core1Init1Done, ".common_flags");
#pragma DATA_SECTION(core2Init1Done, ".common_flags");
#pragma DATA_SECTION(core3Init1Done, ".common_flags");

#pragma DATA_SECTION(core0Done, ".common_flags");
#pragma DATA_SECTION(core1Done, ".common_flags");
#pragma DATA_SECTION(core2Done, ".common_flags");
#pragma DATA_SECTION(core3Done, ".common_flags");

volatile int core0Init0Done = 0;
volatile int core1Init0Done = 0;
volatile int core2Init0Done = 0;
volatile int core3Init0Done = 0;

volatile int core0Init1Done = 0;
volatile int core1Init1Done = 0;
volatile int core2Init1Done = 0;
volatile int core3Init1Done = 0;

volatile int core0Done = 0;
volatile int core1Done = 0;
volatile int core2Done = 0;
volatile int core3Done = 0;

//#define N_MAX_KERNEL_ITERATIONS 10
#define N_MAX_KERNEL_ITERATIONS 1
//#define N_RW_ITERATIONS		10
//#pragma DATA_SECTION(sumX, ".common_flags");
//#pragma DATA_SECTION(sumY, ".common_flags");
//Uint32 sumX[N_RW_ITERATIONS];
//Uint32 sumY[N_RW_ITERATIONS];

#pragma DATA_SECTION(shmncTestVal, ".shared_mem_noncacheable");
Uint32 shmncTestVal;



//-------------------------------
// Kernel code
//-------------------------------
void kernel_0() {
  //while (1)
  int i;
  for (i = 0; i < N_MAX_KERNEL_ITERATIONS; i++)
    BlockKK_j_r_do(kk0);
}

void kernel_1() {
  //while (1)
  int i;
  for (i = 0; i < N_MAX_KERNEL_ITERATIONS; i++)
    BlockKK_j_r_do(kk1);
}

void kernel_2() {
  //while (1)
  int i;
  for (i = 0; i < N_MAX_KERNEL_ITERATIONS; i++)
    BlockKK_j_r_do(kk2);
}

void kernel_3() {
  //while (1)
  int i;
  for (i = 0; i < N_MAX_KERNEL_ITERATIONS; i++)
    BlockKK_j_r_do(kk3);
}


void wait(Uint64 nCycles) {
  volatile CSL_Uint64 t;
  t = CSL_tscRead();
  while (CSL_tscRead() < t + nCycles);
}

#define ENABLE_TIMING

//-------------------------------
// Init code
//-------------------------------
void init0Core0() {
  MEASURE_RUN_TIME(
  setupFifoPtrs();
  )

  setupFifoStates();
  setupFifoLinks();

  setupAtomPtrs();
  setupAtomWiring();
  setupAtomParams();
  //setupDecways();
  setupAtomConfs();
}

void init1Core0() {
  setupStateTable();
  setupInitState();

  test2i_setup_once();
}

void init0Core1() {
  setupFifoPtrs();
  setupAtomPtrs();
}

void init1Core1() {
  setupStateTable();
  setupInitState();


}

void init0Core2() {
  setupFifoPtrs();
  setupAtomPtrs();
}

void init1Core2() {
  setupStateTable();
  setupInitState();

}

void init0Core3() {
  setupFifoPtrs();
  setupAtomPtrs();
}

void init1Core3() {
  setupStateTable();
  setupInitState();

}

extern Uint32 BlockY_totalSum;

//-------------------------------
// Entry point
//-------------------------------
int main() {
  test2i_setup();
  //on all cores
  CSL_tscEnable();
  IPC_init();

  wait(10000);

  //init only on core 0
  if (DNUM == 0) {
    //setupFifos();
    //setupAtoms();
    //wait(10000);
    printf("DNUM = %d, shmncTestVal = %d\n", DNUM, shmncTestVal);
    shmncTestVal = 39;
    printf("DNUM = %d, shmncTestVal = %d\n", DNUM, shmncTestVal);

    init0Core0();
    core0Init0Done = 1;
    while (!(core0Init0Done && core1Init0Done && core2Init0Done && core3Init0Done));

    init1Core0();
    core0Init1Done = 1;
    while (!(core0Init1Done && core1Init1Done && core2Init1Done && core3Init1Done));

    kernel_0();

    core0Done = 1;
    while (!(core0Done && core1Done && core2Done && core3Done));
    printf("All slave cores done, exiting on master core\n");
    wait(10000);
  }
  else if (DNUM == 1) {
    init0Core1();
    while (!core0Init0Done);
    core1Init0Done = 1;
    while (!(core0Init0Done && core1Init0Done && core2Init0Done && core3Init0Done));

    printf("DNUM = %d, shmncTestVal = %d\n", DNUM, shmncTestVal);

    init1Core1();
    core1Init1Done = 1;
    while (!(core1Init1Done && core1Init1Done && core2Init1Done && core3Init1Done));

    kernel_1();

    core1Done = 1;
    while (!(core0Done && core1Done && core2Done && core3Done));

    printf("BlockY_totalSum = %d\n", BlockY_totalSum);

    wait(10000);
  }
  else if (DNUM == 2) {
    init0Core2();
    while (!core0Init0Done);
    core2Init0Done = 1;
    while (!(core0Init0Done && core1Init0Done && core2Init0Done && core3Init0Done));

    printf("DNUM = %d, shmncTestVal = %d\n", DNUM, shmncTestVal);

    init1Core2();
    core2Init1Done = 1;
    while (!(core2Init1Done && core2Init1Done && core2Init1Done && core3Init1Done));

    kernel_2();

    core2Done = 1;
    while (!(core0Done && core1Done && core2Done && core3Done));
    wait(10000);
  }
  else if (DNUM == 3) {
    init0Core3();
    while (!core0Init0Done);
    core3Init0Done = 1;
    while (!(core0Init0Done && core1Init0Done && core2Init0Done && core3Init0Done));

    printf("DNUM = %d, shmncTestVal = %d\n", DNUM, shmncTestVal);

    init1Core3();
    core3Init1Done = 1;
    while (!(core3Init1Done && core3Init1Done && core3Init1Done && core3Init1Done));

    kernel_3();

    core3Done = 1;
    while (!(core0Done && core1Done && core2Done && core3Done));
    wait(10000);
  }

  IPC_close();
}
