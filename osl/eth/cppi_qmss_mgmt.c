/**  
 * @file cppi_qmss_mgmt.c
 *
 * @brief 
 *  This file holds all the APIs required to configure CPPI/QMSS LLDs and 
 *  to send/receive data using PA/QM.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2009, Texas Instruments, Inc.
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

#include <stdio.h>
#include <stdint.h>

#include <ti/drv/qmss/qmss_firmware.h>
/* PA LLD include */
#include <ti/drv/pa/pa.h>
#include <ti/drv/pa/pasahost.h>

#include "cpsw_singlecore.h"
#include "cppi_qmss_mgmt.h"

#if ( !defined( _LITTLE_ENDIAN ) && !defined( _BIG_ENDIAN ) ) \
||  ( defined(_LITTLE_ENDIAN ) && defined( _BIG_ENDIAN ) )
#error either _LITTLE_ENDIAN or _BIG_ENDIAN must be defined
#endif

/* Host Descriptor Region - [Size of descriptor * Number of descriptors] 
 *
 * MUST be 16 byte aligned.
 */
#pragma DATA_ALIGN (gHostDesc, 16)
uint8_t                                   gHostDesc[SIZE_HOST_DESC * NUM_HOST_DESC];

/* High Priority Accumulator List - [((Interrupt Threshold + 1) * 2)] 
 *
 * MUST be 16 byte aligned.
 *
 * The High priority accumulator list consists of 2 buffers Ping and
 * Pong each consisting of the following entries:
 *
 * (1)  Entry count -   specifies number of packets accumulated in
 *                      the list.
 * (2)  Descriptors -   an array of Rx packet descriptors accumulated
 *                      in this list.
 *
 * Hence the size of high priority accumulator list is calculated as 
 * follows:
 *
 * (1)  Get the interrupt threshold, i.e., maximum number of Rx
 *      packets to accumulate before an interrupt is generated.
 * (2)  Add an extra entry to the threshold to track 
 *      entry count of the list.
 * (3)  Double this to accomodate space for Ping/Pong lists.
 * (4)  Each accumulator entry is 4 bytes wide.
 *  
 * size =   ((interrupt threshold + 1) * 2) * 4 bytes
 *
 * Lets allocate here assuming that interrupt threshold is 1, i.e.,
 * interrupt on every Rxed packet.
 */
#pragma DATA_ALIGN (gHiPriAccumList, 16)
uint32_t              gHiPriAccumList[(RX_INT_THRESHOLD + 1) * 2];

/* CPPI/QMSS Handles used by the application */
Qmss_QueueHnd         gGlobalFreeQHnd, gPaTxQHnd [NUM_PA_TX_QUEUES], gTxFreeQHnd, gRxFreeQHnd, gRxQHnd;
Cppi_Handle           gCpdmaHnd;
Cppi_ChHnd            gCpdmaTxChanHnd [NUM_PA_TX_QUEUES], gCpdmaRxChanHnd [NUM_PA_RX_CHANNELS];  
Cppi_FlowHnd          gRxFlowHnd;

/* Number of Tx Free descriptors to allocate */
#define     NUM_TX_DESC                 NUM_HOST_DESC/2

/* Number of Rx Free descriptors to allocate */
#define     NUM_RX_DESC                 NUM_HOST_DESC/2

/* Tx/Rx packet counters */
volatile uint32_t			gTxCounter = 0, gRxCounter = 0;

#pragma DATA_ALIGN (gRxBuffs, 4);
#pragma DATA_SECTION (gRxBuffs, ".pkts");
uint8_t               gRxBuffs[NUM_RX_DESC][1518];

/** ============================================================================
 *   @n@b OSL_Convert_CoreLocal2GlobalAddr
 *
 *   @b Description
 *   @n This API converts a core local L2 address to a global L2 address.
 *
 *   @param[in]  
 *   @n addr            L2 address to be converted to global.
 * 
 *   @return    uint32_t
 *   @n >0              Global L2 address
 * =============================================================================
 */
