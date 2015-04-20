/**
Atomix project, edmaDriver.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
Created on: Jul 14, 2012
*/

#include <ti/csl/csl_tsc.h>
#include <stdio.h>
#include <string.h>
#include <ti/csl/csl_edma3.h>
#include <ti/csl/csl_edma3Aux.h>
#include <ti/csl/src/intc/csl_intc.h>
#include "edmaDriver.h"
#include "edmaIntDispatcher.h"
#include "timestamp.h"
#include <c6x.h>
#include <osl/inc/swpform.h>

//declaring all these globals are far variables because otherwise they don't fit in the .bss section. --MB, 12/28/2012

//---------edma related parameters-------------
//CSL_Edma3HwSetup            hwSetup;
//CSL_Edma3Obj                edmaObj;
far CSL_Edma3Obj                EDR_edmaObj[EDMA_NUM_MODULES];


far CSL_Edma3CmdIntr            EDR_regionIntr;
far CSL_Edma3CmdDrae            EDR_regionAccess;


far CSL_Edma3Context            EDR_edmaContext;



////CSL_Edma3HwDmaChannelSetup	dmahwSetup[CSL_EDMA3_NUM_DMACH] =
//CSL_Edma3HwDmaChannelSetup	dmahwSetup[64] =
//								CSL_EDMA3_DMACHANNELSETUP_DEFAULT;
////CSL_Edma3HwQdmaChannelSetup qdmahwSetup[CSL_EDMA3_NUM_QDMACH] =
//CSL_Edma3HwQdmaChannelSetup qdmahwSetup[8] =
//								CSL_EDMA3_QDMACHANNELSETUP_DEFAULT;

//int tccCh0;

//EDMA_LinkDescriptor linkDescriptors[EDMA_NUM_LINKS_TO_SETUP];


//volatile Uint32      bEdmaCh0Complete = 0;


/* Edma handle */
far CSL_Edma3Handle     EDR_hModule[EDMA_NUM_MODULES];

//---------edma related parameters-------------


far Uint32 EDR_coreNum = 0;
//Uint32 RegionNum;
//Int32 RegionNum;
far Uint16						EDR_paramNum;

far unsigned long long			EDR_llTemp;

far CSL_Status                  	EDR_status;

/* Intc declaration */
far CSL_IntcContext                 EDR_intcContext;
far CSL_IntcEventHandlerRecord      EDR_EventHandler[30];
far CSL_IntcObj                     EDR_intcObjEdma;
far CSL_IntcHandle                  EDR_hIntcEdma;

far CSL_IntcGlobalEnableState       EDR_state;
far CSL_IntcEventHandlerRecord      EDR_EventRecord;
far CSL_IntcParam                   EDR_vectId;


//// ISR for the EDMA interrupt for logical Channel 0, sets bEdmaCh0Complete = 1
//void tccCh0Fxn(void)
//{
////	printf("tccCh0 ISR invoked\n");
//    bEdmaCh0Complete = 1;
//}

void bailOut(Uint32 errCode) {
	printf("EDMA driver: bailing out, err %d\n", errCode);
}


void edmaClearErrorRegisters(
		CSL_Edma3ChannelHandle      hChannel
) {
	CSL_Edma3ChannelErr 		chErrClear;

	chErrClear.missed = TRUE;
	chErrClear.secEvt = TRUE;
	CSL_edma3HwChannelControl (hChannel, CSL_EDMA3_CMD_CHANNEL_DISABLE, NULL);
	CSL_edma3HwChannelControl (hChannel, CSL_EDMA3_CMD_CHANNEL_CLEARERR, &chErrClear);
	CSL_edma3HwChannelControl (hChannel, CSL_EDMA3_CMD_CHANNEL_CLEAR, NULL);
	CSL_edma3HwChannelControl (hChannel, CSL_EDMA3_CMD_CHANNEL_ENABLE, NULL);
}






