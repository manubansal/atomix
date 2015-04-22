/**  
 * @file pa_mgmt.c
 *
 * @brief 
 *  Packet accelerator subsystem management functions.
 *  
 *  \par
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
#include "cpsw_singlecore.h"
#include "cppi_qmss_mgmt.h"

#include <stdio.h>
#include <stdint.h>

/* PA LLD include */
#include <ti/drv/pa/pa.h>

/* PASS RL file */
#include <ti/csl/cslr_device.h>
#include <ti/csl/cslr_pa_ss.h>

/* Firmware images */
#include <ti/drv/pa/fw/pafw.h>

/* PA command response queue handle */
Qmss_QueueHnd                           gPaCfgCmdRespQHnd;

/* Number of PA internal buffers to allocate */
#define     PA_NUM_BUFFERS              3

/* PA definitions */
#define     MAX_NUM_L2_HANDLES          10
#define     MAX_NUM_L3_HANDLES          20
#define     MAX_NUM_L4_HANDLES          40

#define     BUFSIZE_PA_INST             256
#define     BUFSIZE_L2_TABLE            1000
#define     BUFSIZE_L3_TABLE            4000

/* PA instance */
#pragma DATA_SECTION(gPAInst, ".pa")
#pragma DATA_ALIGN(gPAInst, CACHE_LINESZ)
uint8_t                                   gPAInst[SYS_ROUND_UP(BUFSIZE_PA_INST, CACHE_LINESZ)];

/* PA Driver Handle */
Pa_Handle                               gPAInstHnd;

/* Memory used for PA handles */
#pragma DATA_SECTION(gMemL2Ram, ".pa")
#pragma DATA_ALIGN(gMemL2Ram, CACHE_LINESZ)
uint8_t                                   gMemL2Ram[SYS_ROUND_UP(BUFSIZE_L2_TABLE, CACHE_LINESZ)];

#pragma DATA_SECTION(gMemL3Ram, ".pa")
#pragma DATA_ALIGN(gMemL3Ram, CACHE_LINESZ)
uint8_t                                   gMemL3Ram[SYS_ROUND_UP(BUFSIZE_L3_TABLE, CACHE_LINESZ)];

paHandleL2L3_t                          gPaL2Handles[MAX_NUM_L2_HANDLES];
paHandleL2L3_t                          gPaL3Handles[MAX_NUM_L3_HANDLES];
paHandleL4_t                            gPaL4Handles[MAX_NUM_L4_HANDLES];

/* pa configuration command buffer */
uint8_t                                   gPaCmdBuf1[pa_ADD_LUT1_MIN_CMD_BUF_SIZE_BYTES];
uint8_t                                   gPaCmdBuf2[pa_ADD_LUT2_MIN_CMD_BUF_SIZE_BYTES];

extern volatile unsigned int cregister TSCL;

/** ============================================================================
 *   @n@b CycleDelay
 *
 *   @b Description
 *   @n This API implements a clock delay logic using the Time Stamp Counter (TSC)
 *      of the DSP.
 *
 *   @param[in]  
 *   @n count               Number of delay cycles to wait.
 * 
 *   @return    
 *   @n None
 * =============================================================================
 */
void CycleDelay (int32_t count)
{
    uint32_t                  TSCLin;

    if (count <= 0)
        return;

    /* Get the current TSCL  */
    TSCLin = TSCL ;

    while ((TSCL - TSCLin) < (uint32_t)count);
}

/** ============================================================================
 *   @n@b Download_PAFirmware
 *
 *   @b Description
 *   @n This API downloads the PA firmware required for PDSP operation.
 *
 *   @param[in]  
 *   @n None
 * 
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t Download_PAFirmware (void)
{
    int32_t                   i;

    /* Hold the PA in reset state during download */
    Pa_resetControl (gPAInstHnd, pa_STATE_RESET);

    /* PDPSs 0-2 use image c1 */
    for (i = 0; i < 3; i++)
    {
        Pa_downloadImage (gPAInstHnd, i, (Ptr)c1, c1Size);
    }

    /* PDSP 3 uses image c2 */
    Pa_downloadImage (gPAInstHnd, 3, (Ptr)c2, c2Size);

    /* PDSPs 4-5 use image m */
    for (i = 4; i < 6; i++)
    {
        Pa_downloadImage (gPAInstHnd, i, (Ptr)m, mSize);
    }

    /* Enable the PA back */
    Pa_resetControl (gPAInstHnd, pa_STATE_ENABLE);

    return 0;
}        

