/**
Atomix project, cppi_qmss_mgmt.h, TODO: insert summary here
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
*/
#ifndef _CPPI_QMSS_MGMT_H_
#define _CPPI_QMSS_MGMT_H_

/* High Priority QM Rx Interrupt Threshold */
#define		RX_INT_THRESHOLD			1u

/* Accumulator channel to use */
#define		PA_ACC_CHANNEL_NUM			0u

/* QMSS device specific configuration */
extern Qmss_GlobalConfigParams  qmssGblCfgParams;
/* CPPI device specific configuration */
extern Cppi_GlobalConfigParams  cppiGblCfgParams;

extern Qmss_QueueHnd gPaTxQHnd[], gTxFreeQHnd, gRxFreeQHnd, gRxQHnd;
extern uint32_t gHiPriAccumList[];

/* Queue handler type for FDQ */
typedef enum {
    QHANDLER_QPOP_FDQ_NO_ATTACHEDBUF,
    QHANDLER_QPOP_FDQ_ATTACHEDBUF
} QHANDLER_TYPE;

#endif 
