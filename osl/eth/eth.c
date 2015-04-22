/*  ============================================================================
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

// Includes {{{

#include <stdio.h>
#include <stdint.h>
#include <ti/csl/cslr_device.h>
#include <ti/csl/csl_psc.h>
#include <ti/csl/csl_pscAux.h>

#include <osl/inc/eth/eth.h>
#include "cpsw_singlecore.h"
#include "cppi_qmss_mgmt.h"

extern volatile unsigned int cregister TSCL;

//}}}

// Definitions {{{

/* Addresses used to disable the PASS PDSP's */
#define PASS_PDSP0	0x02001000
#define PASS_PDSP1	0x02001100
#define PASS_PDSP2	0x02001200
#define PASS_PDSP3	0x02001300
#define PASS_PDSP4	0x02001400
#define PASS_PDSP5	0x02001500

/* Number of packets to be used for testing the example. */
#define                     MAX_NUM_PACKETS                         10u

/* Counters to track number of packets sent/received by this application */
extern volatile uint32_t				gRxCounter, gTxCounter;

void mdebugHaltPdsp (Int pdspNum);
volatile Int mdebugWait = 1;

//}}}

static void powerup(void) //{{{
{
//    /* PASS power domain is turned OFF by default. It needs to be turned on before doing any 
//     * PASS device register access. This not required for the simulator. */
//
//    /* PASS Power domain cycle end on ON*/
//
//    /* power on and enable the clocks for PASS modules */
//    CSL_PSC_enablePowerDomain (CSL_PSC_PD_PASS);
//    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_PKTPROC, PSC_MODSTATE_ENABLE);
//    /* Start the state transition */
//    CSL_PSC_startStateTransition (CSL_PSC_PD_PASS);
//    /* Wait until the state transition process is completed. */
//    while (!CSL_PSC_isStateTransitionDone (CSL_PSC_PD_PASS))
//
//    CSL_PSC_enablePowerDomain (CSL_PSC_PD_PASS);
//    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_CPGMAC,  PSC_MODSTATE_ENABLE);
//    /* Start the state transition */
//    CSL_PSC_startStateTransition (CSL_PSC_PD_PASS);
//    /* Wait until the state transition process is completed. */
//    while (!CSL_PSC_isStateTransitionDone (CSL_PSC_PD_PASS))
//
//    CSL_PSC_enablePowerDomain (CSL_PSC_PD_PASS);
//    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_Crypto,  PSC_MODSTATE_ENABLE);
//    /* Start the state transition */
//    CSL_PSC_startStateTransition (CSL_PSC_PD_PASS);
//    /* Wait until the state transition process is completed. */
//    while (!CSL_PSC_isStateTransitionDone (CSL_PSC_PD_PASS));
//
//    /* Disabling PA PDSP to prepare for power down */
//    volatile unsigned int *passPDSP;
//    passPDSP = (unsigned int*)PASS_PDSP0;
//    *passPDSP &= 0xFFFFFFFD;
//
//    passPDSP = (unsigned int*)PASS_PDSP1;
//    *passPDSP &= 0xFFFFFFFD;
//
//    passPDSP = (unsigned int*)PASS_PDSP2;
//    *passPDSP &= 0xFFFFFFFD;
//
//    passPDSP = (unsigned int*)PASS_PDSP3;
//    *passPDSP &= 0xFFFFFFFD;
//
//    passPDSP = (unsigned int*)PASS_PDSP4;
//    *passPDSP &= 0xFFFFFFFD;
//
//    passPDSP = (unsigned int*)PASS_PDSP5;
//    *passPDSP &= 0xFFFFFFFD;
//
//    /* Disable the clocks for PASS modules */
//    CSL_PSC_disablePowerDomain (CSL_PSC_PD_PASS);
//    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_PKTPROC, PSC_MODSTATE_SWRSTDISABLE);
//    /* Start the state transition */
//    CSL_PSC_startStateTransition (CSL_PSC_PD_PASS);
//    /* Wait until the state transition process is completed. */
//    while (!CSL_PSC_isStateTransitionDone (CSL_PSC_PD_PASS));
//
//    CSL_PSC_disablePowerDomain (CSL_PSC_PD_PASS);
//    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_CPGMAC,  PSC_MODSTATE_SWRSTDISABLE);
//    /* Start the state transition */
//    CSL_PSC_startStateTransition (CSL_PSC_PD_PASS);
//    /* Wait until the state transition process is completed. */
//    while (!CSL_PSC_isStateTransitionDone (CSL_PSC_PD_PASS));
//
//    CSL_PSC_disablePowerDomain (CSL_PSC_PD_PASS);
//    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_Crypto,  PSC_MODSTATE_SWRSTDISABLE);
//    /* Start the state transition */
//    CSL_PSC_startStateTransition (CSL_PSC_PD_PASS);
//    /* Wait until the state transition process is completed. */
//    while (!CSL_PSC_isStateTransitionDone (CSL_PSC_PD_PASS));

    /* Enable the clocks for PASS modules */
    CSL_PSC_enablePowerDomain (CSL_PSC_PD_PASS);
    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_PKTPROC, PSC_MODSTATE_ENABLE);
    /* Start the state transition */
    CSL_PSC_startStateTransition (CSL_PSC_PD_PASS);
    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (CSL_PSC_PD_PASS));

    CSL_PSC_enablePowerDomain (CSL_PSC_PD_PASS);
    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_CPGMAC,  PSC_MODSTATE_ENABLE);
    /* Start the state transition */
    CSL_PSC_startStateTransition (CSL_PSC_PD_PASS);
    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (CSL_PSC_PD_PASS));

    CSL_PSC_enablePowerDomain (CSL_PSC_PD_PASS);
    CSL_PSC_setModuleNextState (CSL_PSC_LPSC_Crypto,  PSC_MODSTATE_ENABLE);
    /* Start the state transition */
    CSL_PSC_startStateTransition (CSL_PSC_PD_PASS);
    /* Wait until the state transition process is completed. */
    while (!CSL_PSC_isStateTransitionDone (CSL_PSC_PD_PASS));