/** ============================================================================
 *   @n@b Add_MACAddress
 *
 *   @b Description
 *   @n This API adds the switch MAC address to the PA PDSP Lookup table. This 
 *      ensures that all packets destined for this MAC address get processed
 *      for forwarding to the host.
 *
 *   @param[in]  
 *   @n None
 * 
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
 
int32_t Add_MACAddress (uint8_t* mac)
{
    int32_t                       j;
    uint16_t                      cmdSize;
    Qmss_Queue                  cmdReplyQInfo;
    Qmss_Queue                  rxQInfo;
    paEthInfo_t                 ethInfo     =  { { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },        /* Src mac = dont care */   
                                                 { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15 },        /* Dest mac */
                                                    0,                                          /* vlan = dont care */      
                                                    0x0800,                                     /* ether type = ours */     
                                                    0,                                          /* MPLS tag = don't care */
                                                    pa_EMAC_PORT_1                              /* Input EMAC port */
                                               }; 
    paRouteInfo_t               routeInfo   =   {   pa_DEST_HOST,           /* Route a match to the host */   
                                                    0,                      /* Flow ID 0 */                   
                                                    0,                      /* Destination queue */           
                                                    -1,                     /* Multi route disabled */        
                                                    0xaaaaaaaa,             /* SwInfo 0 */                    
                                                    0,                      /* SwInfo 1 is dont care */
                                                    0,                      /* customType = pa_CUSTOM_TYPE_NONE */         \
                                                    0,                      /* customIndex: not used */        \
                                                    0,                      /* pkyType: for SRIO only */       \
                                                    NULL                    /* No commands */
                                                };
	/* Setup the Rx queue as destination for the packets */
	rxQInfo                 =   Qmss_getQueueNumber (gRxQHnd);
	routeInfo.queue       =   rxQInfo.qNum;
	routeInfo.flowId        =   (uint8_t)Cppi_getFlowId(gRxFlowHnd);

    paRouteInfo_t               nFailInfo =     {   pa_DEST_DISCARD,                            /* Toss the packet  */           
	                                                0,                                          /* Flow Id = dont care */        
                                                    0,                                          /* queue = dont care */          
                                                    0,                                          /* mutli route = dont care */    
                                                    0,                                          /* swinfo0 = dont care */        
                                                    0,                                          /* SwInfo 1 is dont care */
                                                    0,                                          /* customType = pa_CUSTOM_TYPE_NONE */         \
                                                    0,                                          /* customIndex: not used */        \
                                                    0,                                          /* pkyType: for SRIO only */       \
                                                    NULL                                        /* No commands */
                                                };
    paCmdReply_t                cmdReplyInfo =  {   pa_DEST_HOST,                               /* Replies go to the host */            
                                                    0,                                          /* User chosen ID to go to swinfo0 */     
                                                    0,                                          /* Destination queue */                   
                                                    0                                           /* Flow ID */  
                                                };
    paReturn_t        retVal = 1000;
    paEntryHandle_t   retHandle;
    int32_t             handleType, cmdDest;
    uint32_t            psCmd       =   ((uint32_t)(4 << 5) << 24);  
    uint32_t            myswinfo[]  =   {0x11112222, 0x33334444};
    Cppi_HostDesc*    pHostDesc;

    /* Get a Tx free descriptor to send a command to the PA PDSP */
    if ((pHostDesc = Qmss_queuePop (gTxFreeQHnd)) == NULL)
    {
        printf ("Error obtaining a Tx free descriptor \n");            
        return -1;
    }

    /* The descriptor address returned from the hardware has the 
     * descriptor size appended to the address in the last 4 bits.
     *
     * To get the true descriptor pointer, always mask off the last 
     * 4 bits of the address.
     */
    pHostDesc = (Ptr) ((uint32_t) pHostDesc & 0xFFFFFFF0);
    
    /* Populate the Rx free descriptor with the fixed command buffer. */
    Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, (uint8_t *)OSL_Convert_CoreLocal2GlobalAddr((uint32_t)gPaCmdBuf1), sizeof(gPaCmdBuf1));

    /* Save original buffer information */
    Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, (uint8_t *)OSL_Convert_CoreLocal2GlobalAddr((uint32_t)gPaCmdBuf1), sizeof(gPaCmdBuf1));    

    cmdSize                 =   pHostDesc->buffLen;
    cmdReplyInfo.replyId    =   0x11111111;  /* unique for each add mac command */

    /* Get the PA response queue number and populate the destination queue number
     * in the PA response configuration.
     */
    cmdReplyQInfo           =   Qmss_getQueueNumber (gPaCfgCmdRespQHnd);
    cmdReplyInfo.queue      =   cmdReplyQInfo.qNum;
    cmdReplyInfo.flowId     =   (uint8_t)Cppi_getFlowId(gRxFlowHnd);
    
    /* Use Source MAC as destination MAC if non-loopback */
    memcpy(ethInfo.dst, mac, 6);

	retVal  =   Pa_addMac  (gPAInstHnd,
							pa_LUT1_INDEX_NOT_SPECIFIED,
							&ethInfo,
							&routeInfo,
							&nFailInfo,
							&gPaL2Handles[0],
							(paCmd_t) pHostDesc->buffPtr,
							&cmdSize,
							&cmdReplyInfo,
							&cmdDest);

    if (retVal != pa_OK)  
    {
        printf ("Pa_addMac returned error %d\n", retVal);
        return -1;
    }
    
    /* This sets the extended info for descriptors, and this is required so PS info
     * goes to the right spot 
     */                   
    Cppi_setSoftwareInfo (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, (uint8_t *)myswinfo);

    /* Set the buffer length to the size used. It will be restored when the descriptor
     * is returned 
     */
    Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, cmdSize);
    pHostDesc->buffLen  =   cmdSize;
    
    /* Mark the packet as a configuration packet */
    Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, (uint8_t *)&psCmd, 4);
       
    /* Send the command to the PA and wait for the return */
    Qmss_queuePush (gPaTxQHnd[cmdDest - pa_CMD_TX_DEST_0], 
                    pHostDesc, 
                    pHostDesc->buffLen, 
                    SIZE_HOST_DESC, 
                    Qmss_Location_TAIL
                   );
    
    /* Poll on the PA response queue to see if response from PA has come */
    for (j = 0; j < 100; j++)  
    {
        CycleDelay (10000);

        if (Qmss_getQueueEntryCount (gPaCfgCmdRespQHnd) > 0)   
        {
            /* We have a response from PA PDSP for the command we submitted earlier for 
             * MAC address addition.
             */
            pHostDesc = Qmss_queuePop (gPaCfgCmdRespQHnd);

            /* Clear the size bytes */
            pHostDesc = (Ptr) ((uint32_t) pHostDesc & 0xFFFFFFF0);

            if (pHostDesc->softwareInfo0 != cmdReplyInfo.replyId)  
            {
                printf ("Found an entry in PA response queue with swinfo0 = 0x%08x, expected 0x%08x\n", 
                                pHostDesc->softwareInfo0, cmdReplyInfo.replyId);
                pHostDesc->buffLen  =   pHostDesc->origBufferLen;
                Qmss_queuePush (gRxFreeQHnd, pHostDesc, pHostDesc->buffLen, SIZE_HOST_DESC, Qmss_Location_TAIL);

                return -1;
            }

            retVal  =   Pa_forwardResult (gPAInstHnd, (Ptr)pHostDesc->buffPtr, &retHandle, &handleType, &cmdDest);
            if (retVal != pa_OK)  
            {
                printf ("PA sub-system rejected Pa_addMac command\n");
                return -1;
            }
        
            /* Reset the buffer lenght and put the descriptor back on the Tx free queue */
            pHostDesc->buffLen = pHostDesc->origBufferLen;
            Qmss_queuePush (gRxFreeQHnd, pHostDesc, pHostDesc->buffLen, SIZE_HOST_DESC, Qmss_Location_TAIL);

            break;
        }
    }

    if (j == 100)  
    {
        printf ("Timeout waiting for reply from PA to Pa_addMac command\n");
        return -1;
    }

    return 0;
}

