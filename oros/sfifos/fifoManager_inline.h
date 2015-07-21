/**
Atomix project, fifoManager_inline.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef FIFOMANAGER_INLINE_H_
#define FIFOMANAGER_INLINE_H_

#include <osl/inc/swpform.h>
#include "fifoFactory.h"
#include "fifoManager.h"
#include "fifoManagerUtil.h"
#include <oros/sysilib/SYS_TimeStamp.h>

//void FIFO_waitF(FIFO_BufferHandle b);
//void FIFO_waitE(FIFO_BufferHandle b);
void FIFO_waitRTC(FIFO_Handle f, FIFO_BufferHandle b);
void FIFO_waitWTC(FIFO_Handle f, FIFO_BufferHandle b);
void FIFO_clearTC(Uint32 linkNum);
void FIFO_error(Uint32 err);

//#define TS_FIFO_WAIT

static inline void FIFO_flush(
	FIFO_Handle restrict f,
	Uint32 nBuffers,
	FIFO_BufferHandle f_b
	){
	Uint32 i;
	FIFO_BufferHandle b;
	f->idxNextReadBuffer = 0;
	f->idxNextWriteBuffer = 0;
	for (i = 0; i < nBuffers; ++i) {
		 b = &(f_b[i]);
		 if (b->status == FIFO_BUFFER_BEING_WRITTEN) {
		     FIFO_waitWTC(f, b);
		 } else if (b->status == FIFO_BUFFER_BEING_READ) {
			 FIFO_waitRTC(f, b);
		 }
		 b->status = FIFO_BUFFER_EMPTY;
		 b->isTCCValid = 0;
	}
	f->anyReadBusyHead = 0;
	f->idxReadBusyHead = 0;
	f->anyWriteBusyHead = 0;
	f->idxWriteBusyHead = 0;
}



static inline FIFO_BufferHandle FIFO_getNextReadBuffer(
	FIFO_Handle restrict f, 
	Uint32 nBuffers,
	FIFO_BufferHandle f_b
	){

  FIFO_BufferHandle b;
  //Uint32 nBuffers = f->nBuffersInFifo;
  Uint32 idxRead = f->idxNextReadBuffer;

  //b = &(f->bufferStates[idxRead]);
  b = &(f_b[idxRead]);

  //DEBUG(
  //Uint32 localCoreId = f->locatedOnCoreId;	//the core on which this fifo is located
  //Uint32 readerCoreId = DNUM;			//the core that is trying to read
  //ASSERT(f->srcOrDst == FIFO_TYPE_SINGULAR || 
  //	(f->srcOrDst == FIFO_TYPE_SRC) || 
  //      readerCoreId == localCoreId);
  //)
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1115ULL);
#endif

  if (b->status != FIFO_BUFFER_FILLED) {
      FIFO_waitWTC(f, b);
  }
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1116ULL);
#endif
  //At this point, we are ready to dish out the buffer for reading
  //DEBUG(
  //if (b->status != FIFO_BUFFER_FILLED) {
  //  DEBUG(printf("ERROR: buffer still not marked filled after deducing writeDone\n"));
  //  FIFO_error(FIFO_ERR_41);
  //}
  //)

//  idxRead = (idxRead + 1) % nBuffers;
  idxRead++;
  idxRead = idxRead == nBuffers ? 0: idxRead;
  f->idxNextReadBuffer = idxRead;
  b->status = FIFO_BUFFER_BEING_READ;

  return b;
}

static inline FIFO_BufferHandle FIFO_getNextReReadBuffer(
	FIFO_Handle restrict f, 
	Uint32 nBuffers,
	FIFO_BufferHandle f_b
	){

  FIFO_BufferHandle b;
  //Uint32 nBuffers = f->nBuffersInFifo;
  Uint32 idxRead = f->idxNextReadBuffer;

  b = &(f->bufferStates[idxRead]);

  //DEBUG(
  //Uint32 localCoreId = f->locatedOnCoreId;	//the core on which this fifo is located
  //Uint32 readerCoreId = DNUM;			//the core that is trying to read
  //ASSERT(f->srcOrDst == FIFO_TYPE_SINGULAR || 
  //	(f->srcOrDst == FIFO_TYPE_SRC) || 
  //      readerCoreId == localCoreId);
  //)
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1115ULL);
#endif

  if (b->status != FIFO_BUFFER_EMPTY) {
      FIFO_waitWTC(f, b);
  }
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1116ULL);
#endif
  //At this point, we are ready to dish out the buffer for reading
  //DEBUG(
  //if (b->status != FIFO_BUFFER_FILLED) {
  //  DEBUG(printf("ERROR: buffer still not marked filled after deducing writeDone\n"));
  //  FIFO_error(FIFO_ERR_41);
  //}
  //)

//  idxRead = (idxRead + 1) % nBuffers;
  //idxRead++;
  idxRead+=0;
  idxRead = idxRead == nBuffers ? 0: idxRead;
  f->idxNextReadBuffer = idxRead;
  b->status = FIFO_BUFFER_BEING_READ;

  return b;
}

static inline FIFO_BufferHandle FIFO_getNextReReIcBuffer(
	FIFO_Handle restrict f, 
	Uint32 nBuffers,
	FIFO_BufferHandle f_b
	){

  FIFO_BufferHandle b;
  //Uint32 nBuffers = f->nBuffersInFifo;
  Uint32 idxRead = f->idxNextReadBuffer;

  b = &(f->bufferStates[idxRead]);

  //DEBUG(
  //Uint32 localCoreId = f->locatedOnCoreId;	//the core on which this fifo is located
  //Uint32 readerCoreId = DNUM;			//the core that is trying to read
  //ASSERT(f->srcOrDst == FIFO_TYPE_SINGULAR || 
  //	(f->srcOrDst == FIFO_TYPE_SRC) || 
  //      readerCoreId == localCoreId);
  //)
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1115ULL);
#endif

  if (b->status != FIFO_BUFFER_FILLED) {
      FIFO_waitWTC(f, b);
  }
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1116ULL);
#endif
  //At this point, we are ready to dish out the buffer for reading
  //DEBUG(
  //if (b->status != FIFO_BUFFER_FILLED) {
  //  DEBUG(printf("ERROR: buffer still not marked filled after deducing writeDone\n"));
  //  FIFO_error(FIFO_ERR_41);
  //}
  //)

//  idxRead = (idxRead + 1) % nBuffers;
  idxRead++;
  idxRead = idxRead == nBuffers ? 0: idxRead;
  f->idxNextReadBuffer = idxRead;
  b->status = FIFO_BUFFER_BEING_READ;

  return b;
}



static inline FIFO_BufferHandle FIFO_getNextReadBuffer_remote(
	FIFO_Handle restrict f, 
	//Uint32 f_nBuffersInFifo, 
	Uint32 nBuffers,
	FIFO_BufferHandle f_b
	){

  FIFO_BufferHandle b;
  //Uint32 nBuffers = f->nBuffersInFifo;
  Uint32 idxRead = f->idxNextReadBuffer;

  b = &(f->bufferStates[idxRead]);

  //DEBUG(
  //Uint32 localCoreId = f->locatedOnCoreId;	//the core on which this fifo is located
  //Uint32 readerCoreId = DNUM;			//the core that is trying to read
  //ASSERT(f->srcOrDst == FIFO_TYPE_SINGULAR || 
  //	(f->srcOrDst == FIFO_TYPE_SRC) || 
  //      readerCoreId == localCoreId);
  //)

#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1117ULL);
#endif
  if (b->status != FIFO_BUFFER_FILLED) {
      FIFO_waitWTC(f, b);
  }
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1118ULL);
#endif
  //At this point, we are ready to dish out the buffer for reading
  //DEBUG(
  //if (b->status != FIFO_BUFFER_FILLED) {
  //  DEBUG(printf("ERROR: buffer still not marked filled after deducing writeDone\n"));
  //  FIFO_error(FIFO_ERR_41);
  //}
  //)

//  idxRead = (idxRead + 1) % nBuffers;
  idxRead++;
  idxRead = idxRead == nBuffers ? 0: idxRead;
  f->idxNextReadBuffer = idxRead;
  b->status = FIFO_BUFFER_BEING_READ;

  return b;
}

static inline FIFO_BufferHandle FIFO_getNextReReadBuffer_remote(
	FIFO_Handle restrict f, 
	//Uint32 f_nBuffersInFifo, 
	Uint32 nBuffers,
	FIFO_BufferHandle f_b
	){

  FIFO_BufferHandle b;
  //Uint32 nBuffers = f->nBuffersInFifo;
  Uint32 idxRead = f->idxNextReadBuffer;

  b = &(f->bufferStates[idxRead]);

  //DEBUG(
  //Uint32 localCoreId = f->locatedOnCoreId;	//the core on which this fifo is located
  //Uint32 readerCoreId = DNUM;			//the core that is trying to read
  //ASSERT(f->srcOrDst == FIFO_TYPE_SINGULAR || 
  //	(f->srcOrDst == FIFO_TYPE_SRC) || 
  //      readerCoreId == localCoreId);
  //)

#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1117ULL);
#endif
  if (b->status != FIFO_BUFFER_FILLED) {
      FIFO_waitWTC(f, b);
  }
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1118ULL);
#endif
  //At this point, we are ready to dish out the buffer for reading
  //DEBUG(
  //if (b->status != FIFO_BUFFER_FILLED) {
  //  DEBUG(printf("ERROR: buffer still not marked filled after deducing writeDone\n"));
  //  FIFO_error(FIFO_ERR_41);
  //}
  //)

//  idxRead = (idxRead + 1) % nBuffers;
  idxRead+=0;
  idxRead = idxRead == nBuffers ? 0: idxRead;
  f->idxNextReadBuffer = idxRead;
  b->status = FIFO_BUFFER_BEING_READ;

  return b;
}

static inline FIFO_BufferHandle FIFO_getNextReReIcBuffer_remote(
	FIFO_Handle restrict f, 
	//Uint32 f_nBuffersInFifo, 
	Uint32 nBuffers,
	FIFO_BufferHandle f_b
	){

  FIFO_BufferHandle b;
  //Uint32 nBuffers = f->nBuffersInFifo;
  Uint32 idxRead = f->idxNextReadBuffer;

  b = &(f->bufferStates[idxRead]);

  //DEBUG(
  //Uint32 localCoreId = f->locatedOnCoreId;	//the core on which this fifo is located
  //Uint32 readerCoreId = DNUM;			//the core that is trying to read
  //ASSERT(f->srcOrDst == FIFO_TYPE_SINGULAR || 
  //	(f->srcOrDst == FIFO_TYPE_SRC) || 
  //      readerCoreId == localCoreId);
  //)

#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1117ULL);
#endif
  if (b->status != FIFO_BUFFER_FILLED) {
      FIFO_waitWTC(f, b);
  }
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1118ULL);
#endif
  //At this point, we are ready to dish out the buffer for reading
  //DEBUG(
  //if (b->status != FIFO_BUFFER_FILLED) {
  //  DEBUG(printf("ERROR: buffer still not marked filled after deducing writeDone\n"));
  //  FIFO_error(FIFO_ERR_41);
  //}
  //)

//  idxRead = (idxRead + 1) % nBuffers;
  idxRead+=1;
  idxRead = idxRead == nBuffers ? 0: idxRead;
  f->idxNextReadBuffer = idxRead;
  b->status = FIFO_BUFFER_BEING_READ;

  return b;
}


static inline FIFO_BufferHandle FIFO_getNextWriteBuffer(
	FIFO_Handle f, 
	Uint32 const nBuffers,
	FIFO_BufferState f_b[]
	){

  FIFO_BufferHandle b;
  //Uint32 nBuffers = f->nBuffersInFifo;
  Uint32 idxWrite;
  //MEASURE_RUN_TIME(
  idxWrite = f->idxNextWriteBuffer;
  //)

  //MEASURE_RUN_TIME(
  //b = &(f->bufferStates[idxWrite]);
  b = &(f_b[idxWrite]);
  //)

  //DEBUG(
  //Uint32 writerCoreId = DNUM;			//the core that is trying to write
  //Uint32 localCoreId = f->locatedOnCoreId;	//the core on which this fifo is located
  //ASSERT(f->srcOrDst == FIFO_TYPE_SINGULAR ||
  //	(f->srcOrDst == FIFO_TYPE_DST) ||
  //      writerCoreId == localCoreId);
  //)

#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1111ULL);
#endif
  //MEASURE_RUN_TIME(
  if (b->status != FIFO_BUFFER_EMPTY) {
      FIFO_waitRTC(f, b);
  }
  //)
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1112ULL);
#endif
  //DEBUG(
  //if (b->status != FIFO_BUFFER_EMPTY) {
  //  DEBUG(printf("ERROR: buffer still not marked empty after deducing readDone\n"));
  //  //exit(1);
  //  FIFO_error(FIFO_ERR_40);
  //}
  ////printf("Write buffer available\n");
  //)

//  idxWrite = (idxWrite + 1) % nBuffers;
  //MEASURE_RUN_TIME(
  idxWrite++;
  idxWrite = idxWrite == nBuffers ? 0: idxWrite;
  //)

  //MEASURE_RUN_TIME(
  f->idxNextWriteBuffer = idxWrite;
  //)
  //MEASURE_RUN_TIME(
  b->status = FIFO_BUFFER_BEING_WRITTEN;
  //)

  return b;
}


static inline FIFO_BufferHandle FIFO_getNextReWriteBuffer(
	FIFO_Handle f, 
	Uint32 const nBuffers,
	FIFO_BufferState f_b[]
	){

  FIFO_BufferHandle b;
  //Uint32 nBuffers = f->nBuffersInFifo;
  Uint32 idxWrite;
  //MEASURE_RUN_TIME(
  idxWrite = f->idxNextWriteBuffer;
  //)

  //MEASURE_RUN_TIME(
  //b = &(f->bufferStates[idxWrite]);
  b = &(f_b[idxWrite]);
  //)

  //DEBUG(
  //Uint32 writerCoreId = DNUM;			//the core that is trying to write
  //Uint32 localCoreId = f->locatedOnCoreId;	//the core on which this fifo is located
  //ASSERT(f->srcOrDst == FIFO_TYPE_SINGULAR ||
  //	(f->srcOrDst == FIFO_TYPE_DST) ||
  //      writerCoreId == localCoreId);
  //)

#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1119ULL);
#endif
  //MEASURE_RUN_TIME(
  if (b->status != FIFO_BUFFER_EMPTY) {
      FIFO_waitRTC(f, b);
  }
  //)
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1120ULL);
#endif
  //DEBUG(
  //if (b->status != FIFO_BUFFER_EMPTY) {
  //  DEBUG(printf("ERROR: buffer still not marked empty after deducing readDone\n"));
  //  //exit(1);
  //  FIFO_error(FIFO_ERR_40);
  //}
  ////printf("Write buffer available\n");
  //)

//  idxWrite = (idxWrite + 1) % nBuffers;
  //MEASURE_RUN_TIME(
  //idxWrite++;
  idxWrite+=0;
  idxWrite = idxWrite == nBuffers ? 0: idxWrite;
  //)

  //MEASURE_RUN_TIME(
  f->idxNextWriteBuffer = idxWrite;
  //)
  //MEASURE_RUN_TIME(
  b->status = FIFO_BUFFER_BEING_WRITTEN;
  //)

  return b;
}


static inline FIFO_BufferHandle FIFO_getNextReWrIcBuffer(
	FIFO_Handle f, 
	Uint32 const nBuffers,
	FIFO_BufferState f_b[]
	){

  FIFO_BufferHandle b;
  //Uint32 nBuffers = f->nBuffersInFifo;
  Uint32 idxWrite;
  //MEASURE_RUN_TIME(
  idxWrite = f->idxNextWriteBuffer;
  //)

  //MEASURE_RUN_TIME(
  //b = &(f->bufferStates[idxWrite]);
  b = &(f_b[idxWrite]);
  //)

  //DEBUG(
  //Uint32 writerCoreId = DNUM;			//the core that is trying to write
  //Uint32 localCoreId = f->locatedOnCoreId;	//the core on which this fifo is located
  //ASSERT(f->srcOrDst == FIFO_TYPE_SINGULAR ||
  //	(f->srcOrDst == FIFO_TYPE_DST) ||
  //      writerCoreId == localCoreId);
  //)

#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1119ULL);
#endif
  //MEASURE_RUN_TIME(
  if (b->status != FIFO_BUFFER_EMPTY) {
      FIFO_waitRTC(f, b);
  }
  //)
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1120ULL);
#endif
  //DEBUG(
  //if (b->status != FIFO_BUFFER_EMPTY) {
  //  DEBUG(printf("ERROR: buffer still not marked empty after deducing readDone\n"));
  //  //exit(1);
  //  FIFO_error(FIFO_ERR_40);
  //}
  ////printf("Write buffer available\n");
  //)

//  idxWrite = (idxWrite + 1) % nBuffers;
  //MEASURE_RUN_TIME(
  idxWrite++;
  idxWrite = idxWrite == nBuffers ? 0: idxWrite;
  //)

  //MEASURE_RUN_TIME(
  f->idxNextWriteBuffer = idxWrite;
  //)
  //MEASURE_RUN_TIME(
  b->status = FIFO_BUFFER_BEING_WRITTEN;
  //)

  return b;
}



static inline FIFO_BufferHandle FIFO_getNextWriteBuffer_remote(
	FIFO_Handle f, 
	Uint32 const nBuffers,
	FIFO_BufferState f_b[]
	){

  FIFO_BufferHandle b;
  //Uint32 nBuffers = f->nBuffersInFifo;
  Uint32 idxWrite;
  //MEASURE_RUN_TIME(
  idxWrite = f->idxNextWriteBuffer;
  //)

  //MEASURE_RUN_TIME(
  //b = &(f->bufferStates[idxWrite]);
  b = &(f_b[idxWrite]);
  //)

  //DEBUG(
  //Uint32 writerCoreId = DNUM;			//the core that is trying to write
  //Uint32 localCoreId = f->locatedOnCoreId;	//the core on which this fifo is located
  //ASSERT(f->srcOrDst == FIFO_TYPE_SINGULAR ||
  //	(f->srcOrDst == FIFO_TYPE_DST) ||
  //      writerCoreId == localCoreId);
  //)

#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1113ULL);
#endif
  //MEASURE_RUN_TIME(
  if (b->status != FIFO_BUFFER_EMPTY) {
      FIFO_waitRTC(f, b);
  }
  //)
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1114ULL);
#endif
  //DEBUG(
  //if (b->status != FIFO_BUFFER_EMPTY) {
  //  DEBUG(printf("ERROR: buffer still not marked empty after deducing readDone\n"));
  //  //exit(1);
  //  FIFO_error(FIFO_ERR_40);
  //}
  ////printf("Write buffer available\n");
  //)

//  idxWrite = (idxWrite + 1) % nBuffers;
  //MEASURE_RUN_TIME(
  idxWrite++;
  idxWrite = idxWrite == nBuffers ? 0: idxWrite;
  //)

  //MEASURE_RUN_TIME(
  f->idxNextWriteBuffer = idxWrite;
  //)
  //MEASURE_RUN_TIME(
  b->status = FIFO_BUFFER_BEING_WRITTEN;
  //)

  return b;
}

static inline FIFO_BufferHandle FIFO_getNextReWriteBuffer_remote(
	FIFO_Handle f, 
	Uint32 const nBuffers,
	FIFO_BufferState f_b[]
	){

  FIFO_BufferHandle b;
  //Uint32 nBuffers = f->nBuffersInFifo;
  Uint32 idxWrite;
  //MEASURE_RUN_TIME(
  idxWrite = f->idxNextWriteBuffer;
  //)

  //MEASURE_RUN_TIME(
  //b = &(f->bufferStates[idxWrite]);
  b = &(f_b[idxWrite]);
  //)

  //DEBUG(
  //Uint32 writerCoreId = DNUM;			//the core that is trying to write
  //Uint32 localCoreId = f->locatedOnCoreId;	//the core on which this fifo is located
  //ASSERT(f->srcOrDst == FIFO_TYPE_SINGULAR ||
  //	(f->srcOrDst == FIFO_TYPE_DST) ||
  //      writerCoreId == localCoreId);
  //)

#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1121ULL);
#endif
  //MEASURE_RUN_TIME(
  if (b->status != FIFO_BUFFER_EMPTY) {
      FIFO_waitRTC(f, b);
  }
  //)
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1122ULL);
#endif
  //DEBUG(
  //if (b->status != FIFO_BUFFER_EMPTY) {
  //  DEBUG(printf("ERROR: buffer still not marked empty after deducing readDone\n"));
  //  //exit(1);
  //  FIFO_error(FIFO_ERR_40);
  //}
  ////printf("Write buffer available\n");
  //)

//  idxWrite = (idxWrite + 1) % nBuffers;
  //MEASURE_RUN_TIME(
  //idxWrite++;
  idxWrite+=0;
  idxWrite = idxWrite == nBuffers ? 0: idxWrite;
  //)

  //MEASURE_RUN_TIME(
  f->idxNextWriteBuffer = idxWrite;
  //)
  //MEASURE_RUN_TIME(
  b->status = FIFO_BUFFER_BEING_WRITTEN;
  //)

  return b;
}


static inline FIFO_BufferHandle FIFO_getNextReWrIcBuffer_remote(
	FIFO_Handle f, 
	Uint32 const nBuffers,
	FIFO_BufferState f_b[]
	){

  FIFO_BufferHandle b;
  //Uint32 nBuffers = f->nBuffersInFifo;
  Uint32 idxWrite;
  //MEASURE_RUN_TIME(
  idxWrite = f->idxNextWriteBuffer;
  //)

  //MEASURE_RUN_TIME(
  //b = &(f->bufferStates[idxWrite]);
  b = &(f_b[idxWrite]);
  //)

  //DEBUG(
  //Uint32 writerCoreId = DNUM;			//the core that is trying to write
  //Uint32 localCoreId = f->locatedOnCoreId;	//the core on which this fifo is located
  //ASSERT(f->srcOrDst == FIFO_TYPE_SINGULAR ||
  //	(f->srcOrDst == FIFO_TYPE_DST) ||
  //      writerCoreId == localCoreId);
  //)

#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1121ULL);
#endif
  //MEASURE_RUN_TIME(
  if (b->status != FIFO_BUFFER_EMPTY) {
      FIFO_waitRTC(f, b);
  }
  //)
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1122ULL);
#endif
  //DEBUG(
  //if (b->status != FIFO_BUFFER_EMPTY) {
  //  DEBUG(printf("ERROR: buffer still not marked empty after deducing readDone\n"));
  //  //exit(1);
  //  FIFO_error(FIFO_ERR_40);
  //}
  ////printf("Write buffer available\n");
  //)

//  idxWrite = (idxWrite + 1) % nBuffers;
  //MEASURE_RUN_TIME(
  idxWrite++;
  idxWrite = idxWrite == nBuffers ? 0: idxWrite;
  //)

  //MEASURE_RUN_TIME(
  f->idxNextWriteBuffer = idxWrite;
  //)
  //MEASURE_RUN_TIME(
  b->status = FIFO_BUFFER_BEING_WRITTEN;
  //)

  return b;
}




static inline void FIFO_readDone(FIFO_Handle f, FIFO_BufferHandle b) {
  //MEASURE_RUN_TIME(
  //DEBUG(printf("FIFO_readDone\n"));
  //ASSERT(b->status == FIFO_BUFFER_BEING_READ);
  //Modifying this since we might declare read done on an already empty buffer 
  //due to bunching of the transfer completion case with the synchronous case.
  ASSERT(b->status == FIFO_BUFFER_BEING_READ || b->status == FIFO_BUFFER_EMPTY);
  b->status = FIFO_BUFFER_EMPTY;
  //);
}

static inline void FIFO_reReadDone(FIFO_Handle f, FIFO_BufferHandle b) {
  //DEBUG_ALL(printf("FIFO_rereadDone\n"));
  //ASSERT(b->status == FIFO_BUFFER_BEING_READ);
  //Modifying this since we might declare read done on an already empty buffer 
  //due to bunching of the transfer completion case with the synchronous case.
  //ASSERT(b->status == FIFO_BUFFER_BEING_READ);
  ASSERT(b->status == FIFO_BUFFER_BEING_READ || b->status == FIFO_BUFFER_EMPTY);
  //b->status = FIFO_BUFFER_EMPTY;
  b->status = FIFO_BUFFER_FILLED;
}

static inline void FIFO_readDone_remote(FIFO_Handle f, FIFO_BufferHandle b) {
  //MEASURE_RUN_TIME(
  //DEBUG(printf("FIFO_readDone\n"));
  //ASSERT(b->status == FIFO_BUFFER_BEING_READ);
  //Modifying this since we might declare read done on an already empty buffer 
  //due to bunching of the transfer completion case with the synchronous case.
  ASSERT(b->status == FIFO_BUFFER_BEING_READ || b->status == FIFO_BUFFER_EMPTY);
  b->status = FIFO_BUFFER_EMPTY;
  //);
}


static inline void FIFO_reReadDone_remote(FIFO_Handle f, FIFO_BufferHandle b) {
  //MEASURE_RUN_TIME(
  //DEBUG(printf("FIFO_readDone\n"));
  //ASSERT(b->status == FIFO_BUFFER_BEING_READ);
  //Modifying this since we might declare read done on an already empty buffer 
  //due to bunching of the transfer completion case with the synchronous case.
  ASSERT(b->status == FIFO_BUFFER_BEING_READ || b->status == FIFO_BUFFER_EMPTY);
  b->status = FIFO_BUFFER_FILLED;
  //);
}


static inline void FIFO_writeDone(FIFO_Handle f, FIFO_BufferHandle b) {
  //MEASURE_RUN_TIME(
  //DEBUG(printf("FIFO_writeDone\n"));
  //ASSERT(b->status == FIFO_BUFFER_BEING_WRITTEN);
  //Modifying this since we might declare write done on an already filled buffer 
  //due to bunching of the transfer completion case with the synchronous case.
  ASSERT(b->status == FIFO_BUFFER_BEING_WRITTEN || b->status == FIFO_BUFFER_FILLED);
  b->status = FIFO_BUFFER_FILLED;
  //);
}

static inline void FIFO_reWriteDone(FIFO_Handle f, FIFO_BufferHandle b) {
  //MEASURE_RUN_TIME(
  //DEBUG(printf("FIFO_writeDone\n"));
  //ASSERT(b->status == FIFO_BUFFER_BEING_WRITTEN);
  //Modifying this since we might declare write done on an already filled buffer 
  //due to bunching of the transfer completion case with the synchronous case.
  ASSERT(b->status == FIFO_BUFFER_BEING_WRITTEN || b->status == FIFO_BUFFER_FILLED);
  b->status = FIFO_BUFFER_EMPTY;
  //);
}


static inline void FIFO_writeDone_remote(FIFO_Handle f, FIFO_BufferHandle b) {
  //MEASURE_RUN_TIME(
  //DEBUG(printf("FIFO_writeDone\n"));
  //ASSERT(b->status == FIFO_BUFFER_BEING_WRITTEN);
  //Modifying this since we might declare write done on an already filled buffer 
  //due to bunching of the transfer completion case with the synchronous case.
  ASSERT(b->status == FIFO_BUFFER_BEING_WRITTEN || b->status == FIFO_BUFFER_FILLED);
  b->status = FIFO_BUFFER_FILLED;
  //);
}

static inline void FIFO_reWriteDone_remote(FIFO_Handle f, FIFO_BufferHandle b) {
  //MEASURE_RUN_TIME(
  //DEBUG(printf("FIFO_writeDone\n"));
  //ASSERT(b->status == FIFO_BUFFER_BEING_WRITTEN);
  //Modifying this since we might declare write done on an already filled buffer 
  //due to bunching of the transfer completion case with the synchronous case.
  ASSERT(b->status == FIFO_BUFFER_BEING_WRITTEN || b->status == FIFO_BUFFER_FILLED);
  b->status = FIFO_BUFFER_EMPTY;
  //);
}



/******************************************
 * Asynchronous read/write API
 ******************************************/