//    /* Enable PDSPs */
//    passPDSP = (unsigned int*)PASS_PDSP0;
//    *passPDSP = 0xFFFFFFFD;
//
//    passPDSP = (unsigned int*)PASS_PDSP1;
//    *passPDSP = 0xFFFFFFFD;
//
//    passPDSP = (unsigned int*)PASS_PDSP2;
//    *passPDSP = 0xFFFFFFFD;
//
//    passPDSP = (unsigned int*)PASS_PDSP3;
//    *passPDSP = 0xFFFFFFFD;
//
//    passPDSP = (unsigned int*)PASS_PDSP4;
//    *passPDSP = 0xFFFFFFFD;
//
//    passPDSP = (unsigned int*)PASS_PDSP5;
//    *passPDSP = 0xFFFFFFFD;
} //}}}

int32_t eth_init(uint8_t* host_mac, uint8_t* amc_mac, uint8_t* wire_mac) //{{{
{
	uint8_t broadcast_mac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

	/* Power up */
	powerup();

	/* Initialize the components
	 *  (1) QMSS
	 *  (2) CPPI
	 *  (3) Ethernet switch subsystem + MDIO + SGMII
	 */
	/* Initialize QMSS */
	if (Init_Qmss () != 0)
	{
		printf ("QMSS init failed \n");
		return -1;
	}
	else
	{
		printf ("QMSS successfully initialized \n");            
	}

	/* Initialize CPPI */
	if (Init_Cppi () != 0)
	{
		printf ("CPPI init failed \n");
		return -1;
	}
	else
	{
		printf ("CPPI successfully initialized \n");            
	}

	/* Init PA LLD */
	if (Init_PASS () != 0)
	{
		printf ("PASS init failed \n");
		return -1;
	}
	else
	{
		printf ("PASS successfully initialized \n");            
	}

	/* Initialize the CPSW switch */
	if (Init_Cpsw (host_mac, amc_mac, wire_mac) != 0)
	{
		printf ("Ethernet subsystem init failed \n");
		return -1;
	}
	else
	{
		printf ("Ethernet subsystem successfully initialized \n");            
	}
    
	/* Setup Tx */
	if (Setup_Tx () != 0)
	{
		printf ("Tx setup failed \n");
		return -1;
	}
	else
	{
		printf ("Tx setup successfully done \n");            
	}

	/* Setup Rx */
	if (Setup_Rx () != 0)
	{
		printf ("Rx setup failed \n");
		return -1;
	}
	else
	{
		printf ("Rx setup successfully done \n");            
	}

	/* Setup PA */
	if (Setup_PASS (host_mac) != 0)
	{
		printf ("PASS setup failed \n");
		return -1;
	}
	else
	{
		printf ("PASS setup successfully done \n");            
	}

	/* Setup PA for broadcast MAC address */
	if (Setup_PASS (broadcast_mac) != 0)
	{
		printf ("PASS setup failed \n");
		return -1;
	}
	else
	{
		printf ("PASS setup successfully done \n");            
	}

	return 0;
} //}}}