/** ============================================================================
 *   @n@b Add_IPAddress
 *
 *   @b Description
 *   @n This API adds the IP Address the application's using to the PA PDSP 
 *      Lookup table. This ensures that all packets destined for this 
 *      IP address get forwarded up to the host.
 *
 *   @param[in]  
 *   @n None
 * 
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t Add_IPAddress (void)
{
    int32_t                       j;
    uint16_t                      cmdSize;
    Qmss_Queue                  cmdReplyQInfo;
    paIpInfo_t                  ipInfo      =    {  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },   /* IP source = dont care */   
                                                    { 0xc0, 0xa8, 0x01, 0xa, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },   /* IP dest */                
                                                    0,         /* SPI = dont care */                                                
                                                    0,         /* flow = dont care */                                                   
                                                    pa_IPV4,   /* IP type */                                                            
                                                    0,         /* GRE protocol */                                                       
                                                    0,         /* Ip protocol = dont care (TCP or UDP or anything else) */              
                                                    0,         /* TOS */                                                                
                                                    FALSE,     /* TOS = dont care (seperate field since TOS=0 is valid */
                                                    0          /* SCTP destination port = dont care */  
                                                };
    paRouteInfo_t               routeInfo   =   {   pa_DEST_CONTINUE_PARSE_LUT2,                /* Continue parsing */             
                                                    0,                                          /* Flow Id = dont care */          
                                                    0,                                          /* queue = dont care */            
                                                    0,                                          /* multi route = dont care */      
                                                    0,                                          /* swinfo0 = dont care */          
                                                    0,                                          /* SwInfo 1 is dont care */
                                                    0,                                          /* customType = pa_CUSTOM_TYPE_NONE */         \
                                                    0,                                          /* customIndex: not used */        \
                                                    0,                                          /* pkyType: for SRIO only */       \
                                                    NULL                                        /* No commands */
                                                };
    paRouteInfo_t               nFailInfo   =   {   pa_DEST_DISCARD,                            /* Toss the packet  */           
	                                                0,                                          /* Flow Id = dont care */        
                                                    0,                                          /* queue = dont care */          
                                                    0,                                          /* mutli route = dont care */    
                                                    0,                                          /* swinfo0 = dont care */        
                                                    0,                                          /* SwInfo 1 is dont care */
                                                    0,                                          /* customType = pa_CUSTOM_TYPE_NONE */         \
                                                    0,                                          /* customIndex: not used */        \
                                                    0,                                          /* pkyType: for SRIO only */       \
                                                    NULL                                        /* No commands */
                                                };
    paCmdReply_t                cmdReplyInfo =  {   pa_DEST_HOST,                               /* Replies go to the host */            
                                                    0,                                          /* User chosen ID to go to swinfo0 */     
                                                    0,                                          /* Destination queue */                   
                                                    0                                           /* Flow ID */  
                                                };
    paReturn_t         retVal;
    paEntryHandle_t    retHandle;
    int32_t              handleType, cmdDest;
    uint32_t             psCmd       =   ((uint32_t)(4 << 5) << 24);  
    uint32_t             myswinfo[]  =   {0x11112222, 0x33334444};
    Cppi_HostDesc*     pHostDesc;

    /* Get a Tx free descriptor to send a command to the PA PDSP */
    if ((pHostDesc = Qmss_queuePop (gTxFreeQHnd)) == NULL)
    {
        printf ("Error obtaining a Tx free descriptor \n");            
        return -1;
    }

    /* The descriptor address returned from the hardware has the 
     * descriptor size appended to the address in the last 4 bits.
     *
     * To get the true descriptor pointer, always mask off the last 
     * 4 bits of the address.
     */
    pHostDesc = (Ptr) ((uint32_t) pHostDesc & 0xFFFFFFF0);
    
    /* Populate the Rx free descriptor with the buffer we just allocated. */
    Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, (uint8_t *)OSL_Convert_CoreLocal2GlobalAddr((uint32_t)gPaCmdBuf1), sizeof(gPaCmdBuf1));

    /* Save original buffer information */
    Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, (uint8_t *)OSL_Convert_CoreLocal2GlobalAddr((uint32_t)gPaCmdBuf1), sizeof(gPaCmdBuf1));    

    cmdSize                 =   pHostDesc->buffLen;
    cmdReplyInfo.replyId    =   0x11111111;  /* unique for each add mac command */

    /* Get the PA response queue number and populate the destination queue number
     * in the PA response configuration.
     */
    cmdReplyQInfo           =   Qmss_getQueueNumber (gPaCfgCmdRespQHnd);
    cmdReplyInfo.queue      =   cmdReplyQInfo.qNum;
    cmdReplyInfo.flowId     =   (uint8_t)Cppi_getFlowId(gRxFlowHnd);

    retVal  =   Pa_addIp    (gPAInstHnd,
                             pa_LUT_INST_NOT_SPECIFIED,
                             pa_LUT1_INDEX_NOT_SPECIFIED,
                            &ipInfo,
                            NULL,                         // AARON this is never set, i think it's their bug
                            &routeInfo,
                            &nFailInfo,
                            &gPaL3Handles[0],
                            (paCmd_t) pHostDesc->buffPtr,
                            &cmdSize,
                            &cmdReplyInfo,
                            &cmdDest);
    if (retVal != pa_OK)  
    {
        printf ("Pa_addIp returned error %d\n", retVal);
        return -1;
    }
    
    /* This sets the extended info for descriptors, and this is required so PS info
     * goes to the right spot 
     */                   
    Cppi_setSoftwareInfo (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, (uint8_t *)myswinfo);

    /* Set the buffer length to the size used. It will be restored when the descriptor
     * is returned 
     */
    Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, cmdSize);
    pHostDesc->buffLen  =   cmdSize;
    
    /* Mark the packet as a configuration packet */
    Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, (uint8_t *)&psCmd, 4);
       
    /* Send the command to the PA and wait for the return */
    Qmss_queuePush (gPaTxQHnd[cmdDest - pa_CMD_TX_DEST_0], 
                    (uint32_t *)OSL_Convert_CoreLocal2GlobalAddr((uint32_t)pHostDesc), 
                    pHostDesc->buffLen, 
                    SIZE_HOST_DESC, 
                    Qmss_Location_TAIL
                    );

    /* Poll on the PA response queue to see if response from PA has come */
    for (j = 0; j < 100; j++)  
    {
        CycleDelay (10000);

        if (Qmss_getQueueEntryCount (gPaCfgCmdRespQHnd) > 0)   
        {
            /* We have a response from PA PDSP for the command we submitted earlier for 
             * MAC address addition.
             */
            pHostDesc = Qmss_queuePop (gPaCfgCmdRespQHnd);

            /* Clear the size bytes */
            pHostDesc = (Ptr) ((uint32_t) pHostDesc & 0xFFFFFFF0);

            if (pHostDesc->softwareInfo0 != cmdReplyInfo.replyId)  
            {
                printf ("Found an entry in PA response queue with swinfo0 = 0x%08x, expected 0x%08x\n", 
                                pHostDesc->softwareInfo0, cmdReplyInfo.replyId);
                pHostDesc->buffLen  =   pHostDesc->origBufferLen;
                Qmss_queuePush (gRxFreeQHnd, pHostDesc, pHostDesc->buffLen, SIZE_HOST_DESC, Qmss_Location_TAIL);

                return -1;
            }

            retVal  =   Pa_forwardResult (gPAInstHnd, (Ptr)pHostDesc->buffPtr, &retHandle, &handleType, &cmdDest);
            if (retVal != pa_OK)  
            {
                printf ("PA sub-system rejected Pa_addIp command\n");
                return -1;
            }
        
            /* Reset the buffer lenght and put the descriptor back on the Tx free queue */
            pHostDesc->buffLen = pHostDesc->origBufferLen;
            Qmss_queuePush (gRxFreeQHnd, pHostDesc, pHostDesc->buffLen, SIZE_HOST_DESC, Qmss_Location_TAIL);

            break;
        }
    }

    if (j == 100)  
    {
        printf ("Timeout waiting for reply from PA to Pa_addMac command\n");
        return -1;
    }

    return 0;
}

