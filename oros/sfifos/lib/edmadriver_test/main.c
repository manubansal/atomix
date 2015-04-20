/**
Atomix project, main.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#include <ti/csl/csl_edma3.h>
#include <edmaDriver.h>
#include <edmaTypes.h>
#include <ti/csl/csl_tsc.h>
#include <stdio.h>
#include <string.h>
#include "timestamp.h"
#include <c6x.h>


#define BUFLEN_W 			80
//#define BUFLEN_W 			800
#define BYTES_IN_A_WORD		4

typedef unsigned int Uint32;
typedef int Int32;
typedef unsigned char Uint8;




//#pragma DATA_SECTION(loBuf, ".G0L2");
#pragma DATA_SECTION(rmRdBuf, ".G1L2");
#pragma DATA_SECTION(rmWrBuf, ".G1L2");
#pragma DATA_ALIGN(loRdBuf, 8);
#pragma DATA_ALIGN(rmRdBuf, 8);
#pragma DATA_ALIGN(loWrBuf, 8);
#pragma DATA_ALIGN(rmWrBuf, 8);
Uint32 loRdBuf[BUFLEN_W];
Uint32 rmRdBuf[BUFLEN_W];
Uint32 loWrBuf[BUFLEN_W];
Uint32 rmWrBuf[BUFLEN_W];

Uint32 loRdBufGA;	//global address of loRdBuf
Uint32 loWrBufGA;


//#pragma FUNC_CANNOT_INLINE(playWithBufs);
void playWithBufs(
	Uint32 inBuf[restrict], 
	Uint32 outBuf[restrict],
	Uint32 length_w
	) {
	
	Uint32 i;
	
	_nassert((Uint32)inBuf % 4 == 0);			
	_nassert((Uint32)outBuf % 4 == 0);
	
	#pragma MUST_ITERATE(4, ,4);
	for (i = 0; i < length_w; i++) {
		outBuf[i] = inBuf[i];
	}
}

void playWithBufsEDMA(
	Uint32 inBuf[restrict],
	Uint32 outBuf[restrict],
	Uint32 length_b
	) {

	Int32 stat;
	Uint32 linkNum;
	EDMA_LinkDescriptor dL;
	EDMA_LinkHandle hL;
	Uint32 ownerCoreNum;

	hL = &dL;

	//---------------------------------------------------------------//
	//EDMA TESTS

	//edma copy operations
//	printf("inBuf  address: %08x\n", (Uint32)inBuf);
//	printf("outBuf address: %08x\n", (Uint32)outBuf);

	/* Initialize and setup edma */
	edmaInit();

	/* Initialize and setup the corresponding intc features */
//	intcInit();

	/* Obtain the handle for the channel we want to use */
//	linkNum = 6;
//	linkNum = 3;
//	linkNum = 10;
//	hL = edmaGetLinkHandle(linkNum);
	ASSERT(length_b % 8 == 0);
	ownerCoreNum = 0;
//	ownerCoreNum = DNUM;
	stat = edmaLinkOpen(hL);		//gives you the next available link
//	if (hL == NULL || hL == 0) {
//		printf("Could not obtain channel handle\n");
	if (stat < 0) {
		printf("Could not obtain link handle, stat = %d\n", stat);
		edmaClose();
//		intcClose();
		return;
	}
	printf("modNo: %d\n", hL->moduleInstNum);
	printf("chNum: %d\n", hL->chNum);
	printf("chTCC: %d\n", hL->chTCC);


	/* Set up specific EDMA transfer parameters */
	edmaSetupTransfer(hL, inBuf, outBuf, length_b/8);

//	/* Query various error statuses */
//	printf("\n----- pre transfer diagnosis ----\n");
////	edmaQueryChannelStatus(hChannel);
////	edmaQueryErrorRegisters(hChannel);
//
//	////edmaQueryActivityStat();
//	////edmaQueryTransferCompletionInterrupt(hL);
//	////edmaQueryTransferCompletionInterrupt(hL);
////	intcQueryStatus();


	/* Manually trigger the channel */
	printf("\n----- triggering the link now ----\n");
	ts5 = CSL_tscRead();
	stat = edmaStartTransfer(hL);
	if (stat < 0) {
		printf("Could not start transfer.\n");
		return;
	}

	/* Wait for edma completion */
    while (!edmaIsTransferComplete(hL));
//	while (((Uint8 *)inBuf)[length_b - 1] != ((Uint8 *)outBuf)[length_b - 1]);
    ts11 = CSL_tscRead();

    printf("\n----- transfer completed ----\n");

    /* Clear the completion flag for reusing the channel */
    edmaClearTransferCompletionFlag(hL);

    /* Compare memory buffers */
    stat = memcmp(inBuf, outBuf, length_b);

    ts12 = CSL_tscRead();

    /* Query various error statuses again */
//    printf("\n----- post transfer diagnosis ----\n");
//	edmaQueryChannelStatus(hChannel);
//	edmaQueryErrorRegisters(hChannel);
//	edmaQueryActivityStat();
//	edmaQueryTransferCompletionInterrupt();
//	intcQueryStatus();

    edmaLinkClose(hL);

    /* Cleanup and close edma */
    edmaClose();

    /* Cleaup and close intc */
