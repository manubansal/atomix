#!/bin/bash
##### Combined compiler and linker command file. ########

#--------------------------------------------------------------------
#Compiler command file. Do not use -z in this file.
#--------------------------------------------------------------------

#-I"${CG_TOOL_ROOT_WIN}/include" 
#-I"${CG_TOOL_ROOT_WIN}/lib"

#I don't like this format but I'll use it for now till I figure out a better way --MB
#-I"`cygpath -m ../inc`"


-mv6600 
-g 
"--"relaxed_ansi 
"--"diag_warning=225 
"--"gen_func_subsections=on 
"--"big_endian 
"--"abi=eabi

#-c
#-stack 0xA000
#-heap  0x3000

#--------------------------------------------------------------------

###### since we're building a static library and not an executable, we must not have a linker section.
#--------------------------------------------------------------------
#Linker command file. The first option in linker command file must be -z. DO NOT REMOVE.
#--------------------------------------------------------------------
-z

#List other options here.
#-I"${CG_TOOL_ROOT_WIN}/lib"
#-llibc.a
"--"reread_libs
"--"rom_model
#--------------------------------------------------------------------



#-lcsl_c6474e.lib
#-lti.csl.ae66e

#MEMORY
#{
#  BOOT_MEM: origin = 0x00800000 length = 0x00000800
#  GEM0_BOOT_MEM: origin = 0x10800000 length = 0x00000800
#  GEM1_BOOT_MEM: origin = 0x11800000 length = 0x00000800
#  GEM2_BOOT_MEM: origin = 0x12800000 length = 0x00000800
#
#/*  TEXT_MEM: origin = 0x00800800 length = 0x0007f800 */
#  GEM0_TEXT_MEM: origin = 0x10800800 length = 0x0007f800
#  GEM1_TEXT_MEM: origin = 0x11800800 length = 0x0007f800
#  GEM2_TEXT_MEM: origin = 0x12800800 length = 0x0001f800
#
#  DATA_MEM: origin = 0x00880000 length = 0x00040000
#  GEM0_DATA_MEM: origin = 0x10880000 length = 0x00040000
#  GEM1_DATA_MEM: origin = 0x11880000 length = 0x00040000
#  GEM2_DATA_MEM: origin = 0x12820000 length = 0x00020000
#
#  
#  L3ROM_MEM: origin = 0x3c000000 length = 0x00010000
#
#  I2C_TB_MEM:  origin = 0xff010000 length = 0x00010000
#  EMAC_TB_MEM: origin = 0xff030000 length = 0x00010000
#  DDR16_0_MEM: origin = 0x80000000 length = 0x10000000
#
#  AIF_DATA_MEM: origin = 0xa0000000 length = 0x10000000
#}
# 
#SECTIONS
#{
#  .boot: > BOOT_MEM
#  .gem0_boot: > GEM0_BOOT_MEM
#  .gem1_boot: > GEM1_BOOT_MEM
#  .gem2_boot: > GEM2_BOOT_MEM
#
#  .text: > GEM0_TEXT_MEM
#  .gem0_text: > GEM0_TEXT_MEM
#  .gem1_text: > GEM1_TEXT_MEM
#  .gem2_text: > GEM2_TEXT_MEM
#
#  .data: > DATA_MEM
#  .gem0_data: > GEM0_DATA_MEM
#  .gem1_data: > GEM1_DATA_MEM
#  .gem2_data: > GEM2_DATA_MEM
#
#  .l3rom: > L3ROM_MEM
#
#  .i2c_tb:  > I2C_TB_MEM
#  .emac_tb: > EMAC_TB_MEM
#  .ddrData: > DDR16_0_MEM
#  .aifData: > AIF_DATA_MEM
#
#  vectors:    >       GEM0_TEXT_MEM
#  .fasttext:  >       GEM0_TEXT_MEM
#  .cinit:     >       GEM0_TEXT_MEM
#  .stack:     >       GEM0_TEXT_MEM
#  .bss:       >       GEM0_TEXT_MEM
#  .const:     >       GEM0_TEXT_MEM
#  .cio        >       GEM0_TEXT_MEM
#  .far:       >       GEM0_TEXT_MEM
#  .switch:    >       GEM0_TEXT_MEM
#  .sysmem:    >       GEM0_TEXT_MEM
#
#  .asmBM  	  > GEM0_TEXT_MEM
#  .asmRefDec  > GEM0_TEXT_MEM
#  Int_Dat	  >	GEM0_TEXT_MEM
#  .csl_vect   > GEM0_TEXT_MEM	
#  .l2data     > GEM0_TEXT_MEM /* misc. data in L2 */
#  .ddr2		> DDR16_0_MEM
#}
###/*****************************************************************************/
##/*                       End of Linker command file                          */
##/*****************************************************************************/