/** ============================================================================
 *   @n@b Add_Port
 *
 *   @b Description
 *   @n This API adds the UDP port the application's using to the PA PDSP 
 *      Lookup table. This ensures that all packets destined for this 
 *      UDP port get forwarded up to the host.
 *
 *   @param[in]  
 *   @n None
 * 
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t Add_Port (void)
{
    int32_t                       j;
    uint16_t                      cmdSize;
    Qmss_Queue                  cmdReplyQInfo;
    Qmss_Queue                  rxQInfo;
    uint16_t                      ports       =   {0x5678};

    int32_t                       ipLink      =   {0};  /* Link this with the first IP address created */
    paRouteInfo_t               routeInfo   =   {   pa_DEST_HOST,           /* Route a match to the host */   
                                                    0,                      /* Flow ID 0 */                   
                                                    0,                      /* Destination queue */           
                                                    -1,                     /* Multi route disabled */        
                                                    0xaaaaaaaa,             /* SwInfo 0 */                    
                                                    0,                      /* SwInfo 1 is dont care */
                                                    0,                      /* customType = pa_CUSTOM_TYPE_NONE */         \
                                                    0,                      /* customIndex: not used */        \
                                                    0,                      /* pkyType: for SRIO only */       \
                                                    NULL                    /* No commands */
                                                };                      
    paCmdReply_t                cmdReplyInfo =  {   pa_DEST_HOST,           /* Replies go to the host */            
                                                    0,                      /* User chosen ID to go to swinfo0 */     
                                                    0,                      /* Destination queue */                   
                                                    0                       /* Flow ID */  
                                                };
    paReturn_t       retVal;
    paEntryHandle_t  retHandle;
    int32_t            handleType, cmdDest;
    uint32_t           psCmd       =   ((uint32_t)(4 << 5) << 24);  
    uint32_t           myswinfo[]  =   {0x11112222, 0x33334444};
    Cppi_HostDesc*   pHostDesc;
    

    /* Get a Tx free descriptor to send a command to the PA PDSP */
    if ((pHostDesc = Qmss_queuePop (gTxFreeQHnd)) == NULL)
    {
        printf ("Error obtaining a Tx free descriptor \n");            
        return -1;
    }

    /* The descriptor address returned from the hardware has the 
     * descriptor size appended to the address in the last 4 bits.
     *
     * To get the true descriptor pointer, always mask off the last 
     * 4 bits of the address.
     */
    pHostDesc = (Ptr) ((uint32_t) pHostDesc & 0xFFFFFFF0);
    
    /* Populate the Rx free descriptor with the buffer we just allocated. */
    Cppi_setData (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, (uint8_t *)OSL_Convert_CoreLocal2GlobalAddr((uint32_t)gPaCmdBuf2), sizeof(gPaCmdBuf2));

    /* Save original buffer information */
    Cppi_setOriginalBufInfo (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, (uint8_t *)OSL_Convert_CoreLocal2GlobalAddr((uint32_t)gPaCmdBuf2), sizeof(gPaCmdBuf2));    

    cmdSize                 =   pHostDesc->buffLen;
    cmdReplyInfo.replyId    =   0x11111111;  /* unique for each add mac command */

    /* Get the PA response queue number and populate the destination queue number
     * in the PA response configuration.
     */
    cmdReplyQInfo           =   Qmss_getQueueNumber (gPaCfgCmdRespQHnd);
    cmdReplyInfo.queue      =   cmdReplyQInfo.qNum;
    cmdReplyInfo.flowId     =   (uint8_t)Cppi_getFlowId(gRxFlowHnd);
    
    /* Setup the Rx queue as destination for the packets */
    rxQInfo                 =   Qmss_getQueueNumber (gRxQHnd);
    routeInfo.queue      	=   rxQInfo.qNum;
    routeInfo.flowId        =   (uint8_t)Cppi_getFlowId(gRxFlowHnd);

    retVal  =   Pa_addPort  (gPAInstHnd,
                            pa_LUT2_PORT_SIZE_16,
                            ports,
                            gPaL3Handles [ipLink],
                            FALSE,                      /* New Entry required */
                            pa_PARAMS_NOT_SPECIFIED,  
                            &routeInfo,
                            gPaL4Handles[0],
                            (paCmd_t) pHostDesc->buffPtr,
                            &cmdSize,
                            &cmdReplyInfo,
                            &cmdDest);
    if (retVal != pa_OK)  
    {
        printf ("Pa_addPort returned error %d\n", retVal);
        return -1;
    }
    
    /* This sets the extended info for descriptors, and this is required so PS info
     * goes to the right spot 
     */                   
    Cppi_setSoftwareInfo (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, (uint8_t *)myswinfo);

    /* Set the buffer length to the size used. It will be restored when the descriptor
     * is returned 
     */
    Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, cmdSize);
    pHostDesc->buffLen  =   cmdSize;
    
    /* Mark the packet as a configuration packet */
    Cppi_setPSData (Cppi_DescType_HOST, (Cppi_Desc *)pHostDesc, (uint8_t *)&psCmd, 4);
       
    /* Send the command to the PA and wait for the return */
    Qmss_queuePush (gPaTxQHnd[cmdDest - pa_CMD_TX_DEST_0], 
                    (uint32_t *)OSL_Convert_CoreLocal2GlobalAddr((uint32_t)pHostDesc), 
                    pHostDesc->buffLen, 
                    SIZE_HOST_DESC, 
                    Qmss_Location_TAIL
                   );

    /* Poll on the PA response queue to see if response from PA has come */
    for (j = 0; j < 100; j++)  
    {
        CycleDelay (10000);

        if (Qmss_getQueueEntryCount (gPaCfgCmdRespQHnd) > 0)   
        {
            /* We have a response from PA PDSP for the command we submitted earlier for 
             * MAC address addition.
             */
            pHostDesc = Qmss_queuePop (gPaCfgCmdRespQHnd);

            /* Clear the size bytes */
            pHostDesc = (Ptr) ((uint32_t) pHostDesc & 0xFFFFFFF0);

            if (pHostDesc->softwareInfo0 != cmdReplyInfo.replyId)  
            {
                printf ("Found an entry in PA response queue with swinfo0 = 0x%08x, expected 0x%08x\n", 
                                pHostDesc->softwareInfo0, cmdReplyInfo.replyId);
                pHostDesc->buffLen  =   pHostDesc->origBufferLen;
                Qmss_queuePush (gRxFreeQHnd, pHostDesc, pHostDesc->buffLen, SIZE_HOST_DESC, Qmss_Location_TAIL);

                return -1;
            }

            retVal  =   Pa_forwardResult (gPAInstHnd, (Ptr)pHostDesc->buffPtr, &retHandle, &handleType, &cmdDest);
            if (retVal != pa_OK)  
            {
                printf ("PA sub-system rejected Pa_addPort command\n");
                return -1;
            }
        
            /* Reset the buffer lenght and put the descriptor back on the Tx free queue */
            pHostDesc->buffLen = pHostDesc->origBufferLen;
            Qmss_queuePush (gRxFreeQHnd, pHostDesc, pHostDesc->buffLen, SIZE_HOST_DESC, Qmss_Location_TAIL);

            break;
        }
    }

    if (j == 100)  
    {
        printf ("Timeout waiting for reply from PA to Pa_addMac command\n");
        return -1;
    }

    return 0;
}

