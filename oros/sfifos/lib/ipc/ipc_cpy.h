/**
Atomix project, ipc_cpy.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef __IPC_CPY_H__
#define __IPC_CPY_H__

#include <osl/inc/swpform.h>
#include <osl/inc/amem4cpy_inl.h>
#include <oros/sfifos/lib/edmadriver/edmaTypes.h>
#include <oros/sfifos/lib/edmadriver/edmaDriver.h>

//static inline 
//void IPC_cpy_shm(Uint32 * restrict dst, Uint32 * restrict src, Uint32 numberOfWords) {
//  ASSERT(FIFO_isAddressGlobal(dst));
//  ASSERT(FIFO_isAddressGlobal(src));
//  _amem4cpy(dst, src, numberOfWords);
//}
//
//void IPC_cpy_edma(Uint32 * restrict dst, Uint32 * restrict src, Uint32 numberOfWords);

typedef enum {
  IPC_INVALID_REQUEST = -5,
  IPC_UNALIGNED_ADDRESS = -4,
  IPC_LOCAL_ADDRESS = -3,
  IPC_DMA_ERROR = -2,
  IPC_PERMISSION_DENIED = -1,
  IPC_SUCCESS = 0,
  IPC_TCF_SET = 1,
  IPC_TCF_CLEAR = 2
} IPC_Status;

typedef EDMA_LinkDescriptor IPC_DMA_ChannelDescriptor;
typedef IPC_DMA_ChannelDescriptor * IPC_DMA_ChannelHandle;
//typedef IPC_CPU_LinkDescriptor * IPC_CPU_LinkHandle;

typedef enum {
  IPC_LINK_TYPE_CPU,
  IPC_LINK_TYPE_DMA
} IPC_LinkType;

typedef struct {
  IPC_LinkType linkType;

  Uint32 linkNum;

  Uint32 ownerCoreNum;
  Uint32 *src;
  Uint32 *dst;
  Uint32 numDblWords;


//  IPC_CPU_LinkDescriptor dCPU;
  IPC_DMA_ChannelDescriptor 	dCh_DMA;
  IPC_DMA_ChannelHandle 	hCh_DMA;
} IPC_LinkDescriptor;

typedef IPC_LinkDescriptor * IPC_LinkHandle;


/**********************************************
 * Module level API
**********************************************/
IPC_Status IPC_init();

IPC_Status IPC_linkOpen(
	INOUT	IPC_LinkHandle hL,
	//IN	Uint32 linkNum,
	//INOUT	IPC_LinkHandle hLSrc,
	//INOUT	IPC_LinkHandle hLDst,

	IN	IPC_LinkType linkType,

	IN	Uint32 ownerCoreNum,
	IN	Uint32 *srcAddr, 
	IN	Uint32 *dstAddr, 
	IN	Uint32 numDblWords
	);

//Uint32 IPC_isTCFSet(IPC_LinkHandle hL);
//void IPC_clearTCF(IPC_LinkHandle hL);
Uint32 IPC_isTCFSet(Uint32 linkNum);
void IPC_clearTCF(Uint32 linkNum);

IPC_Status IPC_linkClose(IN IPC_LinkHandle hL);

IPC_Status IPC_close();


/**********************************************
 * CPU Transfer API
**********************************************/
//IPC_Status IPC_cpu_transfer(IPC_LinkHandle hL);
IPC_Status IPC_cpu_transfer(Uint32 *src, Uint32 *dst, Uint32 numDblWords);
//IPC_Status IPC_cpu_isTCFSet(IPC_LinkHandle hL);
//IPC_Status IPC_cpu_clearTCF(IPC_LinkHandle hL);


/**********************************************
 * DMA Transfer API
**********************************************/
//IPC_Status IPC_dma_transfer(IPC_LinkHandle hL);
IPC_Status IPC_dma_transfer(Uint32 linkNum, Uint32 *src, Uint32 *dst, Uint32 numDblWords);

//IPC_Status IPC_dma_isTCFSet(IPC_LinkHandle hL);
//IPC_Status IPC_dma_clearTCF(IPC_LinkHandle hL);

#endif  //__IPC_CPY_H__
