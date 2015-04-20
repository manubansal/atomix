/**
Atomix project, main.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <osl/inc/or_types.h>
#include <orlib/orlib_blocks/orlib_types_wifi.h>
#include "wifi_parameters.h"
#include <stdio.h>
#include <osl/inc/amem4bitcpy.h>




void test1() {
	//void wifi_setPktState(TxRxPktState *retState, Uint8 dataRate, Uint32 NumInfoOctets);
	TxRxPktState txRxPktState_s;
	TxRxPktState *txRxPktState = &txRxPktState_s;

	Uint8 data_rate = kWifiGee6mbps;
	Uint32 data_length = 1;

	wifi_setPktState(txRxPktState, data_rate, data_length);

	
	//---------
//	Int32 c = 18;	//36, 54, 72
//	Int32 f = 54;	//arbitrary
//	Int32 t = 6;	//no. of tail bits
	//---------

	//---------
	Int32 c = 54;
	Int32 f = 18;
	Int32 t = 6;
	//---------


	Int32 L = txRxPktState->NumInfoBits;			//total number of useful decisions to be received from the viterbi decoder, that is, the total output length
	Int32 nOctets = 1;

	for (nOctets = 1; nOctets < 30; nOctets++) {
		L = nOctets * 8;
		Int32 l = L + t;								//total number of bits to be decoded by the viterbi decoder, that is, the total input length
		Int32 s = txRxPktState->NumDataBitsPerSymbol;

		Int32 n0 = ceil(f+c,s);						//no. of input symbols to cache before doing any decoding

		Int32 n1 = (l < (f + c)) ? 1 : 0;					//no. of headtail mode decodes (0 or 1)
		Int32 l1 = l;
		Int32 d1 = L;
		Int32 L1 = L;
		Int32 p1 = 0;
		Int32 q1 = 0;

		Int32 n2 = 1 - n1;								//no. of head mode convergent decodes (0 or 1, mutually exclusive with the above)
		Int32 l2 = f + c;								//input frame length for head decode
		Int32 d2 = f;									//output frame length for head decode
		Int32 L2 = f;									//no. of useful bits obtained from head decode
		Int32 p2 = 0;									//left strip length for head decode output frame
		Int32 q2 = 0;									//right strip length for head decode output frame

		Int32 n3 = l >= c + f ? floor(l-c, f) - 1: 0;	//no. of mid convergent decodes
		Int32 l3 = c + f + c;							//input frame length for mid decode
		Int32 d3 = c + f;								//output frame length for mid decode
		Int32 L3 = f;									//no. of useful bits obtained from mid decode
		Int32 p3 = c;									//left strip length for mid decode output frame
		Int32 q3 = 0;									//right strip length for mid decode output frame

		Int32 n4 = n2;									//no. of tail mode decodes (0 or 1) (tail-only, not head-tail)
		Int32 l4 = c + l - f * (1 + n3);				//input frame length for tail decode
		Int32 d4 = l4 - t;								//output frame length for tail decode
		Int32 L4 = l4 - c - t;							//no. of useful bits obtained from tail decode
		Int32 p4 = c;									//left strip length for tail decode
		Int32 q4 = 0;									//right strip length for tail decode

		printf("L (total number of input data bits)    : %d\n", L);
		printf("L (total number of output data bits)   : %d\n", L);
		printf("l (total number of decoded data bits)  : %d\n", l);
		printf("c (convergence length)                 : %d\n", c);
		printf("f (tile size/frame length per decode)  : %d\n", f);
		printf("n0 (no. of symbols to cache first)     : %d\n", n0);
		printf("n1 (whether headtail (1) or convgt(0)) : %d, %d, %d, %d, %d, %d\n", n1, l1, d1, L1, p1, q1);
		printf("n2 (whether headmode (1))              : %d, %d, %d, %d, %d, %d\n", n2, l2, d2, L2, p2, q2);
		printf("n3 (no. of mid convgt decodes)         : %d, %d, %d, %d, %d, %d\n", n3, l3, d3, L3, p3, q3);
		printf("n4 (whether tailmode (1))              : %d, %d, %d, %d, %d, %d\n", n4, l4, d4, L4, p4, q4);
		printf("\n");
	}


//	TEST_printDebug_txRxPktState(txRxPktState);



}

//static inline void amem4bitcpy_outskip(
//		OUT	Uint32 *outBuf,
//		IN	Uint32 *inBuf,
//		IN	Uint32 nBitsToCopy,
//		IN	Uint32 nOutBitsToSkip,
//		IN	Uint32 nInpBitsToSkip
void test2() {
	Uint32 a[] = {0xA392045B, 0x5F319D54, 0x98095A12};
	Uint32 b[] = {0x4E78D345, 0x0, 0x0};

	Uint32 i;
	for (i = 0; i < 3; i++) printBitsMsb(a[i]);
	printf("\n");

	for (i = 0; i < 3; i++) printBitsMsb(b[i]);
	printf("\n");

	amem4bitcpy_outskip(b, a, 12, 3, 6);
	for (i = 0; i < 3; i++) printBitsMsb(b[i]);
	printf("\n");

}


void main(void) {
	//test1();
	test2();
}

