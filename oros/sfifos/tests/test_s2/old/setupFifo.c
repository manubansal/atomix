/**
Atomix project, ./test_s2/old/setupFifo.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <c6x.h>
#include <osl/inc/swpform.h>
#include <oros/sfifos/fifoFactory.h>
#include <oros/sfifos/fifoManager.h>
#include <ipc/ipc_cpy.h>

void setupFifo(
    FIFO_Handle ff,
    FIFO_State *ff_state,
    Uint8 ff_mem[],
    FIFO_BufferState *ff_bufferStates[],
    Uint32 ff_BUFFER_SIZE_IN_BYTES,
    Uint32 ff_N_BUFFERS,
    FIFO_TYPE_SRC_OR_DST ff_type
    ) {
  //create and setup fifos ff and f2
  Uint32 i;

  ff = &ff_state;
  ff->mem = ff_mem;
  //global address here because buffer states of a fifo are going to be 
  //accessed by other cores too, and the query should reflect this fifo's state
  ff->bufferStates = (FIFO_BufferState *)FIFO_localToGlobalAddress(ff_bufferStates, DNUM);

  ff->bufferSizeInBytes	= ff_BUFFER_SIZE_IN_BYTES;
  ff->nBuffersInFifo	= ff_N_BUFFERS;	
  ff->locatedOnCoreId	= DNUM;

  ff->idxNextReadBuffer	= 0;
  ff->idxNextWriteBuffer= 0;

  FIFO_setSrcOrDstType(ff, ff_type);
  //if (DNUM == FF_CORE_ID_1) {
  //  FIFO_setSrcOrDstType(ff, FIFO_TYPE_SRC);
  //  FIFO_setFifoRemoteCoreId(ff, FF_CORE_ID_2);
  //}
  //else if (DNUM == FF_CORE_ID_2) {
  //  FIFO_setSrcOrDstType(ff, FIFO_TYPE_DST);
  //  FIFO_setFifoRemoteCoreId(ff, FF_CORE_ID_1);
  //}
  printf("ff     = %08p\n", (Uint32)ff);
  //printf("ff->fr = %08p\n", (Uint32)(ff->fr));

//  f2->idxNextReadBuffer	= 0;
//  f2->nF		= 0;
//  f2->nE		= F2_N_BUFFERS;
//  f2->nW		= 0;
//  f2->nR		= 0;

  ASSERT(ff->bufferSizeInBytes % 8 == 0);
  for (i = 0; i < ff_N_BUFFERS; i++) {
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

  printf("ff bufferSizeInBytes: %d\n", ff->bufferSizeInBytes);
  printf("ff nBuffersInFifo   : %d\n", ff->nBuffersInFifo);
  printf("ff locatedOnCoreId  : %d\n", ff->locatedOnCoreId);
}
