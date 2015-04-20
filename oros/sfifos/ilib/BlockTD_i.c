/**
Atomix project, BlockTD_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include <oros/sfifos/fifoFactory.h>
#include <oros/sfifos/fifoManager.h>
#include "BlockTD_t.h"


void BlockTD_i (
	RAWIN  FIFO_BufferState   * bh_src,
	IN     BlockTD_State	  * inpState,
	RAWOUT FIFO_BufferState   * bh_dst,
	OUT    BlockTD_State	  * outState,
	CF     BlockTD_Conf 	  * conf
	){

  IPC_LinkHandle hL;

  Uint32 linkNum = inpState->nextLinkNum;

  FIFO_clearTC(linkNum);

  //****************************************//
  //** sfifos v3.1 - isTCCValid bit added **//
  //****************************************//
  bh_src->isTCCValid = 1;
  bh_dst->isTCCValid = 1;

  bh_src->linkNum = linkNum;
  bh_dst->linkNum = linkNum;

  DEBUG(printf("TD: transfer from buffer seqNo %d to buffer seqNo %d over linkNum %d\n", bh_src->seqNo, bh_dst->seqNo, linkNum);)

  //IPC_dma_transfer(linkNum, (Uint32 *)bh_src->mem_ga, (Uint32 *)bh_dst->mem_ga, bh_src->lengthInBytes/8);
  IPC_dma_transfer(linkNum, (Uint32 *)bh_src->mem, (Uint32 *)bh_dst->mem, conf->lengthInBytes/8);

  //No readDone and writeDone are declared since this is an asynchronous 
  //transfer. These events will be automatically deduced by the fifo manager.

  linkNum = (linkNum + 1) % conf->nLinks + conf->linkNumStart;
  outState->nextLinkNum = linkNum;
}

void BlockTD_i_conf (
	CF BlockTD_Conf * conf,
	IN Uint32 nLinks,
	IN Uint32 linkNumStart
	){
	conf->nLinks = nLinks;
	conf->linkNumStart = linkNumStart;
}