uint32_t OSL_Convert_CoreLocal2GlobalAddr (uint32_t  addr)
{
	uint32_t coreNum;

    /* Get the core number. */
    coreNum = CSL_chipReadReg(CSL_CHIP_DNUM); 

    /* Compute the global address. */
    return ((1 << 28) | (coreNum << 24) | (addr & 0x00ffffff));
}    

void queueReset()
{
	uint16_t       count, i;

	/* Clear INTD
	 * PA accumulator channel 0 applies for this case
	 * */
	Qmss_ackInterrupt(0, 1);
	Qmss_setEoiVector(Qmss_IntdInterruptType_HIGH, 0);
	int handleNum, handleEnd = 8192;
	for (handleNum = 640; handleNum<handleEnd; handleNum++)
	{
		count = Qmss_getQueueEntryCount(handleNum);
		for (i=0; i < count; i++)
		{
			Qmss_queuePop (handleNum);
		}
	}
}


/** ============================================================================
 *   @n@b Init_Qmss
 *
 *   @b Description
 *   @n This API initializes the QMSS LLD.
 *
 *   @param[in]  
 *   @n None
 * 
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t Init_Qmss (void)
{
    int32_t                       result;
    Qmss_MemRegInfo             memCfg;
    Qmss_InitCfg                qmssInitConfig;
    Cppi_DescCfg                cppiDescCfg;
    uint32_t                      numAllocated;

    /* Initialize QMSS */
    memset (&qmssInitConfig, 0, sizeof (Qmss_InitCfg));

    /* Set up QMSS configuration */

    /* Use internal linking RAM */
    qmssInitConfig.linkingRAM0Base  =   0;   
    qmssInitConfig.linkingRAM0Size  =   0;
    qmssInitConfig.linkingRAM1Base  =   0x0;
    qmssInitConfig.maxDescNum       =   NUM_HOST_DESC;
    
    qmssInitConfig.pdspFirmware[0].pdspId = Qmss_PdspId_PDSP1;
#ifdef _LITTLE_ENDIAN    
    qmssInitConfig.pdspFirmware[0].firmware = (void *) &acc48_le;
    qmssInitConfig.pdspFirmware[0].size = sizeof (acc48_le);
#else
    qmssInitConfig.pdspFirmware[0].firmware = (void *) &acc48_be;
    qmssInitConfig.pdspFirmware[0].size = sizeof (acc48_be);