/** ============================================================================
 *   @n@b Init_PASS
 *
 *   @b Description
 *   @n This API initializes the PASS/PDSP and opens a queue that the application
 *      can use to receive command responses from the PASS.
 *
 *   @param[in]  
 *   @n None
 * 
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t Init_PASS (void)
{
	uint8_t						isAllocated;				
  paSizeInfo_t                paSize;
  paConfig_t                  paCfg;
  int32_t                       retVal;    
  int32_t                       sizes[pa_N_BUFS];
  int32_t                       aligns[pa_N_BUFS];
  void*                       bases[pa_N_BUFS];
    
    memset(&paSize, 0, sizeof(paSizeInfo_t));
    memset(&paCfg, 0, sizeof(paConfig_t));
    
    /* Allocate space for the PA LLD buffers. The buffers we need to 
     * allocate space are:
     *      (1) PA LLD Instance Info Handle
     *      (2) PA LLD L2 Handle database
     *      (3) PA LLD L3 Handle database
     */
    paSize.nMaxL2 = MAX_NUM_L2_HANDLES;
    paSize.nMaxL3 = MAX_NUM_L3_HANDLES;
    paSize.nUsrStats = 0;
    if ((retVal = Pa_getBufferReq(&paSize, sizes, aligns)) != pa_OK)
    {
        printf ("Pa_getBufferReq returned error %d\n", retVal);
        return -1;
    }

    /* Validate the buffer allocations */
    /* The first buffer is always the instance buffer */
    if ((uint32_t)gPAInst & (aligns[0] - 1))  
    {
        printf ("Pa_getBufferReq requires %d alignment for instance buffer, but address is 0x%08x\n", aligns[0], (uint32_t)gPAInst);
        return -1;
    }

    if (sizeof(gPAInst) < sizes[0])  
    {
        printf ("Pa_getBufferReq requires %d bytes for instance buffer, have only %d\n", sizes[0], sizeof(gPAInst));
        return -1;
    }

    bases[0]    =   (void *)gPAInst;

    /* The second buffer is the L2 table */
    if ((uint32_t)gMemL2Ram & (aligns[1] - 1))  
    {
        printf ("Pa_getBufferReq requires %d alignment for buffer 1, but address is 0x%08x\n", aligns[1], (uint32_t)gMemL2Ram);
        return (-1);
    }

    if (sizeof(gMemL2Ram) < sizes[1])  
    {
        printf ("Pa_getBufferReq requires %d bytes for buffer 1, have only %d\n", sizes[1], sizeof(gMemL2Ram));
        return -1;
    }

    bases[1]    =   (void *)gMemL2Ram;

    /* The third buffer is the L3 table */
    if ((uint32_t)gMemL3Ram & (aligns[2] - 1))  
    {
        printf ("Pa_alloc requires %d alignment for buffer 1, but address is 0x%08x\n", aligns[2], (uint32_t)gMemL3Ram);
        return (-1);
    }

    if (sizeof(gMemL3Ram) < sizes[2])  
    {
        printf ("Pa_alloc requires %d bytes for buffer 1, have only %d\n", sizes[2], sizeof(gMemL3Ram));
        return (-1);
    }

    bases[2]    =   (void *)gMemL3Ram;
    
    bases[3]    =   0;

    /* Finally initialize the PA LLD */
    paCfg.initTable =   TRUE;
    paCfg.initDefaultRoute = TRUE;
    paCfg.baseAddr = CSL_PA_SS_CFG_REGS;
    paCfg.sizeCfg   =   &paSize;
    if ((retVal = Pa_create (&paCfg, bases, &gPAInstHnd)) != pa_OK)  
    {
        printf ("Pa_create returned with error code %d\n", retVal);
        return -1;
    }

    /* Download the PASS PDSP firmware */
    if (Download_PAFirmware ())
    {
        return -1;
    }

    /* Open a PA Command Response Queue.
     *
     * This queue will be used to hold responses from the PA PDSP for all the
     * commands issued by the example application.
     *
     * This queue is used only at configuration time to setup the PA PDSP.
     */
    if ((gPaCfgCmdRespQHnd = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, QMSS_PARAM_NOT_SPECIFIED, &isAllocated)) < 0)
    {
        printf ("Error opening a PA Command Response queue \n");
        return -1;
    }            

    /* Init done. Return success. */
    return 0;
}

/** ============================================================================
 *   @n@b Setup_PASS
 *
 *   @b Description
 *   @n This API sets up the PA LLD/PDSP with MAC/IP/UDP configuration used by
 *      the example application.
 *
 *   @param[in]  
 *   @n None
 * 
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t Setup_PASS (uint8_t* mac)
{
    /* Setup the PA PDSP to forward packets matching our switch MAC 
     * address up to the host onto the example application.
     */
    if (Add_MACAddress (mac) != 0)
    	return -1;

    /* Add the IP address the example uses */
    //if (Add_IPAddress () != 0)
    //{
    //    return -1;
    //}

    /* Add the port number on which our application is going to listen on */
    //if (Add_Port () != 0)
    //{
    //    return -1;
    //}

    /* Return success */
    return 0;
}

void mdebugHaltPdsp (int32_t pdspNum)
{
    CSL_Pa_ssRegs *passRegs = (CSL_Pa_ssRegs *)CSL_PA_SS_CFG_REGS; 
	passRegs->PDSP_CTLSTAT[pdspNum].PDSP_CONTROL &= ~(CSL_PA_SS_PDSP_CONTROL_PDSP_ENABLE_MASK);

}
