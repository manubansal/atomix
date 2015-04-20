/**
Atomix project, BlockX_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include "BlockX_t.h"

void BlockX_i(
	OUT EXILIB_tData_1 *bOutData
	) {
  int i = 0;
  int sum = 0;
  int r = _loll(CSL_tscRead()) & 0x00FF;
  //for (i = 0; i < BLOCKX_N_VALS; i++) {
  for (i = 0; i < EXILIB_N_VALS_1; i++) {
    bOutData->val[i] = r * (i + 1);	//fill data
    sum += bOutData->val[i];
  }

  DEBUG(
  printf("X: Buffer sum            %d\n", sum);
  )
}
