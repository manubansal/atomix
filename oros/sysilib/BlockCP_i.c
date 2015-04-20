/**
Atomix project, BlockCP_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include <osl/inc/amem4cpy_inl.h>
#include <oros/sfifos/fifoFactory.h>
#include <oros/sfifos/fifoManager.h>
#include "BlockCP_t.h"
#include <c6x.h>

//This simulates the transfer block T
void BlockCP_i(
	IN  void *bufInp,
	OUT void *bufOut0,
	OUT void *bufOut1,
	OUT void *bufOut2,
	OUT void *bufOut3,
	CF  BlockCP_tConf *conf
	) {
  Uint32 len_in_bytes;
  len_in_bytes = conf->len;

  _amem4cpy((Uint32 *)bufOut0, (Uint32 *)bufInp, len_in_bytes/4);
  _amem4cpy((Uint32 *)bufOut1, (Uint32 *)bufInp, len_in_bytes/4);
  _amem4cpy((Uint32 *)bufOut2, (Uint32 *)bufInp, len_in_bytes/4);
  _amem4cpy((Uint32 *)bufOut3, (Uint32 *)bufInp, len_in_bytes/4);
}

void BlockCP_i_conf(
	CF  BlockCP_tConf *conf,
	IN  Uint32 len_in_bytes
	) {
  conf->len = len_in_bytes;
}
