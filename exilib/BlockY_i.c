/**
Atomix project, BlockY_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include "BlockY_t.h"

Uint32 BlockY_totalSum = 0;

void BlockY_i(
	IN EXILIB_tData_1 *bInpData
	) {
  int i = 0;
  int sum = 0;
  //for (i = 0; i < BLOCKY_N_VALS; i++) {
  for (i = 0; i < EXILIB_N_VALS_1; i++) {
    sum += bInpData->val[i];
  }

  BlockY_totalSum += sum;

  DEBUG(
  printf("Y: Buffer sum            %d\n", sum);
  )
}
