/**
 *   @file  qmss_device.c
 *
 *   @brief   
 *      This file contains the device specific configuration and initialization routines
 *      for QMSS Low Level Driver.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2011, Texas Instruments, Inc.
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
 *  \par
*/

#include <stdint.h>

/* QMSS includes */
#include <ti/drv/qmss/qmss_qm.h>

/* CSL RL includes */
#include <ti/csl/cslr_device.h>
#include <ti/csl/cslr_qm_config.h>
#include <ti/csl/cslr_qm_descriptor_region_config.h>
#include <ti/csl/cslr_qm_queue_management.h>
#include <ti/csl/cslr_qm_queue_status_config.h>
#include <ti/csl/cslr_qm_intd.h>
#include <ti/csl/cslr_pdsp.h>
#include <ti/csl/csl_qm_queue.h>

/** @addtogroup QMSS_LLD_DATASTRUCT
@{ 
*/
/** @brief QMSS LLD initialization parameters */
Qmss_GlobalConfigParams qmssGblCfgParams[] =
{
    /** Maximum number of queue Managers */
    2u,
    /** Maximum number of queues */
    8192u,
    
    {
    /** Base queue number and Maximum supported low priority queues */
    {QMSS_LOW_PRIORITY_QUEUE_BASE, QMSS_MAX_LOW_PRIORITY_QUEUE},
    /** Base queue number and Maximum supported AIF queues */
    {QMSS_AIF_QUEUE_BASE, QMSS_MAX_AIF_QUEUE},
    /** Base queue number and Maximum supported PASS queues */
    {QMSS_PASS_QUEUE_BASE, QMSS_MAX_PASS_QUEUE},
    /** Base queue number and Maximum supported Intc Pend queues */
    {QMSS_INTC_QUEUE_BASE, QMSS_MAX_INTC_QUEUE},
    /** Base queue number and Maximum supported SRIO queues */
    {QMSS_SRIO_QUEUE_BASE, QMSS_MAX_SRIO_QUEUE},
    /** Base queue number and Maximum supported FFTC A queues */
    {QMSS_FFTC_A_QUEUE_BASE, QMSS_MAX_FFTC_A_QUEUE},
    /** Base queue number and Maximum supported FFTC B queues */
    {QMSS_FFTC_B_QUEUE_BASE, QMSS_MAX_FFTC_B_QUEUE},
    /** Base queue number and Maximum supported FFTC C queues */
    {QMSS_FFTC_C_QUEUE_BASE, QMSS_MAX_FFTC_C_QUEUE},
    /** Base queue number and Maximum supported BCP queues */
    {QMSS_BCP_QUEUE_BASE, QMSS_MAX_BCP_QUEUE},
    /** Base queue number and Maximum supported high priority queues */
    {QMSS_HIGH_PRIORITY_QUEUE_BASE, QMSS_MAX_HIGH_PRIORITY_QUEUE},
    /** Base queue number and Maximum supported starvation counter queues */
    {QMSS_STARVATION_COUNTER_QUEUE_BASE, QMSS_MAX_STARVATION_COUNTER_QUEUE},
    /** Base queue number and Maximum supported infrastructure queues */
    {QMSS_INFRASTRUCTURE_QUEUE_BASE, QMSS_MAX_INFRASTRUCTURE_QUEUE},
    /** Base queue number and Maximum supported traffic shaping queues */
    {QMSS_TRAFFIC_SHAPING_QUEUE_BASE, QMSS_MAX_TRAFFIC_SHAPING_QUEUE},
    /** Base queue number and Maximum supported general purpose queues */
    {QMSS_GENERAL_PURPOSE_QUEUE_BASE, QMSS_MAX_GENERAL_PURPOSE_QUEUE},

    /* Unused */
    {0u, 0u},
    {0u, 0u},
    {0u, 0u},
    {0u, 0u},
    {0u, 0u},
    {0u, 0u},
    {0u, 0u},
    {0u, 0u},
    {0u, 0u},
    {0u, 0u},
    {0u, 0u},
    },
    /** Base address for the CPDMA overlay registers */

    /** QM Global Config registers */
    (void *) CSL_QM_SS_CFG_CONFIG_STARVATION_COUNTER_REGS,
    /** QM Descriptor Config registers */
    (void *) CSL_QM_SS_CFG_DESCRIPTION_REGS, 
    /** QM queue Management registers */
    (void *) CSL_QM_SS_CFG_QM_QUEUE_DEQUEUE_REGS,
    /** QM queue Management Proxy registers */
    (void *) CSL_QM_SS_CFG_PROXY_QUEUE_DEQUEUE_REGS,
    /** QM queue status registers */
    (void *) CSL_QM_SS_CFG_QUE_PEEK_REGS,
    /** QM INTD registers */
    (void *) CSL_QM_SS_CFG_INTD_REGS,
    /** QM PDSP 1 command register */
    {
        (void *) CSL_QM_SS_CFG_SCRACH_RAM1_REGS,
        /** QM PDSP 2 command register */
        (void *) CSL_QM_SS_CFG_SCRACH_RAM2_REGS,
    },
    /** QM PDSP 1 control register */
    {
        (void *) CSL_QM_SS_CFG_ADSP1_REGS,
        /** QM PDSP 2 control register */
        (void *) CSL_QM_SS_CFG_ADSP2_REGS,
    },
    /** QM PDSP 1 IRAM register */
    {
        (void *) CSL_QM_SS_CFG_APDSP1_RAM_REGS,
        /** QM PDSP 2 IRAM register */
        (void *) CSL_QM_SS_CFG_APDSP2_RAM_REGS,
    },
    /** QM Status RAM */
    (void *) CSL_QM_SS_CFG_QM_STATUS_RAM_REGS,
    /** QM Linking RAM register */
    (void *) CSL_QM_SS_CFG_LINKING_RAM_REGS,
    /** QM McDMA register */
    (void *) CSL_QM_SS_CFG_MCDMA_REGS,
    /** QM Timer16 register */
    {
        (void *) CSL_QM_SS_CFG_TIMER1_REGS,
        (void *) CSL_QM_SS_CFG_TIMER2_REGS,
    },
    /** QM queue Management registers, accessed via DMA port */
    (void *) CSL_QM_SS_DATA_QM_QUEUE_DEQUEUE_REGS,
    /** QM queue Management Proxy registers, accessed via DMA port */
    (void *) CSL_QM_SS_DATA_PROXY_QUEUE_DEQUEUE_REGS,
    /** Resource Manager Handle should be NULL by default */
    NULL,
};

/**
@}
*/

