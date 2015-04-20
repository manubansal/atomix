/**
Atomix project, vcpdriver.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef DRV_VCP_H_
#define DRV_VCP_H_

#include <osl/inc/swpform.h>
#include <ti/csl/csl_vcp2.h>
#include <ti/csl/csl_edma3.h>
#include <ti/csl/csl_vcp2.h>
#include <ti/csl/csl_vcp2Aux.h>
#include <ti/csl/csl_tsc.h>
#include <ti/csl/csl_edma3.h>
#include "edmaConfig.h"


#define ERROR(MESSAGE) my_error_handler(__FILE__, __LINE__, MESSAGE)
#define SUCCESS(MESSAGE) my_success_handler(__FILE__, __LINE__, MESSAGE)



extern VCP2Handle VCPDRV_hVcp[4];



/************************************************************************************************************
//////////////////////////////////////////// INTERNAL FUNCTIONS /////////////////////////////////////////////
************************************************************************************************************/
void initVcpUnits();
void initVcpEdma();				//should be used without vcp2_initMore and vcp2_configureEdmaAgain
void initVcpEdmaOnlyTopLevel();	//should be used along with vcp2_initMore and vcp2_configureEdmaAgain_optimized



Uint16 vcp2_configureEdmaOnce();


//CSL_Status vcp2_configureEdmaAgain(
//		Uint32 vcpInstanceNumber, 
//		VCP2_ConfigIc *pVcpConfig,
//		Uint32 *branch_metric, 
//		Uint32 *hard_decision, 
//		Uint32 inputBM, 
//		Uint32 outputHD
//		);
//

//this is the one that's use but it's commented because the function is declared static inline in edmaConfig.h
//CSL_Status vcp2_configureEdmaAgain_optimized_try2(
//		Uint32 vcpInstanceNumber, 
//		VCP2_ConfigIc *pVcpConfig,
//		Uint32 *branch_metric, 
//		Uint32 *hard_decision, 
//		Uint32 inputBM, 
//		Uint32 outputHD
//		);

//CSL_Status vcp2_configureEdmaAgain_optimized(
//		Uint32 vcpin, 
//		VCP2_ConfigIc *pVcpConfig,
//		Uint32 *branch_metric, 
//		Uint32 *hard_decision,
//		Uint32 inputBM, 
//		Uint32 outputHD
//		);

CSL_Status refreshEdmaChannels(Uint32 vcpInstNum);
		
void vcp2_closeEdma();

///////////////////////////////////////////////////////////////////////////

//void bailOut();

void inspectVCP(VCP2Handle hVcp2);

void vcp2_queryEdmaErrorRegisters(
	CSL_Edma3ChannelHandle txChannel,
	CSL_Edma3ChannelHandle rxChannel
	);

void initEdmaOncePerVcpAll(
	VCP2_ConfigIc *pVcpConfig,
	Uint64 *outputParams,
    Uint32 inputBM,
    Uint32 outputHD);

void vcp2_queryEdmaChannelStatus(
	CSL_Edma3ChannelHandle      hChannel
	);

void vcp2_printVcpConfigRegs(VCP2Handle hVcp2);

void vcp2_setupTransferCompletionInterruptMasks();
void vcp2_queryTransferCompletionInterrupt(volatile Uint32 *stat, Uint32 vcpin);
void vcp2_enableTransferCompletionInterrupts();
void vcp2_clearTransferCompletionInterrupt(Uint32 vcpin);

void my_error_handler(const char* file, int line, const char* message);
void my_success_handler(const char* file, int line, const char* message);

Uint16 vcp2_configureEdmaOncePerVcp (
	Uint32 vcpInstanceNumber,
	Uint32 idVCPXEVT,
	Uint32 idVCPREVT, 
	VCP2_ConfigIc *pVcpConfig,
	Uint64 *outputParams,
	Uint32 *branch_metric,
	Uint32 *hard_decision,
    Uint32 inputBM,
    Uint32 outputHD
);



static inline
void vcp2_decode(
					IN	Uint32 vcpInstance,	//which vcp to use
					VCP2_ConfigIc *pVcpConfig,
					IN	Uint32 *branch_metric,
					OUT	Uint32 *hard_decision,		//packed decoded bits
					IN	Uint32 nInputBM,
					IN	Uint32 nOutputHD,
					IN	volatile Uint32 *completionWord,		//memory location to indicate completion
					IN	volatile CSL_Uint64 t[]
					) {
	
	CSL_Status status;
	VCP2Handle hVcpThis;
//	volatile Uint32 numSymProc;

	hVcpThis = VCPDRV_hVcp[vcpInstance];
	
	
	/* Configure EDMA based on those parameters */
	status = vcp2_configureEdmaAgain_optimized_try2(
    		vcpInstance,
		pVcpConfig,
    		branch_metric,
    		hard_decision,
		nInputBM,
		nOutputHD
    		);

    
	if (status != CSL_SOK) {
	    DEBUG_ERROR (
	    printf ("\nError in re-configuring EDMA\n");
	    )
	    return;
	}
	
	VCP2_reset (hVcpThis);



//	numSymProc = VCP2_statSymProc (hVcpThis);
//	if (numSymProc > 0) {
//		//DEBUG_ERROR(
//		printf("\nvcpdriver: numSymProc is non-zero after vcp reset\n");
//		//)
//	}

	VCP2_start (hVcpThis);

}

#endif /*DRV_VCP_H_*/
