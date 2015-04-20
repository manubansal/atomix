/**
Atomix project, BlockJn_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include "BlockJn_t.h"

void BlockJn_i (
	OUT Decision_t    *bOutDecision,
	CF  BlockJn_tConf *bInpConf
	) {

  Uint32 nextState;
  Uint32 iterCount;

  Uint32 *transitionMap;
  transitionMap = bInpConf->transitionMap;

  nextState = transitionMap[0];
  iterCount = bInpConf->iterCount;

  bOutDecision->nextState = nextState;
  bOutDecision->iterCount = iterCount;

  DEBUG_VERBOSE(printf("Jn: nextState=%d, iterCount=%d\n", nextState, iterCount));
}

void BlockJn_i_conf(
	CF  BlockJn_tConf *bInpConf,
	Uint32 st0,
	Uint32 iterCount
	) {
  Uint32 *transitionMap;
  transitionMap = bInpConf->transitionMap;

  transitionMap[0] = st0;
  bInpConf->iterCount = iterCount;
}
