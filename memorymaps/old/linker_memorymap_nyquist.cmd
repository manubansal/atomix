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

/* #ifdef ALLCORE */

/* Memory Map All Core */
/* RESBLOCK is used by dsp rom bootloader, can be used for application uninitialized section only */
MEMORY
{
	/*
	NOTE: Regions defined here allow aliased memory allocation of objects.
	Regions defined per-core with global addresses in teh following parts 
	must not overlap the range of any region defined here if both of these
	regions are used independently for data allocation. For example, if the
	aliased L2_DATA region contains a section .data and a global-address 
	region C0_L2_DATA contains a section .gem0_data, if software uses both
	these named sections for object allocation, then L2_DATA and C0_L2_DATA
	must have disjoint memory address regions. If they overlap, then the linker
	could end up allocating two different objects to the same physical address,
	once through aliased address and once through the global address. That will
	lead to memory corruption during runtime. --MB, 01/21/13
	*/
	
	/* 
	Specific note: Since the major memory portions of OpenRadio data 
	are allocated through global address (fifo memory, mainly), we can
	reserve a smaller portion for aliased region and allocate the rest 
	for per-core global regions.
	*/

#ifdef L1P_SRAM_ZERO	
/* default mode, all cache */
        L1PCACHE:            o = 0x00e00000   l = 00008000h 
#elif defined(L1P_SRAM_16K)
        L1PSRAM:             o = 0x00e00000   l = 00004000h 
        L1PCACHE:            o = 0x00e04000   l = 00004000h 
#elif defined(L1P_SRAM_32K)
        L1PSRAM:             o = 0x00e00000   l = 00008000h 
#elif defined(L1P_DISABLE)
	/* define no sections for L1P */
#else			
/* default mode, all cache */
        L1PCACHE:            o = 0x00e00000   l = 00008000h 
#endif

#ifdef L1D_SRAM_ZERO	
/* default mode, all cache */
        L1DCACHE:            o = 0x00f00000   l = 00008000h 
#elif defined(L1D_SRAM_16K)
	/*Total size is 0x4000, but only 0x1000 is being allocated for aliased region,
	while 0x3000 is being allocated to global address regions for L1DSRAM. */
        L1DSRAM:             o = 0x00f00000   l = 00001000h 
        L1DCACHE:            o = 0x00f04000   l = 00004000h 
#elif defined(L1D_SRAM_32K)
	/*Total size is 0x8000, but only 0x1000 is being allocated for aliased region,
	while 0x7000 is being allocated to global address regions for L1DSRAM. */
        L1DSRAM:             o = 0x00f00000   l = 00001000h 
#elif defined(L1D_DISABLE)
	/* define no sections for L1D */
#else			
/* default mode, all cache */
        L1DCACHE:            o = 0x00f00000   l = 00008000h 
#endif


        VECS:                o = 0x00800000   l = 00000200h

        L2_TEXT:             o = 0x00800200   l = 0007fe00h
        /*L2_DATA:             o = 0x00880000   l = 0006cd00h*/
	/*Keeping 0x10000 for aliased region, rest of 0x5cd00 for global regions. */
        L2_DATA:             o = 0x00880000   l = 00010000h

        COMMOUT:             o = 0x008ECD00   l = 00000080h
        COMMIN:              o = 0x008ECD80   l = 00000080h 
        L2_SRIO_DATA_IN:     o = 0x008ECE00   l = 00003000h
        L2_SRIO_DATA_OUT:    o = 0x008EFE00   l = 00003000h 
        RESBLOCK:            o = 0x008F2E00   l = 0000D200h

        DDR:                 o = 0x80000000   l = 40000000h
	/*
        ISHARED:             o = 0x0C000000   l = 00100000h
        ISHAREDMCM:          o = 0x0C100000   l = 00100000h
	*/
        ISHARED_NONCACHEABLE:o = 0xE0000000   l = 00100000h	/* Made non-cacheable through corresponing software setup, otherwise this memory address range is not mapped to any physical memory. */
        ISHAREDMCM:          o = 0x0C100000   l = 00100000h

/* Memory Map Core 0 */
#ifdef L1D_SRAM_16K
        C0_L1DCACHE:            o = 0x10f04000   l = 00004000h 
        C0_L1DSRAM:             o = 0x10f01000   l = 00003000h 
#elif defined(L1D_SRAM_32K) 
        C0_L1DSRAM:             o = 0x10f01000   l = 00007000h 
#elif defined(L1D_DISABLE)
	/* define no sections for L1D */
#endif

        C0_VECS:                o = 0x10800000   l = 00000200h

        /*C0_L2_DATA:             o = 0x10880000   l = 0006cd00h*/
        C0_L2_DATA:             o = 0x10890000   l = 0005cd00h

        C0_COMMOUT:             o = 0x108ECD00   l = 00000080h
        C0_COMMIN:              o = 0x108ECD80   l = 00000080h 
        C0_L2_SRIO_DATA_IN:     o = 0x108ECE00   l = 00003000h
        C0_L2_SRIO_DATA_OUT:    o = 0x108EFE00   l = 00003000h 
        C0_RESBLOCK:            o = 0x108F2E00   l = 0000D200h

/* Memory Map Core 1 */
#ifdef L1D_SRAM_16K
        C1_L1DCACHE:            o = 0x11f04000   l = 00004000h 
        C1_L1DSRAM:             o = 0x11f01000   l = 00003000h 
#elif defined(L1D_SRAM_32K) 
        C1_L1DSRAM:             o = 0x11f01000   l = 00007000h 
#elif defined(L1D_DISABLE)
	/* define no sections for L1D */
#endif


        C1_VECS:                o = 0x11800000   l = 00000200h

        /*C1_L2_DATA:             o = 0x11880000   l = 0006cd00h*/
        C1_L2_DATA:             o = 0x11890000   l = 0005cd00h

        C1_COMMOUT:             o = 0x118ECD00   l = 00000080h
        C1_COMMIN:              o = 0x118ECD80   l = 00000080h 
        C1_L2_SRIO_DATA_IN:     o = 0x118ECE00   l = 00003000h
        C1_L2_SRIO_DATA_OUT:    o = 0x118EFE00   l = 00003000h 
        C1_RESBLOCK:            o = 0x118F2E00   l = 0000D200h

/* Memory Map Core 2 */
#ifdef L1D_SRAM_16K
        C2_L1DCACHE:            o = 0x12f04000   l = 00004000h 
        C2_L1DSRAM:             o = 0x12f01000   l = 00003000h 
#elif defined(L1D_SRAM_32K) 
        C2_L1DSRAM:             o = 0x12f01000   l = 00007000h 
#elif defined(L1D_DISABLE)
	/* define no sections for L1D */
#endif


        C2_VECS:                o = 0x12800000   l = 00000200h

        /*C2_L2_DATA:             o = 0x12880000   l = 0006cd00h*/
        C2_L2_DATA:             o = 0x12890000   l = 0005cd00h

        C2_COMMOUT:             o = 0x128ECD00   l = 00000080h
        C2_COMMIN:              o = 0x128ECD80   l = 00000080h 
        C2_L2_SRIO_DATA_IN:     o = 0x128ECE00   l = 00003000h
        C2_L2_SRIO_DATA_OUT:    o = 0x128EFE00   l = 00003000h 
        C2_RESBLOCK:            o = 0x128F2E00   l = 0000D200h

/* Memory Map Core 3 */
#ifdef L1D_SRAM_16K
        C3_L1DCACHE:            o = 0x13f04000   l = 00004000h 
        C3_L1DSRAM:             o = 0x13f01000   l = 00003000h 
#elif defined(L1D_SRAM_32K) 
        C3_L1DSRAM:             o = 0x13f01000   l = 00007000h 
#elif defined(L1D_DISABLE)
	/* define no sections for L1D */
#endif

        C3_VECS:                o = 0x13800000   l = 00000200h

        /*C3_L2_DATA:             o = 0x13880000   l = 0006cd00h*/
        C3_L2_DATA:             o = 0x13890000   l = 0005cd00h

        C3_COMMOUT:             o = 0x138ECD00   l = 00000080h
        C3_COMMIN:              o = 0x138ECD80   l = 00000080h 
        C3_L2_SRIO_DATA_IN:     o = 0x138ECE00   l = 00003000h
        C3_L2_SRIO_DATA_OUT:    o = 0x138EFE00   l = 00003000h 
        C3_RESBLOCK:            o = 0x138F2E00   l = 0000D200h
}

