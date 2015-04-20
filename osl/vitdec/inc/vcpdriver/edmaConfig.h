/**
Atomix project, edmaConfig.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef EDMACONFIG_H_
#define EDMACONFIG_H_

#include <osl/inc/swpform.h>
#include <ti/csl/csl_vcp2.h>
#include <ti/csl/csl_vcp2Aux.h>
#include <ti/csl/csl_tsc.h>
#include <ti/csl/csl_edma3.h>
#ifdef DEVICE_K2K
#include <ti/csl/device/k2k/src/csl_device_interrupt.h>
#endif
/* EDMA frame size */
#define VCPDRV_DMA_BURST_SIZE  128

/* VCP input configuration transfer size */
#define VCPDRV_VCPIC_DMA_SIZE  24

/* VCP out register transfer size */
#define VCPDRV_VCPOUT_DMA_SIZE  8

#define VCPDRV_EDMA_INSTANCE_NUMBER	2
//using 2 because that's the edma instance tied to vcp events

extern VCP2Handle VCPDRV_hVcp[];


extern Uint32 * VCPDRV_pVcAddrWord[4];
extern Uint32 * VCPDRV_pBmAddrWord[4];
extern Uint32 * VCPDRV_pBmABCntWord[4];
extern Uint32 * VCPDRV_pHdAddrWord[4];
extern Uint32 * VCPDRV_pHdABCntWord[4];
extern Uint32 * VCPDRV_pHdSrcBIdxWord[4];

static inline
CSL_Status vcp2_configureEdmaAgain_optimized_try2(
		Uint32 vcpin, 
		VCP2_ConfigIc *pVcpConfig,
		Uint32 *branch_metric, 
		Uint32 *hard_decision,
		Uint32 inputBM, 
		Uint32 outputHD
		) {
	
    ASSERT_PTR_ALIGNED(pVcpConfig, 8);
    
    *(VCPDRV_pVcAddrWord[vcpin]) = (Uint32) pVcpConfig;
    
    *(VCPDRV_pBmAddrWord[vcpin]) = (Uint32) branch_metric;

    *(VCPDRV_pBmABCntWord[vcpin]) = CSL_EDMA3_CNT_MAKE (VCPDRV_DMA_BURST_SIZE, inputBM);



    *(VCPDRV_pHdABCntWord[vcpin]) = CSL_EDMA3_CNT_MAKE (outputHD, 1);

    *(VCPDRV_pHdAddrWord[vcpin]) = (Uint32) hard_decision;

    *(VCPDRV_pHdSrcBIdxWord[vcpin]) = CSL_EDMA3_BIDX_MAKE (0, outputHD);
	
    
    return CSL_SOK;
}

#endif

