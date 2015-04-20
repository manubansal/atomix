/**
Atomix project, vitdec_profile.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
Created on: Jan 7, 2012
*/

#ifndef VITDEC_STANDARD_PROFILES_H_
#define VITDEC_STANDARD_PROFILES_H_

#include <osl/inc/swpform.h>
//#include "../../inc/vcpdriver/vcpdriver_api.h"

typedef struct
{
	//----fixed parameters------
    /** Code rate */
    IN Uint32 rate;
    /** Constraint length */
    IN Uint8 constLen;

    /** Yamamoto threshold value */
    IN Uint16 yamTh;
    /** Maximum initial state metric value */
    IN Uint8 stateNum;
    /** Traceback convergent mode */
    IN Bool tbConvrgMode;
    /** Output decision type */
    IN Uint8 decision;
    /** Output parameters read flag */
    IN Uint8 readFlag;
    /** Enable/Disable tail biting */
    IN Bool tailBitEnable;
    /** Tailbiting traceback index mode */
    IN Uint16 traceBackIndex;
    /** Hard decision output ordering */
    IN Uint8 outOrder;
    /** Performance and speed */
    IN Uint8 perf;

    /** Polynomial 0 */
    IN Uint8 poly0;
    /** Polynomial 1 */
    IN Uint8 poly1;
    /** Polynomial 2 */
    IN Uint8 poly2;
    /** Polynomial 3 */
    IN Uint8 poly3;

	/** Convergence length/distance guess ahead of actual csl's deduction,
	 * so that the value can be used in user configured frame length value */
	IN  Uint32 c_guess;

	/** Convergence length/distance. Our configuration generator will assert
	 * this to be the same as c_guess populated earlier to ensure that the
	 * user view of convgDistance is consistent with the csl view of it. */
	OUT Uint32 c;
	//--------------------------

    //---varying parameters-----
    /** Frame length */
    IN Uint16 frameLen;

    /** Reliability length */
	OUT Uint32 r;

	/** Number of input frames for branch metric transfer
	 * depending on edma input frame size (if applicable) */
	OUT	Uint32 inputBM_frames;

	/** Number of output hard decisions bytes depending on
	 * edma output frame size (if applicable) */
	OUT	Uint32 outputHD_bytes_rounded;

	/** Number of symbols that are expected to be processed
	 * for the given decoding mode and frameLen.	 */
	OUT Uint32 numSymProcExpected;

//	OUT VCP2_ConfigIc vcpConfig;
	//--------------------------

} Vitdec_Profile;

typedef enum Vitdec_e_Mode {
	VITDEC_HEADCONVGT,
	VITDEC_MIDCONVGT,
	VITDEC_TAILCONVGT,
	VITDEC_HEADTAIL
} Vitdec_Mode;

typedef enum Vitdec_e_ConvergenceLength {
	VITDEC_CL_18 = 18,
	VITDEC_CL_36 = 36,
	VITDEC_CL_54 = 54,
	VITDEC_CL_72 = 72
} Vitdec_ConvergenceLength;

typedef enum Vitdec_e_FrameLength {
	VITDEC_FL_18 = 18,
	VITDEC_FL_54 = 54,
	VITDEC_FL_108 = 108
} Vitdec_FrameLength;

typedef struct {
    //viterbi decoding map parameters
    Int32 n0;

    Int32 n1;
    Int32 l1;
    Int32 d1;
    Int32 L1;
    Int32 p1;
    Int32 q1;

    Int32 n2;
    Int32 l2;
    Int32 d2;
    Int32 L2;
    Int32 p2;
    Int32 q2;

    Int32 n3;
    Int32 l3;
    Int32 d3;
    Int32 L3;
    Int32 p3;
    Int32 q3;

    Int32 n4;
    Int32 l4;
    Int32 d4;
    Int32 L4;
    Int32 p4;
    Int32 q4;

    Int32 nMidIterations;
} Vitdec_ViterbiDecodingMap;


//------------------------------------------------------------------------------------------
void vitdec_profile_init();
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
//void vitdec_profile_wifiGee_populate(
//		OUT Vitdec_Profile* vdp
//		);

void vitdec_profile_wifiGee_populate_convergent(
		OUT Vitdec_Profile* vdp,
		IN  Vitdec_ConvergenceLength convergenceLength
		);

void vitdec_profile_wifiGee_populate_tailed(
		OUT Vitdec_Profile* vdp,
		IN  Vitdec_ConvergenceLength convergenceLength
		);
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
void vitdec_profile_setDataLength_head_convergent(
		IN	Uint32 data_length,
		INOUT 	Vitdec_Profile* headVdp);

void vitdec_profile_setDataLength_mid_convergent(
		IN	Uint32 data_length,
		INOUT 	Vitdec_Profile* midVdp);

void vitdec_profile_setDataLength_tail_nohead(
		IN	Uint32 data_length_bits,
		INOUT 	Vitdec_Profile* tailVdp);

void vitdec_profile_setDataLength_head_tail(
		IN	Uint32 data_length_bits,
		INOUT 	Vitdec_Profile* headTailVdp);
//------------------------------------------------------------------------------------------

void vitdec_wifiGee_setViterbiDecodingMap(
	IN  Uint32 numInfoBits,
	IN  Uint32 numDataBitsPerSymbol,
	IN  Uint32 frameLength, 
	IN  Uint32 convergenceLength,
	OUT Vitdec_ViterbiDecodingMap *vitdecMap
	);


#endif /* VITDEC_STANDARD_PROFILES_H_ */