void edmaQueryErrorRegisters(
		//	CSL_Edma3ChannelHandle      hChannel
		EDMA_LinkHandle hL
) {
	Uint32 errStat;
	CSL_BitMask32 evtMissedStat[3];

	//	CSL_edma3GetHwChannelStatus(hChannel,CSL_EDMA3_QUERY_CHANNEL_ERR, &errStat);
	CSL_edma3GetHwChannelStatus(hL->hChannel,CSL_EDMA3_QUERY_CHANNEL_ERR, &errStat);
	printf("channel errStat: 0x%08X\n", errStat);


	//	CSL_edma3GetHwStatus(hModule, CSL_EDMA3_QUERY_EVENTMISSED, evtMissedStat);
	CSL_edma3GetHwStatus(EDR_hModule[hL->moduleInstNum], CSL_EDMA3_QUERY_EVENTMISSED, evtMissedStat);
	printf("edma module event missed stat 0: 0x%08X\n", evtMissedStat[0]);
	printf("edma module event missed stat 1: 0x%08X\n", evtMissedStat[1]);
	printf("edma module event missed stat 2: 0x%08X\n", evtMissedStat[2]);
}

void edmaQueryChannelStatus(
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


void edmaQueryTransferCompletionInterrupt(EDMA_LinkHandle hL) {

	CSL_Edma3CmdIntr 			transferCompletionInterruptMask;
	volatile Uint32              intr;
	volatile Uint32              intrh;
	Uint32				tcc;

	transferCompletionInterruptMask.region = EDMA_REGION;

	printf("querying tc interrupt register\n");

	CSL_edma3GetHwStatus(EDR_hModule[hL->moduleInstNum], CSL_EDMA3_QUERY_INTRPEND, &transferCompletionInterruptMask);

	printf("interrupt pending status: intrh: 0x%08X  intr: 0x%08X\n",
			transferCompletionInterruptMask.intrh, transferCompletionInterruptMask.intr);

	// clear any set & enabled IPR bits quickly to allow repetition
	//    CSL_edma3HwControl(hModule,CSL_EDMA3_CMD_INTRPEND_CLEAR,&transferCompletionInterruptMask);

	intr  = transferCompletionInterruptMask.intr;
	intrh = transferCompletionInterruptMask.intrh;

	printf("pending interrupts:\n");
	for ( tcc = 0; intr != 0; intr >>= 1, tcc++ )
		if ( intr & 1 )
			printf("%d ", tcc);

	for ( tcc = 32; intrh != 0; intrh >>= 1, tcc++ )
		if ( intrh & 1 )
			//            InvokeHandle( tcc );
			printf("%d ", tcc);
	printf("\n");

}

//void edmaClearTransferCompletionInterrupt() {
//	DEBUG(printf("clearing tc interrupt register\n");)
//	CSL_edma3HwControl (hModule, CSL_EDMA3_CMD_INTRPEND_CLEAR, &transferCompletionInterruptMask);
//}

void edmaQueryActivityStat(Uint32 moduleInstNum) {
	CSL_Edma3ActivityStat	stat;

	CSL_edma3GetHwStatus(EDR_hModule[moduleInstNum], CSL_EDMA3_QUERY_ACTIVITY, &stat);
	printf("edma module activity status	\n");
	printf("active          %d\n", stat.active);
	printf("evtActive       %d\n", stat.evtActive);
	printf("outstandingTcc  %d\n", stat.outstandingTcc);
	printf("qevtActive      %d\n", stat.qevtActive);
	printf("queActive       %d\n", stat.queActive);
	printf("trActive        %d\n", stat.trActive);
}

void intcQueryStatus() {
	Uint32 stat;
	CSL_intcGetHwStatus(EDR_hIntcEdma, CSL_INTC_QUERY_PENDSTATUS, &stat);
	printf("CSL_INTC_QUERY_PENDSTATUS	: 0x%08X\n", stat);
}

//void edmaSimpleChannelSetup(
//		EDMA_LinkDescriptor chD[],
//		Uint32 nCh,
//		Uint32 moduleInstNum
//		) {
//	Uint32 i;
//	for (i = 0; i < nCh; i++) {
//		chD[i].moduleInstNum = moduleInstNum;
//		chD[i].regionNum = CSL_EDMA3_REGION_GLOBAL;
//		chD[i].chNum = i;
//		chD[i].chTCC = i;
//
//
//		//////    /* Edma module setup sets default paramnum and queue for ALL dma and qdma channels */
//		//////    dmahwSetup[i].paramNum = 0;    // no need to change paramNum from default, but you could do it here
//		//////	  dmahwSetup[i].que = CSL_EDMA3_QUE_0;
//	}
//}

void edmaInitLinks(
		EDMA_LinkDescriptor dL[],
		Uint32 nL
) {
	Uint32 i;
	CSL_Edma3ChannelAttr        chAttr;
	//	CSL_Edma3ChannelObj         chObj;
	CSL_Edma3ChannelHandle      hChannel;
	CSL_Edma3ParamHandle        hParam;

	CSL_Status                  status;
	Uint16						paramNum;

	Uint32 moduleInstNum;

	for (i = 0; i < nL; i++) {
		/* Channel open */
		moduleInstNum = dL[i].moduleInstNum;
		chAttr.regionNum = dL[i].regionNum;
		chAttr.chaNum = dL[i].chNum;

		//		hChannel = CSL_edma3ChannelOpen(&chObj, moduleInstNum, &chAttr, &status);
		hChannel = CSL_edma3ChannelOpen(&(dL[i].chObj), moduleInstNum, &chAttr, &status);
		if ( (hChannel == NULL) || (status != CSL_SOK)) { bailOut(EDMA_ERR1); return; }

		/* Store the channel handle */
		dL[i].hChannel = hChannel;

		/* Get the PaRAM number associated with this channel */
		status = CSL_edma3GetHwChannelSetupParam(hChannel,&paramNum);
		if (status != CSL_SOK) { bailOut(EDMA_ERR2); return; }

		/* Get a handle to the param set */
		hParam = CSL_edma3GetParamHandle(hChannel,paramNum,&status);
		if (hParam == NULL) { bailOut(EDMA_ERR3); return; }

		/* Store the PaRAM handle */
		dL[i].hParam = hParam;

		/* Clear error registers, otherwise they will block the channel */
		edmaClearErrorRegisters(hChannel);
	}
}

//evenly distribute the total number of available links
//into all cores
//void edmaSetLinkOwners(
//		EDMA_LinkDescriptor dL[],
//		Uint32 nL,
//		Uint32 nCores
//		) {
//
//	Uint32 nLC, i;
//
//	ASSERT(nL % nCores == 0);
//	nLC = nL/nCores;
//	for (i = 0; i < nL; i++) {
//		dL[i].ownerCoreDNUM = i/nLC;
//	}
//}

void edmaInit() {
	Uint32 i = 0;
	CSL_Status                  status;
	//	printf("edmaInit\n");

	//--------------------------do edma setup-----------------------//

	//	RegionNum = EDMA_REGION;

	/* Module initialization */
	status = CSL_edma3Init(&EDR_edmaContext);
	if (status != CSL_SOK) { bailOut(EDMA_ERR4); return;}

	//    EDR_hModule = CSL_edma3Open(&edmaObj,EDMA_MODULE_INSTANCE_NUM,NULL,&status);
	//    if ( (hModule == NULL) || (status != CSL_SOK)) { bailOut(); return; }

	/* Edma module open */
	for (i = 0; i < EDMA_NUM_MODULES; i++) {

		EDR_hModule[i] = CSL_edma3Open(&EDR_edmaObj[i],i ,NULL,&status);
		if ( (EDR_hModule == NULL) || (status != CSL_SOK)) { bailOut(EDMA_ERR5); return; }
	}

	for (i = 0; i < EDMA_NUM_AVAILABLE_LINKS; i++) {
		EDMA_linkHandles[i] = &EDMA_linkDescriptors[i];
		if (edmaLinkOpenSpecific(EDMA_linkHandles[i], i) < 0) {
			printf("ERROR: EDMA driver: Cannot open edma link\n");
			exit(1);
		}
	}

	////    tccCh0 = 3;		//channel 3
	//    edmaSimpleChannelSetup(
	//    		linkDescriptors,
	//    		EDMA_NUM_LINKS_TO_SETUP,
	//    		EDMA_MODULE_INSTANCE_NUM
	//    		);

	//    hwSetup.dmaChaSetup = dmahwSetup;
	//    hwSetup.qdmaChaSetup = qdmahwSetup;

	//I don't think this is necessary unless we want to modify the default setup. --MB
	//    status = CSL_edma3HwSetup(hModule,&hwSetup);
	//    if (status != CSL_SOK) { bailOut(); return; }

	//    edmaInitLinks(
	//    		linkDescriptors,
	//    		EDMA_NUM_LINKS_TO_SETUP
	//    		);
	//
	//    edmaSetLinkOwners(
	//    		linkDescriptors,
	//    		EDMA_NUM_LINKS_TO_SETUP,
	//    		EDMA_NUM_CORES
	//    		);

	// We don't use interrupts. --MB, 07/25/12
	//    /* Enable interrupts (EDMA3's IER) */
	////    regionIntr.region = RegionNum;
	//    regionIntr.region = CSL_EDMA3_REGION_GLOBAL;
	//
	////    llTemp  = (unsigned long long)1 << tccCh0;
	////    regionIntr.intr = _loll(llTemp);
	////    regionIntr.intrh = _hill(llTemp);
	//
	//    regionIntr.intr = 0xFFFFFFFF;
	//    regionIntr.intrh = 0xFFFFFFFF;
	//
	//    status = CSL_edma3HwControl(hModule,CSL_EDMA3_CMD_INTR_ENABLE,&regionIntr);
	//    //Note: Interrupts from all EDMA channels actually map to the same event: CSL_INTC_EVENTID_EDMA3CC_<region>,
	//    //but they can be distinguished in the application interrupt dispatcher by querying the interrupt pending
	//    //register (IPR) of EDMA3CC to learn which channels have enabled interrupts.
	//
	//    if (status != CSL_SOK) { bailOut(); return; }

}

//void intcInit() {
//
////	printf("intcInit\n");
//
//	//--------------------------do intc setup-----------------------//
//	/* Intc module initialization */
//    intcContext.eventhandlerRecord = EventHandler;
//    intcContext.numEvtEntries = 30;
//    CSL_intcInit(&intcContext);
//
//    /* Enable NMIs */
//    CSL_intcGlobalNmiEnable();
//
//    /* Enable global interrupts */
//    CSL_intcGlobalEnable(&state);
//
//    /* Open an intc handle for the edma interrupt */
//    vectId = CSL_INTC_VECTID_4;
//
//    //The following maps CSL_INTC_EVENTID... identified event from the edma peripheral
//    //to the cpu interrupt identified by CSL_INTC_VECTID...  EVENTID is the input to
//    //intc and VECTID is the output from intc.
//    hIntcEdma = CSL_intcOpen (
//    		&intcObjEdma,
//    		//CSL_INTC_EVENTID_EDMA3CC_GINT+CoreNum,		//global region
//    		//CSL_INTC0_CPU_2_EDMACC_GINT,
//    		//EDMA3_CC0_GINT,
//    		6,
//    		//16,
//    		//24,
//            &vectId ,
//            NULL);
//
//#if ( EDMADISPATCH_METHOD == EDMADISPATCH_IEVAL )
//	// if the faster IEVAL option is chosen for the edmaIntDispatcher, then this interrupt
//	// should be masked out of the Dropped Interrupt detection to avoid generating INTERR
//    CSL_intcHwControl(hIntcEdma,CSL_INTC_CMD_EVTDROPDISABLE,NULL);
//#endif
//	//--------------------------done intc setup-----------------------//
//
//    /* Associate an EDMA event handler with the INTC module, ie. make this the EDMA ISR */
//    EventRecord.handler = &edmaIntDispatcher;
//    EventRecord.arg = (void*)(hModule);
//    CSL_intcPlugEventHandler(hIntcEdma,&EventRecord);
//    //Note that hIntcEdma identifies a particular interrupt from intc, in this case, vectId.
//    //That is, we are setting up the handler for intc's vectId interrupt vector output.
//
//    /* Enable edma interrupt  */
//    CSL_intcHwControl(hIntcEdma,CSL_INTC_CMD_EVTENABLE,NULL);
//
//    /* Clear the edma interrupt (Q: is this required?) */
//    CSL_intcHwControl(hIntcEdma,CSL_INTC_CMD_EVTCLEAR,NULL);
//
////    /* Hook a completion code to a function */
////    edmaEventHook(tccCh0, tccCh0Fxn);
////    //This is only configuring our own interrupt dispatcher/event handler.
////    //It's not affecting the INTC module in any way.
//}

//EDMA_LinkHandle edmaGetLinkHandle(Uint32 linkNum) {
//	if (linkNum < EDMA_NUM_LINKS_TO_SETUP
//			&& linkDescriptors[linkNum].ownerCoreDNUM == DNUM)
//		return &linkDescriptors[linkNum];
//	else
//		return 0;
//}

int 	edmaLinkOpen(
		INOUT 	EDMA_LinkHandle hL
		//				IN		Uint32 ownerCoreNum,
		//				IN		Uint32 *src,
		//				IN		Uint32 *dst,
		//				IN		Uint32 numDblWords
) {


	Uint32 i;

	//		//All links are already allocated
	//		if (EDMA_nChAlloc == EDMA_NUM_AVAILABLE_LINKS)
	//			return -1;
	//
	//		//Find the next unallocated link
	//		//		i = nChAlloc;
	//		for (i = 0; i < EDMA_NUM_AVAILABLE_LINKS; i++) {
	//			if (!EDMA_linkDB[i][EDMA_LINKDB_COL_ISALLOC])
	//				break;
	//		}

	i = 0;
	return edmaLinkOpenSpecific(hL, i);
}

//int	edmaLinkAllocate() {
//	Uint32 linkNum = 0;
//	Uint32 linkAvailable = 0;
//	if (DNUM != 0)
//	  printf("WARNING: edmaLinkAllocate invoked from non-zero core id %d\n", DNUM);
//
//	//if (EDMA_nextFreeLinkNum >= EDMA_NUM_AVAILABLE_LINKS)
//	//  return -1;
//	//else
//	//  return EDMA_nextFreeLinkNum++;
//	for (linkNum = 0; linkNum < EDMA_NUM_AVAILABLE_LINKS; linkNum++) {
//	  if (EDMA_linkDB[linkNum][EDMA_LINKDB_COL_ISALLOC] == 0) {
//	    linkAvailable = 1;
//	    break;
//	  }
//	}
//	if (linkAvailable) {
//	  EDMA_linkDB[linkNum][EDMA_LINKDB_COL_ISALLOC] = 1;
//	  return linkNum;
//	}
//
//	return -1;
//}

int 	edmaLinkOpenSpecific(
		INOUT 	EDMA_LinkHandle hL,
		Uint32	linkNum
) {
	CSL_Edma3ChannelAttr        chAttr;
	CSL_Edma3ChannelHandle      hChannel;
	CSL_Edma3ParamHandle        hParam;

	CSL_Status                  status;
	Uint16						paramNum;

	Uint32 moduleInstNum;

	if (linkNum >= EDMA_NUM_AVAILABLE_LINKS) {
		printf("EDMA driver: Cannot allocate link with linkNum = %d\n", linkNum);
		exit(1);
		return -1;
	}

	//		EDMA_linkDB[linkNum][EDMA_LINKDB_COL_ISALLOC] = 1;

	//if (EDMA_linkDB[linkNum][EDMA_LINKDB_COL_ISALLOC] != 1) {
	//  printf("EDMA driver: ERROR: link not allocated before opening\n");
	//  exit(1);
	//}

	//if (EDMA_linkDB[linkNum][EDMA_LINKDB_COL_ISOPEN] != 0) {
	//  printf("EDMA driver: ERROR: link already open\n");
	//  exit(1);
	//}

	hL->regionNum = EDMA_REGION;
	hL->moduleInstNum = EDMA_linkDB[linkNum][EDMA_LINKDB_COL_MODNUM];
	hL->chNum = EDMA_linkDB[linkNum][EDMA_LINKDB_COL_CHNUM];
	hL->chTCC = hL->chNum;


	moduleInstNum 		= hL->moduleInstNum;
	chAttr.chaNum 		= hL->chNum;
	chAttr.regionNum	= hL->regionNum;

	//		hChannel = CSL_edma3ChannelOpen(&chObj, moduleInstNum, &chAttr, &status);
	hChannel = CSL_edma3ChannelOpen(&(hL->chObj), moduleInstNum, &chAttr, &status);
	if ( (hChannel == NULL) || (status != CSL_SOK)) { bailOut(EDMA_ERR6); return -2; }

	/* Store the channel handle */
	hL->hChannel = hChannel;

	/* Get the PaRAM number associated with this channel */
	status = CSL_edma3GetHwChannelSetupParam(hChannel,&paramNum);
	if (status != CSL_SOK) { bailOut(EDMA_ERR6); return -3; }

	/* Get a handle to the param set */
	hParam = CSL_edma3GetParamHandle(hChannel,paramNum,&status);
	if (hParam == NULL) { bailOut(EDMA_ERR7); return -4; }

	/* Store the PaRAM handle */
	hL->hParam = hParam;

	/* Clear error registers, otherwise they will block the channel */
	edmaClearErrorRegisters(hChannel);


	return 0;

}



//void edmaCloseChannels(
////		EDMA_LinkDescriptor chD[],
////		Uint32 nCh
//		) {
//	Uint32 i;
//	CSL_Status status;
//
//	for (i = 0; i < nCh; i++) {
//		status = CSL_edma3ChannelClose(chD[i].hChannel);
//		if (status != CSL_SOK) { bailOut(); return; }
//	}
//}

void edmaLinkClose(EDMA_LinkHandle hL) {
	CSL_Status status;

	//	if (EDMA_linkDB[hL->linkNum][EDMA_LINKDB_COL_ISALLOC]) {

	status = CSL_edma3ChannelClose(hL->hChannel);
	if (status != CSL_SOK) { bailOut(EDMA_ERR8); return; }

	//		EDMA_linkDB[hL->linkNum][EDMA_LINKDB_COL_ISALLOC] = 0;
	//		EDMA_nLinksAlloc--;
	//	}

}

void edmaClose(
		//    CSL_Edma3ChannelHandle      hChannel
) {
	CSL_Status                  status;
	Uint32 i = 0;

	//	printf("edmaClose\n");

	/* Channel close */
	//	edmaCloseChannels(
	//			linkDescriptors,
	//			EDMA_NUM_LINKS_TO_SETUP
	//			);



	/* Edma module close */
	//    status = CSL_edma3Close(hModule);
	//    if (status != CSL_SOK) { bailOut(); return; }
	for (i = 0; i < EDMA_NUM_MODULES; i++) {
		status = CSL_edma3Close(EDR_hModule[i]);
		if (status != CSL_SOK) { bailOut(EDMA_ERR9); return; }
	}
}

//void intcClose(
//		CSL_IntcHandle hIntcEdma
//		) {
//	CSL_intcClose(hIntcEdma);
//}

//Uint32 edmaIsTransferComplete(EDMA_LinkHandle hL) {
Uint32 edmaIsTransferComplete(Uint32 linkNum) {


	CSL_Edma3CmdIntr 		 tccReg;
	volatile Uint32              intr;
	volatile Uint32              intrh;
	Uint32 		     	 isTc = 0;

	Uint32			 moduleInstNum;
	Uint32			 chTCC;

	moduleInstNum = EDMA_linkDB[linkNum][EDMA_LINKDB_COL_MODNUM];
	chTCC = EDMA_linkDB[linkNum][EDMA_LINKDB_COL_CHNUM];

	tccReg.region = EDMA_REGION;
	//CSL_edma3GetHwStatus(EDR_hModule[hL->moduleInstNum], CSL_EDMA3_QUERY_INTRPEND, &tccReg);
	CSL_edma3GetHwStatus(EDR_hModule[moduleInstNum], CSL_EDMA3_QUERY_INTRPEND, &tccReg);
	isTc = ((tccReg.intr >> chTCC) & 1) || ((tccReg.intrh >> (chTCC - 32)) & 1);
	return isTc;
}

//int edmaClearTransferCompletionFlag(EDMA_LinkHandle hL) {
int edmaClearTransferCompletionFlag(Uint32 linkNum) {
	CSL_Edma3CmdIntr 			 tccReg;
	Uint64				 llTemp;

	Uint32			 moduleInstNum;
	Uint32			 chTCC;

	moduleInstNum = EDMA_linkDB[linkNum][EDMA_LINKDB_COL_MODNUM];
	chTCC = EDMA_linkDB[linkNum][EDMA_LINKDB_COL_CHNUM];

	tccReg.region = EDMA_REGION;
	//llTemp  = (unsigned long long)1 << hL->chTCC;
	llTemp  = (unsigned long long)1 << chTCC;
	tccReg.intr = _loll(llTemp);
	tccReg.intrh = _hill(llTemp);

	//printf("tcc value before clear: %d\n", edmaIsTransferComplete(hL));
	//CSL_edma3HwControl(hModule[hL->moduleInstNum],CSL_EDMA3_CMD_INTRPEND_CLEAR,&tccReg);
	//printf("tcc value after clear: %d\n", edmaIsTransferComplete(hL));

	printf("tcc value before clear: %d\n", edmaIsTransferComplete(linkNum));
	CSL_edma3HwControl(EDR_hModule[moduleInstNum],CSL_EDMA3_CMD_INTRPEND_CLEAR,&tccReg);
	printf("tcc value after clear: %d\n", edmaIsTransferComplete(linkNum));

	return 0;
}


void edmaSetupTransfer(
		//EDMA_LinkHandle hL,
		Uint32 linkNum,
		Uint32 *src,
		Uint32 *dst,
		Uint32 numDblWords
) {

	CSL_Edma3ParamSetup         myParamSetup;
	//		printf("edmaSetupParam\n");

	Uint32 chTCC;
	CSL_Edma3ParamHandle        hParam;
	EDMA_LinkHandle hL;

	hL = EDMA_linkHandles[linkNum];

	chTCC = hL->chTCC;
	hParam = hL->hParam;


	/* Edma parameter entry Setup */
	myParamSetup.option = CSL_EDMA3_OPT_MAKE(CSL_EDMA3_ITCCH_DIS,
			CSL_EDMA3_TCCH_DIS,
			CSL_EDMA3_ITCINT_DIS,
			CSL_EDMA3_TCINT_EN,
			chTCC,
			CSL_EDMA3_TCC_NORMAL,
			CSL_EDMA3_FIFOWIDTH_NONE,
			CSL_EDMA3_STATIC_DIS,
			CSL_EDMA3_SYNC_A,
			CSL_EDMA3_ADDRMODE_INCR,
			CSL_EDMA3_ADDRMODE_INCR);
	//note: Make sure source and destination addreesses (inBuf and outBuf)
	//are global (0x1yyyyyyy), as EDMA transfers fail if set up with
	//local addresses (0x0yyyyyyy).
	//	myParamSetup.srcAddr = (Uint32)inBuf;                  			 	// source address
	//	myParamSetup.aCntbCnt = CSL_EDMA3_CNT_MAKE(length_b, 1);            // Acnt Bcnt
	//	myParamSetup.dstAddr = (Uint32)outBuf;
	myParamSetup.srcAddr = (Uint32)src;                  			 	// source address
	myParamSetup.aCntbCnt = CSL_EDMA3_CNT_MAKE(numDblWords * 8, 1);            // Acnt Bcnt
	myParamSetup.dstAddr = (Uint32)dst;

	//	    printf("srcAddr: 0x%08X\n", myParamSetup.srcAddr);
	//	    printf("dstAddr: 0x%08X\n", myParamSetup.dstAddr);

	myParamSetup.srcDstBidx = CSL_EDMA3_BIDX_MAKE(0,0);                  // B src and dst index
	myParamSetup.linkBcntrld = CSL_EDMA3_LINKBCNTRLD_MAKE (
			//hParamBasic,//0xffff,//
			CSL_EDMA3_LINK_NULL,
			0);           // link and Bcnt reload
	myParamSetup.srcDstCidx = CSL_EDMA3_CIDX_MAKE(0,0);                  // C src and dst index
	myParamSetup.cCnt = 1;                                               // C cnt
	EDR_status = CSL_edma3ParamSetup(hParam,&myParamSetup);
	if (EDR_status != CSL_SOK) { bailOut(EDMA_ERR10); return; }
}

//int edmaStartTransfer(EDMA_LinkHandle hL) {
int edmaStartTransfer(Uint32 linkNum) {
	CSL_Status status;
	EDMA_LinkHandle hL;
	//	ASSERT(hL->ownerCoreDNUM == DNUM);
	//	if (hL->ownerCoreDNUM != DNUM)
	//		return -1;

	//ASSERT(!edmaIsTransferComplete(hL));	//TCC must have been cleared before issuing
	ASSERT(!edmaIsTransferComplete(linkNum));	//TCC must have been cleared before issuing
	//a new transfer request


	//	edmaSetupParam(hL);

	hL = EDMA_linkHandles[linkNum];
	status = CSL_edma3HwChannelControl(hL->hChannel,CSL_EDMA3_CMD_CHANNEL_SET,NULL);
	if (status != CSL_SOK) { bailOut(EDMA_ERR11); return -2; }

	return 0;
}


