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

#pragma DATA_SECTION(core0SeqInitDone, ".common_flags");
#pragma DATA_SECTION(core1SeqInitDone, ".common_flags");
#pragma DATA_SECTION(core2SeqInitDone, ".common_flags");
#pragma DATA_SECTION(core3SeqInitDone, ".common_flags");

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

volatile int core0SeqInitDone = 0;
volatile int core1SeqInitDone = 0;
volatile int core2SeqInitDone = 0;
volatile int core3SeqInitDone = 0;

volatile int core0Done = 0;
volatile int core1Done = 0;
volatile int core2Done = 0;
volatile int core3Done = 0;

//uncomment this to make execution stop after
//a finite number of kernel iterations
//#define LIMIT_KERNEL_ITERATIONS

//#define N_MAX_KERNEL_ITERATIONS		10
//#define N_MAX_KERNEL_ITERATIONS		2000
#define N_MAX_KERNEL_ITERATIONS		20000

//#define N_RW_ITERATIONS		10
//#pragma DATA_SECTION(sumX, ".common_flags");
//#pragma DATA_SECTION(sumY, ".common_flags");
//Uint32 sumX[N_RW_ITERATIONS];
//Uint32 sumY[N_RW_ITERATIONS];

#pragma DATA_SECTION(shmncTestVal, ".shared_mem_noncacheable");
Uint32 shmncTestVal;


#ifdef SNRBER_PKTDATAOUTDDR
#pragma DATA_SECTION(PKTDATAOUT, ".ddr");
#pragma DATA_SECTION(PKTDATAOUT_nBytesWritten, ".shared_mem_noncacheable");
#define PKTDATAOUT_SIZE	400 * 1514
Uint8 PKTDATAOUT[PKTDATAOUT_SIZE];
far Uint32 PKTDATAOUT_nBytesWritten = 0;
#endif

//-------------------------------
// Kernel code
//-------------------------------
void kernel_0() {
#ifdef LIMIT_KERNEL_ITERATIONS
  int i;
  for (i = 0; i < N_MAX_KERNEL_ITERATIONS; i++)
#else
  while (1)
#endif
    BlockKK_j_r_do(kk0);
}

void kernel_1() {
#ifdef LIMIT_KERNEL_ITERATIONS
  int i;
  for (i = 0; i < N_MAX_KERNEL_ITERATIONS; i++)
#else
  while (1)
#endif
    BlockKK_j_r_do(kk1);
}

void kernel_2() {
#ifdef LIMIT_KERNEL_ITERATIONS
  int i;
  for (i = 0; i < N_MAX_KERNEL_ITERATIONS; i++)
#else
  while (1)
#endif
    BlockKK_j_r_do(kk2);
}

void kernel_3() {
#ifdef LIMIT_KERNEL_ITERATIONS
  int i;
  for (i = 0; i < N_MAX_KERNEL_ITERATIONS; i++)
#else
  while (1)
#endif
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
  setupFifoPtrs();

  setupFifoStates();
  setupFifoLinks();

  setupAtomPtrs();
  setupAtomWiring();
  setupAtomParams();
  //setupDecways();
  setupAtomConfs();
  setupAtomDebug();
}

