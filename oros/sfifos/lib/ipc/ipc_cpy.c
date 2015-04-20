/**
Atomix project, ipc_cpy.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <osl/inc/swpform.h>
#include "ipc_cpy.h"
#include <c6x.h>


//void IPC_cpy_edma(Uint32 * restrict dst, Uint32 * restrict src, Uint32 numberOfWords) {
//  ASSERT(FIFO_isAddressGlobal(dst));
//  ASSERT(FIFO_isAddressGlobal(src));
//  
//}

//typedef struct {
//  Uint32 
//} IPC_CPU_LinkDescriptor;


IPC_Status IPC_init() {
  edmaInit();

  return IPC_SUCCESS;
}

IPC_Status IPC_linkOpen(
	INOUT	IPC_LinkHandle hL,
	//IN	Uint32 linkNum,

	IN	IPC_LinkType linkType,

	IN	Uint32 ownerCoreNum,
	IN	Uint32 *srcAddr, 
	IN	Uint32 *dstAddr, 
	IN	Uint32 numDblWords
	) {

  Uint32 linkNum;

  if (DNUM != 0)
    printf("WARNING: IPC_linkOpen invoked from non-zero core id %d\n", DNUM);

  if (!IS_ADDRESS_GLOBAL((Uint32)srcAddr) || !IS_ADDRESS_GLOBAL((Uint32)dstAddr)) return IPC_LOCAL_ADDRESS;
  if (!IS_PTR_ALIGNED(srcAddr, 8) || !IS_PTR_ALIGNED(dstAddr, 8)) return IPC_UNALIGNED_ADDRESS;

  /* Src and Dst addresses together with buffer size 
   * in this call allow implementing a buffer directory
   * to track allocation of buffers to links. It can
   * be used to ensure that no buffer is being transfered
   * over more than one link for multi-core correctness.
   * It can also ensure that no buffer's link is being
   * owned by multiple cores, so that only one core gets
   * to control transfers of the buffer.
   */

  hL->linkType = linkType;

  linkNum = edmaLinkAllocate();
  if (linkNum < 0) {
    printf("ERROR: IPC_linkOpen: Cannot assign link number\n");
    exit(1);
  }
  hL->linkNum = linkNum;

  hL->ownerCoreNum = ownerCoreNum;

  hL->src = srcAddr;

  hL->dst = dstAddr;

  hL->numDblWords = numDblWords;

  hL->hCh_DMA = &hL->dCh_DMA;

  if (linkType == IPC_LINK_TYPE_DMA) {
    //DMA-specific link setup
    Int32 stat;
    stat = edmaLinkOpenSpecific(
	hL->hCh_DMA,
	linkNum
	//ownerCoreNum,
	//src,
	//dst,
	//numDblWords
	);
    if (stat < 0) return IPC_DMA_ERROR;

  }
  return IPC_SUCCESS;
}


//IPC_Status IPC_linkOpen(
//	INOUT	IPC_LinkHandle hLSrc,
//	INOUT	IPC_LinkHandle hLDst,
//	//IN	Uint32 linkNum,
//
//	IN	IPC_LinkType linkType,
//
//	IN	Uint32 ownerCoreNum,
//	IN	Uint32 *srcAddr, 
//	IN	Uint32 *dstAddr, 
//	IN	Uint32 numDblWords
//	) {
//
//  Uint32 linkNum;
//
//  if (DNUM != 0)
//    printf("WARNING: IPC_linkOpen invoked from non-zero core id %d\n", DNUM);
//
//  if (!IS_ADDRESS_GLOBAL((Uint32)srcAddr) || !IS_ADDRESS_GLOBAL((Uint32)dstAddr)) return IPC_LOCAL_ADDRESS;
//  if (!IS_PTR_ALIGNED(srcAddr, 8) || !IS_PTR_ALIGNED(dstAddr, 8)) return IPC_UNALIGNED_ADDRESS;
//
//  /* Src and Dst addresses together with buffer size 
//   * in this call allow implementing a buffer directory
//   * to track allocation of buffers to links. It can
//   * be used to ensure that no buffer is being transfered
//   * over more than one link for multi-core correctness.
//   * It can also ensure that no buffer's link is being
//   * owned by multiple cores, so that only one core gets
//   * to control transfers of the buffer.
//   */
//
//  hLSrc->linkType = linkType;
//  hLDst->linkType = linkType;
//
//  linkNum = edmaLinkAllocate();
//  if (linkNum < 0) {
//    printf("ERROR: IPC_linkOpen: Cannot assign link number\n");
//    exit(1);
//  }
//  hLSrc->linkNum = linkNum;
//  hLDst->linkNum = linkNum;
//
//  hLSrc->ownerCoreNum = ownerCoreNum;
//  hLDst->ownerCoreNum = ownerCoreNum;
//
//  hLSrc->src = srcAddr;
//  hLDst->src = srcAddr;
//
//  hLSrc->dst = dstAddr;
//  hLDst->dst = dstAddr;
//
//  hLSrc->numDblWords = numDblWords;
//  hLDst->numDblWords = numDblWords;
//
//  hLSrc->hCh_DMA = &hLSrc->dCh_DMA;
//  hLDst->hCh_DMA = &hLDst->dCh_DMA;
//
//  if (linkType == IPC_LINK_TYPE_DMA) {
//    //DMA-specific link setup
//    Int32 stat;
//    stat = edmaLinkOpenSpecific(
//	hLSrc->hCh_DMA,
//	linkNum
//	//ownerCoreNum,
//	//src,
//	//dst,
//	//numDblWords
//	);
//    if (stat < 0) return IPC_DMA_ERROR;
//
//    stat = edmaLinkOpenSpecific(
//	hLDst->hCh_DMA,
//	linkNum
//	//ownerCoreNum,
//	//src,
//	//dst,
//	//numDblWords
//	);
//    if (stat < 0) return IPC_DMA_ERROR;
//  }
//  return IPC_SUCCESS;
//}

