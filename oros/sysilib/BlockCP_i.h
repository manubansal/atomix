/**
Atomix project, BlockCP_i.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef __BLOCKCP_I_H__
#define __BLOCKCP_I_H__

#include <osl/inc/swpform.h>
#include "BlockCP_t.h"

void BlockCP_i(
	IN  void *bufInp,
	OUT void *bufOut0,
	OUT void *bufOut1,
	OUT void *bufOut2,
	OUT void *bufOut3,
	CF  BlockCP_tConf *conf
	);

void BlockCP_i_conf(
	CF  BlockCP_tConf *conf,
	IN  Uint32 len_in_bytes
	);

#endif //__BLOCKCP_I_H__
