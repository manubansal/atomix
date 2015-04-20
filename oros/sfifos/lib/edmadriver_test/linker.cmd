/******************************************************************************
 * Copyright (c) 2010 Texas Instruments Incorporated - http://www.ti.com
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 ****************************************************************************
 *
 * Project     EVM
 * @file       linker.cmd
 * 
 * @brief      EVM default linker file
 *  
 * @ingroup    EVM_CMD
 *
 ****************************************************************************
 *                                                                                          
 * "$Revision: 1.10 $"
 * "$Date: 2011/04/08 17:23:20 $"
 *
 ***************************************************************************/

-c
-heap  0x40000
-stack 0x40000


/* Memory Map All Core */
/* RESBLOCK is used by dsp rom bootloader, can be used for application uninitialized section only */
MEMORY
{
        L1D:                 o = 0x00f00000   l = 00008000h 
        L1P:                 o = 0x00e00000   l = 00008000h 
        VECS:                o = 0x00800000   l = 00000200h
        L2:                  o = 0x0081DD24   l = 000E22DCh
        DDR:                 o = 0x80000000   l = 40000000h
        ISHARED:             o = 0x0C000000   l = 00100000h
        ISHAREDMCM:          o = 0x0C100000   l = 00100000h
        
                
        GEM0L2: origin = 0x10800200,  len = 0xc0000
        GEM1L2: origin = 0x11800200,  len = 0xc0000
        /*GEM2L2      : origin = 0x12800000,  len = 0xc0000*/
} 

SECTIONS {
		.G0L2		>		GEM0L2
    	.G1L2		>		GEM1L2
}

-l ti.csl.ae66e
-l ti.csl.intc.ae66e
-l edmadriver.lib