static inline FIFO_BufferHandle FIFO_getNextAsyncReadBuffer(
	FIFO_Handle restrict f, 
	Uint32 nBuffers,
	FIFO_BufferHandle f_b
	){

  FIFO_BufferHandle b;
  Uint32 idxRead = f->idxNextReadBuffer;

  b = &(f->bufferStates[idxRead]);

#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1121ULL);
#endif

  if (b->status != FIFO_BUFFER_FILLED) {
      FIFO_waitWTC(f, b);
  }
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1122ULL);
#endif
  idxRead+=0;
  idxRead = idxRead == nBuffers ? 0: idxRead;
  f->idxNextReadBuffer = idxRead;
  b->status = FIFO_BUFFER_BEING_READ;

  return b;
}

static inline FIFO_BufferHandle FIFO_getNextAsyncReIcBuffer(
	FIFO_Handle restrict f, 
	Uint32 nBuffers,
	FIFO_BufferHandle f_b
	){

  FIFO_BufferHandle b;
  Uint32 idxRead = f->idxNextReadBuffer;

  Uint32 anyReadBusyHead = f->anyReadBusyHead;
  Uint32 idxReadBusyHead = f->idxReadBusyHead;
  idxReadBusyHead = anyReadBusyHead ? idxReadBusyHead : idxRead;
  anyReadBusyHead = 1;
  f->anyReadBusyHead = anyReadBusyHead;
  f->idxReadBusyHead = idxReadBusyHead;

  b = &(f->bufferStates[idxRead]);

#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1121ULL);
#endif

  if (b->status != FIFO_BUFFER_FILLED) {
      FIFO_waitWTC(f, b);
  }
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1122ULL);
#endif
  idxRead+=1;
  idxRead = idxRead == nBuffers ? 0: idxRead;
  f->idxNextReadBuffer = idxRead;

  b->status = FIFO_BUFFER_BEING_READ;

  return b;
}


