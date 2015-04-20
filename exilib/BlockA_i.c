/**
Atomix project, BlockA_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <osl/inc/swpform.h>
#include "BlockA_t.h"

void BlockA_i (
	IN  EXILIB_tData_1 *bInpData,
	OUT EXILIB_tData_1 *bOutData
	) {
  int i = 0;
  int sum = 0;
  //for (i = 0; i < BLOCKA_N_VALS; i++) {
  for (i = 0; i < EXILIB_N_VALS_1; i++) {
    bOutData->val[i] = bInpData->val[i] + 1;
    sum += bOutData->val[i];
  }

  DEBUG(
  printf("A: Buffer sum            %d\n", sum);
  )
}
