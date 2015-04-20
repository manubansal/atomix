/**
Atomix project, wifi_parameters.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#ifndef wifi_PARAMETERS_
#define wifi_PARAMETERS_

#include <osl/inc/or_types.h>
#include <orlib/orlib_blocks/orlib_types_wifi.h>

//#define PHY_SCRAMBLER_EXTENDED_SEQUENCE_LENGTH_IN_BYTES		128
//
//#define MAC_MSS_SIZE_BYTES							2312				//maximum segment size
//#define MAC_FCS_SIZE_BYTES							4				//frame check sequence
//#define MAC_HEADER_SIZE_BYTES						30
//#define MAC_MAX_MPDU_SIZE_BYTES						MAC_HEADER_SIZE_BYTES + MAC_MSS_SIZE_BYTES + MAC_FCS_SIZE_BYTES
//
//#define PHY_MAX_PSDU_SIZE_BYTES						MAC_MAX_MPDU_SIZE_BYTES
////#define NUM_INFO_BYTES							108
////#define MAX_NUM_INFO_BYTES						1512
//#define PHY_MAX_NUM_INFO_BYTES_IN_PKT				PHY_MAX_PSDU_SIZE_BYTES
//#define PHY_MAX_NUM_INFO_BITS_IN_PKT				PHY_MAX_NUM_INFO_BYTES_IN_PKT * 8
//#define PHY_MAX_NUM_INFO_WORDS_IN_PKT				PHY_MAX_NUM_INFO_BYTES_IN_PKT / 4
//
////#define RATETIMES120 								60		//works fine, verified
////#define RATETIMES120								80		//works fine after modifications, verified
////#define RATETIMES120 								90		//works fine after rewrite, verified
//#define MAX_NUM_CODED_BITS 							PHY_MAX_NUM_INFO_BITS_IN_PKT * 2 	//NUM_INFO_BITS * 120/RATETIMES120
//													//set to NUM_INFO_BITS * 2 to be safe,
//													//so that it works for 1/2 and any higher rate
//
//
//#define PHY_HEADER_SIZE_BITS						40
//#define PHY_SIGNAL_SIZE_BITS						24
////#define PHY_SIGNAL_SIZE_BYTES						PHY_SIGNAL_SIZE_BITS/8
//#define PHY_SIGNAL_SIZE_BYTES						4
//#define PHY_SIGNAL_SIZE_SYMBOLS						1
#define PHY_SERVICE_SIZE_BITS						16
//#define	PHY_SERVICE_SIZE_SYMBOLS					1
#define PHY_TAIL_SIZE_BITS							6
//#define	PHY_MAX_PAD_SIZE_BITS						PHY_OFDM_MAX_NDBPS_BITS //actually, it should be 1 less,
//													//because that is the maximum you will ever need to round
//													//your data bits to by adding pad bits, but this is to keep
//													//every even (multiple of 8)
//
//#define PHY_CONSTRAINT_LENGTH						7
//#define PHY_MAX_CONVERGENCE_MULTIPLIER				10	//TODO: this is a hardware/vcp dependent parameter
//														//we will run into problems if this does not match
//														//the vcp value. is there a better way to use this
//														//constant?
//#define PHY_MAX_CONVERGENCE_DISTANCE				(PHY_CONSTRAINT_LENGTH - 1) * PHY_MAX_CONVERGENCE_MULTIPLIER
////#define PHY_CODE_RATE								CONVOLUTIONAL_RATE_1_2
//#define PHY_CODE_RATE_INV							2
//#define PHY_NUM_BRANCH_METRICS_PER_SYMBOL			2	//per coded symbol (that is, one uncoded bit or one coded symbol of two bits)
////#define PHY_POLYNOMIAL_0							0x6C
////#define PHY_POLYNOMIAL_1							0xE4
////#define PHY_POLYNOMIAL_2							0x00
////#define PHY_POLYNOMIAL_3							0x00
//
//
//#define PHY_OFDM_MAX_NDBPS_BITS						216
//#define PHY_OFDM_MAX_NDBPS_BYTES					(PHY_OFDM_MAX_NDBPS_BITS/8)
//#define PHY_OFDM_MIN_NDBPS_BITS						24	//TODO: verify this one!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//#define PHY_OFDM_N_SD								48	//number of data subcarriers
//#define PHY_OFDM_N_SP								4	//number of pilot subcarriers
//#define PHY_OFDM_N_ST								PHY_OFDM_N_SD + PHY_OFDM_N_SP
//
//#define PHY_PREAMBLE_SIZE_SYMBOLS					12
//#define PHY_MAX_DATA_SIZE_BITS						PHY_SERVICE_SIZE_BITS + PHY_MAX_PSDU_SIZE_BYTES * 8 + PHY_TAIL_SIZE_BITS + PHY_MAX_PAD_SIZE_BITS
//#define PHY_MAX_DATA_SIZE_SYMBOLS					PHY_MAX_DATA_SIZE_BITS/PHY_OFDM_MIN_NDBPS_BITS
//
//#define PHY_MAX_PPDU_SIZE_SYMBOLS					PHY_PREAMBLE_SIZE_SYMBOLS + PHY_SIGNAL_SIZE_SYMBOLS + PHY_MAX_DATA_SIZE_SYMBOLS
//
//#define PHY_MAX_FEC_BLOCK_SIZE_BITS					PHY_MAX_DATA_SIZE_BITS
//
//
//#define PHY_OFDM_N_FFT								64
//#define PHY_OFDM_N_CYCLIC_PREFIX					16
//#define PHY_OFDM_N_TIME_SAMPLES						PHY_OFDM_N_FFT + PHY_OFDM_N_CYCLIC_PREFIX
//#define PHY_OFDM_N_PILOTS							4
//
//#define PHY_MAX_NUM_TIME_SAMPLES					PHY_OFDM_N_TIME_SAMPLES * PHY_MAX_PPDU_SIZE_SYMBOLS
//
//#define PHY_PREAMBLE_SIZE_SAMPLES					PHY_PREAMBLE_SIZE_SYMBOLS * PHY_OFDM_N_TIME_SAMPLES
//#define PHY_SIGNAL_SIZE_SAMPLES						PHY_SIGNAL_SIZE_SYMBOLS * PHY_OFDM_N_TIME_SAMPLES
//#define PHY_MAX_PAYLOAD_SIZE_SAMPLES				PHY_MAX_DATA_SIZE_SYMBOLS * PHY_OFDM_N_TIME_SAMPLES
//
////#define PHY_OFDM_N_CYCLIC_PREFIX_TO_DROP 			8
//#define PHY_OFDM_N_CYCLIC_PREFIX_TO_DROP 			16
//
//
//
////-----------------------------  bit precision and gain values related parameters  ---------------------
////tx side
//#define DSPLIB_IFFT_SCALING_FACTOR_BITS				2		//factor is 4, that is, output is divided by 4 (2 bits)
//#define PHY_CONST_MAPPER_UNIT_SCALE					10		//number of bits of precision, that is, Q value, for mapping 1.0
//
////rx side
//#define PHY_ALLOWED_AVERAGE_SAMPLE_WIDTH_AFTER_AGC	8		//we are choosing 8 to keep enough room for FFT to expand
//															//into, as it will add up 64 quantites. dsplib fft does
//															//scale down by 4 (2 bits), so if all samples are strictly
//															//contained in PHY_ALLOWED.. number of bits, we can actually
//															//set it to 10. however, since this is only the average number
//															//of bits in input samples and not the exact number, we are
//															//keeping a headroom of two bits.
//
//#define DSPLIB_FFT_SCALING_FACTOR_BITS				2		//factor is 4, that is, output is divided by 4 (2 bits)
//
//#define PHY_SOFT_SLICER_UNNORM_UNIT_SCALE			6		//Q6  this is because the biggest normalized
//															//magnitude we need to cater to is 4 * 1/sqrt(10) for 16qam
//															//and 9 * 1/sqrt(42) for 64 qam
//															//where any value bigger than these is saturated.
//															//4 * 1/sqrt(10) < 2 and 8 * 1/sqrt(42) < 2 =>
//															//we can form the lookup index as S1I1Q6.
//
//#define PHY_SOFT_BITS_PRECISION						6		//vcp2's dynamic range for rate = 1/2, K = 7 is
//															//[-64,63]
////---------------------------------------------------------------------------------------------------
//
//
////------------------------------- constellation map characteristics ---------------------------------
//#define PHY_QPSK_NORMALIZATION_FACTOR_Q15			23170	//1/sqrt(2)
//#define PHY_16QAM_NORMALIZATION_FACTOR_Q15			10362	//1/sqrt(10)
//#define PHY_64QAM_NORMALIZATION_FACTOR_Q15			5056	//1/sqrt(42)
//
//#define PHY_16QAM_SLAB_P1P3MSB						1		//+1,+3 slab occupies the MSB (see softSlicerTabGen.c)
//#define PHY_16QAM_SLAB_P1P3_TO_SOFT_VAL				1		//+1,+3 slab bits map to +1 (see softSlicerTabGen.c)
//#define PHY_16QAM_SLAB_M1P1_TO_SOFT_VAL				1		//-1,+1 slab bits map to +1 (see softSlicerTabGen.c)
////---------------------------------------------------------------------------------------------------
//


//#define	WIFI_DATARATE_6  							0xB	//bpsk 1/2
//#define	WIFI_DATARATE_9  							0xF	//bpsk 3/4
//#define	WIFI_DATARATE_12 							0xA	//qpsk 1/2
//#define	WIFI_DATARATE_18							0xE	//qpsk 3/4
//#define	WIFI_DATARATE_24 							0x9	//16q  1/2
//#define	WIFI_DATARATE_36 							0xD	//16q  3/4
//#define	WIFI_DATARATE_48							0x8	//64q  2/3
//#define	WIFI_DATARATE_54 							0xC	//64q  3/4


//#define  MODULOFACTOR 								16			//for the interleaver

//typedef struct {
//	Cplx16 preamble[PHY_PREAMBLE_SIZE_SAMPLES];
//	Cplx16 signal[PHY_SIGNAL_SIZE_SAMPLES];
//	Cplx16 payload[PHY_MAX_PAYLOAD_SIZE_SAMPLES];
//} WIFI_PHY_FRAME;


typedef struct S_TxRxPktState {
	/* independent parameters - need to be set for each packet */
	Uint32 NumInfoBits;					//number of uncoded bits in PSDU (including tail and padding) (phy layer payload)
	Uint8  DataRate;					//rate index (determines modulation and coding rate)
	
	/* derived parameters - not set by the user (set automatically) */
	Uint8  ModulationSchemeIndex;		//0 for BPSK, 1 for QPSK, 2 for 16QAM, 3 for 64QAM
	Uint8  NumCodedBitsPerCarrier;  	//N_bpsc (modulation)
	Uint8  CodingRateTimes120;			//		 (coding)
	Uint16 NumInfoOctets;
	
	Uint16 NumCodedBitsPerSymbol;   	//N_cbps (derived from modulation)
	Uint16 NumDataBitsPerSymbol;		//N_dbps (derived from modulation and coding)
	Uint32 NumDataBytesPerSymbol;
	Uint32 NumDataWordsPerSymbol;
	Uint32 NumUncodedBits;				//number of uncoded bits in PPDU (including tail and padding)
	Uint32 NumCodedBits;				//total number of coded bits
	Uint32 NumPadBits;					//number of pad bits, excluding tail bits
	