static inline FIFO_BufferHandle FIFO_getNextAsyncWriteBuffer(
	FIFO_Handle f, 
	Uint32 const nBuffers,
	FIFO_BufferState f_b[]
	){

  FIFO_BufferHandle b;
  Uint32 idxWrite;
  idxWrite = f->idxNextWriteBuffer;

  b = &(f_b[idxWrite]);

#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1123ULL);
#endif
  if (b->status != FIFO_BUFFER_EMPTY) {
      FIFO_waitRTC(f, b);
  }
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1124ULL);
#endif
  idxWrite+=0;
  idxWrite = idxWrite == nBuffers ? 0: idxWrite;
  f->idxNextWriteBuffer = idxWrite;
  b->status = FIFO_BUFFER_BEING_WRITTEN;

  return b;
}

static inline FIFO_BufferHandle FIFO_getNextAsyncWriIcBuffer(
	FIFO_Handle f, 
	Uint32 const nBuffers,
	FIFO_BufferState f_b[]
	){

  FIFO_BufferHandle b;
  Uint32 idxWrite = f->idxNextWriteBuffer;

  Uint32 anyWriteBusyHead = f->anyWriteBusyHead;
  Uint32 idxWriteBusyHead = f->idxWriteBusyHead;
  idxWriteBusyHead = anyWriteBusyHead ? idxWriteBusyHead : idxWrite;
  anyWriteBusyHead = 1;
  f->anyWriteBusyHead = anyWriteBusyHead;
  f->idxWriteBusyHead = idxWriteBusyHead;

  b = &(f_b[idxWrite]);

#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1123ULL);
#endif
  if (b->status != FIFO_BUFFER_EMPTY) {
      FIFO_waitRTC(f, b);
  }
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1124ULL);
#endif
  idxWrite+=1;
  idxWrite = idxWrite == nBuffers ? 0: idxWrite;
  f->idxNextWriteBuffer = idxWrite;

  b->status = FIFO_BUFFER_BEING_WRITTEN;

  return b;
}


