/**
Atomix project, BlockJn_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef BLOCKJN_I_H
#define BLOCKJN_I_H 

#include <osl/inc/swpform.h>
#include "BlockJn_t.h"

void BlockJn_i (
	OUT Decision_t    *bOutDecision,
	CF  BlockJn_tConf *bInpConf
	);

void BlockJn_i_conf(
	CF  BlockJn_tConf *bInpConf,
	Uint32 st0,
	Uint32 iterCount
	);

#endif /* BLOCKJN_I_H */