#endif    

    /* Initialize the Queue Manager */
    result = Qmss_init (&qmssInitConfig, &qmssGblCfgParams);
    if (result != QMSS_SOK)
    {
        printf ("Error initializing Queue Manager SubSystem, Error code : %d\n", result);
        return -1;
    }

    /* Start Queue manager on this core */
    Qmss_start ();

    /* Remove any entries left in the queues */
    queueReset();

    /* Setup the descriptor memory regions. 
     *
     * The Descriptor base addresses MUST be global addresses and
     * all memory regions MUST be setup in ascending order of the
     * descriptor base addresses.
     */

    /* Initialize and setup CPSW Host Descriptors required for example */
    memset (gHostDesc, 0, SIZE_HOST_DESC * NUM_HOST_DESC);
    memCfg.descBase             =   (uint32_t *) OSL_Convert_CoreLocal2GlobalAddr ((uint32_t) gHostDesc);
    memCfg.descSize             =   SIZE_HOST_DESC;
    memCfg.descNum              =   NUM_HOST_DESC;
    memCfg.manageDescFlag       =   Qmss_ManageDesc_MANAGE_DESCRIPTOR;
    memCfg.memRegion            =   Qmss_MemRegion_MEMORY_REGION0;
    memCfg.startIndex           =   0;

    /* Insert Host Descriptor memory region */
    result = Qmss_insertMemoryRegion(&memCfg);
    if (result == QMSS_MEMREGION_ALREADY_INITIALIZED)
    {
        printf ("Memory Region %d already Initialized \n", memCfg.memRegion);
    }
    else if (result < QMSS_SOK)
    {
        printf ("Error: Inserting memory region %d, Error code : %d\n", memCfg.memRegion, result);
        return -1;
    }    

    /* Initialize all the descriptors we just allocated on the
     * memory region above. Setup the descriptors with some well
     * known values before we use them for data transfers.
     */
    memset (&cppiDescCfg, 0, sizeof (cppiDescCfg));
    cppiDescCfg.memRegion       =   Qmss_MemRegion_MEMORY_REGION0;
    cppiDescCfg.descNum         =   NUM_HOST_DESC;
    cppiDescCfg.destQueueNum    =   QMSS_PARAM_NOT_SPECIFIED;     
    cppiDescCfg.queueType       =   Qmss_QueueType_GENERAL_PURPOSE_QUEUE;
    cppiDescCfg.initDesc        =   Cppi_InitDesc_INIT_DESCRIPTOR;
    cppiDescCfg.descType        =   Cppi_DescType_HOST;
    
    /* By default:
     *      (1) Return descriptors to tail of queue 
     *      (2) Always return entire packet to this free queue
     *      (3) Set that PS Data is always present in start of SOP buffer
     *      (4) Configure free q num < 4K, hence qMgr = 0
     *      (5) Recycle back to the same Free queue by default.
     */
    cppiDescCfg.returnPushPolicy            =   Qmss_Location_TAIL;    
    cppiDescCfg.cfg.host.returnPolicy       =   Cppi_ReturnPolicy_RETURN_ENTIRE_PACKET;    
    cppiDescCfg.cfg.host.psLocation         =   Cppi_PSLoc_PS_IN_DESC;         
    cppiDescCfg.returnQueue.qMgr            =   0;    
    cppiDescCfg.returnQueue.qNum            =   QMSS_PARAM_NOT_SPECIFIED; 
    cppiDescCfg.epibPresent                 =   Cppi_EPIB_EPIB_PRESENT;
    
    /* Initialize the descriptors, create a free queue and push descriptors to a global free queue */
    if ((gGlobalFreeQHnd = Cppi_initDescriptor (&cppiDescCfg, &numAllocated)) <= 0)
    {
        printf ("Error Initializing Free Descriptors, Error: %d \n", gGlobalFreeQHnd);
        return -1;
    }        
   
    /* Queue Manager Initialization Done */
    return 0;
}

