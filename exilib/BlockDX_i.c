/**
Atomix project, BlockDX_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include "BlockDX_t.h"

void BlockDX_i (
	IN  EXILIB_tData_1 *bInpData,
	OUT Decision_t    *bOutDecision,
	CF  BlockDX_tConf *bInpConf
	) {

  Uint32 nextState;
  Uint32 iterCount;

  Uint32 *transitionMap;
  transitionMap = bInpConf->transitionMap;

  //int r = _loll(CSL_tscRead()) & 0x00FF;
  Uint32 r = bInpData->val[0];
  DEBUG (
  printf("****** r = %d\n", r);
  )
  if (r < 85) {
    nextState = transitionMap[0];
    iterCount = 1;
  }
  else if (r < 170) {
    nextState = transitionMap[1];
    iterCount = 4;
  }
  else {
    nextState = transitionMap[2];
    iterCount = 4;
  }
  bOutDecision->nextState = nextState;
  bOutDecision->iterCount = iterCount;

  DEBUG(
  printf("DX: r=%d, nextState=%d, iterCount=%d\n", r, nextState, iterCount);
  )
}

void BlockDX_i_conf(
	CF  BlockDX_tConf *bInpConf,
	Uint32 st0,
	Uint32 st1,
	Uint32 st2
	) {
  Uint32 *transitionMap;
  transitionMap = bInpConf->transitionMap;

  transitionMap[0] = st0;
  transitionMap[1] = st1;
  transitionMap[2] = st2;
}
