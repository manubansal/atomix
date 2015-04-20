/**
Atomix project, debug.c, Debug and diagnosis functions for VCP and EDMA.
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include "../../inc/vcpdriver/vcpdriver.h"
#include <string.h>
#include <stdio.h>
#include <ti/csl/csl_vcp2.h>
#include <ti/csl/csl_vcp2Aux.h>
#include <ti/csl/csl_edma3.h>

volatile CSL_Uint64 t[20];

//void bailOut() {
//	printf("bailing out\n");
//}

void my_error_handler(const char* file, int line, const char* message) {
    printf("ERROR: file: %s, line: %d, message: %s\n",
    	file, line, message);
}

void my_success_handler(const char* file, int line, const char* message) {
    printf("SUCCESS: file: %s, line: %d, message: %s\n",
    	file, line, message);
}


void inspectVCP(VCP2Handle hVcp2) {
//	#ifdef DEBUG_MODE
	Uint32 e1, e2, e3, e4, e5, e6, e7;
	e1 = VCP2_statError(hVcp2);
	e2 = VCP2_statPause(hVcp2);
	e3 = VCP2_statRun(hVcp2);
	e4 = VCP2_statWaitIc(hVcp2);
	e5 = VCP2_statInFifo(hVcp2);
	e6 = VCP2_statOutFifo(hVcp2);
	e7 = VCP2_statSymProc(hVcp2);
//	#endif
	
	DEBUG(
	printf("vcp inspect: statError  : %d\n", e1);
	printf("vcp inspect: statPause  : %d\n", e2);
	printf("vcp inspect: statRun    : %d\n", e3);
	printf("vcp inspect: statWaitIc : %d\n", e4);
	printf("vcp inspect: statInFifo : %d\n", e5);
	printf("vcp inspect: statOutFifo: %d\n", e6);
	printf("vcp inspect: statSymProc: %d\n", e7);
	
	printf("vcp inspect: VCPSTAT0   : 0x%08X\n", hVcp2->cfgregs->VCPSTAT0);
	printf("vcp inspect: VCPSTAT1   : 0x%08X\n", hVcp2->cfgregs->VCPSTAT1);
	)
}

void printVcpConfig(VCP2_ConfigIc *vcpConfig) {
	printf("vcp config ic0: 0x%0x\n", vcpConfig->ic0);
	printf("vcp config ic1: 0x%0x\n", vcpConfig->ic1);
	printf("vcp config ic2: 0x%0x\n", vcpConfig->ic2);
	printf("vcp config ic3: 0x%0x\n", vcpConfig->ic3);
	printf("vcp config ic4: 0x%0x\n", vcpConfig->ic4);
	printf("vcp config ic5: 0x%0x\n", vcpConfig->ic5);
}

void vcp2_printVcpConfigRegs(VCP2Handle hVcp2) {
	printf("vcp config reg0: 0x%0x\n", hVcp2->regs->VCPIC0);
	printf("vcp config reg1: 0x%0x\n", hVcp2->regs->VCPIC1);
	printf("vcp config reg2: 0x%0x\n", hVcp2->regs->VCPIC2);
	printf("vcp config reg3: 0x%0x\n", hVcp2->regs->VCPIC3);
	printf("vcp config reg4: 0x%0x\n", hVcp2->regs->VCPIC4);
	printf("vcp config reg5: 0x%0x\n", hVcp2->regs->VCPIC5);
}

void vcp2_queryEdmaErrorRegisters(
	CSL_Edma3ChannelHandle txChannel,
	CSL_Edma3ChannelHandle rxChannel
	) {
	Uint32 errStat;
	CSL_edma3GetHwChannelStatus(txChannel ,CSL_EDMA3_QUERY_CHANNEL_ERR, &errStat);
	DEBUG(printf("tx channel errStat: %d\n", errStat);)
	CSL_edma3GetHwChannelStatus(rxChannel, CSL_EDMA3_QUERY_CHANNEL_ERR, &errStat);
	DEBUG(printf("rx channel errStat: %d\n", errStat);)
}


void vcp2_queryEdmaChannelStatus(
	CSL_Edma3ChannelHandle      hChannel
	){
	
	Bool stat;
	CSL_Edma3ChannelErr 		chErr;
	
	CSL_edma3GetHwChannelStatus(hChannel,CSL_EDMA3_QUERY_CHANNEL_STATUS, &stat);
	printf("channel ER     stat: 0x%08X\n", stat);


	CSL_edma3GetHwChannelStatus(hChannel,CSL_EDMA3_QUERY_CHANNEL_ERR, &chErr);
	printf("channel missed stat: 0x%08X\n", chErr.missed);
 	printf("channel secEvt stat: 0x%08X\n", chErr.secEvt);
 	
 	printf("channel ER reg     : 0x%08X\n", 
 		CSL_FEXTR(hChannel->regs->TPCC_ER, hChannel->chaNum, hChannel->chaNum));

 	printf("channel ERH reg    : 0x%08X\n", 
 		CSL_FEXTR(hChannel->regs->TPCC_ERH, hChannel->chaNum, hChannel->chaNum));

 	printf("channel EER reg    : 0x%08X\n", 
 		CSL_FEXTR(hChannel->regs->TPCC_EER, hChannel->chaNum, hChannel->chaNum));
 	printf("channel EERH reg   : 0x%08X\n", 
 		CSL_FEXTR(hChannel->regs->TPCC_EERH, hChannel->chaNum, hChannel->chaNum));

 	printf("channel EESR reg   : 0x%08X\n", 
 		CSL_FEXTR(hChannel->regs->TPCC_EESR, hChannel->chaNum, hChannel->chaNum));

 	printf("channel EMR reg    : 0x%08X\n", 
 		CSL_FEXTR(hChannel->regs->TPCC_EMR, hChannel->chaNum, hChannel->chaNum));

 	printf("channel EMRH reg   : 0x%08X\n", 
 		CSL_FEXTR(hChannel->regs->TPCC_EMRH, hChannel->chaNum, hChannel->chaNum));

 	printf("channel SER reg    : 0x%08X\n", 
 		CSL_FEXTR(hChannel->regs->TPCC_SER, hChannel->chaNum, hChannel->chaNum));


}
