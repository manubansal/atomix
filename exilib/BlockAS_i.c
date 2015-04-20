/**
Atomix project, BlockAS_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <osl/inc/swpform.h>
#include "BlockAS_t.h"

void BlockAS_i (
	//Uint8 *mem_data_inp, 	//inp
	//Uint8 *mem_state_inp, 	//inp
	//Uint8 *mem_data_out, 	//out
	//Uint8 *mem_state_out, 	//out
	//Uint32 mem_data_lengthInBytes,
	//Uint32 mem_state_lengthInBytes
	BlockAS_tData  *bInpData,
	BlockAS_tState *bInpState,
	BlockAS_tData  *bOutData,
	BlockAS_tState *bOutState
	) {
  int i = 0;
  int sum = 0;
  //int factor = ((Uint32 *)mem_state_inp)[0];
  int factor = bInpState->factor;
  //for (i = 0; i < mem_data_lengthInBytes/4; i++) {
  for (i = 0; i < BLOCKAS_N_VALS; i++) {
    //((Uint32 *)mem_data_out)[i] = ((Uint32 *)mem_data_inp)[i] + factor;
    bOutData->val[i] = bInpData->val[i] + factor;
    //sum += ((Uint32 *)mem_data_out)[i];
    sum += bOutData->val[i];
  }

  factor = factor + 1;
  //((Uint32 *)mem_state_out)[0] = factor;
  bOutState->factor = factor;

  DEBUG(
  printf("AS: Buffer sum            %d\n", sum);
  )
}
