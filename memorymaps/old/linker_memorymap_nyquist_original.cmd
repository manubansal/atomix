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
-heap  0xE0000
-stack 0x10000

#ifdef ALLCORE

/* Memory Map All Core */
/* RESBLOCK is used by dsp rom bootloader, can be used for application uninitialized section only */
MEMORY
{
        L1D:                 o = 0x00f00000   l = 00008000h 
        L1P:                 o = 0x00e00000   l = 00008000h 
        VECS:                o = 0x00800000   l = 00000200h
        L2:                  o = 0x00800200   l = 000ECB00h
        COMMOUT:             o = 0x008ECD00   l = 00000080h
        COMMIN:              o = 0x008ECD80   l = 00000080h 
        L2_SRIO_DATA_IN:     o = 0x008ECE00   l = 00003000h
        L2_SRIO_DATA_OUT:    o = 0x008EFE00   l = 00003000h 
        RESBLOCK:            o = 0x008F2E00   l = 0000D200h
        DDR:                 o = 0x80000000   l = 40000000h
        ISHARED:             o = 0x0C000000   l = 00100000h
        ISHAREDMCM:          o = 0x0C100000   l = 00100000h
} 

#else

#ifdef LCORE1

/* Memory Map Core 1 */
/* RESBLOCK is used by dsp rom bootloader, can be used for application uninitialized section only */
MEMORY
{
        L1D:                 o = 0x11f00000   l = 00008000h 
        L1P:                 o = 0x11e00000   l = 00008000h 
        VECS:                o = 0x11800000   l = 00000200h
        L2:                  o = 0x11800200   l = 000ECB00h
        COMMOUT:             o = 0x118ECD00   l = 00000080h
        COMMIN:              o = 0x118ECD80   l = 00000080h 
        L2_SRIO_DATA_IN:     o = 0x118ECE00   l = 00003000h
        L2_SRIO_DATA_OUT:    o = 0x118EFE00   l = 00003000h 
        RESBLOCK:            o = 0x118F2E00   l = 0000D200h
        DDR:                 o = 0x80000000   l = 40000000h
        ISHARED:             o = 0x0C000000   l = 00100000h
        ISHAREDMCM:          o = 0x0C100000   l = 00100000h
} 

#else

#ifdef LCORE2

/* Memory Map Core 2 */
/* RESBLOCK is used by dsp rom bootloader, can be used for application uninitialized section only */
MEMORY
{
        L1D:                 o = 0x12f00000   l = 00008000h 
        L1P:                 o = 0x12e00000   l = 00008000h 
        VECS:                o = 0x12800000   l = 00000200h
        L2:                  o = 0x12800200   l = 000ECB00h
        COMMOUT:             o = 0x128ECD00   l = 00000080h
        COMMIN:              o = 0x128ECD80   l = 00000080h 
        L2_SRIO_DATA_IN:     o = 0x128ECE00   l = 00003000h
        L2_SRIO_DATA_OUT:    o = 0x128EFE00   l = 00003000h 
        RESBLOCK:            o = 0x128F2E00   l = 0000D200h
        DDR:                 o = 0x80000000   l = 40000000h
        ISHARED:             o = 0x0C000000   l = 00100000h
        ISHAREDMCM:          o = 0x0C100000   l = 00100000h
} 

#else

#ifdef LCORE3

/* Memory Map Core 3 */
/* RESBLOCK is used by dsp rom bootloader, can be used for application uninitialized section only */
MEMORY
{
        L1D:                 o = 0x13f00000   l = 00008000h 
        L1P:                 o = 0x13e00000   l = 00008000h 
        VECS:                o = 0x13800000   l = 00000200h
        L2:                  o = 0x13800200   l = 000ECB00h
        COMMOUT:             o = 0x138ECD00   l = 00000080h
        COMMIN:              o = 0x138ECD80   l = 00000080h 
        L2_SRIO_DATA_IN:     o = 0x138ECE00   l = 00003000h
        L2_SRIO_DATA_OUT:    o = 0x138EFE00   l = 00003000h 
        RESBLOCK:            o = 0x138F2E00   l = 0000D200h
        DDR:                 o = 0x80000000   l = 40000000h
        ISHARED:             o = 0x0C000000   l = 00100000h
        ISHAREDMCM:          o = 0x0C100000   l = 00100000h
} 

#else

/* LCORE0 define can also be used for core 0 */

/* Memory Map Core 0 default */
/* RESBLOCK is used by dsp rom bootloader, can be used for application uninitialized section only */
MEMORY
{
        L1D:                 o = 0x10f00000   l = 00008000h 
        L1P:                 o = 0x10e00000   l = 00008000h 
        VECS:                o = 0x10800000   l = 00000200h
        L2:                  o = 0x10800200   l = 000ECB00h
        COMMOUT:             o = 0x108ECD00   l = 00000080h
        COMMIN:              o = 0x108ECD80   l = 00000080h 
        L2_SRIO_DATA_IN:     o = 0x108ECE00   l = 00003000h
        L2_SRIO_DATA_OUT:    o = 0x108EFE00   l = 00003000h 
        RESBLOCK:            o = 0x108F2E00   l = 0000D200h
        DDR:                 o = 0x80000000   l = 40000000h
        ISHARED:             o = 0x0C000000   l = 00100000h
        ISHAREDMCM:          o = 0x0C100000   l = 00100000h
} 

#endif
#endif
#endif
#endif


SECTIONS
{
   .vectors     :>    VECS,  START(_start_vectors), SIZE(_size_vectors)   
   .memtest     :>    L2
   .text        :>    L2
   .data        :>    L2
   .switch      :>    L2
   .stack       :>    ISHARED, START(_start_stack), SIZE(_size_stack)
   .bss         :>    L2
   .cinit       :>    L2
   .cio         :>    L2
   .const       :>    L2
   .sysmem      :>    ISHARED, START(_start_heap), SIZE(_size_heap)
   .tables      :>    L2   
   var          :>    L2
   init_code    :>    L2
   init_data    :>    L2
   chkcode      :>    L2
   aluchk       :>    L2
   basichk      :>    L2
   multchk      :>    L2
   bitchk       :>    L2
   satchk       :>    L2
   condchk      :>    L2
   circhk       :>    L2
   alu2chk      :>    L2
   memchk       :>    L2
   .asmBM       :>    L2
   .asmRefDec   :>    L2
   .commout		:>	  COMMOUT		
   .commin		:>	  COMMIN		
   .csl_vect    :>    L2,  START(_start_cslvectors), SIZE(_size_cslvectors)
   "ResSec"     :>    RESBLOCK,  START(_start_ressec), SIZE(_size_ressec)
   .SRIOInDat    >    L2_SRIO_DATA_IN     
   .SRIOOutDat   >    L2_SRIO_DATA_OUT 
   .far         :>    L2
   .cppi        :>    ISHARED
   .qmss        :>    ISHARED
   .ramtst      :>    ISHARED, START(_start_ramtst), SIZE(_size_ramtst)
   .hypermcm    :>    ISHAREDMCM,  START(_start_hypermcm), SIZE(_size_hypermcm)

  .shared_mem: > MULTICORE_SHARED_MEM
  .common_flags: > GEM0_DATA_MEM /* Use to store flags that are used for inter-core status communication,
  single writer and multiple readers, such as initDone, procDone etc. By keeping them in the same memory
  section as opposed to per-core memory section (aliased, local), a single copy is created for those flags
  allowed IPC through them. */
}
