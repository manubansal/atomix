/**
Atomix project, vcp2data.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <osl/inc/swpform.h>

#pragma DATA_SECTION(branch_metric, ".gem0_data")
#pragma DATA_SECTION(hard_decision, ".gem0_data")
#pragma DATA_SECTION(hard_decision0, ".gem0_data")
#pragma DATA_SECTION(hard_decision1, ".gem0_data")
#pragma DATA_SECTION(hard_decision2, ".gem0_data")
#pragma DATA_SECTION(hard_decision3, ".gem0_data")
#pragma DATA_ALIGN(branch_metric, 8)
#pragma DATA_ALIGN(hard_decision, 8)
#pragma DATA_ALIGN(hard_decision0, 8)
#pragma DATA_ALIGN(hard_decision1, 8)
#pragma DATA_ALIGN(hard_decision2, 8)
#pragma DATA_ALIGN(hard_decision3, 8)

#pragma DATA_SECTION(branch_metrics_headpad_message_tailed, ".gem0_data")
#pragma DATA_SECTION(branch_metrics_headpad_message_tailpad, ".gem0_data")
#pragma DATA_SECTION(branch_metrics_headpad_message_zext_tail, ".gem0_data")
#pragma DATA_ALIGN(branch_metrics_headpad_message_tailed, 8)
#pragma DATA_ALIGN(branch_metrics_headpad_message_tailpad, 8)
#pragma DATA_ALIGN(branch_metrics_headpad_message_zext_tail, 8)

//Uint32 branch_metric[] = {
//	0xFE00FE00,
//	0xFE00FE00,
//	0xFE00FE00,
//	0xFE00FE00,
//	0xFE00FE00,
//	0xFE00FE00,
//	0xFE00FE00,
//	0xFE00FE00
//};  //-126 0 -126 0 -126 0 -126 0

//Int8 BMTestSeq32[32] = {-126, 0, 0, 126, 
//							-126, 0, -126, 0, 
//							126, 0, 0, -126, 
//							-126, 0, 126, 0, 
//							126, 0, 126, 0, 
//							126, 0, 126, 0, 
//							126, 0, 126, 0, 
//							126, 0, 126, 0};

///* 256 branch metrics ~ 128 symbols at rate 1/2 ~ 128 decoded bits ~ 4 words of decoded output */
//Uint32 branch_metric[] = {
//	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00
//};


Int8 branch_metrics_headpad_message_zext_tail[] = {
    64,
     0,
    64,
     0,
   -62,
     0,
     1,
    63,
   -62,
     0,
   -62,
     0,
   -62,
     0,
    64,
     0,
     1,
    63,
    64,
     0,
    64,
     0,
   -62,
     0,
     1,
   -63,
   -62,
     0,
     1,
    63,
     1,
    63,
    64,
     0,
    64,
     0,
     1,
    63,
     1,
   -63,
   -62,
     0,
    64,
     0,
     1,
    63,
     1,
   -63,
     1,
    63,
   -62,
     0,
     1,
   -63,
     1,
   -63,
     1,
    63,
   -62,
     0,
     1,
   -63,
    64,
     0,
   -62,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
     1,
   -63,
   -62,
     0,
    64,
     0,
     1,
   -63,
     1,
    63,
     1,
    63,
   -62,
     0,
     1,
   -63,
    64,
     0,
     1,
   -63,
    64,
     0,
     1,
    63,
     1,
   -63,
     1,
   -63,
     1,
   -63,
   -62,
     0,
    64,
     0,
    64,
     0,
     1,
   -63,
   -62,
     0,
    64,
     0,
     1,
   -63,
     1,
   -63,
    64,
     0,
    64,
     0,
     1,
    63,
    64,
     0,
   -62,
     0,
     1,
    63,
   -62,
     0,
     1,
   -63,
     1,
   -63,
     1,
    63,
     1,
   -63,
     1,
   -63,
     1,
    63,
     1,
    63,
   -62,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
};

