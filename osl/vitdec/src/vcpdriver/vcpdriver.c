/**
Atomix project, vcpdriver.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/


#include "../../inc/vcpdriver/vcpdriver.h"
#include "../../inc/vcpdriver/vcpdriver_api.h"
#include "../../inc/vcpdriver/edmaConfig.h"

#include <ti/csl/csl_vcp2.h>
#include <ti/csl/csl_vcp2Aux.h>
#include <ti/csl/csl_edma3.h>
#include <ti/csl/csl_tsc.h>
#include <stdio.h>

#include "../../inc/util.h"

#include <osl/inc/swpform.h>
	

VCP2Obj VCPDRV_vcp2Obj[4];
VCP2Handle VCPDRV_hVcp[4];

volatile Uint32 *rVCPEXE[4];
//volatile Uint32 *rVCPEXE0;

//extern CSL_Edma3ChannelHandle hChannelTx[], hChannelRx[];

#define BITS_IN_A_BYTE 8

/************************************************************************************************************
//////////////////////////////////////////// One time operations ////////////////////////////////////////////
************************************************************************************************************/


/********************************************************
 * init function to be called just once at system boot.
 * This does whole module-level initializations
 * independent of operational parameters.
 *******************************************************/
void vcp2_initOnce() {
	initVcpUnits();
	initVcpEdmaOnlyTopLevel();
}

/********************************************************
 * paramater-specific init function, used to configure
 * edma, hence needs to be called every time operational
 * parameters change, but only once per change of
 * parameters. it configures edma for all the four vcp's.
 * the changing parameters in question are the ones in
 * this function's input signature. however, since this
 * is a heavy, full-fledged call, and sets up all the
 * details of the edma channels, for repeated calling
 * for different packets or different modes within a packet,
 * or just change in input length/output length/input buffer
 * or output buffer, use the lighter call ahead.
 *******************************************************/
void vcp2_initPerUse(
	VCP2_ConfigIc *pVcpConfig,
	Uint64 *outputParams,
    Uint32 inputBM,
    Uint32 outputHD
	) {
	
	initEdmaOncePerVcpAll(
		pVcpConfig,
		outputParams,
	    inputBM,
	    outputHD
	    );
}

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
//		Uint32 outputHD) {
//
//	vcp2_configureEdmaAgain(
//			vcpin,
//			pVcpConfig,
//			branch_metric,
//			hard_decision,
//			inputBM,
//			outputHD);
//}

/********************************************************
 * Clean close function. This should be called once
 * after all vcp use is done.
 *******************************************************/
void vcp2_close() {
	vcp2_closeEdma();
}



/************************************************************************************************************
//////////////////////////////////////////// Every time operations //////////////////////////////////////////
************************************************************************************************************/


//Moving this also to an inline function.

//Every time operations
//void vcp2_decode(
//					IN	Uint32 vcpInstance,	//which vcp to use
//					VCP2_ConfigIc *pVcpConfig,
//					IN	Uint32 *branch_metric,
//					OUT	Uint32 *hard_decision,		//packed decoded bits
//					IN	Uint32 nInputBM,
//					IN	Uint32 nOutputHD,
//					IN	volatile Uint32 *completionWord,		//memory location to indicate completion
//					IN	volatile CSL_Uint64 t[]
//					) {
//	
//	CSL_Status status;
//	VCP2Handle hVcpThis;
//	
////#define DEBUG(x) x
////	DEBUG(
////	static int count = 0;
////
////	printf("vcp2_decode: input branch metrics, count = %d\n", count++);
////	printOctetsDecSigned((Int8 *)branch_metric, 120);
////	)
//
//	hVcpThis = VCPDRV_hVcp[vcpInstance];
//	
//	//t[2] = CSL_tscRead();
//	
//	/* Configure EDMA based on those parameters */
//	status = vcp2_configureEdmaAgain_optimized_try2(
//    		vcpInstance,
//		pVcpConfig,
//    		branch_metric,
//    		hard_decision,
//		nInputBM,
//		nOutputHD
//    		);
//
//	//status = vcp2_configureEdmaAgain(
//	//		vcpInstance,
//	//		pVcpConfig,
//	//		branch_metric,
//	//		hard_decision,
//	//		nInputBM,
//	//		nOutputHD);
//    
//	DEBUG(
//    if (status != CSL_SOK) {
//        printf ("\nError in re-configuring EDMA\n");
//        return;
//    }
//	)
//	
////	refreshEdmaChannels(vcpin);
//	
//    //t[3] = CSL_tscRead();
//    
//	VCP2_reset (hVcpThis);
//
//	/* according to VCP2_reset documentation:
//     * "All registers in the VCP are reset except for the execution register,
//     * endian register, emulation register and other internal registers."
//     * That probably includes nSymProc (stat0 register) since that counter is
//     * not getting reset. Therefore, we need to explicitly reset it to 0.
//     * However, the vcp2 data sheet says stat0's numSymProc part is read only
//     * and is set to 0 after reset. I'm confused.
//     * --MB, 2/14/12
//     */
//
////	hVcpThis->cfgregs->VCPSTAT0 &= 0x000000FF;		//to reset the top 24 bits (corresponding to numSymProc part)
//	
////	volatile Uint32 numSymProc = VCP2_statSymProc(hVcpThis);
////	printf("vcp instance %d: no. of syms proc after reset, just before decode start: %d\n", vcpInstance, numSymProc);
//
//	//As a hack, let's wait till numSymProc does actually get set to 0.
////	while (numSymProc > 0)
////		numSymProc = VCP2_statSymProc(hVcpThis);
////	printf("vcp instance %d: no. of syms proc after reset, just before decode start: %d\n", vcpInstance, numSymProc);
//
//
//
//
////	VCP2_emuDisable (hVcpThis);
//	//t[4] = CSL_tscRead();
//	
//
//	VCP2_start (hVcpThis);
//	//t[5] = CSL_tscRead();
//	
////	printf("------------- hChannelTx ----------\n");
////  vcp2_queryEdmaChannelStatus(hChannelTx[vcpin]);
//}