static inline FIFO_BufferHandle FIFO_getNextAsyncReadBusyBuffer(
	FIFO_Handle restrict f, 
	Uint32 nBuffers,
	FIFO_BufferHandle f_b
	){

  FIFO_BufferHandle b;
  Uint32 idxRead = f->idxNextReadBuffer;

  b = &(f->bufferStates[idxRead]);

#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1125ULL);
#endif
  //if (b->status != FIFO_BUFFER_BEING_READ) {
  //    FIFO_error(FIFO_ERR_42);
  //}
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1126ULL);
#endif
  idxRead+=1;
  idxRead = idxRead == nBuffers ? 0: idxRead;
  f->idxNextReadBuffer = idxRead;
  //b->status = FIFO_BUFFER_BEING_READ;

  return b;
}

static inline FIFO_BufferHandle FIFO_getNextAsyncReIcBusyBuffer(
	FIFO_Handle restrict f, 
	Uint32 nBuffers,
	FIFO_BufferHandle f_b
	){

  FIFO_BufferHandle b;
  Uint32 anyReadBusyHead = f->anyReadBusyHead;
  Uint32 idxReadBusyHead = f->idxReadBusyHead;
  Uint32 idxRead = f->idxNextReadBuffer;


  b = &(f->bufferStates[idxReadBusyHead]);

#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1125ULL);
#endif
  //if (b->status != FIFO_BUFFER_BEING_READ) {
  //    FIFO_error(FIFO_ERR_42);
  //}
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1126ULL);
#endif
  idxReadBusyHead+=1;
  idxReadBusyHead = idxReadBusyHead == nBuffers ? 0: idxReadBusyHead;

  anyReadBusyHead = idxReadBusyHead == idxRead ? 0 : anyReadBusyHead;

  f->anyReadBusyHead = anyReadBusyHead;
  f->idxReadBusyHead = idxReadBusyHead;

  //b->status = FIFO_BUFFER_BEING_READ;

  return b;
}