//Uint32 IPC_isTCFSet(IPC_LinkHandle hL) {
//  if (hL->linkType == IPC_LINK_TYPE_DMA) {
//    return IPC_dma_isTCFSet(hL);
//  }
//  else {
//    printf("ERROR: Link type not supported for TCF set\n");
//    exit(1);
//  }
//
//  return 0;
//}
//
//void IPC_clearTCF(IPC_LinkHandle hL) {
//  if (hL->linkType == IPC_LINK_TYPE_DMA) {
//    IPC_dma_clearTCF(hL);
//  }
//  else {
//    printf("ERROR: Link type not supported for TCF clear\n");
//    exit(1);
//  }
//
//  return 0;
//}


Uint32 IPC_isTCFSet(Uint32 linkNum) {
  return IPC_dma_isTCFSet(linkNum);
}

void IPC_clearTCF(Uint32 linkNum) {
  IPC_dma_clearTCF(linkNum);
}

IPC_Status IPC_linkClose(IN IPC_LinkHandle hL) {
  if (hL->ownerCoreNum != DNUM) return IPC_PERMISSION_DENIED;

  if (hL->linkType == IPC_LINK_TYPE_DMA) {
    edmaLinkClose(hL->hCh_DMA);
  }

  return IPC_SUCCESS;
}

IPC_Status IPC_close() {
  edmaClose();

  return IPC_SUCCESS;
}

/**********************************************
 * CPU Transfer API
**********************************************/

//IPC_Status IPC_cpu_transfer(IPC_LinkHandle hL) {
IPC_Status IPC_cpu_transfer(Uint32 *src, Uint32 *dst, Uint32 numDblWords) {
  //ASSERT(FIFO_isAddressGlobal(hL->dst));
  //ASSERT(FIFO_isAddressGlobal(hL->src));
  //_amem4cpy(hL->dst, hL->src, hL->numDblWords * 2);
  _amem4cpy(dst, src, numDblWords * 2);
  return IPC_SUCCESS;
}

//void IPC_cpu_isTCFSet(IPC_LinkHandle hL);
//void IPC_cpu_clearTCF(IPC_LinkHandle hL);

/**********************************************
 * DMA Transfer API
**********************************************/

//IPC_Status IPC_dma_transfer(IPC_LinkHandle hL) {
//  Int32 stat;
//  if (hL->linkType != IPC_LINK_TYPE_DMA) return IPC_INVALID_REQUEST;
//  if (hL->ownerCoreNum != DNUM) return IPC_PERMISSION_DENIED;
//
//  edmaSetupTransfer(hL->hCh_DMA, hL->src, hL->dst, hL->numDblWords);
//  stat = edmaStartTransfer(hL->hCh_DMA);
//  if (stat < 0) return IPC_DMA_ERROR;
//  
//  return IPC_SUCCESS;
//  
//}
//
IPC_Status IPC_dma_transfer(Uint32 linkNum, Uint32 *src, Uint32 *dst, Uint32 numDblWords) {
  Int32 stat;
  //if (hL->linkType != IPC_LINK_TYPE_DMA) return IPC_INVALID_REQUEST;
  //if (hL->ownerCoreNum != DNUM) return IPC_PERMISSION_DENIED;

  edmaSetupTransfer(linkNum, src, dst, numDblWords);
  stat = edmaStartTransfer(linkNum);
  if (stat < 0) return IPC_DMA_ERROR;
  
  return IPC_SUCCESS;
  
}


//IPC_Status IPC_dma_isTCFSet(IPC_LinkHandle hL) {
//  if (hL->linkType != IPC_LINK_TYPE_DMA) {
//    printf("IPC_INVALID_REQUEST\n");
//    exit(1);
//    //return IPC_INVALID_REQUEST;
//  }
//  if (edmaIsTransferComplete(hL->hCh_DMA)) 
//    return IPC_TCF_SET;
//  else
//    return IPC_TCF_CLEAR;
//}
//
//IPC_Status IPC_dma_clearTCF(IPC_LinkHandle hL) {
//  if (hL->linkType != IPC_LINK_TYPE_DMA) return IPC_INVALID_REQUEST;
//  if (hL->ownerCoreNum != DNUM) return IPC_PERMISSION_DENIED;
//
//  edmaClearTransferCompletionFlag(hL->hCh_DMA);
//
//  return IPC_SUCCESS;
//}


IPC_Status IPC_dma_isTCFSet(Uint32 linkNum) {
  if (edmaIsTransferComplete(linkNum)) 
    return IPC_TCF_SET;
  else
    return IPC_TCF_CLEAR;
}

IPC_Status IPC_dma_clearTCF(Uint32 linkNum) {
  edmaClearTransferCompletionFlag(linkNum);

  return IPC_SUCCESS;
}
