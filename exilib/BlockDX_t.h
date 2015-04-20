/**
Atomix project, BlockDX_t.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef __BLOCKDX_T_H__
#define __BLOCKDX_T_H__

#include "BlockX_t.h"			//for BlockX's output type
#include <oros/sysilib/Decision_t.h>	//for Decision_t type

#define BLOCKDX_N_DECWAYS 3

typedef struct {
  Uint32 transitionMap[BLOCKDX_N_DECWAYS];
} BlockDX_tConf;


#endif //__BLOCKDX_T_H__
