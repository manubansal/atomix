/**
Atomix project, BlockJ_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef __BLOCKJ_I_H__
#define __BLOCKJ_I_H__

#include <osl/inc/swpform.h>
#include "BlockJ_t.h"

void BlockJ_i (
	OUT Decision_t *bOutDecision,
	CF  BlockJ_tConf *bInpConf
	);

void BlockJ_i_conf(
	CF  BlockJ_tConf *bInpConf,
	Uint32 st0
	);

#endif
