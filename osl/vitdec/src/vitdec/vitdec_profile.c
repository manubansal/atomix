/**
Atomix project, vitdec_profile.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
Created on: Jan 7, 2012
*/

#include <osl/inc/swpform.h>
#include "../../inc/vcpdriver/vcpdriver_api.h"
//#include <orlib/orlib_blocks/orlib_types_wifi.h>

//csl defines the following performance modes that directly translate into convergence
//distance values. This is done internally in the csl, but we need the convergence
//distance to include in frame length even before we call the csl configurator. So
//we'll forward define a translator from performance level to convergence distance
//and assert later that the value we picked matches what csl sets up, so that the
//configuration is consistent. --MB, 2/16/12

///** Speed critical */
//#define VCP2_SPEED_CRITICAL          0
///** Speed most critical */
//#define VCP2_SPEED_MOST_CRITICAL     1
///** Performance critical */
//#define VCP2_PERF_CRITICAL           2
///** Performance most critical */
//#define VCP2_PERF_MOST_CRITICAL      3

Uint32 vitdec_profile_convgDistFromPerfLevel(
		IN Uint32 perfLevel
		) {
	switch (perfLevel) {
	case 1:
		return 18;
	case 0:
		return 36;
	case 2:
		return 54;
	case 3:
		return 72;
	default:
		return 18;
	}
}

Uint32 vitdec_profile_perfLevelFromConvgDist(
		IN Uint32 convergenceLength
		) {
	switch (convergenceLength) {
	case 18:
		return VCP2_SPEED_MOST_CRITICAL;
	case 36:
		return VCP2_SPEED_CRITICAL;
	case 54:
		return VCP2_PERF_CRITICAL;
	case 72:
		return VCP2_PERF_MOST_CRITICAL;
	default:
		printf("ERROR: BAD CONVERGENCE LENGTH\n");
	}
}


///* This table is expected to be configured based on the scheduler's output
// * and hardware capabilities. The intention is that this is the only variable
// * parameter from which all others will be derived through this suite of
// * profile configuration functions, so that we can encapsulate all the calculations
// * here.
// */
//static far const int vitdec_profile_number_of_tiles_vs_data_rate[8] = {
//		1,	//for 6mbps
//		1,	//for 9mbps
//		1,	//for 12mbps
//		1,	//for 18mbps
//		2,	//for 24mbps
//		3,	//for 36mbps
//		4,	//for 48mbps
//		4	//for 54mbps
//};

//static far int vitdec_profile_tile_size_vs_data_rate[8];
//static far int vitdec_profile_tile_size_vs_data_rate_initialized = 0;

/** This function sets up the tile size table at runtime initialization. It needs to be
 * called once during system init. */
void vitdec_profile_init() {
	Uint32 nTiles;
	Uint32 ndbps;
	Uint32 nDataBitsPerTile;
	Uint32 data_rate;

	Uint32 i;

	//for (i = 0; i < 8; i++) {
	//	data_rate = i;

	//	nTiles = vitdec_profile_number_of_tiles_vs_data_rate[data_rate];
	//	ndbps = kWifiGeeDataRatesInfo[data_rate].num_data_bits_per_ofdm_symbol;

	//	assert(ndbps % nTiles == 0);
	//	nDataBitsPerTile = ndbps/nTiles;

	//	assert (data_rate < 8);
	//	vitdec_profile_tile_size_vs_data_rate[data_rate] = nDataBitsPerTile;
	//}
	//vitdec_profile_tile_size_vs_data_rate_initialized = 1;
}

//void vitdec_profile_assert_init() {
//	if (!vitdec_profile_tile_size_vs_data_rate_initialized)
//		printf("ERROR: vitdec_profile_init() has not been invoked before accessing vitdec_profile_tile_size_vs_data_rate table.\n");
//	assert(vitdec_profile_tile_size_vs_data_rate_initialized == 1);
//}

//Uint32 vitdec_profile_getTileSize(
//		IN	Uint32 data_rate
//		) {
//	vitdec_profile_assert_init();
//	return vitdec_profile_tile_size_vs_data_rate[data_rate];
//}

