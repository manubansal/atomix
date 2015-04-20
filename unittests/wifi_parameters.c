
#include <osl/inc/or_types.h>
#include <orlib/orlib_blocks/orlib_types_wifi.h>
#include "wifi_parameters.h"

//Uint8  DataRate;				//rate index (determines modulation and coding rate)
//Uint8  NumCodedBitsPerCarrier;  //N_bpsc (modulation)
//Uint8  CodingRateTimes120;		//		 (coding)
//Uint16 NumCodedBitsPerSymbol;   //N_cbps (derived from modulation)
//Uint16 NumDataBitsPerSymbol;	//N_dbps (derived from modulation and coding)
//Uint8  ModulationSchemeIndex;
//Uint32 NumCodedBits;
//Uint32 NumConstPnts;
//TxRxPktState txRxPktState;




//Int8   SlicerScaleFactor;
//Uint16 SlicerInputRMS;
//Uint16 SlicerRMS;
//Uint16 MapperScaleFactor;

//ProtocolParams protocolParams;

//void (*softSlicer)(
//                  Cplx16   ConstPnts[restrict],
//                  Uint32   NumInputConstPnts,
//                  Uint8    NumCodedBitsPerCarrier,
//                  Int8     ScaleFactor,
//                  Int32    SoftBitsPrecision,
//                  Uint16   Rms, 
//                  Uint16   Cinr[restrict],
//                  Int8     OutputSoftBits[restrict]) = NULL;				



//Int32  CM_2QAM_TABLE[2], CM_4QAM_TABLE[4], CM_16QAM_TABLE[16], CM_64QAM_TABLE[64];
//Int32  *tablePtrs[4] ;

//Cplx16 fFftScratchPad[PHY_OFDM_N_FFT + PHY_OFDM_N_CYCLIC_PREFIX];
//Cplx16 tOfdmSymbol[PHY_OFDM_N_FFT + PHY_OFDM_N_CYCLIC_PREFIX];

//Int8 pilotPolaritySequence[] = //length is 127
//{1,1,1,1, -1,-1,-1,1, -1,-1,-1,-1, 1,1,-1,1, -1,-1,1,1, -1,1,1,-1, 1,1,1,1, 1,1,-1,1,
//1,1,-1,1, 1,-1,-1,1, 1,1,-1,1, -1,-1,-1,1, -1,1,-1,-1, 1,-1,-1,1, 1,1,1,1, -1,-1,1,1,
//-1,-1,1,-1, 1,-1,1,1, -1,-1,-1,1, 1,-1,-1,-1, -1,1,-1,-1, 1,-1,1,1, 1,1,-1,1, -1,1,-1,1,
//-1,-1,-1,-1, -1,1,-1,1, 1,-1,1,-1, 1,1,1,-1, -1,1,-1,-1, -1,1,1,1, -1,-1,-1,-1, -1,-1,-1};

//returns ceil(a/b)
Uint32 ceil(Uint32 a, Uint32 b) {
	Uint32 q, r;
	q = a/b; r = a - q*b;
//	q = q + _min(1, r);
	q = q + (r > 0);
	return q; 
}

//returns floor(a/b)
Uint32 floor(Uint32 a, Uint32 b) {
	Uint32 q;
	q = a/b;
	return q;
}

