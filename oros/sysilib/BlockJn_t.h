/**
Atomix project, BlockJn_t.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef BLOCKJN_T_H
#define BLOCKJN_T_H 

#include <oros/sysilib/Decision_t.h>	//for Decision_t type

#define BLOCKJn_N_DECWAYS 1

typedef struct {
  Uint32 transitionMap[BLOCKJn_N_DECWAYS];
  Uint32 iterCount;
} BlockJn_tConf;

#endif /* BLOCKJN_T_H */
