/**
Atomix project, ./test2/main.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include <c6x.h>
#include <ti/csl/csl_tsc.h>

#include "fifos.h"	//fifos for this test module
#include <ipc/ipc_cpy.h>

#include <osl/inc/timing.h>

extern FIFO_Handle ff;
//extern FIFO_Handle f2;

extern FIFO_State ff_state;
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
#pragma DATA_SECTION(core1InitDone, ".common_flags");
#pragma DATA_SECTION(core2InitDone, ".common_flags");
#pragma DATA_SECTION(core1Done, ".common_flags");
#pragma DATA_SECTION(core2Done, ".common_flags");
volatile int core1InitDone = 0;
volatile int core2InitDone = 0;
volatile int core1Done = 0;
volatile int core2Done = 0;

#define N_RW_ITERATIONS		10
#pragma DATA_SECTION(sumA, ".common_flags");
#pragma DATA_SECTION(sumB, ".common_flags");
Uint32 sumA[N_RW_ITERATIONS];
Uint32 sumB[N_RW_ITERATIONS];

void init() {
  setupFifos();
  switch (DNUM) {
    case 1:
      core1InitDone = 1;
      break;
    case 2:
      core2InitDone = 1;
      break;
    default:
  }
}


//This simulates entity A, the data producer on core 1
void doA(FIFO_BufferHandle bh) {
  static char seqNo = 0;
  int i = 0;
  int sum = 0;
  static int iterNo = 0;
  for (i = 0; i < bh->lengthInBytes; i++) {
    bh->mem[i] = seqNo * (i+1);	//fill data
    sum += bh->mem[i];
  }
  sumA[iterNo++] = sum;
  printf("A: Filled buffer seq no. %2d\n", bh->mem[0]);
  printf("A: Buffer sum            %d\n", sum);
  seqNo++;
}

//This simulates entity B, the data consumer on core 2
void doB(FIFO_BufferHandle bh) {
  int i = 0;
  int sum = 0;
  static int iterNo = 0;
  for (i = 0; i < bh->lengthInBytes; i++) {
    sum += bh->mem[i];
  }
  sumB[iterNo++] = sum;
  printf("B: Read   buffer seq no. %2d\n", bh->mem[0]);
  printf("B: Buffer sum            %d\n", sum);
}

////This simulates the transfer block T
//void doT(FIFO_BufferHandle bh_dst, FIFO_BufferHandle bh_src) {
//  Uint32 sa = bh_src->mem_ga;
//  Uint32 da = bh_dst->mem_ga;
//  Uint32 nw = bh_dst->lengthInBytes >> 2;
//  printf("T: Transfering %d words from src 0x%08x to dst 0x%08x\n", nw, sa, da);
//  //IPC_cpy_shm(bh_dst->mem_ga, bh_src->mem_ga, bh_dst->lengthInBytes >> 2);
//  IPC_cpy_shm(da, sa, nw);
//}

//This simulates the transfer block T
//void doT(FIFO_BufferHandle bh) {
void doT(FIFO_Handle ff) {
  FIFO_BufferHandle bh_dst, bh_src;
  FIFO_Handle ff_r;

  //setup data transfer from F1 to F2
  ff_r = FIFO_getFifoRemoteHandle(ff);

  bh_dst = FIFO_getNextWriteBuffer(ff_r);

  bh_src = FIFO_getNextReadBuffer(ff);

  //TODO: These events will be deduced from TC; writeDone will be deduced on core 2,
  //readDone will be deduced on core1.
  //FIFO_writeDone(FIFO_getFifoRemoteHandle(ff), bh_dst);
  //FIFO_readDone(ff, bh_src);

  FIFO_clearTC(bh_src);
  FIFO_clearTC(bh_dst);

  //printf("T: Transfering %d words from src 0x%08x to dst 0x%08x\n", nw, sa, da);
  //IPC_cpy_shm(bh_dst->mem_ga, bh_src->mem_ga, bh_dst->lengthInBytes >> 2);
  //printf("T: transfer buffer seqNo %d over linkNum %d\n", bh->seqNo, bh->hL->linkNum);
  //IPC_cpu_transfer(bh->hL);
  printf("T: transfer buffer seqNo %d over linkNum %d\n", bh_src->seqNo, bh_src->hL->linkNum);
  //IPC_cpu_transfer(bh_src->hL);
  IPC_dma_transfer(bh_src->hL);
}

void doCore1() {
  printf("doCore1\n");
  FIFO_BufferHandle bh;
  int i;

  //core1: A

  for (i = 0; i < N_RW_ITERATIONS; i++) {
    printf("doCore1: iteration %d\n", i);

    //get a write buffer from F1
    bh = FIFO_getNextWriteBuffer(ff);

    //produce data and put it into the fifo F1
    doA(bh);

    //declare write done
    FIFO_writeDone(ff, bh);

    //transfer the next produced buffer over to core 2
    doT(ff);

  }
}

void doCore2() {

  int i;

  printf("doCore2\n");
  FIFO_BufferHandle bh;

  //core2: B
  for (i = 0; i < N_RW_ITERATIONS; i++) {
    printf("doCore2: iteration %d\n", i);

    //get next read buffer from F2
    //bh = FIFO_getNextReadBuffer(f2);
    bh = FIFO_getNextReadBuffer(ff);
    printf("doCore2: bh: %08x\n", (Uint32)bh);

    //consume data from F2
    doB(bh);

    //declare read done
    FIFO_readDone(ff, bh);
  }
}

void verify_test() {
  int i;
  for (i = 0; i < N_RW_ITERATIONS; i++) {
    if (sumA[i] != sumB[i]) {
      printf("TEST FAIL on iteration %d, sumA = %d, sumB = %d\n", i, sumA[i], sumB[i]);
      return;
    }
    else {
      printf("TEST PASS on iteration %d\n", i);
    }
  }
  //printf("TEST PASS.");
}

void wait(Uint64 nCycles) {
  volatile CSL_Uint64 t;
  t = CSL_tscRead();
  while (CSL_tscRead() < t + nCycles);
}

int main() {
  //on all cores
  CSL_tscEnable();
  IPC_init();

  //init only on core 0
  if (DNUM == 0) {
    //init();
    while (!core1Done);
    printf("Both cores done, exiting on core 0\n");
  }
  else if (DNUM == 1) {
    init();
    while (!core2InitDone);
    doCore1();
    while (!core2Done);
    core1Done = 1;
    wait(10000);
  }
  else if (DNUM == 2) {
    init();
    while (!core1InitDone);
    doCore2();
    verify_test();
    core2Done = 1;
    wait(10000);
  }

  IPC_close();
  //printf("core %d, no init\n", DNUM);
}
