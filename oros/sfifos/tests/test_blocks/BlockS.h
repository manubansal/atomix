/**
Atomix project, ./test_blocks/BlockS.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef __BLOCKS_H__
#define __BLOCKS_H__

#include <osl/inc/swpform.h>
#include <oros/sfifos/fifoFactory.h>

typedef struct {
  FIFO_Handle ff_inp;
  FIFO_Handle ff_out1;
  FIFO_Handle ff_out2;
} BlockS;

void BlockS_setup(
	BlockS *bli, 
	FIFO_Handle ff_inp,	//input fifo from which to split out
	FIFO_Handle ff_out1,	//output fifo 1 into which the input is being split
	FIFO_Handle ff_out2	//output fifo 2 into which the input is being split
	);

//This simulates entity A, the data producer on core 1
void BlockS_do (
	BlockS *bli
	//Uint32 *log_sumA,			//log
	//Uint32 log_nextIdx
	);

#endif
