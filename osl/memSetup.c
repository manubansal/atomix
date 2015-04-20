/**
Atomix project, memSetup.c, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/

#include <ti/csl/csl_cacheAux.h>
#include <osl/inc/swpform.h>

//source: http://e2e.ti.com/support/dsp/c6000_multi-core_dsps/f/639/p/187759/674796.aspx#674796

struct pax_t{
  unsigned long low;
  unsigned long high;
};

#define PAX ((volatile struct pax_t*)0x08000000)

//example parameters: priority = 3, virtual = 0xE000 0000, physical = 0x0c00 0000, size_kbytes = 1024d = 1MB = 0100 0000 0000b = 0x400h  (note:
//the corresponding size in bytes (to be used in the linker memory map file) will be 0x400 << 10 = 0x0010 0000h)

//MPAX remap function allows remapping a physical memory region to a virtual memory region on which the MAR 
//can be set to turn off cacheability. That is the primary use of this function.
//Note: (unverified): Each core has its own MPAX and MAR registers, which means, to get this uncacheability effect,
//this function must be invoked with the same parameters from all cores at startup.

//TO DOCUMENT: Failure conditions (ex. request to make a region uncacheable that cannot be remapped).

void MEMSETUP_remapAndTurnCachingOff (
	unsigned int priority, 
	unsigned long virtual, 
	unsigned long physical, 
	unsigned long size_kbytes
	){
  const unsigned int perm=0xF6; //cannot execute  (0xFF=all permission)
  unsigned long size=size_kbytes>>2;
  unsigned int k=0x0A;
  for(; size; size=size>>1)
    ++k;
  unsigned long pax_h=virtual|k;
  unsigned long pax_l=0;
  if (physical>=0x80000000) {
    pax_l|=0x80000000;
    physical=physical & 0x0FFFFFFF;
  }
  pax_l=pax_l|(((physical>>12)<<8)|perm);

  PAX[priority].high=pax_h;
  PAX[priority].low=pax_l;

  DEBUG_INIT(
  printf("pax high: %08X\n", pax_h);
  printf("pax low : %08X\n", pax_l);
  )

  //CACHE_setMemRegionInfo(MAPPED_VIRTUAL_ADDRESS >> 24, 0, 0);
  //Example: 0xE000 0000 >> 24 = 0xE0 = 224, which is the right MAR number for the 
  //memory address region E000 0000 to E0FF FFFF (2^24B = 16MB address region starting
  //at 0xE000 0000.
  CACHE_setMemRegionInfo(virtual >> 24, 0, 0);

  //As per sprugw0a, para.7.3.1, logical RADDR (0xF0000000) bit31:8 must be written in MPAX_L bit 31:8, while the physical BADDR (0x0C000000) correspont to 0:0x0C000000 (the first zero is to identify the MCSM in the core internal 36bits addressing)
}


//To configure MPAX register (continue)
//The protection bits are 00110110 (two reserved bits, Supervisor read, write, execute, user read, write, execute)
//Segment 3 registers are at addresses 0x0800 0018 (low register) and 0x0800 001c (high register) 
//Segment 3 has the following values:
//Size = 1M = 10011b = 0x13  - 5 LSB of low register
//7 bits reserved, written as zeros 0000000b
//Logical base address 0x00E00  (12 bits, with the 20 zero bits from the size the logical base address is 0xE0000000)
//So the low register at address 0x08000018 is  
//0000 0000 1110 0000 0000 0000 0001 0011  
//Physical (replacement) base address 0x000c0 (16 bits, with the 20 bits from the size the physical base address is 0x0c000000)
//So the high register at address 0x0800001C is
//0000 0000 0000 1110 0000 0011 0110 
//
//Uint32 * regAddrH = 0x08000018;
//Uint32 * regAddrL = 0x0800001C;
//
//0x08000018
//
//0000 0000 1110 0000 0000 0000 0001 0011  
//0x00E00013 (0x00E00 is the logical base address, then 7 reserved 0 bits, and 0x13's five lower bits as the size bits equalling 1MB)
//
//BADDR Base Address Upper bits of address range to match in C66x CorePac’s native 32-bit address space
//(original)
//
//Since the size is 1MB, upper 12 bits of the 32-bit logical address will be matched to the upper 12 bits of BADDR to determine if it is in the segment.
//
//Upper 20 bits
//Address base: 0x0c000000: upper 20 bits of this: 0x0c000
//
//
//
//
//0x0800001C
//
////0000 0000 0000 1110 0000 0011 0110 
//0000 0000 0000 1110 0000 0011 1111 
//0x000EO3F
//
//0x000c0?3F
//
//4-bit extension of address space (32-bits ==> 36-bits)
//
////http://e2e.ti.com/cfs-file.ashx/__key/CommunityServer-Discussions-Components-Files/639/1541.XMC-_1320_-external-memory-Controller.ppt
////http://www.ti.com/lit/ug/sprugw0b/sprugw0b.pdf sec 7.3
