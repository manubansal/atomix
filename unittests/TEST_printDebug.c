/**
Atomix project, TEST_printDebug.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
Created on: Feb 8, 2012
*/

#include <stdio.h>
#include "wifi_parameters.h"
#include <osl/inc/util.h>

void TEST_printDebug_txRxPktState(TxRxPktState *txRxPktState) {
	printf("txRxPktState->NumInfoBits                    : %d\n",		txRxPktState->NumInfoBits);
	printf("txRxPktState->DataRate                       : %d\n",        txRxPktState->DataRate);
	printf("txRxPktState->ModulationSchemeIndex          : %d\n",        txRxPktState->ModulationSchemeIndex);
	printf("txRxPktState->NumCodedBitsPerCarrier         : %d\n",        txRxPktState->NumCodedBitsPerCarrier);
	printf("txRxPktState->CodingRateTimes120             : %d\n",        txRxPktState->CodingRateTimes120);
	printf("txRxPktState->NumCodedBitsPerSymbol          : %d\n",        txRxPktState->NumCodedBitsPerSymbol);
	printf("txRxPktState->NumDataBitsPerSymbol           : %d\n",        txRxPktState->NumDataBitsPerSymbol);
	printf("txRxPktState->NumUncodedBits                 : %d\n",        txRxPktState->NumUncodedBits);
	printf("txRxPktState->NumCodedBits                   : %d\n",        txRxPktState->NumCodedBits);
	printf("txRxPktState->NumPadBits                     : %d\n",        txRxPktState->NumPadBits);
	printf("txRxPktState->NumConstPnts                   : %d\n",        txRxPktState->NumConstPnts);
	printf("txRxPktState->NumDataSymbols                 : %d\n",        txRxPktState->NumDataSymbols);
}

void TEST_printDebug_descrambledBits(Uint8 *bytes, Uint32 nBytes) {
	int i;
	for (i = 0; i < nBytes; i++) {
		if (i % 4 == 0 && i > 0)	printf("\n");
		printBitsMsb8(bytes[i]); printf(" ");
	}
	printf("\n");
}

void TEST_printDebug_decodedBits(Uint8 *bytes, Uint32 nBytes) {
	int i;
	for (i = 0; i < nBytes; i++) {
		if (i % 4 == 0 && i > 0)	printf("\n");
		printBitsMsb8(bytes[i]); printf(" ");
	}
	printf("\n");
}