//================== static parameters =====================================================
//------------------------------------------------------------------------------------------
void vitdec_profile_wifiGee_populate(
		OUT Vitdec_Profile* vdp,
		IN Vitdec_ConvergenceLength convergenceLength
		) {

    vdp->rate 			= VCP2_RATE_1_2;
    vdp->constLen 		= 7;
    vdp->yamTh 			= 0;
    vdp->stateNum 		= 0;
    vdp->tbConvrgMode 		= TRUE;
    vdp->decision 		= VCP2_DECISION_HARD;
    vdp->readFlag 		= 1;
    vdp->tailBitEnable 		= FALSE;
    vdp->traceBackIndex 	= 0;
    vdp->outOrder 		= VCP2_OUTORDER_31_0; /* (msb oldest, i believe) */

//    vdp->perf 			= VCP2_SPEED_MOST_CRITICAL;
//    vdp->perf 		= VCP2_PERF_MOST_CRITICAL;
    vdp->perf			= vitdec_profile_perfLevelFromConvgDist(convergenceLength);

    vdp->poly0 			= 0x6C; /** G1 = 1 + D2 + D3 + D5 + D6 (for the A output) */
    vdp->poly1 			= 0xE4; /** G0 = 1 + D + D2 + D3 + D6 (for the B output)  */
    vdp->poly2 			= 0x00;
    vdp->poly3 			= 0x00;

//    vdp->c_guess		= vitdec_profile_convgDistFromPerfLevel(vdp->perf);
    vdp->c_guess = convergenceLength;

    //This is not being set so that the order in which vitdec_profile_setFrameLen and
    //vitdec_profile_populateWifiGee are called does not matter. --MB, 2/16/12
//	vdp->frameLen		= 54;		//just default value, should be set by the user before use
}

void vitdec_profile_wifiGee_populate_convergent(
		OUT Vitdec_Profile* vdp,
		IN  Vitdec_ConvergenceLength convergenceLength
		) {
	vitdec_profile_wifiGee_populate(vdp, convergenceLength);
	vdp->tbConvrgMode = TRUE;
}

void vitdec_profile_wifiGee_populate_tailed(
		OUT Vitdec_Profile* vdp,
		IN  Vitdec_ConvergenceLength convergenceLength
		) {
	vitdec_profile_wifiGee_populate(vdp, convergenceLength);
	vdp->tbConvrgMode = FALSE;
}
//------------------------------------------------------------------------------------------


//================== dynamic parameters ====================================================
//------------------------------------------------------------------------------------------
void vitdec_profile_generic_setFrameLength(
		IN  Uint32 frameLen,
		OUT Vitdec_Profile* vdp) {
	vdp->frameLen = frameLen;
}

void vitdec_profile_setDataLength_head_convergent(
		IN	Uint32 data_length,
		INOUT 	Vitdec_Profile* headVdp) {

	Uint32 c;

	c = headVdp->c_guess;
	headVdp->frameLen			= data_length;
	headVdp->numSymProcExpected		= headVdp->frameLen + c;
}

void vitdec_profile_setDataLength_mid_convergent(
		IN	Uint32 data_length,
		INOUT 	Vitdec_Profile* midVdp) {

	Uint32 c;


	c = midVdp->c_guess;
	midVdp->frameLen			= data_length + c;
	midVdp->numSymProcExpected		= midVdp->frameLen + c;
}

void vitdec_profile_setDataLength_tail_nohead(
		IN	Uint32 data_length_bits,
		INOUT 	Vitdec_Profile* tailVdp) {

	Uint32 c;

	c = tailVdp->c_guess;
	tailVdp->c_guess = 0;
	tailVdp->frameLen			= data_length_bits + c;
	tailVdp->numSymProcExpected		= tailVdp->frameLen + (tailVdp->constLen - 1);
}

void vitdec_profile_setDataLength_head_tail(
		IN	Uint32 data_length_bits,
		INOUT 	Vitdec_Profile* headTailVdp) {

	headTailVdp->frameLen 			= (data_length_bits);
	headTailVdp->numSymProcExpected		= headTailVdp->frameLen + (headTailVdp->constLen - 1);
}

//---------------------------------------------------------------------------------------------------------------

