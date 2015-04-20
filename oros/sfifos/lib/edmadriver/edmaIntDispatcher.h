/*
 * edmaIntDispatcher.h
 *
 * This is a utility function used by edma as an interrupt dispatcher
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/ 
 * 
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

/** ============================================================================
 *
 *   @file  edmaIntDispatcher.h
 *
 *   @path  
 *
 *   @desc  Header file for EDMA interrupt example
 *
 * =============================================================================
 */
 
#ifndef _EDMAINTDISPATCHER_H_
#define _EDMAINTDISPATCHER_H_

#ifdef __cplusplus
extern "C" {
#endif

// if a shadow region is used, define the index value of that region, n
#define SHADOW_REGION	CSL_chipReadReg( CSL_CHIP_DNUM )

// select which region this EDMA dispatcher will be using, CSL_EDMA3_REGION_GLOBAL or a shadow CSL_EDMA3_REGION_n
//#define EDMA_REGION		SHADOW_REGION
#define EDMA_REGION		CSL_EDMA3_REGION_GLOBAL
/*
	The dispatcher can handle race conditions either of two ways. The race condition comes up when one or more EDMA
	interrupt bits are set in IPR at the start of this dispatcher, and then more come in after IPR is read and during
	the processing of the first set of interrupt bits. Without special handling, interrupts could get missed or
	never be activated again.


	1. Re-read IPR after clearing the processed bits. If any bits are set, repeat the processing until IPR == 0, 
	which will guarantee that any new bits will assert the interrupt flag. 
	
	Advantage: if it is likely that more IPR bits will be set after processing other IPR bits, then this loop 
	will save the overhead of another interrupt context save/restore.

	Disadvantage: 2 or 4 ConfigBus reads will occur at the end when no more IPR bits are left, and these are
	very slow reads.


	2. Assert IEVAL = 1 after processing all enabled IPR bits. This will guarantee that any recently-set IPR bits
	will properly assert the interrupt flag. 
	
	Advantage: if it is common for only one EDMA interrupt flag to be asserted at a time, this requires a single 
	ConfigBus write to IEVAL (which is very fast) and avoids four (4) ConfigBus reads of IPR/IPRH/IER/IERH 
	(which are very slow).

	Disadvantage: a race condition is unavoidable that could lead to a double pulse on the EDMAINT line to the
	CPU interrupt input. This can cause an indication of a dropped interrupt and assert the INTERR system 
	interrupt. It is the author's recommendation to mask out the EDMA interrupt from causing the IDROP/INTERR 
	event. This will have to be done outside of this routine, probably in main() or wherever the EDMA 
	system initialization is done.
*/
#define EDMADISPATCH_REREAD_IPR 1
#define EDMADISPATCH_IEVAL      2
#define EDMADISPATCH_METHOD     EDMADISPATCH_IEVAL


/* Macro for tcc handler */
#define InvokeHandle(num)       TccHandlerTable[num]()
typedef void (*EdmaTccHandler)(void);

/* Funtion which registers individual event handlers in a table */
void edmaEventHook(Uint16 ,EdmaTccHandler);

/* Forward declaration */
void edmaIntDispatcher();

#ifdef __cplusplus
}
#endif

#endif  /* _EDMAINTDISPATCHER_H_ */
