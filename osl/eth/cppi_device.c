/**
 *   @file  cppi_device.c
 *
 *   @brief   
 *      This file contains the device specific configuration and initialization routines
 *      for CPPI Low Level Driver.
 *
 *  Copyright (c) Texas Instruments Incorporated 2009
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
 */

#include <stdint.h>

/* CPPI includes */
#include <ti/drv/cppi/cppi_drv.h>

/* CSL RL includes */
#include <ti/csl/cslr_device.h>
#include <ti/csl/cslr_cppidma_global_config.h>
#include <ti/csl/cslr_cppidma_rx_channel_config.h>
#include <ti/csl/cslr_cppidma_rx_flow_config.h>
#include <ti/csl/cslr_cppidma_tx_channel_config.h>
#include <ti/csl/cslr_cppidma_tx_scheduler_config.h>
#include <ti/csl/csl_cppi.h>

/** @addtogroup CPPI_LLD_DATASTRUCT
@{ 
*/
/** @brief CPPI LLD initialization parameters */
Cppi_GlobalConfigParams cppiGblCfgParams[CPPI_MAX_CPDMA] =
{
    {
        /** CPDMA this configuration belongs to */
        Cppi_CpDma_SRIO_CPDMA,
        /** Maximum supported Rx Channels */
        16u,
        /** Maximum supported Tx Channels */
        16u,
        /** Maximum supported Rx Flows */
        20u,
        /** Priority for all Rx transactions of this CPDMA */
        0u,
        /** Priority for all Tx transactions of this CPDMA */
        0u,

        /** Base address for the CPDMA overlay registers */

        /** Global Config registers */
        (void *) CSL_SRIO_CONFIG_CPPI_DMA_GLOBAL_CFG_REGS,
        /** Tx Channel Config registers */
        (void *) CSL_SRIO_CONFIG_CPPI_DMA_TX_CFG_REGS,
        /** Rx Channel Config registers */
        (void *) CSL_SRIO_CONFIG_CPPI_DMA_RX_CFG_REGS,
        /** Tx Channel Scheduler registers */
        (void *) CSL_SRIO_CONFIG_CPPI_DMA_TX_SCHEDULER_CFG_REGS,
        /** Rx Flow Config registers */
        (void *) CSL_SRIO_CONFIG_CPPI_DMA_RX_FLOW_CFG_REGS,
    },
    {
        /** CPDMA this configuration belongs to */
        Cppi_CpDma_AIF_CPDMA,
        /** Maximum supported Rx Channels */
        129u,
        /** Maximum supported Tx Channels */
        129u,
        /** Maximum supported Rx Flows */
        129u,
        /** Priority for all Rx transactions of this CPDMA */
        0u,
        /** Priority for all Tx transactions of this CPDMA */
        0u,

        /** Base address for the CPDMA overlay registers */

        /** Global Config registers */
        (void *) CSL_AIF2_CFG_CPPI_DMA_GLOBAL_CFG_REGS,
        /** Tx Channel Config registers */
        (void *) CSL_AIF2_CFG_CPPI_DMA_TX_CFG_REGS,
        /** Rx Channel Config registers */
        (void *) CSL_AIF2_CFG_CPPI_DMA_RX_CFG_REGS,
        /** Tx Channel Scheduler registers */
        (void *) CSL_AIF2_CFG_CPPI_DMA_TX_SCHEDULER_CFG_REGS,
        /** Rx Flow Config registers */
        (void *) CSL_AIF2_CFG_CPPI_DMA_RX_FLOW_CFG_REGS,
    },
    {
        /** CPDMA this configuration belongs to */
        Cppi_CpDma_FFTC_A_CPDMA,
        /** Maximum supported Rx Channels */
        4u,
        /** Maximum supported Tx Channels */
        4u,
        /** Maximum supported Rx Flows */
        8u,
        /** Priority for all Rx transactions of this CPDMA */
        0u,
        /** Priority for all Tx transactions of this CPDMA */
        0u,

        /** Base address for the CPDMA overlay registers */

        /** Global Config registers */
        (void *) CSL_FFTC_A_CONFIG_CPPI_DMA_GLOBAL_CFG_REGS,
        /** Tx Channel Config registers */
        (void *) CSL_FFTC_A_CONFIG_CPPI_DMA_TX_CFG_REGS,
        /** Rx Channel Config registers */
        (void *) CSL_FFTC_A_CONFIG_CPPI_DMA_RX_CFG_REGS,
        /** Tx Channel Scheduler registers */
        (void *) CSL_FFTC_A_CONFIG_CPPI_DMA_TX_SCHEDULER_CFG_REGS,
        /** Rx Flow Config registers */
        (void *) CSL_FFTC_A_CONFIG_CPPI_DMA_RX_FLOW_CFG_REGS,
    },
    {
        /** CPDMA this configuration belongs to */
        Cppi_CpDma_FFTC_B_CPDMA,
        /** Maximum supported Rx Channels */
        4u,
        /** Maximum supported Tx Channels */
        4u,
        /** Maximum supported Rx Flows */
        8u,
        /** Priority for all Rx transactions of this CPDMA */
        0u,
        /** Priority for all Tx transactions of this CPDMA */
        0u,

        /** Base address for the CPDMA overlay registers */

        /** Global Config registers */
        (void *) CSL_FFTC_B_CONFIG_CPPI_DMA_GLOBAL_CFG_REGS,
        /** Tx Channel Config registers */
        (void *) CSL_FFTC_B_CONFIG_CPPI_DMA_TX_CFG_REGS,
        /** Rx Channel Config registers */
        (void *) CSL_FFTC_B_CONFIG_CPPI_DMA_RX_CFG_REGS,
        /** Tx Channel Scheduler registers */
        (void *) CSL_FFTC_B_CONFIG_CPPI_DMA_TX_SCHEDULER_CFG_REGS,
        /** Rx Flow Config registers */
        (void *) CSL_FFTC_B_CONFIG_CPPI_DMA_RX_FLOW_CFG_REGS,
    },
    {
        /** CPDMA this configuration belongs to */
        Cppi_CpDma_PASS_CPDMA,
        /** Maximum supported Rx Channels */
        24u,
        /** Maximum supported Tx Channels */
        9u,
        /** Maximum supported Rx Flows */
        32u,
        /** Priority for all Rx transactions of this CPDMA */
        0u,
        /** Priority for all Tx transactions of this CPDMA */
        0u,

        /** Base address for the CPDMA overlay registers */

        /** Global Config registers */
        (void *) CSL_PA_SS_CFG_CPPI_DMA_GLOBAL_CFG_REGS,
        /** Tx Channel Config registers */
        (void *) CSL_PA_SS_CFG_CPPI_DMA_TX_CFG_REGS,
        /** Rx Channel Config registers */
        (void *) CSL_PA_SS_CFG_CPPI_DMA_RX_CFG_REGS,
        /** Tx Channel Scheduler registers */
        (void *) CSL_PA_SS_CFG_CPPI_DMA_TX_SCHEDULER_CFG_REGS,
        /** Rx Flow Config registers */
        (void *) CSL_PA_SS_CFG_CPPI_DMA_RX_FLOW_CFG_REGS,
    },
    {
        /** CPDMA this configuration belongs to */
        Cppi_CpDma_QMSS_CPDMA,
        /** Maximum supported Rx Channels */
        32u,
        /** Maximum supported Tx Channels */
        32u,
        /** Maximum supported Rx Flows */
        64u,
        /** Priority for all Rx transactions of this CPDMA */
        0u,
        /** Priority for all Tx transactions of this CPDMA */
        0u,

        /** Base address for the CPDMA overlay registers */

        /** Global Config registers */
        (void *) CSL_QM_SS_CFG_CPPI_DMA_GLOBAL_CFG_REGS,
        /** Tx Channel Config registers */
        (void *) CSL_QM_SS_CFG_CPPI_DMA_TX_CFG_REGS,
        /** Rx Channel Config registers */
        (void *) CSL_QM_SS_CFG_CPPI_DMA_RX_CFG_REGS,
        /** Tx Channel Scheduler registers */
        (void *) CSL_QM_SS_CFG_CPPI_DMA_TX_SCHEDULER_CFG_REGS,
        /** Rx Flow Config registers */
        (void *) CSL_QM_SS_CFG_CPPI_DMA_RX_FLOW_CFG_REGS,
    },
    {
        /** CPDMA this configuration belongs to */
        Cppi_CpDma_FFTC_C_CPDMA,
        /** Maximum supported Rx Channels */
        (uint32_t) 4u,
        /** Maximum supported Tx Channels */
        (uint32_t) 4u,
        /** Maximum supported Rx Flows */
        (uint32_t) 8u,
        /** Priority for all Rx transactions of this CPDMA */
        (uint8_t) 0u,
        /** Priority for all Tx transactions of this CPDMA */
        (uint8_t) 0u,

        /** Base address for the CPDMA overlay registers */

        /** Global Config registers */
        (void *) CSL_FFTC_C_CONFIG_CPPI_DMA_GLOBAL_CFG_REGS,
        /** Tx Channel Config registers */
        (void *) CSL_FFTC_C_CONFIG_CPPI_DMA_TX_CFG_REGS,
        /** Rx Channel Config registers */
        (void *) CSL_FFTC_C_CONFIG_CPPI_DMA_RX_CFG_REGS,
        /** Tx Channel Scheduler registers */
        (void *) CSL_FFTC_C_CONFIG_CPPI_DMA_TX_SCHEDULER_CFG_REGS,
        /** Rx Flow Config registers */
        (void *) CSL_FFTC_C_CONFIG_CPPI_DMA_RX_FLOW_CFG_REGS,
    },    
    {
        /** CPDMA this configuration belongs to */
        Cppi_CpDma_BCP_CPDMA,
        /** Maximum supported Rx Channels */
        (uint32_t) 8u,
        /** Maximum supported Tx Channels */
        (uint32_t) 8u,
        /** Maximum supported Rx Flows */
        (uint32_t) 64u,
        /** Priority for all Rx transactions of this CPDMA */
        (uint8_t) 0u,
        /** Priority for all Tx transactions of this CPDMA */
        (uint8_t) 0u,

        /** Base address for the CPDMA overlay registers */

        /** Global Config registers */
        (void *) CSL_BCP_CONFIG_CPPI_DMA_GLOBAL_CFG_REGS,
        /** Tx Channel Config registers */
        (void *) CSL_BCP_CONFIG_CPPI_DMA_TX_CFG_REGS,
        /** Rx Channel Config registers */
        (void *) CSL_BCP_CONFIG_CPPI_DMA_RX_CFG_REGS,
        /** Tx Channel Scheduler registers */
        (void *) CSL_BCP_CONFIG_CPPI_DMA_TX_SCHEDULER_CFG_REGS,
        /** Rx Flow Config registers */
        (void *) CSL_BCP_CONFIG_CPPI_DMA_RX_FLOW_CFG_REGS,
    },    
};

/**
@}
*/

