/*
 * edmaIntDispatcher.c
 *
 * This is a utility function used by edma as an interrupt dispatcher
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/ 
 * 
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
*/

/** ============================================================================
 *
 *   @file  edmaIntDispatcher.c
 *
 *   @path  
 *
 *   @desc This is a utility function used by edma as interrupt dispatcher 
 *
 * ============================================================================
 */

#include <stdio.h>
#include <ti/csl/csl_edma3.h>
#include <ti/csl/csl_chip.h>
#include <ti/csl/csl_tsc.h>
#include "edmaIntDispatcher.h"
#include "timestamp.h"

/* Global Edma Tcc handler table */
#pragma DATA_SECTION(TccHandlerTable,".far:TccHandlerTable");
EdmaTccHandler TccHandlerTable[64];

/*
 * =============================================================================
 *   @func  edmaIntDispatcher
 *  
 *   @arg
 *      handle        - Edma module handle
 *
 *   @desc
 *      This is the interrupt dispatcher routine for edma interrupts
 *
 *   @return
 *      NONE
 *
 * =============================================================================
 */
void edmaIntDispatcher (
    void        *handle
)
{
    CSL_Edma3Handle     hModule = (CSL_Edma3Handle)handle;
    CSL_Edma3CmdIntr    regionIntr;
    Uint32              tcc;
    Uint32              intr;
    Uint32              intrh;
	Int32				EdmaRegion;
    
    
    printf("EDMAIntDispatcher invoked\n");
	ts6 = CSL_tscRead();
    
    /* Read the IPR & mask with IER */
	EdmaRegion = EDMA_REGION;

    regionIntr.region = EdmaRegion;
    CSL_edma3GetHwStatus(hModule,CSL_EDMA3_QUERY_INTRPEND, &regionIntr);

	// read IER and keep only enabled interrupts from IPR
	if ( EdmaRegion == CSL_EDMA3_REGION_GLOBAL )
	{
//	    regionIntr.intr  &= hModule->regs->IER;
//	    regionIntr.intrh &= hModule->regs->IERH;
	    regionIntr.intr  &= hModule->regs->TPCC_IER;
	    regionIntr.intrh &= hModule->regs->TPCC_IERH;
	}
	else  // get the IER from the shadow region
	{
//		regionIntr.intr  &= hModule->regs->SHADOW[EdmaRegion].IER;
//	    regionIntr.intrh &= hModule->regs->SHADOW[EdmaRegion].IERH;
	    regionIntr.intr  &= hModule->regs->SHADOW[EdmaRegion].TPCC_IER;
	    regionIntr.intrh &= hModule->regs->SHADOW[EdmaRegion].TPCC_IERH;
	}
    
#if EDMADISPATCH_METHOD == EDMADISPATCH_REREAD_IPR
    while (regionIntr.intr || regionIntr.intrh)
    {
#endif
        intr  = regionIntr.intr;
        intrh = regionIntr.intrh;
        
		// clear any set & enabled IPR bits quickly to allow repetition
        CSL_edma3HwControl(hModule,CSL_EDMA3_CMD_INTRPEND_CLEAR,&regionIntr);       

        for ( tcc = 0; intr != 0; intr >>= 1, tcc++ )
            if ( intr & 1 )
                InvokeHandle( tcc );
        
        for ( tcc = 32; intrh != 0; intrh >>= 1, tcc++ )
            if ( intrh & 1 )
                InvokeHandle( tcc );
        
#if EDMADISPATCH_METHOD == EDMADISPATCH_REREAD_IPR
		// re-read IPR
        CSL_edma3GetHwStatus(hModule,CSL_EDMA3_QUERY_INTRPEND,&regionIntr);         

		// read IER and keep only enabled interrupts from IPR
		if ( EdmaRegion == CSL_EDMA3_REGION_GLOBAL )
		{
		    regionIntr.intr  &= hModule->regs->IER;
		    regionIntr.intrh &= hModule->regs->IERH;
		}
		else  // get the IER from the shadow region
		{
			regionIntr.intr  &= hModule->regs->SHADOW[EdmaRegion].IER;
		    regionIntr.intrh &= hModule->regs->SHADOW[EdmaRegion].IERH;
		}
    }           
#else  // EDMADISPATCH_IEVAL
		// write to the EVAL bit of the IEVAL register for this region
		if ( EdmaRegion == CSL_EDMA3_REGION_GLOBAL )
		{
//		    hModule->regs->IEVAL = 1;
		    hModule->regs->TPCC_IEVAL = 1;
		}
		else
		{
//			hModule->regs->SHADOW[EdmaRegion].IEVAL = 1;
			hModule->regs->SHADOW[EdmaRegion].TPCC_IEVAL = 1;
		}
#endif
	
	ts7 = CSL_tscRead();
}

/*
 * =============================================================================
 *   @func  edmaEventHook
 *  
 *   @arg
 *      tcc        - Tcc number
 *      fxn        - Pointer to function which points to edma isr
 *
 *   @desc
 *      This loads the hook table for the edma interrupt dispatcher
 *
 *   @return
 *      NONE
 *
 * =============================================================================
 */
void edmaEventHook (
    Uint16            tcc, 
    EdmaTccHandler    fxn
)
{
    TccHandlerTable[tcc] = (fxn);
}

