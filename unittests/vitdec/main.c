/**
Atomix project, main.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/


#define VCP2_BREAK_ON_NUM_SYM_PROC
//#define VCP2_BREAK_ON_EDMA_COMPLETION


#include <stdio.h>
#include <osl/inc/swpform.h>
#include <osl/vitdec/inc/vcpdriver/vcpdriver_api.h>
#include <osl/vitdec/inc/vitdec/vitdec_profile.h>
#include <ti/csl/csl_tsc.h>

#pragma DATA_SECTION(vcpConfig, ".gem0_data")
#pragma DATA_ALIGN(vcpConfig, 8);
VCP2_ConfigIc vcpConfig;

/* Space for the banch metrics */
extern Uint32 branch_metric[];

/* Space for the hard decisions */
//extern Uint32 hard_decision[];
extern Uint32 hard_decision0[];
extern Uint32 hard_decision1[];
extern Uint32 hard_decision2[];
extern Uint32 hard_decision3[];
extern Int8 branch_metrics_headpad_message_tailpad[];
extern Int8 branch_metrics_headpad_message_tailed[];
extern Int8 branch_metrics_headpad_message_zext_tail[];

Uint64 outputParams;


//Uint32 f = 108;	//no. of data bits i want decoded on every call
Uint32 f[] = {900, 800, 700, 600, 500, 400, 300, 200, 100, 50};
Uint32 r[20], c[20], inpLength[20], outLength[20];
CSL_Uint64 dt_decode[20], dt_config[20], dt_issuego[20], dt_edmaset[20], dt_overhead[20];
Uint32 i;
Uint32 vcpi0 = 0, vcpi1 = 1, vcpi2 = 2, vcpi3 = 3;
Uint32 vcpiThis = 0;
volatile Uint32 cword0 = 0, 
		cword1 = 0, cword2 = 0, cword3 = 0;

Uint32 status = 0;
volatile CSL_Uint64 tp0, tp1, tp2;
volatile CSL_Uint64 tt[20];


#define BITS_IN_A_BYTE 8

void print_output (
	Uint32 *hard_decision, 
	Uint32 nbits) {
				
	Uint32 index;

	for (index = 0; index < (ceil(ceil(nbits, BITS_IN_A_BYTE), 4)); index++) {
		printBitsMsb(hard_decision[index]);
		printf("%d\n", hard_decision[index]);
	}
}

void print_output_hex (
	Uint32 *hard_decision, 
	Uint32 nbits) {
				
	Uint32 index;
	Uint8 * bytes = (Uint8 *)hard_decision;

	for (index = 0; index < ceil(nbits, 8); index++) {
		if (index % 4 == 0)
		  printf("\n");
		printf("%02X", bytes[index]);
	}
	printf("\n");
}


//One time configurations
void init() {
	vitdec_profile_init();

	vcp2_initOnce();
	vcp2_initPerUse(0,0,0,0);
	
	printf("vcp driver initialized\n");
	
	CSL_tscEnable();
}

//WORKS
void test_head_convgt() {
	tt[0] = CSL_tscRead();
	volatile CSL_Uint64 tt1[20];
	volatile CSL_Uint64 tt2[20];
	volatile CSL_Uint64 tt3[20];
	int len;

	Uint32 f;
	Vitdec_Profile vdp;
	//vitdec_profile_wifiGee_populate_convergent(&vdp, VITDEC_CL_18);
	vitdec_profile_wifiGee_populate_convergent(&vdp, VITDEC_CL_36);
	
	//f = 54;
	f = 32;

	vitdec_profile_setDataLength_head_convergent(f, &vdp);

	vcp2_genConf(&vdp, &vcpConfig);

	vcp2_initPerUse(&vcpConfig, &outputParams,
				vdp.inputBM_frames, vdp.outputHD_bytes_rounded);

	tt[1] = CSL_tscRead();
	vcp2_decode(vcpi0, &vcpConfig, &branch_metric[0], hard_decision0,
		vdp.inputBM_frames, vdp.outputHD_bytes_rounded, &cword0, tt);
	vcp2_decode(vcpi1, &vcpConfig, &branch_metric[16], hard_decision1,
		vdp.inputBM_frames, vdp.outputHD_bytes_rounded, &cword0, tt1);
	vcp2_decode(vcpi2, &vcpConfig, &branch_metric[32], hard_decision2,
		vdp.inputBM_frames, vdp.outputHD_bytes_rounded, &cword0, tt2);
	vcp2_decode(vcpi3, &vcpConfig, &branch_metric[48], hard_decision3,
		vdp.inputBM_frames, vdp.outputHD_bytes_rounded, &cword0, tt3);


	//Wait for completion
	tp0 = CSL_tscRead();

	len = f + vdp.c;
	vcp2_waitForCompletion(len, vcpi0, BREAKTYPE_SYM);
	vcp2_waitForCompletion(len, vcpi1, BREAKTYPE_SYM);
	vcp2_waitForCompletion(len, vcpi2, BREAKTYPE_SYM);
	vcp2_waitForCompletion(len, vcpi3, BREAKTYPE_SYM);


	tp2 = CSL_tscRead();
	printf("Here\n");
	vcp2_postDecodeStat(vcpi0, hard_decision0, vdp.frameLen);
	print_output(hard_decision0, vdp.frameLen);

	vcp2_postDecodeStat(vcpi1, hard_decision1, vdp.frameLen);
	print_output(hard_decision1, vdp.frameLen);

	vcp2_postDecodeStat(vcpi2, hard_decision2, vdp.frameLen);
	print_output(hard_decision2, vdp.frameLen);
	
	vcp2_postDecodeStat(vcpi3, hard_decision3, vdp.frameLen);
	print_output(hard_decision3, vdp.frameLen);

	vcp2_close();

}