//	Uint32 NumSymbols;					//number of ofdm symbols (NumCodedBits/NumCodedBitsPerSymbol)
	
	Uint32 NumConstPnts;
	Uint32 NumDataSymbols;
	Uint32 NumDataSymbolsWithServicePlusDataParts;	//span of data and service portion, excluding tail and pad bits
	Uint32 isSpecialCase;
	
//	Int8   SlicerScaleFactor;
//	Uint16 SlicerInputRMS;
//	Uint16 SlicerRMS;
	
	
//	void (*softSlicer)(
//                  Cplx16   txConstPnts[restrict],
////                  Uint32   NumInputConstPnts,
////                  Uint8    NumCodedBitsPerCarrier,
////                  Int8     ScaleFactor,
////                  Int32    SoftBitsPrecision,
////                  Uint16   Rms,
//                  Uint16   Cinr[restrict],
//                  Int8     OutputSoftBits[restrict],
//                  struct S_TxRxPktState *pktState);
    
    Uint32 currPktPortion;
    Uint32 currSymbolIdx; //index of the current OFDM symbol we are processing
	
} TxRxPktState;

//typedef TxRxPktState *PTxRxPktState;

//enum {
//	PKT_PORTION_PREAMBLE,
//	PKT_PORTION_SIGNAL,
//	PKT_PORTION_DATA
//};

