/**
Atomix project, BlockTD_t.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef BLOCKTD_T_H_
#define BLOCKTD_T_H_

#include <osl/inc/swpform.h>

typedef struct {
  Uint32 nextLinkNum;
} BlockTD_State;

typedef struct {
  Uint32 nLinks;
  Uint32 linkNumStart;
} BlockTD_Conf;

#endif

