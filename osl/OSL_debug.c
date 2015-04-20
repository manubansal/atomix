/**
Atomix project, OSL_debug.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <osl/inc/swpform.h>
#include <stdio.h>



static int OSL_debug_atids[10];
static int OSL_debug_next = 0;
void OSL_printAtomId(
	Uint32 atomId,
	Uint32 identify
	){
  if (atomId > 0 && identify == 1){
    //printf("AtomId: %d\n", atomId);
    OSL_debug_atids[OSL_debug_next++] = atomId;
    OSL_debug_next = OSL_debug_next % 10;
    OSL_debug_atids[OSL_debug_next] = 0;
  }
}