int32_t eth_send(uint8_t* payload, uint32_t payload_len) //{{{
{
	Cppi_Desc* pCppiDesc;

	/* Get a free descriptor from the global free queue we setup 
	 * during initialization.
	 */
	if ((pCppiDesc = Qmss_queuePop(gTxFreeQHnd)) == NULL)
	{
		printf("No Tx free descriptor. Cant run send/rcv test \n");
		return -1;
	}

	/* The descriptor address returned from the hardware has the 
	 * descriptor size appended to the address in the last 4 bits.
	 *
	 * To get the true descriptor size, always mask off the last 
	 * 4 bits of the address.
	 */
	pCppiDesc = (Ptr) ((uint32_t) pCppiDesc & 0xFFFFFFF0);    

	Cppi_setData(Cppi_DescType_HOST, 
							(Cppi_Desc *) pCppiDesc,
							(uint8_t*)OSL_Convert_CoreLocal2GlobalAddr((uint32_t)payload),
							payload_len);

	Cppi_setPacketLen (Cppi_DescType_HOST, (Cppi_Desc *)pCppiDesc, payload_len);
	
	Cppi_setPSFlags(Cppi_DescType_HOST, (Cppi_Desc *)pCppiDesc, 0);

	/* Send the packet out the mac */
	Qmss_queuePush (gPaTxQHnd[8], pCppiDesc, payload_len, SIZE_HOST_DESC, Qmss_Location_TAIL);

	/* Increment the application transmit counter */
	gTxCounter++;

	// TODO maybe we need a cycle delay to wait for the PA

	return 0;
} //}}}

static int32_t eth_recv_from_queue(Cppi_Desc* pCppiDesc, uint8_t** payload, uint32_t* payload_len) //{{{
{
	Cppi_HostDesc*       pHostDesc;

	pHostDesc = (Cppi_HostDesc *)pCppiDesc;
       
	/* Verify the application software info we received is same
	 * as what we had sent earlier.
	 */
	if (pHostDesc->softwareInfo0 != 0xaaaaaaaa)  
	{
		printf ("VerifyPacket: Found an entry in receive queue with swinfo0 = 0x%08x, expected 0x%08x\n", 
			pHostDesc->softwareInfo0, 0xaaaaaaaa);
								 
		pHostDesc->buffLen = pHostDesc->origBufferLen;
		Qmss_queuePush (gRxFreeQHnd, (Ptr)pHostDesc, pHostDesc->buffLen, SIZE_HOST_DESC, Qmss_Location_TAIL);
				
		return -1;
	}
    
	/* Set the buffer to the packet */
	*payload = (uint8_t*)pHostDesc->buffPtr;
	*payload_len = pHostDesc->buffLen;
       
	/* Increment Rx counter to indicate the number of successfully
	 * received packets by the example app.
	 */
	gRxCounter++;

	/* Reset the buffer length and put the descriptor back on the free queue */
	pHostDesc->buffLen = pHostDesc->origBufferLen;
	Qmss_queuePush (gRxFreeQHnd, (Ptr)pHostDesc, pHostDesc->buffLen, SIZE_HOST_DESC, Qmss_Location_TAIL);

  /* Verify packet done. Return success. */
	return 0;
} //}}}

