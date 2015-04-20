/**
Atomix project, vcpdriver_api.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
Created on: Jan 7, 2012
*/

#ifndef VCPDRIVER_API_H_
#define VCPDRIVER_API_H_

#include <osl/inc/swpform.h>
#include <osl/vitdec/inc/vitdec/vitdec_profile.h>
#include <ti/csl/csl_vcp2.h>
#include <ti/csl/csl_edma3.h>
#include "vcpdriver.h"

enum {
	BREAKTYPE_INT,
	BREAKTYPE_SYM
};

/************************************************************************************************************
//////////////////////////////////////////// API FUNCTIONS //////////////////////////////////////////////////
************************************************************************************************************/


/***********************************************************
/////////////////// One time operations ////////////////////
***********************************************************/

/********************************************************
 * VCP configuration generation function. This generates
 * the configuration structure for VCP given a viterbi
 * decoding profile of parameters.
 *******************************************************/
void vcp2_genConf(
					INOUT 		Vitdec_Profile* vdp,
					OUT		VCP2_ConfigIc* pVcpConfig
					);


/********************************************************
 * init function to be called just once at system boot.
 * This does whole module-level initializations
 * independent of operational parameters.
 *******************************************************/
void vcp2_initOnce();

/********************************************************
 * paramater-specific init function, used to configure
 * edma, hence needs to be called every time operational
 * parameters change, but only once per change of
 * parameters.
 *******************************************************/
void vcp2_initPerUse(
					VCP2_ConfigIc *pVcpConfig,
					Uint64 *outputParams,
					Uint32 inputBM,
					Uint32 outputHD
					);


///********************************************************
// * A lighter reconfiguration call that is supposed to be
// * used repeatedly. Also, this call sets only the specified
// * vcp's parameters, not all vcps'.
// *******************************************************/
//void vcp2_updateSingleVcpSpecifics(
//		Uint32 vcpin,
//		VCP2_ConfigIc *pVcpConfig,
//		Uint32 *branch_metric,
//		Uint32 *hard_decision,
//		Uint32 inputBM,
//		Uint32 outputHD);

/********************************************************
 * Clean close function. This should be called once
 * after all vcp use is done.
 *******************************************************/
void vcp2_close();


/***********************************************************
/////////////////// Every time operations //////////////////
***********************************************************/


void vcp2_decode (
					IN	Uint32 vcpInstanceNumber,				//which vcp to use
					VCP2_ConfigIc *pVcpConfig,					//vcp configuration generated earlier
					IN	Uint32 *branch_metric,					//input branch metric array
					OUT	Uint32 *hard_decision,					//packed hard-decision bit array
					IN	Uint32 inputBM,							//number of input branch metrics (unit?)
					IN	Uint32 outputHD,						//number of output hard decisions (unit?)
					IN	volatile Uint32 *completionWord,		//memory location to indicate completion
					IN	volatile CSL_Uint64 t[]					//debug timestamp array (size needed?)
					);

/***********************************************************
/////////////////// Debugging functions (to be removed) ////
***********************************************************/

/********************************************************
 * Wait on a vcp to finish decoding. You could pass either
 * BREAKTYPE_INT (transfer completion INTerrupt) or
 * BREAKTYPE_SYM (numbe of SYMbols processed) to the
 * btype parameter. If BREAKTYPE_SYM is used, nSymProcExpected
 * should be set of the number of symbols after which vcp
 * execution is considered completed.
 *
 * Note: Interrupt detection is probably buggy.
 *******************************************************/
void vcp2_waitForCompletion(
					IN	Uint32 nSymProcExpected,
					IN	Uint32 vcpInstance,
//					OUT	volatile CSL_Uint64 *rettime,
					IN	Uint32 btype
//					OUT	volatile CSL_Uint64 t[]
					);

/********************************************************
 * A debug function to print out stats of vcp debug
 * registers and print out decoded bits in an easy-to-read
 * format.
 *******************************************************/
void vcp2_postDecodeStat(
				Uint32 vcpInstanceNumber,
				Uint32 *hard_decision,
				Uint32 nDecisionBits);




#endif /* VCPDRIVER_API_H_ */
