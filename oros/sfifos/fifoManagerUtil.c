/**
Atomix project, fifoManagerUtil.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "fifoManager.h"
#include <c6x.h>
#include <osl/inc/timing.h>


#include <oros/sysilib/SYS_TimeStamp.h>


//////////void FIFO_rereadDone(FIFO_Handle f, FIFO_BufferHandle b) {
//////////  DEBUG_ALL(printf("FIFO_rereadDone\n"));
//////////  //ASSERT(b->status == FIFO_BUFFER_BEING_READ);
//////////  //Modifying this since we might declare read done on an already empty buffer 
//////////  //due to bunching of the transfer completion case with the synchronous case.
//////////  ASSERT(b->status == FIFO_BUFFER_BEING_READ);
//////////  //b->status = FIFO_BUFFER_EMPTY;
//////////  b->status = FIFO_BUFFER_FILLED;
//////////}

//void FIFO_waitBR(FIFO_BufferHandle b) {
//  while (b->status != FIFO_BUFFER_BEING_READ);
//}
//
//void FIFO_waitBW(FIFO_BufferHandle b) {
//  while (b->status != FIFO_BUFFER_BEING_WRITTEN);
//}

////////void FIFO_waitF(FIFO_BufferHandle b) {
////////  while (b->status != FIFO_BUFFER_FILLED);
////////}
////////
////////void FIFO_waitE(FIFO_BufferHandle b) {
////////  while (b->status != FIFO_BUFFER_EMPTY);
////////}

//Deprecated. Use one of FIFO_waitRTC or FIFO_waitWTC functions instead.
//void FIFO_waitTC(FIFO_BufferHandle b) {
//  //printf("FIFO_waitTC_pre\n");
//  //while (!IPC_isTCFSet(b->hL));
//
//  //**************************************//
//  //* sfifos v3.0 without TCC validation *//
//  //**************************************//
//  //while (IPC_isTCFSet(b->hL) != IPC_TCF_SET);
//
//  //printf("FIFO_waitTC_post\n");
//
//  //**************************************//
//  //* sfifos v3.1 with TCC validation    *//
//  //**************************************//
//  while (!((IPC_isTCFSet(b->hL) == IPC_TCF_SET) && b->isTCCValid));
//  b->isTCCValid = 0;
//}

static inline Uint32 FIFO_waitRTC_inner(FIFO_BufferHandle b) {
  volatile Uint32 be;
  volatile Uint32 tc;
  volatile Uint32 rd;
  be = (b->status == FIFO_BUFFER_EMPTY);
  tc = (b->isTCCValid && IPC_isTCFSet(b->linkNum) == IPC_TCF_SET);
  rd = (be || tc);
  return rd;
}

void FIFO_waitRTC(FIFO_Handle f, FIFO_BufferHandle b) {
  //while (!((IPC_isTCFSet(b->hL) == IPC_TCF_SET) && b->isTCCValid));
  //In the case of synchronous reads (TR block), readDone will be declared after the read without the
  //use of transfer completion flags. For that case, we must end the wait on Read Transfer Completion
  //upon buffer becoming empty.
  ////while (!(((IPC_isTCFSet(b->hL) == IPC_TCF_SET) && b->isTCCValid) || b->status == FIFO_BUFFER_EMPTY));
//  while (!(((IPC_isTCFSet(b->linkNum) == IPC_TCF_SET) && b->isTCCValid) || b->status == FIFO_BUFFER_EMPTY));
//  while (!((b->status == FIFO_BUFFER_EMPTY || (b->isTCCValid && IPC_isTCFSet(b->linkNum) == IPC_TCF_SET))));
#ifdef TS_FIFO_WAIT_I
	SYS_TimeStamp_aliased(1131ULL);
#endif
	volatile Uint32 rd;
	do {
		rd = FIFO_waitRTC_inner(b);
	} while (!rd);
    b->isTCCValid = 0;

    FIFO_readDone(f, b);
#ifdef TS_FIFO_WAIT_I
	SYS_TimeStamp_aliased(1132ULL);
#endif
}


