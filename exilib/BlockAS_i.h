/**
Atomix project, BlockAS_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef __BLOCKAS_I_H__
#define __BLOCKAS_I_H__

#include <osl/inc/swpform.h>
#include "BlockAS_t.h"

void BlockAS_i (
	IN  BlockAS_tData  *bInpData,
	IN  BlockAS_tState *bInpState,
	OUT BlockAS_tData  *bOutData,
	OUT BlockAS_tState *bOutState
	);


#endif
