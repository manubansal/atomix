/**
Atomix project, ./test3/blockA.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef __BLOCKA_H__
#define __BLOCKA_H__

#include <osl/inc/swpform.h>
#include <oros/sfifos/fifoFactory.h>

void doA(
	FIFO_BufferHandle bh_data_out,
	Uint32 *log_sumA,			//log
	Uint32 log_nextIdx
	);

#endif