static inline FIFO_BufferHandle FIFO_getNextAsyncWriteBusyBuffer(
	FIFO_Handle f, 
	Uint32 const nBuffers,
	FIFO_BufferState f_b[]
	){

  FIFO_BufferHandle b;
  Uint32 idxWrite;
  idxWrite = f->idxNextWriteBuffer;

  b = &(f_b[idxWrite]);

#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1127ULL);
#endif
  //if (b->status != FIFO_BUFFER_BEING_WRITTEN) {
  //    FIFO_error(FIFO_ERR_43);
  //}
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1128ULL);
#endif
  idxWrite+=1;
  idxWrite = idxWrite == nBuffers ? 0: idxWrite;
  f->idxNextWriteBuffer = idxWrite;
  //b->status = FIFO_BUFFER_BEING_WRITTEN;

  return b;
}


static inline FIFO_BufferHandle FIFO_getNextAsyncWriIcBusyBuffer(
	FIFO_Handle f, 
	Uint32 const nBuffers,
	FIFO_BufferState f_b[]
	){

  FIFO_BufferHandle b;
  Uint32 anyWriteBusyHead = f->anyWriteBusyHead;
  Uint32 idxWriteBusyHead = f->idxWriteBusyHead;
  Uint32 idxWrite = f->idxNextWriteBuffer;

  b = &(f_b[idxWriteBusyHead]);

#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1127ULL);
#endif
  //if (b->status != FIFO_BUFFER_BEING_WRITTEN) {
  //    FIFO_error(FIFO_ERR_43);
  //}
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1128ULL);
#endif
  idxWriteBusyHead+=1;
  idxWriteBusyHead = idxWriteBusyHead == nBuffers ? 0: idxWriteBusyHead;

  anyWriteBusyHead = idxWriteBusyHead == idxWrite ? 0 : anyWriteBusyHead;

  f->anyWriteBusyHead = anyWriteBusyHead;
  f->idxWriteBusyHead = idxWriteBusyHead;

  //b->status = FIFO_BUFFER_BEING_WRITTEN;

  return b;
}

