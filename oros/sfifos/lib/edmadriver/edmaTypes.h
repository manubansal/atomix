/**
Atomix project, edmaTypes.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
Created on: Jul 26, 2012
*/

#ifndef EDMADRIVER_TYPES_H_
#define EDMADRIVER_TYPES_H_

#include <ti/csl/csl_edma3.h>
#include <ti/csl/csl_edma3Aux.h>

/* A "channel" is defined by the tuple (EDMA3CC_INST_NUM, chNum, chTCC, regionNum, paramNum),
 * that is, the EDMA controller module, channel number, the channel
 * transfer completion code assigned to it, the shadow region number
 * and the parameter RAM number assigned to it. These fields are also
 * maintained by the TI CSL structure for the channel object but we
 * maintain it ourselves to keep the driver API independent of the
 * CSL API. (This decision can be revisited).
 */

/* A "link" is an extension of the channel identification by attaching
 * an owner core to each channel. The owner core is the only core that
 * can use the channel (write-access), that is, set up the PaRAM, start
 * and transfer, query transfer completion and clear transfer completion
 * code. A non-owner core can only query the transfer-completion status
 * (read-only).
 */

typedef struct {
	/** channel fields **/
	Uint32 					moduleInstNum;
	Int32 					regionNum;
	Uint32 					chNum;
	Uint32 					chTCC;

	CSL_Edma3ChannelObj 	chObj;			//memory for storing CSL's ch obj
	CSL_Edma3ChannelHandle 	hChannel;		//pointer to chObj; stupid, yeah
											//that's how CSL's API is set up
	CSL_Edma3ParamHandle   	hParam;

//	/** link fields **/
//	Uint32 					ownerCoreDNUM;
//	Uint32					*src;
//	Uint32					*dst;
//	Uint32					numDblWords;

} EDMA_LinkDescriptor;

typedef EDMA_LinkDescriptor * EDMA_LinkHandle;



#endif /* EDMADRIVER_TYPES_H_ */
