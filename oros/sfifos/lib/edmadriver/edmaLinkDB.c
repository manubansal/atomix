/**
Atomix project, edmaLinkDB.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
Created on: Jul 31, 2012
*/

#include <osl/inc/swpform.h>

#include "edmaLinkDB.h"


//static Uint32 EDMA_nLinksAlloc = 0;

//#pragma DATA_SECTION(EDMA_linkDB, ".global_flags");
//Uint32 EDMA_linkDB[EDMA_NUM_AVAILABLE_LINKS][3] = {
//		{1, 0, 0},
//		{1, 1, 0},
//		{1, 2, 0},
//		{1, 3, 0},
//////		{1, 4, 0},
//////		{1, 5, 0},
//////		{1, 6, 0},
//////		{1, 7, 0},
//};


Uint32 EDMA_linkDB[EDMA_NUM_AVAILABLE_LINKS][2] = {
		{1, 0},
		{1, 1},
		{1, 2},
		{1, 3},
};


EDMA_LinkDescriptor EDMA_linkDescriptors[EDMA_NUM_AVAILABLE_LINKS];
EDMA_LinkHandle EDMA_linkHandles[EDMA_NUM_AVAILABLE_LINKS];
