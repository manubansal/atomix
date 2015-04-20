/**
Atomix project, ./test3/blockB.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef __BLOCKB_H__
#define __BLOCKB_H__

#include <osl/inc/swpform.h>
#include <oros/sfifos/fifoFactory.h>

void doB(
	FIFO_BufferHandle bh_data_in,
	Uint32 *log_sumB,			//log
	Uint32 log_nextIdx
	);

#endif