//static Uint16 log_numSymProc[100];
//static Uint16 log_numSymProcExpected[100];
//static Uint8 log_vcpin[100];
//static Uint32 logidx = 0;

void vcp2_waitForCompletion(
	//IN		VCP2_STATE	*vcpState
	IN	Uint32 nSymProcExpected,
	IN	Uint32 vcpin,
	//OUT	volatile CSL_Uint64 *rettime,
	IN	Uint32 btype
	//OUT	volatile CSL_Uint64 t[]
	) {
	Uint32 i;
	volatile Uint32 numSymProc = 0;
	volatile Uint32 stat;
	volatile CSL_Uint64 tin, tout;
	
	//t[6] = CSL_tscRead();
	
	VCP2Handle hVcpThis;
	
	hVcpThis = VCPDRV_hVcp[vcpin];

	if (nSymProcExpected == 0) {

	}
	else {

		//#ifdef VCP2_BREAK_ON_EDMA_COMPLETION
		if (btype == BREAKTYPE_INT) {
			i = 0;
			do {
				vcp2_queryTransferCompletionInterrupt(&stat, vcpin);
				i++;
			} while (!stat);

			//*rettime = CSL_tscRead();
			//t[7] = *rettime;

			//////	DEBUG(printf("queried completion interrupt register %d times\n", i);)

			vcp2_clearTransferCompletionInterrupt(vcpin);

			//////	DEBUG(printf("querying tc interrupt register after clearing\n");)
			vcp2_queryTransferCompletionInterrupt(NULL, vcpin);

			//ERROR("BREAKTYPE_INT is not well tested.");
			//exit(1);

		}
		else if (btype == BREAKTYPE_SYM) {

			//#else
			//#ifdef VCP2_BREAK_ON_NUM_SYM_PROC
			i = 0;

#ifdef TS_FIFO_WAIT_I
			SYS_TimeStamp_aliased(1141ULL);
#endif
			do {
				i++;
				numSymProc = VCP2_statSymProc(hVcpThis);
				//	} while (VCP2_statSymProc() < vcpState->nSymProcExpected);
			} while (numSymProc < nSymProcExpected);

			//*rettime = CSL_tscRead();
			//t[7] = *rettime;
			//#else
			//#error: no vcp2 break condition specified
			//#endif
			//#endif

			//Add a wait for edma transfer completion
			Uint64 N_CYCLES_TO_WAIT = 400;
#warning An empirical delay is used in vcp wait to account for edma transfer completion. This parameter may need adjustment. Too small a value can cause race conditions in using vitdec's output.
			tin = CSL_tscRead();
			do {
				tout = CSL_tscRead();
			} while (tout < tin + N_CYCLES_TO_WAIT);
#ifdef TS_FIFO_WAIT_I
			SYS_TimeStamp_aliased(1142ULL);
#endif

		}
		else {
			ERROR("Invalid break type condition.");
		}
	}
//	log_vcpin[logidx] = vcpin;
//	log_numSymProcExpected[logidx] = nSymProcExpected;
//	log_numSymProc[logidx++] = numSymProc;

	//t[8] = CSL_tscRead();
	//    DEBUG(printf ("VCP2 processing of data over\n");)
	//    DEBUG(printf ("\nRESULT\n");)
	//    DEBUG(printf ("~~~~~~\n");)
}

//Every time operation upon decode completion
void vcp2_postDecodeStat(
			Uint32 vcpin, 
			Uint32 *hard_decision, 
			Uint32 f) {
				
	VCP2Handle hVcpThis;
	Uint32 numSymProc;
	//Uint32 index = 0;
	VCP2_Errors error;
	
	hVcpThis = VCPDRV_hVcp[vcpin];

	//for (index = 0; index < (ceil((f / BITS_IN_A_BYTE), 4)); index++) {
	//	printBitsMsb(hard_decision[index]);
	//	printf("%d\n", hard_decision[index]);
	//}

    if (VCP2_statError (hVcpThis)) {
          VCP2_getErrors (hVcpThis, &error);
          printf("tbnaErr: %d\n", error.tbnaErr);
    	  printf("ftlErr : %d\n", error.ftlErr);
    	  printf("fctlErr: %d\n", error.fctlErr);
    	  printf("maxmiErr: %d\n",error.maxminErr);
    	  printf("symxErr: %d\n", error.symxErr);
    	  printf("symrErr: %d\n", error.symrErr);
    } // end if 

////    printf("Edma error register status after vcp processing:\n");
////    vcp2_queryEdmaErrorRegisters(hChannelTx[vcpin], hChannelRx[vcpin]);
////    printf("----------- hChannelTx ------------\n");
////    vcp2_queryEdmaChannelStatus(hChannelTx[vcpin]);
////    printf("----------- hChannelRx ------------\n");
////    vcp2_queryEdmaChannelStatus(hChannelRx[vcpin]);	



	numSymProc = VCP2_statSymProc (hVcpThis);
	printf("nSymProc on instance %d: %d\n", 
			vcpin, numSymProc);

//////	inspectVCP(hVcpThis);

//////    printf("configuration registers after vcp operation\n");
//////    vcp2_printVcpConfigRegs(hVcpThis);
}
