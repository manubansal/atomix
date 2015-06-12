/**
Atomix project, BlockKK_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include "Decision_t.h"
#include "BlockKK_stateTable.h"

#ifdef _TMS320C6X
#include <c6x.h>
#endif

void BlockKK_i(
	IN Decision_t *bufInpDec
	) {
  Uint32 nextState;
  Uint32 iterCount;
  KK_StateFxnPtr func;

  nextState = bufInpDec->nextState;
  iterCount = bufInpDec->iterCount;

  DEBUG_VERBOSE(
  printf("KK: KK on core %d will invoke stateNum %d with iterCount %d\n", DNUM, nextState, iterCount);
  )

  func = __KK_stateTable__[nextState];
  func(iterCount);
}

