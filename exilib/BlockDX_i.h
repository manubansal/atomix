/**
Atomix project, BlockDX_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef __BLOCKDX_I_H__
#define __BLOCKDX_I_H__

#include <osl/inc/swpform.h>
#include "BlockDX_t.h"

void BlockDX_i (
	IN  EXILIB_tData_1 *bInpData,
	OUT Decision_t *bOutDecision,
	CF  BlockDX_tConf *bInpConf
	);


void BlockDX_i_conf(
	CF  BlockDX_tConf *bInpConf,
	Uint32 st0,
	Uint32 st1,
	Uint32 st2
	);
#endif
