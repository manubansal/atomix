/*
 *  Copyright 2008 by Spectrum Digital Incorporated.
 *  All rights reserved.  Property of Spectrum Digital Incorporated.
 *
 *  Not for distribution.
 */

/*
 *  Linker command file
 *
 */


/*-l rts64pluse.lib			/* big-endian version */
/* -l evm6474bslBE.lib			/* big-endian version */
/*-l csl_c6474e.lib 			/* big-endian version */ 
/*-l csl_c64xplus_intce.lib 	/* big-endian version */
-l ti.csl.ae66e
-l ti.csl.intc.ae66e
/*-l dsp64pluse.lib			/* big-endian version */
/*-l IQmath_c64x+e.lib		/* big-endian version */ 
/*-l RadiOS_Common.lib*/

-stack          0x00002000      /* Stack Size */
-heap           0x00002000      /* Heap Size */

MEMORY
{
    VECS:       o = 0x10800000  l = 0x00000200
    BOOT:       o = 0x10800200  l = 0x00000200
    /* IRAM:       o = 0x10801400  l = 0x0003FA00 */
    IRAM:       o = 0x10801400  l = 0x000FFC00
    DDR2:       o = 0x80000000  l = 0x10000000
}

SECTIONS
{
    .bss        >   IRAM
    .cinit      >   IRAM
    .cio        >   IRAM
    .const      >   IRAM
    .data       >   IRAM
    .far        >   IRAM
    .stack      >   IRAM
    .switch     >   IRAM
    .sysmem     >   IRAM
    .text       >   IRAM
    .ddr2       >   DDR2
    
    .csl_vect	>	IRAM
}
