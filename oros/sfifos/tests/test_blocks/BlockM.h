/**
Atomix project, ./test_blocks/BlockM.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef __BLOCKM_H__
#define __BLOCKM_H__

#include <osl/inc/swpform.h>
#include <oros/sfifos/fifoFactory.h>

typedef struct {
  FIFO_Handle ff_inp1;
  FIFO_Handle ff_inp2;
  FIFO_Handle ff_out;
} BlockM;

void BlockM_setup(
	BlockM *bli, 
	FIFO_Handle ff_inp1,
	FIFO_Handle ff_inp2,	
	FIFO_Handle ff_out
	);

void BlockM_do (
	BlockM *bli
	//Uint32 *log_sumA,			//log
	//Uint32 log_nextIdx
	);

#endif
