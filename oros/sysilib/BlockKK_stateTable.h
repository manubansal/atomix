/**
Atomix project, BlockKK_stateTable.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef __BLOCKKK_STATETABLE_H__
#define __BLOCKKK_STATETABLE_H__

#include <osl/inc/swpform.h>

typedef void (*KK_StateFxnPtr)(Uint32);

//State function pointer table for each core in its local memory.
//This table is set differently for different cores. In order for
//the setup to work correctly, it is important that this data 
//structure is allocated in each memory (default) and accessible
//using aliased addresses.
extern far KK_StateFxnPtr *__KK_stateTable__;

#endif
