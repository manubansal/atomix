/**
Atomix project, BlockTR_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef __BLOCKTR_I_H__
#define __BLOCKTR_I_H__

#include <osl/inc/swpform.h>
#include "BlockTR_t.h"

void BlockTR_i(
	IN  void *bufInp,
	OUT void *bufOut,
	CF  BlockTR_tConf *conf
	);

void BlockTR_i_conf(
	CF  BlockTR_tConf *conf,
	IN  Uint32 len_in_bytes
	);

#endif //__BLOCKTR_I_H__
