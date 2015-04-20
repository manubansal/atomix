/**
Atomix project, ./test_blocks/BlockM.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <c6x.h>
#include <osl/inc/swpform.h>
#include <ti/csl/csl_tsc.h>

#include <oros/sfifos/fifoFactory.h>
#include <oros/sfifos/fifoManager.h>

#include "BlockM.h"
#include "BlockM_internal.h"

void BlockM_setup(
	BlockM *bli, 
	FIFO_Handle ff_inp1,
	FIFO_Handle ff_inp2,
	FIFO_Handle ff_out
	) {
  bli->ff_inp1 = ff_inp1;
  bli->ff_inp2 = ff_inp2;
  bli->ff_out = ff_out;
}

void BlockM_do (
	BlockM *bli
	//Uint32 *log_sumX,			//log
	//Uint32 log_nextIdx
	) {
  FIFO_BufferHandle bhi1, bhi2, bho1, bho2;
  bhi1 = FIFO_getNextReadBuffer(bli->ff_inp1);
  bhi2 = FIFO_getNextReadBuffer(bli->ff_inp2);
  bho1 = FIFO_getNextWriteBuffer(bli->ff_out);
  bho2 = FIFO_getNextWriteBuffer(bli->ff_out);
  BlockM_do_internal(
  	bhi1->mem, 
  	bhi2->mem, 
	bho1->mem,
	bho2->mem,
	bhi1->lengthInBytes
	//log_sumX, log_nextIdx
	);
  FIFO_readDone(bli->ff_inp1, bhi1);
  FIFO_readDone(bli->ff_inp2, bhi2);
  FIFO_writeDone(bli->ff_out, bho1);
  FIFO_writeDone(bli->ff_out, bho2);
  printf("M: Merge buffer seq no. %2d and %2d\n", bhi1->seqNo, bhi2->seqNo); 
}