static inline FIFO_BufferHandle FIFO_getNextAsyncReadBuffer_remote(
	FIFO_Handle restrict f, 
	Uint32 nBuffers,
	FIFO_BufferHandle f_b
	){

  FIFO_BufferHandle b;
  Uint32 idxRead = f->idxNextReadBuffer;

  b = &(f->bufferStates[idxRead]);

#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1131ULL);
#endif
  if (b->status != FIFO_BUFFER_FILLED) {
      FIFO_waitWTC(f, b);
  }
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1132ULL);
#endif
  idxRead+=0;
  idxRead = idxRead == nBuffers ? 0: idxRead;
  f->idxNextReadBuffer = idxRead;
  b->status = FIFO_BUFFER_BEING_READ;

  return b;
}


static inline FIFO_BufferHandle FIFO_getNextAsyncReIcBuffer_remote(
	FIFO_Handle restrict f, 
	Uint32 nBuffers,
	FIFO_BufferHandle f_b
	){

  FIFO_BufferHandle b;
  Uint32 idxRead = f->idxNextReadBuffer;

  Uint32 anyReadBusyHead = f->anyReadBusyHead;
  Uint32 idxReadBusyHead = f->idxReadBusyHead;
  idxReadBusyHead = anyReadBusyHead ? idxReadBusyHead : idxRead;
  anyReadBusyHead = 1;
  f->anyReadBusyHead = anyReadBusyHead;
  f->idxReadBusyHead = idxReadBusyHead;

  b = &(f->bufferStates[idxRead]);

#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1131ULL);
#endif
  if (b->status != FIFO_BUFFER_FILLED) {
      FIFO_waitWTC(f, b);
  }
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1132ULL);
#endif
  idxRead+=1;
  idxRead = idxRead == nBuffers ? 0: idxRead;
  f->idxNextReadBuffer = idxRead;

  b->status = FIFO_BUFFER_BEING_READ;

  return b;
}