//void WIFILIB_initFixedParams() {
//
////	int i;
////	protocolParams.MapperScaleFactor = 1000;
////	protocolParams.MapperScaleFactor = 1024;	//corresponding to Q10
//
//	/* input constellation points in frequency domain can use at most 11 bits, including the integer
//	 * part, so that 4 more bits are available for the ifft to expand into. this is because a 64 point
//	 * fft will need 6 bits to expand to avoid overflow if not scaled at all, but DSPLIB_ifft16x16 scales
//	 * down a 64 point ifft by 4, thus requiring only 4 bit room. (we only have 15 significant bits
//	 * available as this is a signed quantity).
//	 *
//	 * now, maximum constellation point size magnitude of either I or Q component is 1.0 for bpsk,
//	 * 1/sqrt(2) < 0.8 for qpsk, 3/sqrt(10) < 1 for 16qam and 7/sqrt(42) ~ 1.9 < 2.0, thus, maximum
//	 * number of integer bits required is 1. We can therefore work with a Q10 constellation mapping
//	 * as the highest precision.
//	 */
////	protocolParams.MapperScaleFactor = 1024;	//this is the factor that is applied to 1.0. thus,
//												//1 will be mapped to it's Q10 representation if
//												//it is multiplied by MapperScaleFactor = 1024
//	protocolParams.MapperScaleFactor = 1 << PHY_CONST_MAPPER_UNIT_SCALE;
//
//	/* Note that the above description means that after dsplib ifft, the resulting time domain
//	 * signal will actually be expressed as Q14 representation of the actual regular ifft'ed time
//	 * domain signal, that is, one obtained by computing (1/N)sigma X(w)exp(...). MATLAB, for
//	 * example, computes (1/N)sigma X(w)exp(...). Thus, for a matlab generated preamble sequence
//	 * to be on the same scale as our ifft'ed signal and data symbol outputs, MATLAB ifft'ed preamble
//	 * should be scaled up by a factor of 16. In other words, it should already be expressed as Q14.
//	 */
//
//	/* InputRMS = N*2^11/sqrt(Nused)/32 */
////////////    txRxPktState->SlicerInputRMS = 4522;
//    //manu: I have no idea how this works or how it's calculated; just copied from WIMAX for now
//
////    for(i = 0; i < NumInfoBits * 2; i++)
////      CINR[i] = 0x7FFF;
//
//	tablePtrs[0] = CM_2QAM_TABLE;
//	tablePtrs[1] = CM_4QAM_TABLE;
//	tablePtrs[2] = CM_16QAM_TABLE;
//	tablePtrs[3] = CM_64QAM_TABLE;
//
//    WIFILIB_constMapperTabGen(
//            protocolParams.MapperScaleFactor,
//            tablePtrs);
//
//    WIFILIB_genDerotTable();
//
//    WIFILIB_genSoftSlicingTable16qam(
//			PHY_SOFT_SLICER_UNNORM_UNIT_SCALE,
//			PHY_16QAM_NORMALIZATION_FACTOR_Q15,
//			PHY_SOFT_BITS_PRECISION,
//			PHY_16QAM_SLAB_P1P3MSB,
//			PHY_16QAM_SLAB_P1P3_TO_SOFT_VAL,
//			PHY_16QAM_SLAB_M1P1_TO_SOFT_VAL
//			);
//
//    WIFILIB_genSoftSlicingTable64qam(
//    		PHY_SOFT_SLICER_UNNORM_UNIT_SCALE,
//    		PHY_64QAM_NORMALIZATION_FACTOR_Q15,
//    		PHY_SOFT_BITS_PRECISION
//    		);
//
////	memset(fFftScratchPad, 0, (PHY_OFDM_N_FFT + PHY_OFDM_N_CYCLIC_PREFIX) * sizeof(Cplx16));
////	fFftScratchPad = (Cplx16  *)mem1;
////	tOfdmSymbol = (Cplx16 *)mem2;
////	for (i = 0; i < PHY_OFDM_N_FFT + PHY_OFDM_N_CYCLIC_PREFIX; i++) {
////////////	for (i = 0; i < PHY_OFDM_N_FFT; i++) {
////////////		txFFftScratchPad[i].real = 0;
////////////		txFFftScratchPad[i].imag = 0;
////////////	}
////////////	for (i = 0; i < PHY_OFDM_N_FFT + PHY_OFDM_N_CYCLIC_PREFIX; i++) {
////////////		txTOfdmSymbol[i].real = 0;
////////////		txTOfdmSymbol[i].imag = 0;
////////////	}
//}

//void wifi_setSigPktState(TxRxPktState *sigPktState) {
//	wifi_setPktState(sigPktState, WIFI_DATARATE_6, 3, INCLUDING_TAIL);
//}


//kWifiGee6mbps = 0,
//kWifiGee9mbps = 1,
//kWifiGee12mbps = 2,
//kWifiGee18mbps = 3,
//kWifiGee24mbps = 4,
//kWifiGee36mbps = 5,
//kWifiGee48mbps = 6,
//kWifiGee54mbps = 7

/* set the modulation scheme and coding rate, together with 
 * affected parameters */