int32_t eth_recv(uint8_t** payload, uint32_t* payload_len, uint8_t blocking) //{{{
{
	static uint8_t sIsPingListUsed = 0;
	Cppi_Desc*     pCppiDesc;        
	uint32_t       count, i;

	/* Wait for accumulator interrupt, configured to only interrupt when there is one packet */
	if (blocking)
	{
		while (qmssGblCfgParams.qmQueIntdReg->INTCNT_REG[PA_ACC_CHANNEL_NUM] <= 0);
	}
	else
	{
		if (qmssGblCfgParams.qmQueIntdReg->INTCNT_REG[PA_ACC_CHANNEL_NUM] <= 0)
		{
			return -1;
		}
	}

	/* Clear INTD and tell the accumulator it can move on to the ping or pong buffer */
	Qmss_ackInterrupt(PA_ACC_CHANNEL_NUM, 1);
	Qmss_setEoiVector(Qmss_IntdInterruptType_HIGH, PA_ACC_CHANNEL_NUM);

	/* Get the number of entries in accumulator list. 
	* The hardware enqueues data alternatively to Ping/Pong buffer lists in
	* the accumulator. Hence, we need to track which list (Ping/Pong)
	* we serviced the last time and accordingly process the other one
	* this time around.
	*/ 
	if (!sIsPingListUsed)
	{
		/* Serviced Pong list last time. So read off the Ping list now */     	
		count = gHiPriAccumList[0];     	
	}
	else
	{
		/* Serviced Ping list last time. So read off the Pong list now */     	
		count = gHiPriAccumList[RX_INT_THRESHOLD + 1];     	
	}

	/* Process all the Results received 
	 *
	 * Skip the first entry in the list that contains the 
	 * entry count and proceed processing results.
	 */
	for (i = 1; i <= count; i ++)
	{
		/* Get the result descriptor.
		 *
		 * The hardware enqueues data alternatively to Ping/Pong buffer lists in
		 * the accumulator. Hence, we need to track which list (Ping/Pong)
		 * we serviced the last time and accordingly process the other one
		 * this time around.
		 */
		if (!sIsPingListUsed)
		{
			/* Serviced Pong list last time. So read off the Ping list now */                
			pCppiDesc   =   (Cppi_Desc *) gHiPriAccumList[i];
		}
		else
		{
			/* Serviced Ping list last time. So read off the Pong list now 
			*
			* Skip over Ping list length to arrive at Pong list start.
			*/                
			pCppiDesc   =   (Cppi_Desc *) gHiPriAccumList[i + RX_INT_THRESHOLD + 1];
		}

		/* Descriptor size appended to the address in the last 4 bits.
		 *
		 * To get the true descriptor size, always mask off the last 
		 * 4 bits of the address.
		 */
		pCppiDesc = (Ptr) ((uint32_t) pCppiDesc & 0xFFFFFFF0);    
		eth_recv_from_queue(pCppiDesc, payload, payload_len);
	}

	/* Clear the accumulator list and save whether we used Ping/Pong
	 * list information for next time around.
	 */
	if (count > 0)
	{
		if (!sIsPingListUsed)
		{
			/* Just processed Ping list */
			sIsPingListUsed = 1;

			/* Clear the accumulator list after processing */
			memset((void *) &gHiPriAccumList[0], 0, sizeof (uint32_t) * (RX_INT_THRESHOLD + 1));
		}
		else
		{
			/* Just processed Pong list */
			sIsPingListUsed = 0;
	
			/* Clear the accumulator list after processing */
			memset((void *) &gHiPriAccumList[RX_INT_THRESHOLD + 1], 0, sizeof (uint32_t) * (RX_INT_THRESHOLD + 1));
		}
	}

	/* Done processing interrupt. Return */
	return 0;
} //}}}


//void eth_getMACAddress(uint8_t* macAddress) {
//  //Uint8   macAddress [6]; 
//  Uint8 portNum;
//  portNum =   0;
//
//  CSL_CPSW_3GF_getPortMACAddress (portNum, macAddress);
//}

void eth_printMAC(uint8_t * mac) {
  int i;
  for (i = 0; i < 5; i++)
    printf("%02x:", mac[i]);
  printf("%02x", mac[i]);
}

void eth_printUDPPayloadChars(NET_ip_packet * ip_pkt, Uint32 nChars) {
  unsigned char * udp_payload = ip_pkt->data + sizeof(NET_udphdr);
  int i; 
  for (i = 0; i < nChars; i++) {
    printf("%c", udp_payload[i]);
  }
}