Int8 branch_metrics_headpad_message_tailed[] = {
    64,
     0,
    64,
     0,
   -62,
     0,
     1,
    63,
   -62,
     0,
   -62,
     0,
   -62,
     0,
    64,
     0,
     1,
    63,
    64,
     0,
    64,
     0,
   -62,
     0,
     1,
   -63,
   -62,
     0,
     1,
    63,
     1,
    63,
    64,
     0,
    64,
     0,
     1,
    63,
     1,
   -63,
   -62,
     0,
    64,
     0,
     1,
    63,
     1,
   -63,
     1,
    63,
   -62,
     0,
     1,
   -63,
     1,
   -63,
     1,
    63,
   -62,
     0,
     1,
   -63,
    64,
     0,
   -62,
     0,
    64,
     0,
    64,
     0,
    64,
     0,
     1,
   -63,
   -62,
     0,
    64,
     0,
     1,
   -63,
     1,
    63,
     1,
    63,
   -62,
     0,
     1,
   -63,
    64,
     0,
     1,
   -63,
    64,
     0,
     1,
    63,
     1,
   -63,
     1,
   -63,
     1,
   -63,
   -62,
     0,
    64,
     0,
    64,
     0,
     1,
   -63,
   -62,
     0,
    64,
     0,
     1,
   -63,
     1,
   -63,
    64,
     0,
    64,
     0,
     1,
    63,
    64,
     0,
   -62,
     0,
     1,
    63,
   -62,
     0,
     1,
   -63,
     1,
   -63,
     1,
    63,
     1,
   -63,
     1,
   -63,
     1,
    63,
     1,
    63,
   -62,
     0,
};


//#Int8 branch_metrics_headpad_message_tailed[] = {
//#    64,
//#     0,
//#   -62,
//#     0,
//#     1,
//#    63,
//#    64,
//#     0,
//#     1,
//#    63,
//#     1,
//#   -63,
//#     1,
//#   -63,
//#   -62,
//#     0,
//#   -62,
//#     0,
//#     1,
//#   -63,
//#    64,
//#     0,
//#   -62,
//#     0,
//#    64,
//#     0,
//#     1,
//#    63,
//#    64,
//#     0,
//#   -62,
//#     0,
//#     1,
//#    63,
//#    64,
//#     0,
//#   -62,
//#     0,
//#     1,
//#   -63,
//#    64,
//#     0,
//#   -62,
//#     0,
//#     1,
//#    63,
//#     1,
//#   -63,
//#    64,
//#     0,
//#     1,
//#   -63,
//#     1,
//#   -63,
//#     1,
//#   -63,
//#    64,
//#     0,
//#     1,
//#   -63,
//#    64,
//#     0,
//#   -62,
//#     0,
//#     1,
//#   -63,
//#   -62,
//#     0,
//#   -62,
//#     0,
//#     1,
//#   -63,
//#    64,
//#     0,
//#     1,
//#    63,
//#     1,
//#    63,
//#   -62,
//#     0,
//#     1,
//#   -63,
//#     1,
//#    63,
//#   -62,
//#     0,
//#     1,
//#   -63,
//#    64,
//#     0,
//#     1,
//#   -63,
//#    64,
//#     0,
//#     1,
//#    63,
//#     1,
//#   -63,
//#     1,
//#   -63,
//#     1,
//#   -63,
//#   -62,
//#     0,
//#    64,
//#     0,
//#    64,
//#     0,
//#     1,
//#   -63,
//#   -62,
//#     0,
//#    64,
//#     0,
//#     1,
//#   -63,
//#     1,
//#   -63,
//#    64,
//#     0,
//#    64,
//#     0,
//#     1,
//#    63,
//#    64,
//#     0,
//#   -62,
//#     0,
//#     1,
//#    63,
//#   -62,
//#     0,
//#     1,
//#   -63,
//#     1,
//#   -63,
//#     1,
//#    63,
//#     1,
//#   -63,
//#     1,
//#   -63,
//#     1,
//#    63,
//#     1,
//#    63,
//#   -62,
//#     0,
//#};

