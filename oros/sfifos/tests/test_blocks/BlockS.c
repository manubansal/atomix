/**
Atomix project, ./test_blocks/BlockS.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <c6x.h>
#include <osl/inc/swpform.h>
#include <ti/csl/csl_tsc.h>

#include <oros/sfifos/fifoFactory.h>
#include <oros/sfifos/fifoManager.h>

#include "BlockS.h"
#include "BlockS_internal.h"

void BlockS_setup(
	BlockS *bli, 
	FIFO_Handle ff_inp,	//input fifo from which to split out
	FIFO_Handle ff_out1,	//output fifo 1 into which the input is being split
	FIFO_Handle ff_out2	//output fifo 2 into which the input is being split
	) {
  bli->ff_inp = ff_inp;
  bli->ff_out1 = ff_out1;
  bli->ff_out2 = ff_out2;
}

void BlockS_do (
	BlockS *bli
	//Uint32 *log_sumX,			//log
	//Uint32 log_nextIdx
	) {
  FIFO_BufferHandle bhi1, bhi2, bho1, bho2;
  bhi1 = FIFO_getNextReadBuffer(bli->ff_inp);
  bhi2 = FIFO_getNextReadBuffer(bli->ff_inp);
  bho1 = FIFO_getNextWriteBuffer(bli->ff_out1);
  bho2 = FIFO_getNextWriteBuffer(bli->ff_out2);
  BlockS_do_internal(
  	bhi1->mem, 
  	bhi2->mem, 
	bho1->mem,
	bho2->mem,
	bhi1->lengthInBytes
	//log_sumX, log_nextIdx
	);
  FIFO_readDone(bli->ff_inp, bhi1);
  FIFO_readDone(bli->ff_inp, bhi2);
  FIFO_writeDone(bli->ff_out1, bho1);
  FIFO_writeDone(bli->ff_out2, bho2);
  printf("S: Split buffer seq no. %2d and %2d\n", bhi1->seqNo, bhi2->seqNo); 
}