void vitdec_wifiGee_setViterbiDecodingMap(
	IN  Uint32 numInfoBits,
	IN  Uint32 numDataBitsPerSymbol,
	IN  Uint32 frameLength, 
	IN  Uint32 convergenceLength,
	OUT Vitdec_ViterbiDecodingMap *vitdecMap
	){

	Int32 nWays;

	//---- wifi 802.11g specific params -----
	Int32 f = frameLength;
    	Int32 c = convergenceLength;
	Int32 t = 6;				//tailLength
	Int32 s = 16;				//serviceLength
	Int32 ndbps = numDataBitsPerSymbol;
	//---- wifi 802.11g specific params -----

	//calculate the total number of bits to be decoded by the viterbi decoder
	Int32 l = numInfoBits;					
	l += s + t;						//include service and tail
	l = ceil(l, ndbps) * ndbps;				//include padding length too
	Int32 L = 0;


	Int32 n0 = ceil(f+c,ndbps);				//no. of input symbols to cache before doing any decoding

	//EITHER THIS (single headtail decode)
	L = l - t;						//output length including pad length in decoded output
	Int32 n1 = (L <= f ) ? 1 : 0;				//no. of headtail mode decodes (0 or 1)
	Int32 l1 = l;
	Int32 d1 = L;
	Int32 L1 = L;
	Int32 p1 = 0;
	Int32 q1 = 0;

	//OR REST OF THIS (convergent decode)
	L = l;							//total number of output bits, including tail and pad
	l += c;							//decoded length; we decode the last frame convergent mode too

	Int32 n2 = 1 - n1;					//no. of head mode convergent decodes (0 or 1, mutually 
								//exclusive with the above)

	Int32 l2 = f + c;					//input frame length for head decode
	Int32 d2 = f;						//output frame length for head decode
	Int32 L2 = f;						//no. of useful bits obtained from head decode
	Int32 p2 = 0;						//left strip length for head decode output frame
	Int32 q2 = 0;						//right strip length for head decode output frame

	Int32 n3 = n2 ? L/f - 2: 0;		//no. of mid convergent decodes
									//NOTE: We assumed that f is a factor of ndbps
	Int32 l3 = c + f + c;					//input frame length for mid decode
	Int32 d3 = c + f;					//output frame length for mid decode
	Int32 L3 = f;						//no. of useful bits obtained from mid decode
	Int32 p3 = c;						//left strip length for mid decode output frame
	Int32 q3 = 0;						//right strip length for mid decode output frame


	Int32 n4 = n2;						//no. of tail mode decodes (0 or 1) 
								//<del>(tail-only, not head-tail)</del>
								//Since I could not get tail-mode decode to work,
								//tail symbol will also be decoded as mid-convergent
								//with zero-padded extension of convergent length.
								//--MB 12/20/2013

	Int32 l4 = c + f + c;			//input frame length for tail decode
	Int32 d4 = c + f;					//output frame length for tail decode
	Int32 L4 = f;					//no. of useful bits obtained from tail decode
	Int32 p4 = c;						//left strip length for tail decode
	Int32 q4 = 0;						//right strip length for tail decode
	//END EITHER-OR

	
	vitdecMap->n0 = n0;

	vitdecMap->n1 = n1;
	vitdecMap->l1 = l1;
	vitdecMap->d1 = d1;
	vitdecMap->L1 = L1;
	vitdecMap->p1 = p1;
	vitdecMap->q1 = q1;

	vitdecMap->n2 = n2;
	vitdecMap->l2 = l2;
	vitdecMap->d2 = d2;
	vitdecMap->L2 = L2;
	vitdecMap->p2 = p2;
	vitdecMap->q2 = q2;

	vitdecMap->n3 = n3;
	vitdecMap->l3 = l3;
	vitdecMap->d3 = d3;
	vitdecMap->L3 = L3;
	vitdecMap->p3 = p3;
	vitdecMap->q3 = q3;

	vitdecMap->n4 = n4;
	vitdecMap->l4 = l4;
	vitdecMap->d4 = d4;
	vitdecMap->L4 = L4;
	vitdecMap->p4 = p4;
	vitdecMap->q4 = q4;

//	vitdecMap->nMidIterations = (n3 - 6)/4;
	nWays = ndbps/f;
	vitdecMap->nMidIterations = (n3 - (nWays - 2 + nWays * 2))/nWays;	//(nWays - 2) for head, nWays * 2 for mid-pre

	DEBUG_DATA(
	printf("nb (total number of input data bits)   : %d\n", numInfoBits);
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
	)
	//}

}
//------------------------------------------------------------------------------------------


//// wimax parameters?

//////    vcpBaseParams.rate = VCP2_RATE_1_3;
//////    vcpBaseParams.constLen = CONSTRAINT_LEN;
//////    vcpBaseParams.frameLen = FRAME_LENGTH;
//////    vcpBaseParams.yamTh = 0;
//////    vcpBaseParams.stateNum = 0;
//////    vcpBaseParams.tbConvrgMode = TRUE;
//////    vcpBaseParams.decision = VCP2_DECISION_HARD;
//////    vcpBaseParams.readFlag = 1;
//////    vcpBaseParams.tailBitEnable = FALSE;
//////    vcpBaseParams.traceBackIndex = 0;
//////    vcpBaseParams.outOrder = 0;