Int8 branch_metrics_headpad_message_tailpad[] = {
    64,
     0,
   -62,
     0,
     1,
    63,
    64,
     0,
     1,
    63,
     1,
   -63,
     1,
   -63,
   -62,
     0,
   -62,
     0,
     1,
   -63,
    64,
     0,
   -62,
     0,
    64,
     0,
     1,
    63,
    64,
     0,
   -62,
     0,
     1,
    63,
    64,
     0,
   -62,
     0,
     1,
   -63,
    64,
     0,
   -62,
     0,
     1,
    63,
     1,
   -63,
    64,
     0,
     1,
   -63,
     1,
   -63,
     1,
   -63,
    64,
     0,
     1,
   -63,
    64,
     0,
   -62,
     0,
     1,
   -63,
   -62,
     0,
   -62,
     0,
     1,
   -63,
    64,
     0,
     1,
    63,
     1,
    63,
   -62,
     0,
     1,
   -63,
     1,
    63,
   -62,
     0,
     1,
   -63,
    64,
     0,
     1,
   -63,
    64,
     0,
     1,
    63,
     1,
   -63,
     1,
   -63,
     1,
   -63,
   -62,
     0,
    64,
     0,
    64,
     0,
     1,
   -63,
   -62,
     0,
    64,
     0,
     1,
   -63,
     1,
   -63,
    64,
     0,
    64,
     0,
     1,
    63,
    64,
     0,
   -62,
     0,
     1,
    63,
   -62,
     0,
     1,
   -63,
     1,
   -63,
     1,
    63,
     1,
    63,
   -62,
     0,
     1,
    63,
    64,
     0,
     1,
    63,
     1,
   -63,
   -62,
     0,
   -62,
     0,
     1,
   -63,
    64,
     0,
   -62,
     0,
    64,
     0,
     1,
    63,
    64,
     0,
   -62,
     0,
     1,
    63,
    64,
     0,
   -62,
     0,
     1,
   -63,
    64,
     0,
   -62,
     0,
     1,
    63,
     1,
   -63,
    64,
     0,
     1,
   -63,
     1,
   -63,
     1,
   -63,
    64,
     0,
     1,
   -63,
    64,
     0,
   -62,
     0,
     1,
   -63,
   -62,
     0,
   -62,
     0,
     1,
   -63,
};






/* 768 branch metrics ~ 384 symbols at rate 1/2 ~ 384 decoded bits ~ 12 words of decoded output*/	
Uint32 branch_metric[] = {
	
	//BLOCK TYPE 1
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	
	//BLOCK TYPE 2
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	
	//BLOCK TYPE 3
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	
	//BLOCK TYPE 4
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00,
	
	//----------------------- f = 128 ---------------
	
	//BLOCK TYPE 4
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00,
	
	//BLOCK TYPE 3
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
    0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,

	//BLOCK TYPE 2
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	
	//BLOCK TYPE 1
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	
	//----------------------- f = 256 ---------------
	
	//BLOCK TYPE 1
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	
	//BLOCK TYPE 2
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	
	//BLOCK TYPE 3
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	
	//BLOCK TYPE 4
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00,
	
	//----------------------- f = 384 ---------------
	
	//BLOCK TYPE 2
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	
	//BLOCK TYPE 3
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	
	//BLOCK TYPE 4
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00,
	
	//BLOCK TYPE 1
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	
	//----------------------- f = 512 ---------------
	
	
	
		//BLOCK TYPE 1
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	
	//BLOCK TYPE 2
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	
	//BLOCK TYPE 3
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	
	//BLOCK TYPE 4
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00,
	
	//----------------------- f = 128 ---------------
	
	//BLOCK TYPE 4
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00,
	
	//BLOCK TYPE 3
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
    0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,

	//BLOCK TYPE 2
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	
	//BLOCK TYPE 1
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	
	//----------------------- f = 256 ---------------
	
	//BLOCK TYPE 1
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	
	//BLOCK TYPE 2
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	
	//BLOCK TYPE 3
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	
	//BLOCK TYPE 4
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00,
	
	//----------------------- f = 384 ---------------
	
	//BLOCK TYPE 2
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	
	//BLOCK TYPE 3
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	
	//BLOCK TYPE 4
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00,
	
	//BLOCK TYPE 1
	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	
	//----------------------- f = 1024 ---------------
	
	
	/* zero padding */
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
	
	
};

///* 1536 branch metrics ~ 768 symbols at rate 1/2 ~ 768 decoded bits ~ 24 words of decoded output*/	
//Uint32 branch_metric[] = {
//	0xFE00007E, 0XFE00FE00, 0x7E0000FE, 0XFE007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00,
//	0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00, 0X7E007E00
//};


