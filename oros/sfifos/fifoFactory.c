/**
Atomix project, fifoFactory.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <osl/inc/swpform.h>
#include <osl/inc/timing.h>
#include "fifoFactory.h"
#include "fifoManager.h"
#include <ipc/ipc_cpy.h>

#ifdef _TMS320C6X
#include <c6x.h>
#endif


//////DEBUG(
//////void FIFO_setSrcOrDstType(FIFO_Handle f, Uint32 srcOrDst) {
//////  f->srcOrDst = srcOrDst;
//////}
//////)

Uint32 FIFO_nextSeqNo() {
  static Uint32 nextSeqNo = 0;
  return nextSeqNo++;
}


void FIFO_setupFifo(
    FIFO_Handle ff,
    Uint8 ff_mem[],
    FIFO_BufferState ff_bufferStates[],
    Uint32 ff_N_BUFFERS,
    Uint32 ff_BUFFER_SIZE_IN_BYTES,
    FIFO_TYPE_SRC_OR_DST ff_type,
    Uint32 ff_CORE_ID,
    Uint32 isFilled
    ) {

  Uint32 i, status;

  //ff->mem = ff_mem;
  ff->bufferStates = (FIFO_BufferState *)FIFO_localToGlobalAddress(ff_bufferStates, ff_CORE_ID);

  ff->nBuffersInFifo	= ff_N_BUFFERS;	

  //////DEBUG(
  ff->bufferSizeInBytes	= ff_BUFFER_SIZE_IN_BYTES;
  //////ff->locatedOnCoreId	= DNUM;
  //////FIFO_setSrcOrDstType(ff, ff_type);
  //////)

  ff->idxNextReadBuffer	= 0;
  ff->idxNextWriteBuffer= 0;
  ff->anyReadBusyHead = 0;
  ff->idxReadBusyHead = 0;
  ff->anyWriteBusyHead = 0;
  ff->idxWriteBusyHead = 0;

  DEBUG_INIT(printf("ff     = %08p\n", ff));

  if (isFilled == 1)
    status = FIFO_BUFFER_FILLED;
  else
    status = FIFO_BUFFER_EMPTY;

  ASSERT(ff_BUFFER_SIZE_IN_BYTES % 8 == 0);
  for (i = 0; i < ff_N_BUFFERS; i++) {
    ////DEBUG(
    ////ff->bufferStates[i].seqNo = FIFO_nextSeqNo();
    ////ff->bufferStates[i].lengthInBytes = ff_BUFFER_SIZE_IN_BYTES;
    ////ff->bufferStates[i].linkOwnerCoreNum = 1000;	//effectively disabled to begin with
    ////)
    //ff_bufferStates[i].hL = &(ff_bufferStates[i].dL);
    ////ff_bufferStates[i].mem_ga = (Uint8 *)FIFO_localToGlobalAddress(ff_bufferStates[i].mem, DNUM);
    //ff->bufferStates[i].mem = ff->mem + i * ff_BUFFER_SIZE_IN_BYTES;
    //ff->bufferStates[i].mem_ga = (Uint8 *)FIFO_localToGlobalAddress(ff->bufferStates[i].mem, ff_CORE_ID);
    ff->bufferStates[i].mem = (Uint8 *)FIFO_localToGlobalAddress(ff_mem + i * ff_BUFFER_SIZE_IN_BYTES, ff_CORE_ID);
    //ff->bufferStates[i].status = FIFO_BUFFER_EMPTY;
    ff->bufferStates[i].status = status;
    ff->bufferStates[i].isTCCValid = 0;
  }

  ////DEBUG(printf("ff bufferSizeInBytes: %d\n", ff->bufferSizeInBytes));
  ////DEBUG(printf("ff nBuffersInFifo   : %d\n", ff->nBuffersInFifo));
  ////DEBUG(printf("ff locatedOnCoreId  : %d\n", ff->locatedOnCoreId));
}

