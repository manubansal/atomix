/**
Atomix project, ./test_s2/old/fifos.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef __FIFOS_H__
#define __FIFOS_H__

#include <osl/inc/swpform.h>
#include <oros/sfifos/fifoFactory.h>
#include <oros/sfifos/fifoManager.h>

//#define FF_BUFFER_SIZE_IN_BYTES		8
//#define	FF_N_BUFFERS			11
//#define FF_TOTAL_SIZE_IN_BYTES		(FF_BUFFER_SIZE_IN_BYTES * FF_N_BUFFERS)

extern FIFO_Handle ff;

void setupFifos();

#endif //__FIFOS_H__