//WORKS
void test_mid_convgt() {
	tt[0] = CSL_tscRead();
	volatile CSL_Uint64 tt1[20];
	volatile CSL_Uint64 tt2[20];
	volatile CSL_Uint64 tt3[20];
	int len;

	Uint32 f;
	Vitdec_Profile vdp;
	vitdec_profile_wifiGee_populate_convergent(&vdp, VITDEC_CL_36);
	
	f = 32;

	vitdec_profile_setDataLength_mid_convergent(f, &vdp);

	vcp2_genConf(&vdp, &vcpConfig);

	vcp2_initPerUse(&vcpConfig, &outputParams,
				vdp.inputBM_frames, vdp.outputHD_bytes_rounded);

	tt[1] = CSL_tscRead();
	vcp2_decode(vcpi0, &vcpConfig, (Uint32 *)branch_metrics_headpad_message_tailpad, hard_decision0,
		vdp.inputBM_frames, vdp.outputHD_bytes_rounded, &cword0, tt);


	//Wait for completion
	tp0 = CSL_tscRead();

	len = vdp.c + f + vdp.c;
	vcp2_waitForCompletion(len, vcpi0, BREAKTYPE_SYM);


	tp2 = CSL_tscRead();
	printf("Here\n");
	vcp2_postDecodeStat(vcpi0, hard_decision0, vdp.frameLen);
	print_output_hex(hard_decision0, vdp.c + f);

	//correct output: 5918AC2B9 4E59D18F x, where x denotes don't care

	vcp2_close();

}

//DOES NOT WORK
void test_mid_convgt_tailed() {
	tt[0] = CSL_tscRead();
	volatile CSL_Uint64 tt1[20];
	volatile CSL_Uint64 tt2[20];
	volatile CSL_Uint64 tt3[20];
	int len;

	Uint32 f, c;
	Vitdec_Profile vdp;

	vitdec_profile_wifiGee_populate_tailed(&vdp, VITDEC_CL_36);
	c = 36;
	f = 32;

	vitdec_profile_setDataLength_tail_nohead(f, &vdp);

	vcp2_genConf(&vdp, &vcpConfig);

	vcp2_initPerUse(&vcpConfig, &outputParams,
				vdp.inputBM_frames, vdp.outputHD_bytes_rounded);

	tt[1] = CSL_tscRead();
	vcp2_decode(vcpi1, &vcpConfig, branch_metrics_headpad_message_tailed, hard_decision1,
		vdp.inputBM_frames, vdp.outputHD_bytes_rounded, &cword0, tt);


	//Wait for completion
	tp0 = CSL_tscRead();

	len = f + vdp.c + 6;
	vcp2_waitForCompletion(len, vcpi0, BREAKTYPE_SYM);


	tp2 = CSL_tscRead();
	printf("Here\n");
	vcp2_postDecodeStat(vcpi1, hard_decision1, vdp.frameLen);
	//print_output_hex(hard_decision1, c + f + c);
	print_output_hex(hard_decision1, 512);

	//correct output: xxxxxxxxx 4E59D18F x, where x denotes don't care

	vcp2_close();

}

void test_mid_convgt_zext_tail() {
	tt[0] = CSL_tscRead();
	volatile CSL_Uint64 tt1[20];
	volatile CSL_Uint64 tt2[20];
	volatile CSL_Uint64 tt3[20];
	int len;

	Uint32 f;
	Vitdec_Profile vdp;
	vitdec_profile_wifiGee_populate_convergent(&vdp, VITDEC_CL_36);
	
	f = 32;

	vitdec_profile_setDataLength_mid_convergent(f, &vdp);

	vcp2_genConf(&vdp, &vcpConfig);

	vcp2_initPerUse(&vcpConfig, &outputParams,
				vdp.inputBM_frames, vdp.outputHD_bytes_rounded);

	tt[1] = CSL_tscRead();

	vcp2_decode(vcpi0, &vcpConfig, (Uint32 *)branch_metrics_headpad_message_zext_tail, hard_decision0,
		vdp.inputBM_frames, vdp.outputHD_bytes_rounded, &cword0, tt);


	//Wait for completion
	tp0 = CSL_tscRead();

	len = vdp.c + f + vdp.c;
	vcp2_waitForCompletion(len, vcpi0, BREAKTYPE_SYM);


	tp2 = CSL_tscRead();
	printf("Here\n");
	vcp2_postDecodeStat(vcpi0, hard_decision0, vdp.frameLen);
	print_output_hex(hard_decision0, vdp.c + f);

	//correct output: xxxxxxxxx 4E59D18F x, where x denotes don't care

	vcp2_close();

}


void main () {
	init();

//	printf("\n\n----------------- test_head_convgt --------------- \n\n");
//	test_head_convgt();

	//printf("\n\n----------------- test_mid_convgt ---------------- \n\n");
	//test_mid_convgt();

	//printf("\n\n----------------- test_mid_convgt_tailed --------- \n\n");
	//test_mid_convgt_tailed();

	printf("\n\n----------------- test_mid_convgt_zext_tail -------- \n\n");
	test_mid_convgt_zext_tail();

}
