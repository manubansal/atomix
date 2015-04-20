/**
Atomix project, vcpConfig.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "../../inc/vcpdriver/vcpdriver.h"
#include "../../inc/vcpdriver/vcpdriver_api.h"
#include <osl/inc/swpform.h>
#include <stdio.h>
#include <ti/csl/csl_vcp2.h>
#include <ti/csl/csl_vcp2Aux.h>
#include <ti/csl/csl_tsc.h>
#include <assert.h>

#define CONSTRAINT_LEN 7
#define BITS_IN_A_BYTE 8

extern VCP2Obj VCPDRV_vcp2Obj[4];
extern VCP2Handle VCPDRV_hVcp[4];
extern CSL_Uint64 t[];
extern volatile Uint32 *rVCPEXE[4];


void vcp2_genConf(
					INOUT 	Vitdec_Profile* vdp,
					OUT		VCP2_ConfigIc* pVcpConfig
					) {
	Uint32 outputHD_bytes, outputHD_words;

	VCP2_Params vcpParams;
    VCP2_BaseParams vcpBaseParams;

	ASSERT_PTR_ALIGNED(pVcpConfig, 8);

    t[2] = CSL_tscRead();

    /* VCP configuration */

    vcpBaseParams.rate 			= vdp->rate;
    vcpBaseParams.constLen 		= vdp->constLen;
    vcpBaseParams.frameLen		= vdp->frameLen;
    vcpBaseParams.yamTh 		= vdp->yamTh;
    vcpBaseParams.stateNum 		= vdp->stateNum;
    vcpBaseParams.tbConvrgMode 	= vdp->tbConvrgMode;
    vcpBaseParams.decision 		= vdp->decision;
    vcpBaseParams.readFlag 		= vdp->readFlag;
    vcpBaseParams.tailBitEnable = vdp->tailBitEnable;
    vcpBaseParams.traceBackIndex= vdp->traceBackIndex;
    vcpBaseParams.outOrder 		= vdp->outOrder;
    vcpBaseParams.perf 			= vdp->perf;

    /* VCP configuration */
    VCP2_genParams (&vcpBaseParams, &vcpParams);
    t[5] = CSL_tscRead();

    vcpParams.poly0 			= vdp->poly0;
    vcpParams.poly1 			= vdp->poly1;
    vcpParams.poly2 			= vdp->poly2;
    vcpParams.poly3 			= vdp->poly3;

    VCP2_genIc (&vcpParams, pVcpConfig);

    t[6] = CSL_tscRead();

    /* Number of EDMA frames (transfers) for the branch metrics */
    vdp->inputBM_frames 		= vcpParams.numBmFrames;

    t[7] = CSL_tscRead();
 /*   DEBUG(
    printf("derived: inputBM      : %d\n", vdp->inputBM_frames);
    printf("derived: relLen R     : %d\n", vcpParams.relLen);
    printf("derived: convDist C   : %d\n", vcpParams.convDist);
    printf("derived: traceBack    : %d\n", vcpParams.traceBack);
    ) */

    t[8] = CSL_tscRead();

    vdp->r 						= vcpParams.relLen;
    vdp->c 						= vcpParams.convDist;

    if (vdp->tbConvrgMode) {
    	if (vdp->c_guess != vdp->c) {
		DEBUG_ERROR(
    		printf("ERROR: convergence lengths c_guess and c in the vdp don't match. Check your settings.\n");
    		printf("ERROR: c_guess: %d  c: %d\n", vdp->c_guess, vdp->c);
		)
    	}
    	assert(vdp->c_guess == vdp->c);
    }

    /* Output hard decisions in terms of bytes rounded to upper byte */
    outputHD_bytes =
        VCP2_normalCeil (vcpBaseParams.frameLen + vcpBaseParams.constLen - 1,
        BITS_IN_A_BYTE);

    /* Output hard decisions in terms of words rounded to upper words */
    outputHD_words = VCP2_normalCeil (outputHD_bytes, sizeof (Uint32));

    /* Total number of bytes required to be transfered for the hard decisions */
    vdp->outputHD_bytes_rounded = outputHD_words * sizeof (Uint32);

    t[9] = CSL_tscRead();
}



void initVcpUnit(VCP2Handle hVcp2, Uint32 instNum, int *pStatus) {
    VCP2_PID vcpPid;
    VCP2_PID* hPid = &vcpPid; 
    
    VCP2_init(hVcp2, instNum, (int *)pStatus);
    DEBUG_INIT(printf("VCP2_init status: %d\n", *pStatus);)
    DEBUG_INIT(printf("hVcp2: %d\n", hVcp2);)
    DEBUG_INIT(printf("hVcp2->perNum: %d\n", hVcp2->perNum);)

    VCP2_getPeripheralID(hVcp2, hPid);
    
    DEBUG_INIT(
    printf("vcp->pidclass: %d\n", hPid->pid_class);
    printf("vcp->type    : %d\n", hPid->type);
    printf("vcp->rev     : %d\n", hPid->rev);
    )
    
    //manu
    #ifdef _BIG_ENDIAN
    VCP2_setBmEndian(hVcp2, VCP2_END_NATIVE);
    #else
    #error: little-endian is not supported
    #endif    
    

    /* Enable the VCP2 in power sleep controller */
//    enableVCP();		////!!!!!!!!!!!!!! TODO
}


void initVcpUnits() {
	int status;
	
	VCPDRV_hVcp[0] = &VCPDRV_vcp2Obj[0];
	VCPDRV_hVcp[1] = &VCPDRV_vcp2Obj[1];
	VCPDRV_hVcp[2] = &VCPDRV_vcp2Obj[2];
	VCPDRV_hVcp[3] = &VCPDRV_vcp2Obj[3];
	
	
	
	initVcpUnit(VCPDRV_hVcp[0], 0, &status); 
	if (CSL_SOK != status) {
		DEBUG_INIT(printf("status: %d\n", status);)
		ERROR("initVcpUnit 0");
	}
	
	initVcpUnit(VCPDRV_hVcp[1], 1, &status); 
	if (CSL_SOK != status) {
		DEBUG_INIT(printf("status: %d\n", status);)
		ERROR("initVcpUnit 1");
	}
	
	initVcpUnit(VCPDRV_hVcp[2], 2, &status); 
	if (CSL_SOK != status) {
		DEBUG_INIT(printf("status: %d\n", status);)
		ERROR("initVcpUnit 2");
	}
	
	initVcpUnit(VCPDRV_hVcp[3], 3, &status); 
	if (CSL_SOK != status) {
		DEBUG_INIT(printf("status: %d\n", status);)
		ERROR("initVcpUnit 3");
	}
	
	rVCPEXE[0] = &(VCPDRV_hVcp[0]->cfgregs->VCPEXE);
	rVCPEXE[1] = &(VCPDRV_hVcp[1]->cfgregs->VCPEXE);
	rVCPEXE[2] = &(VCPDRV_hVcp[2]->cfgregs->VCPEXE);
	rVCPEXE[3] = &(VCPDRV_hVcp[3]->cfgregs->VCPEXE);
}
