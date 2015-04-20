/**
Atomix project, edmaDriver.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
Created on: Jul 14, 2012
*/

#ifndef EDMADRIVER_H_
#define EDMADRIVER_H_

#include <ti/csl/csl_edma3.h>
#include <ti/csl/csl_edma3Aux.h>
#include "edmaTypes.h"
#include <osl/inc/swpform.h>

#include "edmaLinkDB.h"

#define EDMA_NUM_MODULES				3

//#define EDMA_NUM_CORES				4
//links will be divided evenly among all cores



//An EDMA link is identified by (moduleNumber, channelNumber)


/////////////////////////////////////////////////
//#define EDMA_MODULE_INSTANCE_NUM 0
//#define EDMA_MODULE_INSTANCE_NUM 1


///////////////////////////////////////

////// API functions //////////
void 	edmaInit();
//void 			intcInit();

int	edmaLinkAllocate();

//Open a link with the next available link. This is
//useful when only one core is interested in using the channel
//or observing it, so that a single copy of the link on that
//particular channel will work.
int 	edmaLinkOpen(
		INOUT 	EDMA_LinkHandle hL
//		IN		Uint32 ownerCoreNum,
//		IN		Uint32 *src,
//		IN		Uint32 *dst,
//		IN		Uint32 numDblWords
				);

//Open a link with a specific link number. This is
//useful when multiple cores are inspecting the same
//EDMA link, so they each need a handle to the same EDMA
//link. They can each call this function with the same channel
//number.
int 	edmaLinkOpenSpecific(
		INOUT 	EDMA_LinkHandle hL,
		Uint32	linkNum
				);

void 	edmaSetupTransfer(
			//EDMA_LinkHandle hL,
			Uint32 linkNum,
			Uint32 *src,
			Uint32 *dst,
			Uint32 numDblWords
		);

//int 	edmaStartTransfer(EDMA_LinkHandle hL);
int 	edmaStartTransfer(Uint32 linkNum);

//Uint32 	edmaIsTransferComplete(EDMA_LinkHandle hL);
//int	edmaClearTransferCompletionFlag(EDMA_LinkHandle hL);
Uint32 	edmaIsTransferComplete(Uint32 linkNum);
int	edmaClearTransferCompletionFlag(Uint32 linkNum);

void 	edmaLinkClose(EDMA_LinkHandle hL);

void 	edmaClose();
//void 			intcClose();


typedef enum {
  EDMA_ERR1=1,
  EDMA_ERR2,
  EDMA_ERR3,
  EDMA_ERR4,
  EDMA_ERR5,
  EDMA_ERR6,
  EDMA_ERR7,
  EDMA_ERR8,
  EDMA_ERR9,
  EDMA_ERR10,
  EDMA_ERR11,
  EDMA_ERR12,
  EDMA_ERR13,
  EDMA_ERR14,
  EDMA_ERR15,
  EDMA_ERR16,
  EDMA_ERR17,
  EDMA_ERR18,
  EDMA_ERR19,
  EDMA_ERR20,
} EDMA_ErrorCode;

#endif /* EDMADRIVER_H_ */
