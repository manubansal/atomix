/**
Atomix project, BlockB_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include "BlockB_t.h"

void BlockB_i (
	IN  EXILIB_tData_1 *bInpData,
	OUT EXILIB_tData_1 *bOutData
	) {
  int i = 0;
  int sum = 0;
  for (i = 0; i < EXILIB_N_VALS_1; i++) {
    bOutData->val[i] = bInpData->val[i] - 1;
    sum += bOutData->val[i];
  }

  DEBUG(
  printf("B: Buffer sum            %d\n", sum);
  )
}