static inline FIFO_BufferHandle FIFO_getNextAsyncWriteBuffer_remote(
	FIFO_Handle f, 
	Uint32 const nBuffers,
	FIFO_BufferState f_b[]
	){

  FIFO_BufferHandle b;
  Uint32 idxWrite;
  idxWrite = f->idxNextWriteBuffer;

  b = &(f_b[idxWrite]);

#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1133ULL);
#endif
  if (b->status != FIFO_BUFFER_EMPTY) {
      FIFO_waitRTC(f, b);
  }
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1134ULL);
#endif
  idxWrite+=0;
  idxWrite = idxWrite == nBuffers ? 0: idxWrite;
  f->idxNextWriteBuffer = idxWrite;
  b->status = FIFO_BUFFER_BEING_WRITTEN;

  return b;
}

static inline FIFO_BufferHandle FIFO_getNextAsyncWriIcBuffer_remote(
	FIFO_Handle f, 
	Uint32 const nBuffers,
	FIFO_BufferState f_b[]
	){

  FIFO_BufferHandle b;
  Uint32 idxWrite = f->idxNextWriteBuffer;

  Uint32 anyWriteBusyHead = f->anyWriteBusyHead;
  Uint32 idxWriteBusyHead = f->idxWriteBusyHead;
  idxWriteBusyHead = anyWriteBusyHead ? idxWriteBusyHead : idxWrite;
  anyWriteBusyHead = 1;
  f->anyWriteBusyHead = anyWriteBusyHead;
  f->idxWriteBusyHead = idxWriteBusyHead;

  b = &(f_b[idxWrite]);

#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1133ULL);
#endif
  if (b->status != FIFO_BUFFER_EMPTY) {
      FIFO_waitRTC(f, b);
  }
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1134ULL);
#endif
  idxWrite+=1;
  idxWrite = idxWrite == nBuffers ? 0: idxWrite;
  f->idxNextWriteBuffer = idxWrite;

  b->status = FIFO_BUFFER_BEING_WRITTEN;

  return b;
}