//    intcClose();

    /* Report buffer match status */
	if (stat == 0) {	//0 if match, non-zero if not
		printf("Buffers match. Test OK.\n");
	}
	else {
		printf("Buffers do not match. ERROR.\n");
	}

	printf("Timestamps: %lld, %lld, %lld, %lld\n", ts5, ts6, ts7, ts11);
	printf("Buffer transfer time from SET to INTR   : %lld\n", ts11 - ts5);
//	printf("Buffer transfer time from SET to INTR   : %lld\n", ts6 - ts5);
//	printf("Time spent in EdmaIntDispatcher func    : %lld\n", ts7 - ts6);
//	printf("End of EdmaIntDispatcher to flag assert : %lld\n", ts11 - ts7);

}



int main() {
	Uint32 i;
	
	CSL_tscEnable();

	//--------------------------------------------------------------//
	//DIRECT BUFFER ACCESS TESTS
	
	//initialize buffers to non-trivial values
	for (i = 0; i < BUFLEN_W; i++) {
		loRdBuf[i] = i;
		rmRdBuf[i] = i * 2;
	}
	
	memset(loWrBuf, 0, BUFLEN_W * sizeof(Uint32));
	memset(rmWrBuf, 0, BUFLEN_W * sizeof(Uint32));

//	//direct buffer copy operations
//	ts0 = CSL_tscRead();
//
//	playWithBufs(loRdBuf, loWrBuf, BUFLEN_W);
//	ts1 = CSL_tscRead();
//
//	playWithBufs(rmRdBuf, loWrBuf, BUFLEN_W);
//	ts2 = CSL_tscRead();
//
//	playWithBufs(loRdBuf, rmWrBuf, BUFLEN_W);
//	ts3 = CSL_tscRead();
//
//	playWithBufs(rmRdBuf, rmWrBuf, BUFLEN_W);
//	ts4 = CSL_tscRead();
//
//
//	printf("loRd, loWr cpu  buf operation time: %lld\n",ts1 - ts0);
//	printf("rmRd, loWr cpu  buf operation time: %lld\n",ts2 - ts1);
//	printf("loRd, rmWr cpu  buf operation time: %lld\n",ts3 - ts2);
//	printf("rmRd, rmWr cpu  buf operation time: %lld\n",ts4 - ts3);
	
	/* RESULTS:
	 * loRd, loWr buf operation time: 129
	 * rmRd, loWr buf operation time: 2354
	 * loRd, rmWr buf operation time: 656
	 * rmRd, rmWr buf operation time: 3063
	 * 
	 * loRd, loWr buf operation time: 275
	 * rmRd, loWr buf operation time: 3437
	 * loRd, rmWr buf operation time: 618
	 * rmRd, rmWr buf operation time: 4158
	 * 
	 * loRd, loWr buf operation time: 231
	 * rmRd, loWr buf operation time: 2386
	 * loRd, rmWr buf operation time: 723
	 * rmRd, rmWr buf operation time: 3015
	 */

	//--------------------------------------------------------------//
	//EDMA BUFFER ACCESS TESTS

	memset(loWrBuf, 0, BUFLEN_W * sizeof(Uint32));
	memset(rmWrBuf, 0, BUFLEN_W * sizeof(Uint32));

	//edma copy operations
//	printf("loRd address: %08x\n", (Uint32)loRdBuf);
//	printf("rmRd address: %08x\n", (Uint32)rmRdBuf);
	
	
	
	//note: When adding 0x10000000, make sure the memory address has been
	//casted into a Uint32. The result of 0x10000000 + loRdBuf will increment
	//the address of loRdBuf by sizeof(Uint32) * 0x10000000, because loRdBuf
	//is Uint32 *, and this would then become pointer arithmetic instead of
	//regular arithmetic. 
	loRdBufGA = (Uint32)loRdBuf;
	loWrBufGA = (Uint32)loWrBuf;
	loRdBufGA += 0x10000000;
	loWrBufGA += 0x10000000;
	
//	printf("EDMA with loRd, loWr\n");
	playWithBufsEDMA(
		(Uint32 *)loRdBufGA, 
		(Uint32 *)loWrBufGA, 
		BUFLEN_W * BYTES_IN_A_WORD
		);
//	printf("loRd, loWr edma buf operation time: %lld\n",ts6 - ts5);
	printf("loRd, loWr edma buf operation time: %lld\n",ts11 - ts5);
	
//	printf("EDMA with rmRd, loWr\n");
	playWithBufsEDMA(
		rmRdBuf,
		(Uint32 *)loWrBufGA,
		BUFLEN_W * BYTES_IN_A_WORD
		);
//	printf("rmRd, loWr edma buf operation time: %lld\n",ts6 - ts5);
	printf("rmRd, loWr edma buf operation time: %lld\n",ts11 - ts5);

//	printf("EDMA with loRd, rmWr\n");
	playWithBufsEDMA(
		(Uint32 *)loRdBufGA,
		rmWrBuf,
		BUFLEN_W * BYTES_IN_A_WORD
		);
//	printf("loRd, rmWr edma buf operation time: %lld\n",ts6 - ts5);
	printf("loRd, rmWr edma buf operation time: %lld\n",ts11 - ts5);

//	printf("EDMA with rmRd, rmWr\n");
	playWithBufsEDMA(
		rmRdBuf,
		rmWrBuf,
		BUFLEN_W * BYTES_IN_A_WORD
		);
//	printf("rmRd, rmWr edma buf operation time: %lld\n",ts6 - ts5);
	printf("rmRd, rmWr edma buf operation time: %lld\n",ts11 - ts5);
	
}