//void wifi_setPktState(TxRxPktState *txRxPktState, Uint8 dataRate, Uint32 numInfoOctets, Uint32 includingTail) {
void wifi_setPktState(TxRxPktState *txRxPktState, Uint8 dataRate, Uint32 numInfoOctets) {
//	TxRxPktState *txRxPktState = retState;
	txRxPktState->DataRate = dataRate;
	txRxPktState->NumInfoBits = numInfoOctets * 8;
	txRxPktState->NumInfoOctets = numInfoOctets;
	switch(dataRate) {
		case kWifiGee6mbps:
		  txRxPktState->NumCodedBitsPerCarrier	= 1; //BPSK
	      txRxPktState->CodingRateTimes120 		= 60;
	      break;
	    case kWifiGee9mbps:
		  txRxPktState->NumCodedBitsPerCarrier	= 1; //BPSK
	      txRxPktState->CodingRateTimes120 		= 90;
	      break;
	    case kWifiGee12mbps:
		  txRxPktState->NumCodedBitsPerCarrier	= 2; //QPSK
	      txRxPktState->CodingRateTimes120 		= 60;
	      break;
	    case kWifiGee18mbps:
		  txRxPktState->NumCodedBitsPerCarrier	= 2; //QPSK
	      txRxPktState->CodingRateTimes120 		= 90;
	      break;
	    case kWifiGee24mbps:
		  txRxPktState->NumCodedBitsPerCarrier	= 4; //16QAM
	      txRxPktState->CodingRateTimes120 		= 60;
	      break;
	    case kWifiGee36mbps:
		  txRxPktState->NumCodedBitsPerCarrier	= 4; //16QAM
	      txRxPktState->CodingRateTimes120 		= 90;
	      break;
	    case kWifiGee48mbps:
		  txRxPktState->NumCodedBitsPerCarrier	= 6; //64QAM
	      txRxPktState->CodingRateTimes120 		= 80;
	      break;
	    case kWifiGee54mbps:
		  txRxPktState->NumCodedBitsPerCarrier	= 6; //64QAM
	      txRxPktState->CodingRateTimes120 		= 90;
	      break;
	}
	
	txRxPktState->NumCodedBitsPerSymbol = 48 * txRxPktState->NumCodedBitsPerCarrier;
	txRxPktState->NumDataBitsPerSymbol = txRxPktState->NumCodedBitsPerSymbol * txRxPktState->CodingRateTimes120 / 120;
	txRxPktState->NumDataBytesPerSymbol = txRxPktState->NumDataBitsPerSymbol >> 3; /*bytes*/ 
	txRxPktState->NumDataWordsPerSymbol = (txRxPktState->NumDataBytesPerSymbol >> 2) /*words*/ 
											+ ((txRxPktState->NumDataBytesPerSymbol & 0x3) ? 1:0);
	
	
	Uint32 includingTail = 0;
//	txRxPktState->NumUncodedBits = ceil((txRxPktState->NumInfoBits + (1 - includingTail) * PHY_TAIL_SIZE_BITS),
	txRxPktState->NumUncodedBits = ceil((txRxPktState->NumInfoBits + (1 - includingTail) * PHY_TAIL_SIZE_BITS   + PHY_SERVICE_SIZE_BITS),
									txRxPktState->NumDataBitsPerSymbol) * txRxPktState->NumDataBitsPerSymbol; 
	txRxPktState->NumPadBits = txRxPktState->NumUncodedBits - txRxPktState->NumInfoBits - (1 - includingTail) * PHY_TAIL_SIZE_BITS;
	txRxPktState->NumCodedBits = (txRxPktState->NumUncodedBits * 120)/txRxPktState->CodingRateTimes120;
	txRxPktState->NumConstPnts = txRxPktState->NumCodedBits/txRxPktState->NumCodedBitsPerCarrier;
	txRxPktState->ModulationSchemeIndex = txRxPktState->NumCodedBitsPerCarrier >> 1;
	txRxPktState->NumDataSymbols = txRxPktState->NumUncodedBits/txRxPktState->NumDataBitsPerSymbol;
	txRxPktState->NumDataSymbolsWithServicePlusDataParts = ceil((txRxPktState->NumInfoBits + PHY_SERVICE_SIZE_BITS),
				txRxPktState->NumDataBitsPerSymbol);
	txRxPktState->isSpecialCase = !(txRxPktState->NumDataSymbolsWithServicePlusDataParts == txRxPktState->NumDataSymbols);
	
	////viterbi decoding related computation


	
	
//	switch (txRxPktState->NumCodedBitsPerCarrier) {
//		case 1:
//		  txRxPktState->SlicerScaleFactor = 1;
//		  txRxPktState->SlicerRMS = txRxPktState->SlicerInputRMS;
////////		  txRxPktState->softSlicer = &WIFILIB_softSlicerBPSKTab;
//		  break;
//		case 2:
//		  txRxPktState->SlicerScaleFactor = 1;
//		  txRxPktState->SlicerRMS = (txRxPktState->SlicerInputRMS * 23170 + 0x4000) >> 15;
////////		  txRxPktState->softSlicer = &WIFILIB_softSlicerQPSKTab;
//		  break;
//		case 4:
//		  //SlicerScaleFactor = -1;
//		  txRxPktState->SlicerScaleFactor = 1;
//		  txRxPktState->SlicerRMS= (txRxPktState->SlicerInputRMS * 10362 + 0x4000) >> 15;
////////		  txRxPktState->softSlicer = &WIFILIB_softSlicerQAM16Tab;
//		  break;
//		case 6:
//		  //SlicerScaleFactor = -2;
//		  txRxPktState->SlicerScaleFactor = 1;
//		  txRxPktState->SlicerRMS = (txRxPktState->SlicerInputRMS * 5056 + 0x4000) >> 15;  //RMS/sqrt(42)
////////		  txRxPktState->softSlicer = &WIFILIB_softSlicerQAM64Tab;
//		  break;
//	}
	
//	txRxPktState->NumSymbols = txRxPktState->NumCodedBits/txRxPktState->NumCodedBitsPerSymbol;
//	if (retState != NULL)
//		retState = &txRxPktState;
}