SECTIONS
{

   /*
   .bss         :>    L2_TEXT
   .neardata    :>    L2_TEXT
   */

#if defined(L1P_SRAM_16K) || defined(L1P_SRAM_32K)
   .textL1PSRAM	:>    L1PSRAM
#endif

#if defined(L1D_SRAM_16K) || defined(L1D_SRAM_32K)
   .dataL1DSRAM	:>    L1DSRAM
#endif


   .vectors     :>    VECS,  START(_start_vectors), SIZE(_size_vectors)   
   .memtest     :>    L2_TEXT


   /*.text        :>    L2*/
   /*.data        :>    L2*/
   .text        :>    L2_TEXT
   /*.text	:>    ISHARED*/

   .data        :>    L2_DATA

   .switch      :>    L2_TEXT
   /*.stack       :>    ISHARED, START(_start_stack), SIZE(_size_stack) */
   .stack       :>    L2_TEXT, START(_start_stack), SIZE(_size_stack)

   /*
   .bss         :>    L2_TEXT
   .neardata    :>    L2_TEXT
   */

   .cinit       :>    L2_TEXT
   .cio         :>    L2_TEXT
   .const       :>    L2_TEXT

   /*.sysmem      :>    ISHARED, START(_start_heap), SIZE(_size_heap)*/
   .sysmem      :>    ISHARED_NONCACHEABLE, START(_start_heap), SIZE(_size_heap)

   .tables      :>    L2_TEXT   
   var          :>    L2_TEXT
   init_code    :>    L2_TEXT
   init_data    :>    L2_TEXT
   chkcode      :>    L2_TEXT
   aluchk       :>    L2_TEXT
   basichk      :>    L2_TEXT
   multchk      :>    L2_TEXT
   bitchk       :>    L2_TEXT
   satchk       :>    L2_TEXT
   condchk      :>    L2_TEXT
   circhk       :>    L2_TEXT
   alu2chk      :>    L2_TEXT
   memchk       :>    L2_TEXT
   .asmBM       :>    L2_TEXT
   .asmRefDec   :>    L2_TEXT
   .commout		:>	  COMMOUT		
   .commin		:>	  COMMIN		
   .csl_vect    :>    L2_TEXT,  START(_start_cslvectors), SIZE(_size_cslvectors)
   "ResSec"     :>    RESBLOCK,  START(_start_ressec), SIZE(_size_ressec)
   .SRIOInDat    >    L2_SRIO_DATA_IN     
   .SRIOOutDat   >    L2_SRIO_DATA_OUT 
   .far         :>    L2_TEXT
   /*
   .cppi        :>    ISHARED
   .qmss        :>    ISHARED
   .ramtst      :>    ISHARED, START(_start_ramtst), SIZE(_size_ramtst)
   */
   .cppi        :>    ISHARED_NONCACHEABLE
   .qmss        :>    ISHARED_NONCACHEABLE
   .ramtst      :>    ISHARED_NONCACHEABLE, START(_start_ramtst), SIZE(_size_ramtst)

   .hypermcm    :>    ISHAREDMCM,  START(_start_hypermcm), SIZE(_size_hypermcm)

  .gem0_data: > C0_L2_DATA
  .gem1_data: > C1_L2_DATA
  .gem2_data: > C2_L2_DATA
  .gem3_data: > C3_L2_DATA

#if defined(L1D_SRAM_16K) || defined(L1D_SRAM_32K)
   .gem0_dataL1DSRAM	:>    C0_L1DSRAM
   .gem1_dataL1DSRAM	:>    C1_L1DSRAM
   .gem2_dataL1DSRAM	:>    C2_L1DSRAM
   .gem3_dataL1DSRAM	:>    C3_L1DSRAM
#endif

  /*.shared_mem   :> ISHARED	*/
  .shared_mem_noncacheable   :> ISHARED_NONCACHEABLE
  /* Any read-only shared data structures can be placed in the shared_mem
  section. This way, they will be allocated only once in the L2 memory and will be equally accessible 
  with L2 latency to all the cores.*/

  .common_flags :> C0_L2_DATA  /* Use to store flags that are used for inter-core status communication,
  single writer and multiple readers, such as initDone, procDone etc. By keeping them in the same memory
  section as opposed to per-core memory section (aliased, local), a single copy is created for those flags
  allowed IPC through them. */
}
