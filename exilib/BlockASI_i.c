/**
Atomix project, BlockASI_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include "BlockASI_t.h"

void BlockASI_i(
	OUT BlockASI_tState *bOutState
	) {

  int factor;

  factor = 1;
  bOutState->factor = factor;

  DEBUG(
  printf("ASI: Initialization factor = %d\n", factor);
  )
}
