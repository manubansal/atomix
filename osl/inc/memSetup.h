/**
Atomix project, memSetup.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef MEMSETUP_H_
#define MEMSETUP_H_

#include <osl/inc/swpform.h>

void MEMSETUP_remapAndTurnCachingOff (
	unsigned int priority, 
	unsigned long virtual, 
	unsigned long physical, 
	unsigned long size_kbytes
	);

#endif

