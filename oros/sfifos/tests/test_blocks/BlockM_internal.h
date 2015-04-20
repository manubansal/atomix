/**
Atomix project, ./test_blocks/BlockM_internal.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef __BLOCKM_INTERNAL_H__
#define __BLOCKM_INTERNAL_H__

#include <osl/inc/swpform.h>

void BlockM_do_internal (
	Uint8 *mem_data_in1, 	//in
	Uint8 *mem_data_in2, 	//in
	Uint8 *mem_data_out1, 	//out
	Uint8 *mem_data_out2, 	//out
	Uint32 mem_data_lengthInBytes
	//Uint32 *log_sumA,			//log
	//Uint32 log_nextIdx
	);

#endif
