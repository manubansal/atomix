/**
Atomix project, fifoManager.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef __FIFO_MANAGER_H__
#define __FIFO_MANAGER_H__

#include <osl/inc/swpform.h>
#include "fifoFactory.h"
#include "fifoManager_inline.h"
#include "fifoManagerUtil.h"

/*
 * API for the regular and synchronized fifo managers of type F1 or F2. The three
 * kinds of fifos differ only in the fifo manager implementation but not in the API.
 */

enum {
  FIFO_ERR_1 = 1,
  FIFO_ERR_2,
  FIFO_ERR_3,
  FIFO_ERR_4,
  FIFO_ERR_5,
  FIFO_ERR_6,
  FIFO_ERR_7,
  FIFO_ERR_8,
  FIFO_ERR_9,
  FIFO_ERR_10,
  FIFO_ERR_11,
  FIFO_ERR_12,
  FIFO_ERR_13,
  FIFO_ERR_14,
  FIFO_ERR_15,
  FIFO_ERR_16,
  FIFO_ERR_17,
  FIFO_ERR_18,
  FIFO_ERR_19,
  FIFO_ERR_20,
  FIFO_ERR_21,
  FIFO_ERR_22,
  FIFO_ERR_23,
  FIFO_ERR_24,
  FIFO_ERR_25,
  FIFO_ERR_26,
  FIFO_ERR_27,
  FIFO_ERR_28,
  FIFO_ERR_29,
  FIFO_ERR_30,
  FIFO_ERR_31,
  FIFO_ERR_32,
  FIFO_ERR_33,
  FIFO_ERR_34,
  FIFO_ERR_35,

  FIFO_ERR_36,
  FIFO_ERR_37,
  FIFO_ERR_38,
  FIFO_ERR_39,

  FIFO_ERR_40,
  FIFO_ERR_41,

  FIFO_ERR_42,
  FIFO_ERR_43
};


//------------------------------------------------------------------------------------
//the main API functions
//------------------------------------------------------------------------------------
//FIFO_BufferHandle FIFO_getNextReadBuffer(FIFO_Handle handle);
//FIFO_BufferHandle FIFO_getNextWriteBuffer(FIFO_Handle handle);

////FIFO_BufferHandle FIFO_getNextReadBuffer(
////	FIFO_Handle restrict f, 
////	Uint32 const nBuffers,
////	FIFO_BufferState f_b[]
////	);
////
////FIFO_BufferHandle FIFO_getNextWriteBuffer(
////	FIFO_Handle restrict f, 
////	Uint32 const nBuffers,
////	FIFO_BufferState f_b[]
////	);

//FIFO_BufferHandle FIFO_getNextReadBuffer_remote(FIFO_Handle handle);
//FIFO_BufferHandle FIFO_getNextWriteBuffer_remote(FIFO_Handle handle);

////FIFO_BufferHandle FIFO_getNextReadBuffer_remote(
////	FIFO_Handle restrict f, 
////	Uint32 const nBuffers,
////	FIFO_BufferState f_b[]
////	);
////
////FIFO_BufferHandle FIFO_getNextWriteBuffer_remote(
////	FIFO_Handle restrict f, 
////	Uint32 const nBuffers,
////	FIFO_BufferState f_b[]
////	);

//void FIFO_readDone(FIFO_Handle f, FIFO_BufferHandle b);
//void FIFO_writeDone(FIFO_Handle f, FIFO_BufferHandle b);
//void FIFO_readDone_remote(FIFO_Handle f, FIFO_BufferHandle b);
//void FIFO_writeDone_remote(FIFO_Handle f, FIFO_BufferHandle b);

//void FIFO_rereadDone(FIFO_Handle f, FIFO_BufferHandle b);

//------------------------------------------------------------------------------------
//utility API
//------------------------------------------------------------------------------------
//FIFO_Handle FIFO_remoteFifoHandle(FIFO_Handle f, Uint32 remoteCoreId);
Uint32 FIFO_localToGlobalAddress(void *ptr, Uint32 coreId);

//void FIFO_waitF(FIFO_BufferHandle b);
//void FIFO_waitE(FIFO_BufferHandle b);
//void FIFO_waitRTC(FIFO_Handle f, FIFO_BufferHandle b);
//void FIFO_waitWTC(FIFO_Handle f, FIFO_BufferHandle b);
//void FIFO_clearTC(Uint32 linkNum);
//void FIFO_error(Uint32 err);
 


#endif //__FIFO_MANAGER_H__
