/**  
 *  ============================================================================
 *  @n   (C) Copyright 2009-2012, Texas Instruments, Inc.
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
#ifndef _CPSW_SINGLECORE_H_
#define _CPSW_SINGLECORE_H_

/* C Standard library Include */
#include <string.h>
#include <stdint.h>

/* Chip Level definitions include */
#include <ti/csl/csl_chip.h>

/* CSL EMAC include */
#include <ti/csl/csl_cpsw.h>
#include <ti/csl/csl_cpsgmii.h>
#include <ti/csl/csl_cpsgmiiAux.h>
#include <ti/csl/csl_mdio.h>
#include <ti/csl/csl_mdioAux.h>

/* BootCfg module include */
#include <ti/csl/csl_bootcfg.h>
#include <ti/csl/csl_bootcfgAux.h>

/* CPPI LLD include */
#include <ti/drv/cppi/cppi_drv.h>
#include <ti/drv/cppi/cppi_desc.h>

/* QMSS LLD include */
#include <ti/drv/qmss/qmss_drv.h>

/** Number of ports in the ethernet subsystem */
#define         NUM_PORTS                   3u

/** Number of MAC/GMII ports in the ethernet switch */
#define         NUM_MAC_PORTS               2u

/** Number of host descriptors used by the CPSW example program */
#define         NUM_HOST_DESC               32

/** Host descriptor size. 
 *
 *  Big enough to hold the mandatory fields of the 
 *  host descriptor and PA Control Data
 * 
 *  = 32 bytes for Host desc + PA Control data
 */
#define         SIZE_HOST_DESC              80 

/** Number of PA Tx queues available */
#define         NUM_PA_TX_QUEUES            9

/** Number of PA Rx channels available */
#define         NUM_PA_RX_CHANNELS          24

#define CACHE_LINESZ    128
#define SYS_ROUND_UP(x,y)   ((x) + ((y) -1))/(y)*(y)

/* Define LoopBack modes */  
#define CPSW_LOOPBACK_NONE           0
#define CPSW_LOOPBACK_INTERNAL       1
#define CPSW_LOOPBACK_EXTERNAL       2

extern int32_t cpswLpbkMode;
extern int32_t cpswEvm6678;
extern Cppi_FlowHnd gRxFlowHnd;

int32_t Cpsw_SwitchOpen (void);
int32_t Mdio_Open (void);
int32_t Sgmii_Open (void);
int32_t Init_Qmss (void);
int32_t Init_Cppi (void);
int32_t Init_Cpsw (uint8_t* host_mac, uint8_t* amc_mac, uint8_t* wire_mac);
int32_t Init_PASS (void);
int32_t Setup_Tx (void);
int32_t Setup_Rx (void);
int32_t Setup_PASS (uint8_t* mac);
uint32_t OSL_Convert_CoreLocal2GlobalAddr (uint32_t  addr);
int32_t SendPacket (void);
void CycleDelay (int32_t count);
int32_t VerifyPacket (Cppi_Desc* pCppiDesc);

#endif
