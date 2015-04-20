/**
Atomix project, fifoFactory.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef __FIFO_FACTORY_H__ 
#define __FIFO_FACTORY_H__

#include <osl/inc/swpform.h>
#include <oros/sfifos/lib/ipc/ipc_cpy.h>
//#include <orlib/orlib_sfifos/python/inc/fifoBufferTypes.h>

//Note: One buffer is one element of the fifo. A fifo is made up multiple buffers.
//------------------------------------------------------------------------------------
//per-buffer state data structures for s-fifos
//------------------------------------------------------------------------------------
typedef enum {
  FIFO_BUFFER_EMPTY,
  FIFO_BUFFER_BEING_WRITTEN,
  FIFO_BUFFER_FILLED,
  FIFO_BUFFER_BEING_READ
} FIFO_BufferStatus;

typedef enum {
  FIFO_TYPE_SINGULAR = 0,	// F/0 - local queue (not involved in multicore transfers)
  FIFO_TYPE_SRC = 1,		// F/1
  FIFO_TYPE_DST			// F/2
} FIFO_TYPE_SRC_OR_DST;

typedef struct __attribute__((__packed__)) FIFO_BufferState {
  //DEBUG(
  //Uint32 seqNo;
  //Uint32 lengthInBytes;
  //Uint32 linkOwnerCoreNum;
  //)

  //Uint8 *mem;			//local memory address
  //Uint8 *mem_ga;		//global memory address
  //Uint32 filled;		//validity of data (filled (1) == valid, empty (0) == invalid)
  //Uint32 free;		//ownership of buffer (free (1) == unlocked/unowned, busy (0) = locked/owned)
  //IPC_LinkHandle hL;		//handle to the link used to transfer this buffer to its mirror
  //  IPC_LinkDescriptor dL;	//link descriptor
  //Moving out the link descriptors so that they can be allocated one per link in shared memory,
  //as opposed to be allocated twice per link (one in each of the src and dst buffer states). --MB, 11/16/12
  //Moreover, they will be allocated only when links are actually present. For intra-core fifos, links
  //are not allocted.

  Uint8 *mem;			//global memory address
  Uint8  status;		//equivalent to (filled, free) state tuple
  Uint16 linkNum;		//linkNum being currently used to transfer this buffer to/from its mirror
  Uint8  isTCCValid;		//is the transfer-completion code valid to be used for readDone or writeDone deduction?
} FIFO_BufferState; 


typedef FIFO_BufferState * FIFO_BufferHandle;

//------------------------------------------------------------------------------------
//fifo state data structures for all fifos
//------------------------------------------------------------------------------------
typedef struct __attribute__((__packed__)) FIFO_State {
  //DEBUG(
  ////for office use only
  //OUT	Uint32 id;
  //OUT	Uint32 locatedOnCoreId;
  Uint32 bufferSizeInBytes;
  //IN	Uint32 srcOrDst;
  //OUT Uint8 *mem;
  //)

  FIFO_BufferState *bufferStates;		//individual buffer states array
  Uint8 nBuffersInFifo;
  Uint8 idxNextReadBuffer;			//next-in-sequence filled buffer
  Uint8 idxNextWriteBuffer;			//next-in-sequence empty buffer
  Uint8 unused;

  Uint8 anyReadBusyHead;
  Uint8 idxReadBusyHead;
  Uint8 anyWriteBusyHead;
  Uint8 idxWriteBusyHead;
} FIFO_State; 

typedef FIFO_State * FIFO_Handle;

typedef enum {
  FIFO_FAILURE = 0,
  FIFO_SUCCESS = 1
} FIFO_Status;



//FIFO_Status FIFO_createFifo(
//	INOUT FIFO_Handle handle
//	);

//void FIFO_registerWriter(
//	INOUT Handle *handle,
//	);
//

//void FIFO_registerReader(
//	INOUT Handle *handle,
//	);

//deprecated
//void FIFO_setFifoRemoteCoreId(FIFO_Handle f, Uint32 remoteCoreId);

//deprecated
//FIFO_Handle FIFO_getFifoRemoteHandle(FIFO_Handle f);


//deprecated
//FIFO_Handle FIFO_getSrcFifoHandle(FIFO_Handle f);

//deprecated
//FIFO_Handle FIFO_getDstFifoHandle(FIFO_Handle f);

Uint32 FIFO_nextSeqNo();

void FIFO_setSrcOrDstType(FIFO_Handle f, Uint32 srcOrDst);

void FIFO_setupFifo(
    FIFO_Handle ff,
    //FIFO_State *ff_state,
    Uint8 ff_mem[],
    FIFO_BufferState ff_bufferStates[],
    Uint32 ff_N_BUFFERS,
    Uint32 ff_BUFFER_SIZE_IN_BYTES,
    FIFO_TYPE_SRC_OR_DST ff_type,
    Uint32 ff_CORE_ID,
    Uint32 isFilled
    );

//void FIFO_linkFifos(
//    FIFO_Handle ff1,
//    FIFO_Handle ff2,
//    Uint32 transferOwnerCoreId,
//    IPC_LinkDescriptor ld[]
//    );
// 

#endif //__FIFO_FACTORY_H__