/** ============================================================================
 *   @n@b Init_Cppi
 *
 *   @b Description
 *   @n This API initializes the CPPI LLD, opens the PASS CPDMA and opens up
 *      the Tx, Rx channels required for data transfers.
 *
 *   @param[in]  
 *   @n None
 * 
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t Init_Cppi (void)
{
    int32_t                       result, i;        
    Cppi_CpDmaInitCfg           cpdmaCfg;
    uint8_t                       isAllocated;        
    Cppi_TxChInitCfg            txChCfg;
    Cppi_RxChInitCfg            rxChInitCfg;

    /* Initialize CPPI LLD */
    result = Cppi_init (&cppiGblCfgParams);
    if (result != CPPI_SOK)
    {
        printf ("Error initializing CPPI LLD, Error code : %d\n", result);
        return -1;
    }

    /* Initialize PASS CPDMA */
    memset (&cpdmaCfg, 0, sizeof (Cppi_CpDmaInitCfg));
    cpdmaCfg.dmaNum     = Cppi_CpDma_PASS_CPDMA;
    if ((gCpdmaHnd = Cppi_open (&cpdmaCfg)) == NULL)
    {
        printf ("Error initializing CPPI for PASS CPDMA %d \n", cpdmaCfg.dmaNum);
        return -1;
    }    

    /* Open all CPPI Tx Channels. These will be used to send data to PASS/CPSW */             
    for (i = 0; i < NUM_PA_TX_QUEUES; i ++)
    {
        txChCfg.channelNum      =   i;       /* CPPI channels are mapped one-one to the PA Tx queues */
        txChCfg.txEnable        =   Cppi_ChState_CHANNEL_DISABLE;  /* Disable the channel for now. */
        txChCfg.filterEPIB      =   0;
        txChCfg.filterPS        =   0;
        txChCfg.aifMonoMode     =   0;
        txChCfg.priority        =   2;
        if ((gCpdmaTxChanHnd[i] = Cppi_txChannelOpen (gCpdmaHnd, &txChCfg, &isAllocated)) == NULL)
        {
            printf ("Error opening Tx channel %d\n", txChCfg.channelNum);
            return -1;
        }

        Cppi_channelEnable (gCpdmaTxChanHnd[i]);
    }

    /* Open all CPPI Rx channels. These will be used by PA to stream data out. */
    for (i = 0; i < NUM_PA_RX_CHANNELS; i++)
    {
        /* Open a CPPI Rx channel that will be used by PA to stream data out. */
        rxChInitCfg.channelNum  =   i; 
        rxChInitCfg.rxEnable    =   Cppi_ChState_CHANNEL_DISABLE; 
        if ((gCpdmaRxChanHnd[i] = Cppi_rxChannelOpen (gCpdmaHnd, &rxChInitCfg, &isAllocated)) == NULL)
        {
            printf ("Error opening Rx channel: %d \n", rxChInitCfg.channelNum);
            return -1;
        }

        /* Also enable Rx Channel */
        Cppi_channelEnable (gCpdmaRxChanHnd[i]);    
    }
    
    /* Clear CPPI Loobpack bit in PASS CDMA Global Emulation Control Register */
    Cppi_setCpdmaLoopback(gCpdmaHnd, 0);   

    /* CPPI Init Done. Return success */
    return 0;
}    

/** ============================================================================
 *   @n@b Setup_Tx
 *
 *   @b Description
 *   @n This API sets up all relevant data structures and configuration required
 *      for sending data to PASS/Ethernet. It sets up a Tx free descriptor queue,
 *      PASS Tx queues required for send.
 *
 *   @param[in]  
 *   @n None
 * 
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t Setup_Tx (void)
{
    uint8_t                       isAllocated, i;        
    Qmss_Queue                  qInfo;
    Ptr                   		pCppiDesc;

    /* Open all Transmit (Tx) queues. 
     *
     * These queues are used to send data to PA PDSP/CPSW.
     */
    for (i = 0; i < NUM_PA_TX_QUEUES; i ++)
    {
            
        if ((gPaTxQHnd[i] = Qmss_queueOpen (Qmss_QueueType_PASS_QUEUE, QMSS_PARAM_NOT_SPECIFIED, &isAllocated)) < 0)
        {
            printf ("Error opening PA Tx queue \n");
            return -1;
        }            
    }

    /* Open a Tx Free Descriptor Queue (Tx FDQ). 
     *
     * This queue will be used to hold Tx free decriptors that can be filled
     * later with data buffers for transmission onto wire.
     */
    if ((gTxFreeQHnd = Qmss_queueOpen (Qmss_QueueType_STARVATION_COUNTER_QUEUE, QMSS_PARAM_NOT_SPECIFIED, &isAllocated)) < 0)
    {
        printf ("Error opening Tx Free descriptor queue \n");
        return -1;
    }            

    qInfo = Qmss_getQueueNumber (gTxFreeQHnd);

    /* Attach some free descriptors to the Tx free queue we just opened. */
    for (i = 0; i < NUM_TX_DESC; i++)
    {
        /* Get a free descriptor from the global free queue we setup 
         * during initialization.
         */
        if ((pCppiDesc = Qmss_queuePop (gGlobalFreeQHnd)) == NULL)
        {
            break;                
        }

        /* The descriptor address returned from the hardware has the 
         * descriptor size appended to the address in the last 4 bits.
         *
         * To get the true descriptor size, always mask off the last 
         * 4 bits of the address.
         */
        pCppiDesc = (Ptr) ((uint32_t) pCppiDesc & 0xFFFFFFF0);

        /* Setup the Completion queue:
         *
         * Setup the return policy for this desc to return to the free q we just
         * setup instead of the global free queue.
         */
        Cppi_setReturnQueue ((Cppi_DescType) Cppi_DescType_HOST, pCppiDesc, qInfo);

        /* Push descriptor to Tx free queue */
        Qmss_queuePushDescSize (gTxFreeQHnd, pCppiDesc, SIZE_HOST_DESC);           
    }
    if (i != NUM_TX_DESC)
    {
        printf ("Error allocating Tx free descriptors \n");            
        return -1;
    }

    /* All done with Rx configuration. Return success. */
    return 0;
}

