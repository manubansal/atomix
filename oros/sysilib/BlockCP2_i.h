/**
Atomix project, BlockCP2_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef __BLOCKCP2_I_H__
#define __BLOCKCP2_I_H__

#include <osl/inc/swpform.h>
#include "BlockCP2_t.h"

void BlockCP2_i(
	IN  void *bufInp,
	OUT void *bufOut0,
	OUT void *bufOut1,
	CF  BlockCP2_tConf *conf
	);

void BlockCP2_i_conf(
	CF  BlockCP2_tConf *conf,
	IN  Uint32 len_in_bytes
	);

#endif //__BLOCKCP2_I_H__