static inline Uint32 FIFO_waitWTC_inner(FIFO_BufferHandle b) {
  volatile Uint32 bf;
  volatile Uint32 tc;
  volatile Uint32 wd;
  bf = (b->status == FIFO_BUFFER_FILLED);
  tc = (b->isTCCValid && IPC_isTCFSet(b->linkNum) == IPC_TCF_SET);
  wd = (bf || tc);
  return wd;
}

void FIFO_waitWTC(FIFO_Handle f, FIFO_BufferHandle b) {
  //while (!((IPC_isTCFSet(b->hL) == IPC_TCF_SET) && b->isTCCValid));
  //In the case of synchronous writes (TR block), writeDone will be declared after the write without the
  //use of transfer completion flags. For that case, we must end the wait on Write Transfer Completion
  //upon buffer becoming filled.
  ////while (!(((IPC_isTCFSet(b->hL) == IPC_TCF_SET) && b->isTCCValid) || b->status == FIFO_BUFFER_FILLED));

	//while (!((b->status == FIFO_BUFFER_FILLED || (b->isTCCValid && IPC_isTCFSet(b->linkNum) == IPC_TCF_SET))));
	////volatile Uint32 bf;
	////volatile Uint32 tc;
#ifdef TS_FIFO_WAIT_I
	SYS_TimeStamp_aliased(1133ULL);
#endif
	volatile Uint32 wd;
	do {
		////bf = (b->status == FIFO_BUFFER_FILLED);
		////tc = (b->isTCCValid && IPC_isTCFSet(b->linkNum) == IPC_TCF_SET);
		////wd = (bf || tc);
		wd = FIFO_waitWTC_inner(b);

	} while (!wd);

	b->isTCCValid = 0;

    FIFO_writeDone(f, b);
#ifdef TS_FIFO_WAIT_I
	SYS_TimeStamp_aliased(1134ULL);
#endif
}

//void FIFO_clearTC(FIFO_BufferHandle b) {
void FIFO_clearTC(Uint32 linkNum) {
  //Not repeating this check here since it is already 
  //performed in the IPC module, which guarantees only
  //the owner will be able to clear the TCF
  //if (b->hL->ownerCoreNum == DNUM) {
  ////IPC_clearTCF(b->hL);
  ////IPC_clearTCF(b->linkNum);
  IPC_clearTCF(linkNum);
  //}
}

void FIFO_error(Uint32 err) {
  DEBUG(printf("FIFO Error %d", err));
  asm( " .long 0x1001E000" );
  exit(1);
}

//------------------------------------------------------------------------------------
//utility API
//------------------------------------------------------------------------------------
//FIFO_Handle FIFO_remoteFifoHandle(FIFO_Handle f, Uint32 remoteCoreId) {
//  //FIFO_Handle fr = (Uint32)((remoteCoreId << 24) | 0x10000000 | (Uint32)f);
//  FIFO_Handle fr = (FIFO_Handle)FIFO_localToGlobalAddress((Uint32)f, remoteCoreId);
//  return fr;
//}

Uint32 FIFO_isAddressGlobal(Uint32 addr) {
  //return (addr << 3) >> 31;	//The top hex digit is 1 for a global address

  //Actually, since the only local addresses we want to detect are those in L2,
  //they have the top two hex digits 0x00 (16MB of address space from zero address). 
  //Any other address will just be considered
  //global (ex. 0x0C..., an address in the shared memory, or 0xE..., an address in
  //a mapped memory range.
  return (addr >> 24);
}

Uint32 FIFO_localToGlobalAddress(void *ptr, Uint32 coreId) {
  //return (coreId << 24) | 0x10000000 | (Uint32)ptr;
  if (FIFO_isAddressGlobal((Uint32)ptr))
    return (Uint32)ptr;
  else
    return (coreId << 24) | 0x10000000 | (Uint32)ptr;
}




