/**
Atomix project, edmaLinkDB.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
Created on: Jul 31, 2012
*/

#ifndef EDMADRIVER_AVAILABLELINKS_H_
#define EDMADRIVER_AVAILABLELINKS_H_

#include "edmaTypes.h"


//#define EDMA_NUM_AVAILABLE_LINKS		40
#define EDMA_NUM_AVAILABLE_LINKS		4

//first column is the module instance number
//second column is the channel number on that module
//third column is the allocation status
//extern Uint32 EDMA_linkDB[EDMA_NUM_AVAILABLE_LINKS][3];
extern Uint32 EDMA_linkDB[EDMA_NUM_AVAILABLE_LINKS][2];
//extern Uint32 EDMA_nLinksAlloc;

#define EDMA_LINKDB_COL_MODNUM		0
#define EDMA_LINKDB_COL_CHNUM		1
//#define EDMA_LINKDB_COL_ISALLOC 	2

extern EDMA_LinkDescriptor EDMA_linkDescriptors[EDMA_NUM_AVAILABLE_LINKS];
extern EDMA_LinkHandle EDMA_linkHandles[EDMA_NUM_AVAILABLE_LINKS];

#endif /* EDMADRIVER_AVAILABLELINKS_H_ */