void init1Core0() {
  setupStateTable();
  setupInitState();

  app_wifirx54mc_setup_once();
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

void initSeqCore0() {
  app_wifirx54mc_setup_each_seq();
}

void initSeqCore1() {
  app_wifirx54mc_setup_each_seq();
}

void initSeqCore2() {
  app_wifirx54mc_setup_each_seq();
}

void initSeqCore3() {
  app_wifirx54mc_setup_each_seq();

#ifdef SNRBER_PKTDATAOUTDDR
  memset(PKTDATAOUT, 0xFF, PKTDATAOUT_SIZE);
#endif
}

//-------------------------------
// Entry point
//-------------------------------
int main() {

#ifdef LOG_CONSOLE
  LOG_setup();
#endif

  app_wifirx54mc_setup();
  //on all cores
  CSL_tscEnable();
  IPC_init();

  wait(10000);

  //init only on core 0
  if (DNUM == 0) {
    //setupFifos();
    //setupAtoms();
    //wait(10000);
    DEBUG_INIT(printf("DNUM = %d, shmncTestVal = %d\n", DNUM, shmncTestVal);)
    shmncTestVal = 39;
    DEBUG_INIT(printf("DNUM = %d, shmncTestVal = %d\n", DNUM, shmncTestVal);)

    //init stage 0
    init0Core0();
    core0Init0Done = 1;
    while (!(core0Init0Done && core1Init0Done && core2Init0Done && core3Init0Done));

    //init stage 1
    init1Core0();
    core0Init1Done = 1;
    while (!(core0Init1Done && core1Init1Done && core2Init1Done && core3Init1Done));

    //sequential init
    initSeqCore0();
    core0SeqInitDone = 1;
    while (!(core0SeqInitDone && core1SeqInitDone && core2SeqInitDone && core3SeqInitDone));

    kernel_0();

    core0Done = 1;
    while (!(core0Done && core1Done && core2Done && core3Done));
    DEBUG_INFO(printf("All slave cores done, exiting on master core\n");)
    wait(10000);
  }
  else if (DNUM == 1) {

    //init stage 0
    init0Core1();
    while (!core0Init0Done);
    core1Init0Done = 1;
    while (!(core0Init0Done && core1Init0Done && core2Init0Done && core3Init0Done));

    DEBUG_INIT(printf("DNUM = %d, shmncTestVal = %d\n", DNUM, shmncTestVal);)

    //init stage 1
    init1Core1();
    core1Init1Done = 1;
    while (!(core1Init1Done && core1Init1Done && core2Init1Done && core3Init1Done));

    //sequential init
    while (!core0SeqInitDone);
    initSeqCore1();
    core1SeqInitDone = 1;
    while (!(core0SeqInitDone && core1SeqInitDone && core2SeqInitDone && core3SeqInitDone));

    kernel_1();

    core1Done = 1;
    while (!(core0Done && core1Done && core2Done && core3Done));
    wait(10000);
  }
  else if (DNUM == 2) {
    //init stage 0
    init0Core2();
    while (!core0Init0Done);
    core2Init0Done = 1;
    while (!(core0Init0Done && core1Init0Done && core2Init0Done && core3Init0Done));

    DEBUG_INIT(printf("DNUM = %d, shmncTestVal = %d\n", DNUM, shmncTestVal);)

    //init stage 1
    init1Core2();
    core2Init1Done = 1;
    while (!(core2Init1Done && core2Init1Done && core2Init1Done && core3Init1Done));

    //sequential init
    while (!core1SeqInitDone);
    initSeqCore2();
    core2SeqInitDone = 1;
    while (!(core0SeqInitDone && core1SeqInitDone && core2SeqInitDone && core3SeqInitDone));


    kernel_2();

    core2Done = 1;
    while (!(core0Done && core1Done && core2Done && core3Done));
    wait(10000);
  }
  else if (DNUM == 3) {
    //init stage 0
    init0Core3();
    while (!core0Init0Done);
    core3Init0Done = 1;
    while (!(core0Init0Done && core1Init0Done && core2Init0Done && core3Init0Done));

    DEBUG_INIT(printf("DNUM = %d, shmncTestVal = %d\n", DNUM, shmncTestVal);)

    //init stage 1
    init1Core3();
    core3Init1Done = 1;
    while (!(core3Init1Done && core3Init1Done && core3Init1Done && core3Init1Done));

    //sequential init
    while (!core2SeqInitDone);
    initSeqCore3();
    core3SeqInitDone = 1;
    while (!(core0SeqInitDone && core1SeqInitDone && core2SeqInitDone && core3SeqInitDone));


    kernel_3();

    core3Done = 1;
    while (!(core0Done && core1Done && core2Done && core3Done));
    wait(10000);
  }

  IPC_close();

#ifdef LOG_CONSOLE
  LOG_close();
#endif
}
