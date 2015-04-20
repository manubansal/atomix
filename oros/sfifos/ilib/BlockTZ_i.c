/**
Atomix project, BlockTZ_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include <oros/sfifos/fifoFactory.h>
#include <oros/sfifos/fifoManager.h>
#include "BlockTZ_t.h"


void BlockTZ_i (
	IN  FIFO_BufferState    * bh_src,
	OUT FIFO_BufferState    * bh_dst,
	CF  BlockTZ_Conf  	* conf
	){

  Uint32 linkNum = conf->linkNum;

  FIFO_clearTC(linkNum);

  //****************************************//
  //** sfifos v3.1 - isTCCValid bit added **//
  //****************************************//
  bh_src->isTCCValid = 1;
  bh_dst->isTCCValid = 1;

  bh_src->linkNum = linkNum;
  bh_dst->linkNum = linkNum;

  //DEBUG(printf("TZ: transfer from buffer seqNo %d to buffer seqNo %d over linkNum %d\n", bh_src->seqNo, bh_dst->seqNo, linkNum);)
  DEBUG(printf("TZ: transfer over linkNum %d\n", linkNum);)

  //IPC_dma_transfer(linkNum, (Uint32 *)bh_src->mem_ga, (Uint32 *)bh_dst->mem_ga, bh_src->lengthInBytes/8);
  IPC_dma_transfer(linkNum, (Uint32 *)bh_src->mem, (Uint32 *)bh_dst->mem, conf->lengthInDblWords);

  //No readDone and writeDone are declared since this is an asynchronous 
  //transfer. These events will be automatically deduced by the fifo manager.

}

void BlockTZ_i_conf (
	CF BlockTZ_Conf * conf,
	IN Uint32 lengthInDblWords,
	IN Uint32 linkNum
	){
	conf->linkNum = linkNum;
	conf->lengthInDblWords = lengthInDblWords;
}