///* The banch metrics data */
//Uint32 branch_metric[] = {
//    0x29d7d583,
//    0x83d7d72b,
//    0xd42a82d8,
//    0xd52983d7,
//    0x7e2c2ad8,
//    0x81d7d52b,
//    0x297fd52b,
//    0x28d6d684,
//    0x29d77d2b,
//    0x2bd77f2b,
//    0xd62a2a7e,
//    0x29d57f2b,
//    0x29d77d2b,
//    0x2ad8d684,
//    0xd6842ad8,
//    0xd52b297f,
//    0x2a7fd62c,
//    0x7f2b2bd7,
//    0x2bd7d783,
//    0x2ad6d682,
//    0x7d292bd7,
//    0x2a7ed82c,
//    0x2ad8d684,
//    0xd62a84d8,
//    0x2a7fd62c,
//    0x2a7ed82c,
//    0x2ad67f2c,
//    0x81d7d72d,
//    0xd62a2a7e,
//    0x7e2a2cd8,
//    0xd5832bd9,
//    0x81d5d72b,
//    0x83d7d72b,
//    0xd58329d7,
//    0x7e2a2cd8,
//    0x2ad8d684,
//    0x29d7d785,
//    0x81d5d72b,
//    0x29d77d2b,
//    0xd58329d7,
//    0x2ad87e2c,
//    0x7e2c2ad8,
//    0x81d6d62c,
//    0xd58329d7,
//    0x81d7d72d,
//    0x7d292bd7,
//    0xd62a84d8,
//    0x29d77d2b,
//    0x29d7d785,
//    0x287ed62c,
//    0x7e2c2ad8,
//    0x2ad6d682,
//    0x2ad6d682,
//    0x7f2b2bd7,
//    0xd6822cd8,
//    0x81d6d62c,
//    0x7f2b2bd7,
//    0xd52b81d7,
//    0x29d77d2b,
//    0xd62a2c7f,
//    0xd72b83d7,
//    0x2a7ed82c,
//    0xd6822ad6,
//    0xd7832bd7,
//    0xd6822ad6,
//    0x297dd72b,
//    0x2ad87e2c,
//    0x7d292bd7,
//    0x81d6d62c,
//    0x2b7fd72b,
//    0xd5292b7f,
//    0x297fd52b,
//    0x81d7d72d,
//    0x7d292bd7,
//    0x29d77f2d,
//    0x2a7ed82c,
//    0x287ed62c,
//    0xd52b83d9,
//    0x29d7d785,
//    0xd6822ad6,
//    0xd42a2a7f,
//    0xd6842ad8,
//    0x2ad8d684,
//    0x29d77d2b,
//    0x29d57f2b,
//    0x2ad8d684,
//    0xd58329d7,
//    0xd5292b7f,
//    0x297fd72d,
//    0x81d5d72b,
//    0x7c2a2ad8,
//    0xd72b83d7,
//    0x29d77f2d,
//    0xd42a2a7f,
//    0xd42a82d8,
//    0x2a7ed82c,
//    0xd6842ad8,
//    0x2a7ed82c,
//    0x28d67e2c,
//    0x81d6d62c,
//    0xd52b297f,
//    0x81d5d72b,
//    0xd62a84d8,
//    0x29d57f2b,
//    0xd62c2a7f,
//    0xd62a84d8,
//    0x2a7fd62c,
//    0x2a7fd62c,
//    0x29d57f2b,
//    0x82d6d82c,
//    0xd52b2b7f,
//    0x7f2b2bd7,
//    0x2b7fd72b,
//    0x2ad8d684,
//    0xd62a82d6,
//    0x297fd52b,
//    0x297dd72b,
//    0x28d67e2c,
//    0x81d7d72d,
//    0x29d7d785,
//    0x2bd7d783,
//    0x81d7d72d,
//    0xd52983d7,
//    0xd62a84d8,
//    0x7e2a2cd8,
//    0x82d6d62a,
//    0xd6822cd8,
//    0x7e2c2ad8,
//    0x2ad8d684,
//    0x29d7d583,
//    0x7d292bd7,
//    0xd6822cd8,
//    0x81d7d52b,
//    0x7f2b2bd7,
//    0x29d77d2b,
//    0x297dd72b,
//    0xd6822ad6,
//    0xd6822ad6,
//    0x287ed62c,
//    0xd62a82d6,
//    0xd62a2c7f,
//    0x29d77d2b,
//    0xd52983d7,
//    0x81d7d72d,
//    0xd62a2a7e,
//    0x7c2a2ad8,
//    0xd58329d7,
//    0x7c2a2ad8,
//    0xd62c2a7f,
//    0x82d6d62a,
//    0x2ad67f2c,
//    0xd5812bd7,
//    0x29d77f2d,
//    0x7e2c2ad8,
//    0x82d8d62c,
//    0xd4822ad8,
//    0x7d2b29d7,
//    0x28d6d684,
//    0xd42a2a7f,
//    0x29d7d785,
//    0x7d2b2bd9,
//    0xd5832bd9,
//    0x7d292bd7,
//    0xd42a2a7f,
//    0x81d7d52b,
//    0xd52983d7,
//    0xd62a84d8,
//    0x7d2b2bd9,
//    0x7d2b29d7,
//    0x2ad67f2c,
//    0xd4822ad8,
//    0x2ad87e2c,
//    0x81d5d72b,
//    0xd62a2a7e,
//    0x82d6d82c,
//    0x29d57f2b,
//    0xd58329d7,
//    0x28d67e2c,
//    0x81d7d72d,
//    0x2ad6d884,
//    0xd62c2a7f,
//    0x28d6d684,
//    0x7e2a2ad6,
//    0xd7832bd7,
//    0x81d7d72d,
//    0x83d7d72b,
//    0xd6822cd8,
//    0x82d8d62c,
//    0x7d292bd7,
//    0xd72b83d7,
//    0xd42a82d8,
//    0x7e2a2cd8,
//    0x81d5d72b,
//    0x297dd72b,
//    0xd52b2b7f,
//    0xd6822cd8,
//    0x28d6d684,
//    0xd52b81d7,
//    0x297dd72b,
//    0xd5812bd7,
//    0x2a7fd62c,
//    0xd52983d7,
//    0xd52b297f,
//    0x2ad67f2c,
//    0x29d57f2b,
//    0x29d7d785,
//    0xd6822ad6,
//    0xd62a2c7f,
//    0x297fd72d,
//    0x7d292bd7,
//    0xd72b2b7f,
//    0x82d6d62a,
//    0x2bd77f2b,
//    0x2b7fd72b,
//    0x297fd72d,
//    0xd42a82d8,
//    0x29d7d583,
//    0xd5812bd7,
//    0x29d7d583,
//    0x2ad67e2a,
//    0xd52b83d9,
//    0x82d6d82c,
//    0xd52b2b7f,
//    0x7e2c2ad8,
//    0x2a7ed82c,
//    0xd62c2a7f,
//    0x2b7fd72b,
//    0x81d7d52b,
//    0x81d5d72b,
//    0xd5812bd7,
//    0x81d7d52b,
//    0x82d6d82c,
//    0xd6822ad6,
//    0x7e2c2ad8,
//    0x2ad8d684,
//    0x2bd7d783,
//    0x7e2a2cd8,
//    0xd5812bd7,
//    0x81d7d72d,
//    0x82d8d62c,
//    0x297fd72d,
//    0x2ad8d684,
//    0xd62a84d8,
//    0x297fd72d,
//    0x287ed62c,
//    0xd52b83d9,
//    0x29d5d783,
//    0xd6822cd8,
//    0x28d6d684,
//    0x2ad67e2a,
//    0xd52b83d9,
//    0x7d2b29d7,
//    0x7e2a2ad6,
//    0xd52983d7,
//    0x29d77d2b,
//    0x2bd7d783,
//    0x2a7fd62c,
//    0x81d6d62c,
//    0x7e2a2ad6,
//    0x29d77f2d,
//    0xd58329d7,
//    0x29d77f2d,
//    0x82d8d62c,
//    0x28d6d684,
//    0xd5292b7f,
//    0xd52b297f,
//    0x2ad8d684,
//    0x82d8d62c,
//    0x29d77f2d,
//    0x297dd72b,
//    0xd5832bd9,
//    0xd58329d7,
//    0xd7832bd7,
//    0x2b7fd72b,
//    0xd62a82d6,
//    0x29d5d783,
//    0x287ed62c,
//    0x82d6d62a,
//    0x7f2b2bd7,
//    0xd42a82d8,
//    0x29d77d2b,
//    0x29d7d583,
//    0x287ed62c,
//    0x7e2c2ad8,
//    0x29d7d583,
//    0xd62a2c7f,
//    0x29d7d583,
//    0x7e2c2ad8,
//    0x2b7fd72b,
//    0xd62a2c7f,
//    0xd6842ad8,
//    0x2ad6d682,
//    0xd52983d7,
//    0x2b7fd72b,
//    0x297fd52b,
//    0x28d67e2c,
//    0x82d8d62c,
//    0x29d7d583,
//    0xd72b2b7f,
//    0x2bd7d783,
//    0x83d7d72b,
//    0x2ad87e2c,
//    0xd5832bd9,
//    0xd52b83d9,
//    0x28d6d684,
//    0xd4822ad8,
//    0xd52b297f,
//    0xd58329d7,
//    0xd72b2b7f,
//    0xd4822ad8,
//    0x2bd7d783,
//    0xd72b83d7,
//    0xd5812bd7,
//    0x28d67e2c,
//    0x81d5d72b,
//    0xd62a2c7f,
//    0x7e2c2ad8,
//    0xd6822cd8,
//    0x83d7d72b,
//    0x7d292bd7,
//    0xd62a84d8,
//    0xd62c82d8,
//    0x83d7d72b,
//    0x2ad6d884,
//    0x29d7d785,
//    0x82d6d62a,
//    0xd52983d7,
//    0xd42a82d8,
//    0x81d5d72b,
//    0xd5292b7f,
//    0x7e2c2ad8,
//    0xd6822ad6,
//    0x82d8d62c,
//    0x81d7d52b,
//    0x2b7fd72b,
//    0xd62a2a7e,
//    0xd5812bd7,
//    0xd62c2a7f,
//    0xd58329d7,
//    0x28d6d684,
//    0x29d77d2b,
//    0x29d57f2b,
//    0xd62a2c7f,
//    0xd62a84d8,
//    0xd4822ad8,
//    0x29d77d2b,
//    0x81d6d62c,
//    0x2bd7d783,
//    0xd62a2c7f,
//    0x28d6d684,
//    0x7d2b2bd9,
//    0xd5832bd9,
//    0x81d7d72d,
//    0x81d7d52b,
//    0x2a7ed62a,
//    0xd62a2c7f,
//    0xd6842ad8,
//    0xd62a2a7e,
//    0x2a7ed82c,
//    0x82d6d62a,
//    0x00000000,
//    0x00000000,
//    0x00000000,
//    0x00000000,
//    0x00000000,
//    0x00000000,
//    0x00000000,
//    0x00000000
//};