static inline FIFO_BufferHandle FIFO_getNextAsyncWriIcBusyBuffer_remote(
	FIFO_Handle f, 
	Uint32 const nBuffers,
	FIFO_BufferState f_b[]
	){

  FIFO_BufferHandle b;
  Uint32 anyWriteBusyHead = f->anyWriteBusyHead;
  Uint32 idxWriteBusyHead = f->idxWriteBusyHead;
  Uint32 idxWrite = f->idxNextWriteBuffer;

  b = &(f_b[idxWriteBusyHead]);

#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1127ULL);
#endif
  //if (b->status != FIFO_BUFFER_BEING_WRITTEN) {
  //    FIFO_error(FIFO_ERR_43);
  //}
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1128ULL);
#endif
  idxWriteBusyHead+=1;
  idxWriteBusyHead = idxWriteBusyHead == nBuffers ? 0: idxWriteBusyHead;

  anyWriteBusyHead = idxWriteBusyHead == idxWrite ? 0 : anyWriteBusyHead;

  f->anyWriteBusyHead = anyWriteBusyHead;
  f->idxWriteBusyHead = idxWriteBusyHead;

  //b->status = FIFO_BUFFER_BEING_WRITTEN;

  return b;
}



static inline FIFO_BufferHandle FIFO_getNextAsyncReadBusyBuffer_remote(
	FIFO_Handle restrict f, 
	Uint32 nBuffers,
	FIFO_BufferHandle f_b
	){

  FIFO_BufferHandle b;
  Uint32 idxRead = f->idxNextReadBuffer;

  b = &(f->bufferStates[idxRead]);

#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1135ULL);
#endif
  //if (b->status != FIFO_BUFFER_BEING_READ) {
  //  FIFO_error(FIFO_ERR_42);
  //}
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1136ULL);
#endif
  idxRead+=1;
  idxRead = idxRead == nBuffers ? 0: idxRead;
  f->idxNextReadBuffer = idxRead;
  b->status = FIFO_BUFFER_BEING_READ;

  return b;
}

static inline FIFO_BufferHandle FIFO_getNextAsyncReIcBusyBuffer_remote(
	FIFO_Handle restrict f, 
	Uint32 nBuffers,
	FIFO_BufferHandle f_b
	){

  FIFO_BufferHandle b;
  Uint32 anyReadBusyHead = f->anyReadBusyHead;
  Uint32 idxReadBusyHead = f->idxReadBusyHead;
  Uint32 idxRead = f->idxNextReadBuffer;

  b = &(f->bufferStates[idxReadBusyHead]);

#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1135ULL);
#endif
  //if (b->status != FIFO_BUFFER_BEING_READ) {
  //  FIFO_error(FIFO_ERR_42);
  //}
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1136ULL);
#endif
  idxReadBusyHead+=1;
  idxReadBusyHead = idxReadBusyHead == nBuffers ? 0: idxReadBusyHead;

  anyReadBusyHead = idxReadBusyHead == idxRead ? 0 : anyReadBusyHead;

  f->anyReadBusyHead = anyReadBusyHead;
  f->idxReadBusyHead = idxReadBusyHead;

  //b->status = FIFO_BUFFER_BEING_READ;

  return b;
}


static inline FIFO_BufferHandle FIFO_getNextAsyncWriteBusyBuffer_remote(
	FIFO_Handle f, 
	Uint32 const nBuffers,
	FIFO_BufferState f_b[]
	){

  FIFO_BufferHandle b;
  Uint32 idxWrite = f->idxNextWriteBuffer;

  b = &(f_b[idxWrite]);

#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1137ULL);
#endif
  //if (b->status != FIFO_BUFFER_BEING_WRITTEN) {
  //    FIFO_error(FIFO_ERR_43);
  //}
#ifdef TS_FIFO_WAIT
	SYS_TimeStamp_aliased(1138ULL);
#endif
  idxWrite+=1;
  idxWrite = idxWrite == nBuffers ? 0: idxWrite;
  f->idxNextWriteBuffer = idxWrite;
  b->status = FIFO_BUFFER_BEING_WRITTEN;

  return b;
}


#endif

