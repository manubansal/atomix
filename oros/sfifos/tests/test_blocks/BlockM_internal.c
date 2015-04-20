/**
Atomix project, ./test_blocks/BlockM_internal.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include <osl/inc/amem4cpy_inl.h>

void BlockM_do_internal (
	Uint8 *mem_data_inp1, 	//inp
	Uint8 *mem_data_inp2, 	//inp
	Uint8 *mem_data_out1, 	//out
	Uint8 *mem_data_out2, 	//out
	Uint32 mem_data_lengthInBytes
	//Uint32 *log_sumA,			//log
	//Uint32 log_nextIdx
	) {
  int i = 0;

  ASSERT_PTR_ALIGNED(mem_data_inp1, 4);
  ASSERT_PTR_ALIGNED(mem_data_inp2, 4);
  ASSERT_PTR_ALIGNED(mem_data_out1, 4);
  ASSERT_PTR_ALIGNED(mem_data_out2, 4);
  ASSERT(mem_data_lengthInBytes % 4 == 0);

  _amem4cpy((Uint32 *)mem_data_out1, (Uint32 *)mem_data_inp1, mem_data_lengthInBytes/4);
  _amem4cpy((Uint32 *)mem_data_out2, (Uint32 *)mem_data_inp2, mem_data_lengthInBytes/4);

}
