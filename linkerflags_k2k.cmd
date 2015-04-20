###### Combined compiler and linker command file. ########
#
##--------------------------------------------------------------------
##Compiler command file. Do not use -z in this file.
##--------------------------------------------------------------------
#
##-I"${CG_TOOL_ROOT_WIN}/include" 
##-I"${CG_TOOL_ROOT_WIN}/lib"
#
##/*I don't like this format but I'll use it for now till I figure out a better way --MB */
##-I"`cygpath -m ../inc`"
#
#
#-mv6600 
#-g 
#"--"relaxed_ansi 
#"--"diag_warning=225 
#"--"gen_func_subsections=on 
#"--"big_endian 
#"--"abi=eabi
##--------------------------------------------------------------------


#--------------------------------------------------------------------
#Linker command file. The first option in linker command file must be -z. DO NOT REMOVE.
#--------------------------------------------------------------------
-z

#List other options here.
#-I"${CG_TOOL_ROOT_WIN}/lib"
#do not list any libc here - cl6x picks the right one itself
#-llibc.a
"--"reread_libs
"--"rom_model
#"--"heap_size=8000
"--"heap_size=0xF000
-lti.csl.ae66e
-llib.dsplib.a
-lti.drv.qmss.ae66e
-lti.drv.pa.ae66e
-lti.drv.cppi.ae66e
-l ti.platform.evmk2h.ae66e

#"--"zero_init=off

#-I"${CG_TOOL_ROOT_WIN}/lib" 
#-I"${CSL_ROOT}/ti/csl/lib" 
#-I"${DSPLIB_ROOT}/lib" 
#-I"${IQMATH_ROOT}/include" 
#-I"${UNITY_ROOT}/src"		
#--------------------------------------------------------------------



#--define=ENABLE_L1SRAM
--define=ENABLE_L1SRAM_32K


