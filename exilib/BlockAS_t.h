/**
Atomix project, BlockAS_t.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef __BLOCKAS_T_H__
#define __BLOCKAS_T_H__

#include <osl/inc/swpform.h>

#define BLOCKAS_N_VALS	4

typedef struct {
  Uint32 factor;
} BlockAS_tState;

typedef struct {
  Uint32 val[BLOCKAS_N_VALS];
} BlockAS_tData;

#endif //__BLOCKAS_T_H__
