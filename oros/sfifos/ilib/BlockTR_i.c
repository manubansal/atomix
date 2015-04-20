/**
Atomix project, BlockTR_i.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include <oros/sfifos/fifoFactory.h>
#include <oros/sfifos/fifoManager.h>
#include "BlockTR_i.h"
#include <c6x.h>

void BlockTR_i(
	IN  void *bufInp,
	OUT void *bufOut,
	CF  BlockTR_tConf *conf
	) {
  Uint32 len;	//bytes
  len = conf->len;

  IPC_cpu_transfer((Uint32 *)bufInp, (Uint32 *)bufOut, len/8);

  DEBUG_VERBOSE(printf("TR: %d bytes transferred\n", conf->len));
}

void BlockTR_i_conf(
	CF  BlockTR_tConf *conf,
	IN  Uint32 len_in_bytes
	) {
  conf->len = len_in_bytes;
}