/** ============================================================================
 *   @n@b Setup_Rx
 *
 *   @b Description
 *   @n This API sets up all relevant data structures and configuration required
 *      for receiving data from PASS/Ethernet. It sets up a Rx free descriptor queue
 *      with some empty pre-allocated buffers to receive data, and an Rx queue
 *      to which the Rxed data is streamed for the example application. This API
 *      also sets up the QM high priority accumulation interrupts required to
 *      receive data from the Rx queue.
 *
 *   @param[in]  
 *   @n None
 * 
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t Setup_Rx (void)
{
    int32_t            result;
    uint8_t            isAllocated, accChannelNum, i;        
    uint16_t             numAccEntries, intThreshold;
    Qmss_Queue         rxFreeQInfo, rxQInfo;
    Ptr                pCppiDesc;
    Qmss_AccCmdCfg     accCfg;
    Cppi_RxFlowCfg     rxFlowCfg;
    Ptr                pDataBuffer;
    uint32_t           mySWInfo[] = {0x11112222, 0x33334444};
    
    /* Open a Receive (Rx) queue. 
     *
     * This queue will be used to hold all the packets received by PASS/CPSW
     *
     * Open the next available High Priority Accumulation queue for Rx.
     */
    if ((gRxQHnd = Qmss_queueOpen (Qmss_QueueType_HIGH_PRIORITY_QUEUE, QMSS_PARAM_NOT_SPECIFIED, &isAllocated)) < 0)
    {
        printf ("Error opening a High Priority Accumulation Rx queue \n");
        return -1;
    }            
    rxQInfo = Qmss_getQueueNumber (gRxQHnd);

    /* Setup high priority accumulation interrupts on the Rx queue. 
     *
     * Let's configure the accumulator with the following settings:
     *      (1) Interrupt pacing disabled.
     *      (2) Interrupt on every received packet
     */
    intThreshold    =   RX_INT_THRESHOLD;
    numAccEntries   =   (intThreshold + 1) * 2;
    accChannelNum   =   PA_ACC_CHANNEL_NUM;

    /* Initialize the accumulator list memory */
    memset ((void *) gHiPriAccumList, 0, numAccEntries * 4);

    /* Ensure that the accumulator channel we are programming is not 
     * in use currently.
     */
    result = Qmss_disableAccumulator (Qmss_PdspId_PDSP1, accChannelNum);
    if (result != QMSS_ACC_SOK && result != QMSS_ACC_CHANNEL_NOT_ACTIVE)
    {
        printf ("Error Disabling high priority accumulator for channel : %d error code: %d\n",
                      accChannelNum, result);
        return -1;
    }

    /* Setup the accumulator settings */
    accCfg.channel             =   accChannelNum;
    accCfg.command             =   Qmss_AccCmd_ENABLE_CHANNEL;
    accCfg.queueEnMask         =   0;
    accCfg.listAddress         =   OSL_Convert_CoreLocal2GlobalAddr((uint32_t) gHiPriAccumList);
    accCfg.queMgrIndex         =   gRxQHnd;
    accCfg.maxPageEntries      =   (intThreshold + 1); /* Add an extra entry for holding the entry count */
    accCfg.timerLoadCount      =   0;
    accCfg.interruptPacingMode =   Qmss_AccPacingMode_LAST_INTERRUPT;
    accCfg.listEntrySize       =   Qmss_AccEntrySize_REG_D;
    accCfg.listCountMode       =   Qmss_AccCountMode_ENTRY_COUNT;
    accCfg.multiQueueMode      =   Qmss_AccQueueMode_SINGLE_QUEUE;
  
    /* Program the accumulator */
    if ((result = Qmss_programAccumulator (Qmss_PdspId_PDSP1, &accCfg)) != QMSS_ACC_SOK)
    {
        printf ("Error Programming high priority accumulator for channel : %d queue : %d error code : %d\n",
                        accCfg.channel, accCfg.queMgrIndex, result);
        return -1;
    }

    /* Open a Rx Free Descriptor Queue (Rx FDQ). 
     *
     * This queue will hold all the Rx free decriptors. These descriptors will be
     * used by the PASS CPDMA to hold data received via CPSW.
     */
    if ((gRxFreeQHnd = Qmss_queueOpen (Qmss_QueueType_STARVATION_COUNTER_QUEUE, QMSS_PARAM_NOT_SPECIFIED, &isAllocated)) < 0)
    {
        printf ("Error opening Rx Free descriptor queue \n");
        return -1;
    }            
    rxFreeQInfo = Qmss_getQueueNumber (gRxFreeQHnd);

    /* Attach some free descriptors to the Rx free queue we just opened. */
    for (i = 0; i < NUM_RX_DESC; i++)
    {
        /* Get a free descriptor from the global free queue we setup 
         * during initialization.
         */
        if ((pCppiDesc = Qmss_queuePop (gGlobalFreeQHnd)) == NULL)
        {
            break;                
        }

        /* The descriptor address returned from the hardware has the 
         * descriptor size appended to the address in the last 4 bits.
         *
         * To get the true descriptor size, always mask off the last 
         * 4 bits of the address.
         */
        pCppiDesc = (Ptr) ((uint32_t) pCppiDesc & 0xFFFFFFF0);

				// BEGIN AARON don't malloc the buffers for rx
				
				pDataBuffer = gRxBuffs[i]; 

        //if ((pDataBuffer = (Ptr) malloc(1518)) == NULL)
        //{
        //    printf ("Error allocating memory for Rx data buffer \n");
        //    break;
        //}
				
				// END AARON
        
        /* Populate the Rx free descriptor with the buffer we just allocated. */
        Cppi_setData (Cppi_DescType_HOST, pCppiDesc, (uint8_t *)OSL_Convert_CoreLocal2GlobalAddr((uint32_t)pDataBuffer), 1518);

        /* Save original buffer information */
        Cppi_setOriginalBufInfo (Cppi_DescType_HOST, pCppiDesc, (uint8_t *)OSL_Convert_CoreLocal2GlobalAddr((uint32_t)pDataBuffer), 1518);

        /* Setup the Completion queue:
         *
         * Setup the return policy for this desc to return to the free q we just
         * setup instead of the global free queue.
         */
        Cppi_setReturnQueue (Cppi_DescType_HOST, pCppiDesc, rxFreeQInfo);

        Cppi_setSoftwareInfo (Cppi_DescType_HOST, pCppiDesc, (uint8_t *) mySWInfo);

        Cppi_setPacketLen    (Cppi_DescType_HOST, pCppiDesc, 1518);
        
        /* Push descriptor to Tx free queue */
        Qmss_queuePushDescSize (gRxFreeQHnd, pCppiDesc, SIZE_HOST_DESC);           
    }        
    if (i != NUM_RX_DESC)
    {
        printf ("Error allocating Rx free descriptors \n");
        return -1;
    }

    /* Setup a Rx Flow.
     *
     * A Rx flow encapsulates all relevant data properties that CPDMA would
     * have to know in order to succefully receive data.
     */
    /* Initialize the flow configuration */
    memset (&rxFlowCfg, 0, sizeof(Cppi_RxFlowCfg));

    /* Let CPPI pick the next available flow */
    rxFlowCfg.flowIdNum             =   CPPI_PARAM_NOT_SPECIFIED;    

    rxFlowCfg.rx_dest_qmgr          =   rxQInfo.qMgr;    
    rxFlowCfg.rx_dest_qnum          =   rxQInfo.qNum;  
    rxFlowCfg.rx_desc_type          =   Cppi_DescType_HOST; 

    rxFlowCfg.rx_ps_location        =   Cppi_PSLoc_PS_IN_DESC;  
    rxFlowCfg.rx_psinfo_present     =   1;    /* Enable PS info */
    
    rxFlowCfg.rx_error_handling     =   0;    /* Drop the packet, do not retry on starvation by default */       
    rxFlowCfg.rx_einfo_present      =   1;    /* EPIB info present */       
    
    rxFlowCfg.rx_dest_tag_lo_sel    =   0;    /* Disable tagging */
    rxFlowCfg.rx_dest_tag_hi_sel    =   0;    
    rxFlowCfg.rx_src_tag_lo_sel     =   0;    
    rxFlowCfg.rx_src_tag_hi_sel     =   0;    

    rxFlowCfg.rx_size_thresh0_en    =   0;    /* By default, we disable Rx Thresholds */
    rxFlowCfg.rx_size_thresh1_en    =   0;    /* By default, we disable Rx Thresholds */
    rxFlowCfg.rx_size_thresh2_en    =   0;    /* By default, we disable Rx Thresholds */
    rxFlowCfg.rx_size_thresh0       =   0x0;
    rxFlowCfg.rx_size_thresh1       =   0x0;
    rxFlowCfg.rx_size_thresh2       =   0x0;

    rxFlowCfg.rx_fdq0_sz0_qmgr      =   rxFreeQInfo.qMgr; /* Setup the Receive free queue for the flow */
    rxFlowCfg.rx_fdq0_sz0_qnum      =   rxFreeQInfo.qNum;    
    rxFlowCfg.rx_fdq0_sz1_qnum      =   0x0; 
    rxFlowCfg.rx_fdq0_sz1_qmgr      =   0x0;
    rxFlowCfg.rx_fdq0_sz2_qnum      =   0x0;
    rxFlowCfg.rx_fdq0_sz2_qmgr      =   0x0;
    rxFlowCfg.rx_fdq0_sz3_qnum      =   0x0;
    rxFlowCfg.rx_fdq0_sz3_qmgr      =   0x0;

    rxFlowCfg.rx_fdq1_qnum          =   rxFreeQInfo.qNum;  /* Use the Rx Queue to pick descriptors */
    rxFlowCfg.rx_fdq1_qmgr          =   rxFreeQInfo.qMgr;
    rxFlowCfg.rx_fdq2_qnum          =   rxFreeQInfo.qNum;  /* Use the Rx Queue to pick descriptors */
    rxFlowCfg.rx_fdq2_qmgr          =   rxFreeQInfo.qMgr;
    rxFlowCfg.rx_fdq3_qnum          =   rxFreeQInfo.qNum;  /* Use the Rx Queue to pick descriptors */
    rxFlowCfg.rx_fdq3_qmgr          =   rxFreeQInfo.qMgr;

    /* Configure the Rx flow */
    if ((gRxFlowHnd = Cppi_configureRxFlow (gCpdmaHnd, &rxFlowCfg, &isAllocated)) == NULL)
    {
        printf ("Error configuring Rx flow \n");
        return -1;
    }

    /* All done with Rx configuration. Return success. */
    return 0;
}
