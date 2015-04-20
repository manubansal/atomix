/**
Atomix project, ./test3/blockT.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include <oros/sfifos/fifoFactory.h>

//This simulates the transfer block T
//void doT(FIFO_BufferHandle bh) {
void doT_dma(FIFO_Handle ff) {
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