/* Space for the hard decisions, filled with all 1s*/
Uint32 hard_decision[] = {
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF
    ,
    
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF
//    ,
//    
//    0xFFFFFFFF,
//    0xFFFFFFFF
};

///* a long enough output receive buffer */
//Uint32 hard_decision[] = {
//    0x00000000,
//    0x00000000,
//    0x00000000,
//    0x00000000,
//    0x00000000,
//    0x00000000,
//    0x00000000,
//    0x00000000,
//    0x00000000,
//    0x00000000,
//    0x00000000,
//    0x00000000
//};



/* Pre-computed hard decisions */
//Uint32 hard_decisionRef[] = {
//    0x91E856D3,
//    0xFD90B270,
//    0xF656AF29,
//    0x130F47B1,
//    0x82711BF2,
//    0x9920E398,
//    0x46BC56FA,
//    0x59AF4849,
//    0x4165FC0E,
//    0xC8EAB83D,
//    0x20ED5230,
//    0x00000095
//};


Uint32 hard_decision0[] = {
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF
    ,
    
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF
//    ,
//    
//    0xFFFFFFFF,
//    0xFFFFFFFF
};


Uint32 hard_decision1[] = {
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF
    ,
    
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF
//    ,
//    
//    0xFFFFFFFF,
//    0xFFFFFFFF
};

Uint32 hard_decision2[] = {
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF
    ,
    
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF
//    ,
//    
//    0xFFFFFFFF,
//    0xFFFFFFFF
};

Uint32 hard_decision3[] = {
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF
    ,
    
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF
//    ,
//    
//    0xFFFFFFFF,
//    0xFFFFFFFF
};
