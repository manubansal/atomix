/**
Atomix project, ./test3/fifos.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <c6x.h>
#include <osl/inc/swpform.h>
#include <oros/sfifos/fifoFactory.h>
#include <oros/sfifos/fifoManager.h>

#include "fifos.h"

#include <ipc/ipc_cpy.h>


//#define FF_CORE_ID			1
//#define F2_CORE_ID			2

#define FF_CORE_ID_1			1
#define FF_CORE_ID_2			2

//#pragma DATA_SECTION(ff, ".gem##FF_CORE_ID##_data");
//#pragma DATA_SECTION(f2, ".gem##F2_CORE_ID##_data");
//#pragma DATA_SECTION(ff_state, ".gem##FF_CORE_ID##_data");
//#pragma DATA_SECTION(f2_state, ".gem##F2_CORE_ID##_data");
//#pragma DATA_SECTION(ff_mem, ".gem##FF_CORE_ID##_data");
//#pragma DATA_SECTION(f2_mem, ".gem##F2_CORE_ID##_data");

#pragma DATA_ALIGN(ff_mem, 8);
//#pragma DATA_ALIGN(f2_mem, 8);

FIFO_Handle ff;
//FIFO_Handle f2;
FIFO_State ff_state;
//FIFO_State f2_state;
Uint8 ff_mem[FF_TOTAL_SIZE_IN_BYTES];
//Uint8 f2_mem[FF_TOTAL_SIZE_IN_BYTES];
FIFO_BufferState ff_bufferStates[FF_N_BUFFERS];
//FIFO_BufferState f2_bufferStates[F2_N_BUFFERS];

Uint32 ff_transferOwner[FF_N_BUFFERS] = {
  FF_CORE_ID_1,
  FF_CORE_ID_1,
  FF_CORE_ID_1,
  FF_CORE_ID_1,
  FF_CORE_ID_1,
  FF_CORE_ID_1,
  FF_CORE_ID_1,
  FF_CORE_ID_1,
  FF_CORE_ID_1,
  FF_CORE_ID_1,
  FF_CORE_ID_1,
};


void setupFifos() {
  //create and setup fifos ff and f2
  Uint32 i;

  ff = &ff_state;
//  f2 = &f2_state;

  ff->mem = ff_mem;
//  f2->mem = f2_mem;
////  ff->bufferStates = ff_bufferStates;
//  f2->bufferStates = f2_bufferStates;
  //global address here because buffer states of a fifo are going to be 
  //accessed by other cores too, and the query should reflect this fifo's state
  ff->bufferStates = (FIFO_BufferState *)FIFO_localToGlobalAddress(ff_bufferStates, DNUM);

  ff->bufferSizeInBytes	= FF_BUFFER_SIZE_IN_BYTES;
  ff->nBuffersInFifo	= FF_N_BUFFERS;	
//  ff->locatedOnCoreId	= FF_CORE_ID;	//core 1
  ff->locatedOnCoreId	= DNUM;	//core 1

//  f2->bufferSizeInBytes	= F2_BUFFER_SIZE_IN_BYTES;
//  f2->nBuffersInFifo	= F2_N_BUFFERS;	
//  f2->locatedOnCoreId	= F2_CORE_ID;	//core 2

  ff->idxNextReadBuffer	= 0;
  ff->idxNextWriteBuffer= 0;
//  ff->nF		= 0;
//  ff->nE		= FF_N_BUFFERS;
//  ff->nW		= 0;
//  ff->nR		= 0;
//
  if (DNUM == FF_CORE_ID_1) {
    FIFO_setSrcOrDstType(ff, FIFO_TYPE_SRC);
    FIFO_setFifoRemoteCoreId(ff, FF_CORE_ID_2);
  }
  else if (DNUM == FF_CORE_ID_2) {
    FIFO_setSrcOrDstType(ff, FIFO_TYPE_DST);
    FIFO_setFifoRemoteCoreId(ff, FF_CORE_ID_1);
  }
  printf("ff     = %08p\n", (Uint32)ff);
  printf("ff->fr = %08p\n", (Uint32)(ff->fr));

//  f2->idxNextReadBuffer	= 0;
//  f2->nF		= 0;
//  f2->nE		= F2_N_BUFFERS;
//  f2->nW		= 0;
//  f2->nR		= 0;

  ASSERT(ff->bufferSizeInBytes % 8 == 0);
  for (i = 0; i < FF_N_BUFFERS; i++) {
    ff_bufferStates[i].seqNo = FIFO_nextSeqNo();
    ff_bufferStates[i].status = FIFO_BUFFER_EMPTY;
    ff_bufferStates[i].lengthInBytes = ff->bufferSizeInBytes;
    ff_bufferStates[i].mem = ff->mem + i * ff->bufferSizeInBytes;
    ff_bufferStates[i].mem_ga = (Uint32 *)FIFO_localToGlobalAddress(ff_bufferStates[i].mem, DNUM);
    ff_bufferStates[i].hL = &(ff_bufferStates[i].dL);


    if (DNUM == FF_CORE_ID_1) {
      IPC_linkOpen(
	  ff_bufferStates[i].hL,
	  ff_bufferStates[i].seqNo,
	  IPC_LINK_TYPE_DMA,
	  ff_transferOwner[i],
	  ff_bufferStates[i].mem_ga,
	  FIFO_localToGlobalAddress(ff_bufferStates[i].mem, FF_CORE_ID_2),
	  ff_bufferStates[i].lengthInBytes/8
	  );
    }
    else if (DNUM == FF_CORE_ID_2) {
      IPC_linkOpen(
      	ff_bufferStates[i].hL,
	ff_bufferStates[i].seqNo,
	IPC_LINK_TYPE_DMA,
	ff_transferOwner[i],
	FIFO_localToGlobalAddress(ff_bufferStates[i].mem, FF_CORE_ID_1),
	ff_bufferStates[i].mem_ga,
	ff_bufferStates[i].lengthInBytes/8
	);
    }
  }
//  for (i = 0; i < F2_N_BUFFERS; i++) {
//    f2_bufferStates[i].status = FIFO_BUFFER_EMPTY;
//    f2_bufferStates[i].lengthInBytes = f2->bufferSizeInBytes;
//    f2_bufferStates[i].mem = f2->mem + i * f2->bufferSizeInBytes;
//  }

////  if (FIFO_createFifo_BUF_Cplx32(ff) != FIFO_SUCCESS) {
//  if (FIFO_createFifo(ff) != FIFO_SUCCESS) {
//    printf("Error in creating ff\n");
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

  printf("ff bufferSizeInBytes: %d\n", ff->bufferSizeInBytes);
  printf("ff nBuffersInFifo   : %d\n", ff->nBuffersInFifo);
  printf("ff locatedOnCoreId  : %d\n", ff->locatedOnCoreId);
//  printf("f2 bufferSizeInBytes: %d\n", f2->bufferSizeInBytes);
//  printf("f2 nBuffersInFifo   : %d\n", f2->nBuffersInFifo);
//  printf("f2 locatedOnCoreId  : %d\n", f2->locatedOnCoreId);
}