//typedef struct {
//	Uint32 MapperScaleFactor;
//} ProtocolParams;

//extern Cplx16 preambleSamples[];
//declared in WIFILIB_preamble.h instead

//extern TxRxPktState txRxPktState;
//extern ProtocolParams protocolParams;

//extern Int32  *tablePtrs[4] ;

//extern Uint16 CINR[NUM_INFO_BITS * 2];
//extern Uint16 CINR[];

//#define INCLUDING_TAIL 		1
//#define NOT_INCLUDING_TAIL 	0
//void wifi_setPktState(TxRxPktState *retState, Uint8 dataRate, Uint32 NumInfoOctets, Uint32 includingTail);
void wifi_setPktState(TxRxPktState *retState, Uint8 dataRate, Uint32 NumInfoOctets);

//void wifi_setSigPktState(TxRxPktState *sigPktState);

/** Initialize fixed parameters for wifi, such as constellation lookup tables
 * and constellation rotation/derotation tables and perform one time initializations
 * such as zeroing out certain buffers. This function needs to be called only once
 * at system boot.
 */
//void wifi_initFixedParams();

//void (*softSlicer)(
//            IN      Cplx16,
//            IN      Uint32,
//            IN      Uint8,
//            IN      Int8,
//            IN      Int32,
//            IN      Uint16, 
//            IN      Uint16,
//            OUT     Int8) = NULL;

//extern void (*softSlicer)(
//                  Cplx16   ConstPnts[restrict],
//                  Uint32   NumInputConstPnts,
//                  Uint8    NumCodedBitsPerCarrier,
//                  Int8     ScaleFactor,
//                  Int32    SoftBitsPrecision,
//                  Uint16   Rms, 
//                  Uint16   Cinr[restrict],
//                  Int8     OutputSoftBits[restrict]);
//            = NULL;				

#endif /*wifi_PARAMETERS_*/
