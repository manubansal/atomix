/**
Atomix project, ./test1/fifos.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <c6x.h>
#include <osl/inc/swpform.h>
#include <orlib/orlib_sfifos/fifoFactory.h>
#include <orlib/orlib_sfifos/fifoManager.h>

#include "fifos.h"


//#define F1_CORE_ID			1
//#define F2_CORE_ID			2

#define F1_CORE_ID_1			1
#define F1_CORE_ID_2			2

//#pragma DATA_SECTION(f1, ".gem##F1_CORE_ID##_data");
//#pragma DATA_SECTION(f2, ".gem##F2_CORE_ID##_data");
//#pragma DATA_SECTION(f1_state, ".gem##F1_CORE_ID##_data");
//#pragma DATA_SECTION(f2_state, ".gem##F2_CORE_ID##_data");
//#pragma DATA_SECTION(f1_mem, ".gem##F1_CORE_ID##_data");
//#pragma DATA_SECTION(f2_mem, ".gem##F2_CORE_ID##_data");

#pragma DATA_ALIGN(f1_mem, 8);
//#pragma DATA_ALIGN(f2_mem, 8);

FIFO_Handle f1;
//FIFO_Handle f2;
FIFO_State f1_state;
//FIFO_State f2_state;
Uint8 f1_mem[F1_TOTAL_SIZE_IN_BYTES];
//Uint8 f2_mem[F1_TOTAL_SIZE_IN_BYTES];
FIFO_BufferState f1_bufferStates[F1_N_BUFFERS];
//FIFO_BufferState f2_bufferStates[F2_N_BUFFERS];


void setupFifos() {
  //create and setup fifos f1 and f2
  Uint32 i;

  f1 = &f1_state;
//  f2 = &f2_state;

  f1->mem = f1_mem;
//  f2->mem = f2_mem;
////  f1->bufferStates = f1_bufferStates;
//  f2->bufferStates = f2_bufferStates;
  //global address here because buffer states of a fifo are going to be 
  //accessed by other cores too, and the query should reflect this fifo's state
  f1->bufferStates = FIFO_localToGlobalAddress(f1_bufferStates, DNUM);

  f1->bufferSizeInBytes	= F1_BUFFER_SIZE_IN_BYTES;
  f1->nBuffersInFifo	= F1_N_BUFFERS;	
//  f1->locatedOnCoreId	= F1_CORE_ID;	//core 1
  f1->locatedOnCoreId	= DNUM;	//core 1

//  f2->bufferSizeInBytes	= F2_BUFFER_SIZE_IN_BYTES;
//  f2->nBuffersInFifo	= F2_N_BUFFERS;	
//  f2->locatedOnCoreId	= F2_CORE_ID;	//core 2

  f1->idxNextReadBuffer	= 0;
  f1->idxNextWriteBuffer= 0;
//  f1->nF		= 0;
//  f1->nE		= F1_N_BUFFERS;
//  f1->nW		= 0;
//  f1->nR		= 0;
//
  if (DNUM == F1_CORE_ID_1) {
    FIFO_setFifoRemoteCoreId(f1, F1_CORE_ID_2);
  }
  else {
    FIFO_setFifoRemoteCoreId(f1, F1_CORE_ID_1);
  }
  printf("f1     = %08x\n", (Uint32)f1);
  printf("f1->fr = %08x\n", (Uint32)(f1->fr));

//  f2->idxNextReadBuffer	= 0;
//  f2->nF		= 0;
//  f2->nE		= F2_N_BUFFERS;
//  f2->nW		= 0;
//  f2->nR		= 0;

  for (i = 0; i < F1_N_BUFFERS; i++) {
    f1_bufferStates[i].status = FIFO_BUFFER_EMPTY;
    f1_bufferStates[i].lengthInBytes = f1->bufferSizeInBytes;
    f1_bufferStates[i].mem = f1->mem + i * f1->bufferSizeInBytes;
    f1_bufferStates[i].mem_ga = FIFO_localToGlobalAddress(f1_bufferStates[i].mem, DNUM);
  }
//  for (i = 0; i < F2_N_BUFFERS; i++) {
//    f2_bufferStates[i].status = FIFO_BUFFER_EMPTY;
//    f2_bufferStates[i].lengthInBytes = f2->bufferSizeInBytes;
//    f2_bufferStates[i].mem = f2->mem + i * f2->bufferSizeInBytes;
//  }

////  if (FIFO_createFifo_BUF_Cplx32(f1) != FIFO_SUCCESS) {
//  if (FIFO_createFifo(f1) != FIFO_SUCCESS) {
//    printf("Error in creating f1\n");
//    exit(1);
//  }
//
////  if (FIFO_createFifo_BUF_Funny(&f2) != FIFO_SUCCESS) {
////    printf("Error in creating f2\n");
////    exit(1);
////  }
////  printf("f2 bufferSizeInBytes: %d\n", f2.bufferSizeInBytes);
////
////  if (FIFO_createFifo_BUF_Cplx32(f2) != FIFO_SUCCESS) {
//  if (FIFO_createFifo(f2) != FIFO_SUCCESS) {
//    printf("Error in creating f2\n");
//    exit(1);
//  }

  printf("f1 bufferSizeInBytes: %d\n", f1->bufferSizeInBytes);
  printf("f1 nBuffersInFifo   : %d\n", f1->nBuffersInFifo);
  printf("f1 locatedOnCoreId  : %d\n", f1->locatedOnCoreId);
//  printf("f2 bufferSizeInBytes: %d\n", f2->bufferSizeInBytes);
//  printf("f2 nBuffersInFifo   : %d\n", f2->nBuffersInFifo);
//  printf("f2 locatedOnCoreId  : %d\n", f2->locatedOnCoreId);
}
