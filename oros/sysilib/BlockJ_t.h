/**
Atomix project, BlockJ_t.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef __BLOCKJ_T_H__
#define __BLOCKJ_T_H__

#include <oros/sysilib/Decision_t.h>	//for Decision_t type

#define BLOCKJ_N_DECWAYS 1

typedef struct {
  Uint32 transitionMap[BLOCKJ_N_DECWAYS];
} BlockJ_tConf;



#endif //__BLOCKJ_T_H__
