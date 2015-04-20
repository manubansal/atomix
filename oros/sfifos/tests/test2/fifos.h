/**
Atomix project, ./test2/fifos.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef __FIFOS_H__
#define __FIFOS_H__

#include <osl/inc/swpform.h>
#include <oros/sfifos/fifoFactory.h>
#include <oros/sfifos/fifoManager.h>

#define FF_BUFFER_SIZE_IN_BYTES		8
//#define FF_BUFFER_SIZE_IN_BYTES		1024
//#define	FF_N_BUFFERS			3
//#define	FF_N_BUFFERS			11
#define	FF_N_BUFFERS			41
#define FF_TOTAL_SIZE_IN_BYTES		(FF_BUFFER_SIZE_IN_BYTES * FF_N_BUFFERS)

//#define F2_BUFFER_SIZE_IN_BYTES		4
//#define	F2_N_BUFFERS			3
//#define F2_TOTAL_SIZE_IN_BYTES		(F2_BUFFER_SIZE_IN_BYTES * F2_N_BUFFERS)

void setupFifos();

#endif //__FIFOS_H__
