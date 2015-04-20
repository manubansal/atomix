/**  
 * @file cpsw_mgmt.c
 *
 * @brief 
 *  This file holds all the Ethernet subsystem (CPSW + MDIO + SGMII) components
 *  initialization and setup code.
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
/* C Standard library Include */
#include <string.h>

/* Chip Level definitions include */
#include <ti/csl/csl_chip.h>
#include <ti/csl/csl_bootcfgAux.h>

/* CSL EMAC include */
#include <ti/csl/csl_cpsw.h>
#include <ti/csl/csl_cpsgmii.h>
#include <ti/csl/csl_cpsgmiiAux.h>
#include <ti/csl/cslr_cpsgmii.h>
#include <ti/csl/csl_mdio.h>
#include <ti/csl/csl_mdioAux.h>

/* BootCfg module include */
#include <ti/csl/csl_bootcfg.h>
#include <ti/csl/csl_bootcfgAux.h>

#include "cpsw_singlecore.h"

/** Number of ports in the ethernet subsystem */
#define         NUM_PORTS                   3u

/** Number of MAC/GMII ports in the ethernet switch */
#define         NUM_MAC_PORTS               2u

/** ============================================================================
 *   @n@b Init_SGMII
 *
 *   @b Description
 *   @n SGMII peripheral initialization code.
 *
 *   @param[in]  
 *   @n macPortNum      MAC port number for which the SGMII port setup must
 *                      be performed.
 * 
 *   @return
 *   @n None
 * =============================================================================
 */
int32_t Init_SGMII (uint32_t macPortNum)
{  
    CSL_SGMII_ADVABILITY    sgmiiCfg;
	CSL_SGMII_STATUS        sgmiiStatus;
    
    /* Configure SGMII Port 1 only since it is connected to RJ45 at all known EVMs */
    if(macPortNum == 1)
    {
        /* Reset the port before configuring it */
        CSL_SGMII_doSoftReset (macPortNum);        
        while (CSL_SGMII_getSoftResetStatus (macPortNum) != 0);   

        /* Hold the port in soft reset and set up
        * the SGMII control register:
        *      (1) Enable Master Mode (default)
        *      (2) Enable Auto-negotiation
        */
        CSL_SGMII_startRxTxSoftReset (macPortNum);  
        CSL_SGMII_disableMasterMode (macPortNum);
    
	    /* Setup the Advertised Ability register for this port:
        *      (1) Enable Full duplex mode
        *      (2) Enable Auto Negotiation
        */
        sgmiiCfg.linkSpeed      =   CSL_SGMII_1000_MBPS;
        sgmiiCfg.duplexMode     =   CSL_SGMII_FULL_DUPLEX;
        CSL_SGMII_setAdvAbility (macPortNum, &sgmiiCfg);
    
        CSL_SGMII_enableAutoNegotiation (macPortNum);
        CSL_SGMII_endRxTxSoftReset (macPortNum);   
        
	    /* Wait for SGMII Link */
				do
				{
						CSL_SGMII_getStatus(macPortNum, &sgmiiStatus);
				} while (sgmiiStatus.bIsLinkUp != 1);
					
				/* Wait for SGMII Autonegotiation to complete without error */
				do
				{
						CSL_SGMII_getStatus(macPortNum, &sgmiiStatus);
						if (sgmiiStatus.bIsAutoNegError != 0)
								return -1;
				} while (sgmiiStatus.bIsAutoNegComplete != 1);
					
					/* 
					 * May need to wait some more time for the external PHY to be ready to transmit packets reliabily.
					 * It is possible to access the PHY status register through the MDIO interface to check when 
					 * the PHY is ready.
					 * To avoid platform-dependent code, we just introduce about 2ms wait here
					 */ 
					CycleDelay(2000000);
    }

    /* All done with configuration. Return Now. */
    return 0;
}

/** ============================================================================
 *   @n@b Init_MAC
 *
 *   @b Description
 *   @n This API initializes the CPGMAC Sliver (MAC Port) port.
 *
 *   @param[in]  
 *   @n macPortNum      MAC port number for which the initialization must be done.
 *
 *   @param[in]  
 *   @n macAddress      MAC address to configure on this port.
 * 
 *   @param[in]  
 *   @n mtu             Maximum Frame length to configure on this port.
 *
 *   @return
 *   @n None
 * =============================================================================
 */
int Init_MAC (uint32_t macPortNum, uint8_t macAddress[6], uint32_t mtu)
{
    /* Reset MAC Sliver 0 */            
    CSL_CPGMAC_SL_resetMac (macPortNum);
    while (CSL_CPGMAC_SL_isMACResetDone (macPortNum) != TRUE);

    /* Setup the MAC Control Register for this port:
     *      (1) Enable Full duplex
     *      (2) Enable GMII
     *      (3) Enable Gigabit 
     *      (4) Enable External Configuration. This enables 
     *          the "Full duplex" and "Gigabit" settings to be
     *          controlled externally from SGMII
     *      (5) Don't enable any control/error/short frames
     */
    CSL_CPGMAC_SL_enableFullDuplex (macPortNum);
    CSL_CPGMAC_SL_enableGMII (macPortNum);
    CSL_CPGMAC_SL_enableGigabit (macPortNum);
    CSL_CPGMAC_SL_enableExtControl (macPortNum);

    /* Configure the MAC address for this port */
    //CSL_CPSW_3GF_setPortMACAddress (macPortNum, macAddress);

    /* Configure VLAN ID/CFI/Priority.
     *
     * For now, we are not using VLANs so just configure them
     * to all zeros.
     */
    CSL_CPSW_3GF_setPortVlanReg (macPortNum, 0, 0, 0);

    /* Configure the Receive Maximum length on this port,
     * i.e., the maximum size the port can receive without
     * any errors.
     *
     * Set the Rx Max length to the MTU configured for the
     * interface.
     */
    CSL_CPGMAC_SL_setRxMaxLen (macPortNum, mtu);  

    /* Done setting up the MAC port */
    return 0;
}

/** ============================================================================
 *   @n@b Init_MDIO
 *
 *   @b Description
 *   @n Not supported at moment. MDIO is not simulated yet.
 *
 *   @param[in]  
 *   @n None
 *
 *   @return
 *   @n None
 * =============================================================================
 */
void Init_MDIO (void)
{
    /* Return success. */        
    return;        
}

/** ============================================================================
 *   @n@b Init_Switch
 *
 *   @b Description
 *   @n This API sets up the ethernet switch subsystem and its Address Lookup 
 *      Engine (ALE) in "Switch" mode.
 *
 *   @param[in]  
 *   @n mtu             Maximum Frame length to configure on the switch.
 *
 *   @return
 *   @n None
 * =============================================================================
 */
void Init_Switch (uint32_t mtu)
{
    CSL_CPSW_3GF_PORTSTAT               portStatCfg;

    /* Enable the CPPI port, i.e., port 0 that does all 
     * the data streaming in/out of EMAC.
     */
    CSL_CPSW_3GF_enablePort0 ();
    CSL_CPSW_3GF_disableVlanAware ();
    CSL_CPSW_3GF_setPort0VlanReg (0, 0, 0);
    CSL_CPSW_3GF_setPort0RxMaxLen (mtu);

    /* Disable statistics on both the port groups:
     *
     * MAC Sliver ports -   Port 1, Port 2
     * CPPI Port        -   Port 0
     */
    portStatCfg.p0AStatEnable   =   0;
    portStatCfg.p0BStatEnable   =   0;
    portStatCfg.p1StatEnable    =   0;
    portStatCfg.p2StatEnable    =   0;
    CSL_CPSW_3GF_setPortStatsEnableReg (&portStatCfg);

    /* Setup the Address Lookup Engine (ALE) Configuration:
     *      (1) Enable ALE.
     *      (2) Clear stale ALE entries.
     *      (3) Disable VLAN Aware lookups in ALE since
     *          we are not using VLANs by default.
     *      (4) No Flow control
     *      (5) Configure the Unknown VLAN processing 
     *          properties for the switch, i.e., which 
     *          ports to send the packets to.
     */
    CSL_CPSW_3GF_enableAle ();
    CSL_CPSW_3GF_clearAleTable ();
    CSL_CPSW_3GF_enableAleBypass();
    
    CSL_CPSW_3GF_disableAleVlanAware ();
    CSL_CPSW_3GF_disableAleTxRateLimit ();
    CSL_CPSW_3GF_setAlePrescaleReg (125000000u/1000u);
    CSL_CPSW_3GF_setAleUnkownVlanReg (7, 3, 3, 7);
    
    /* Done with switch configuration */
    return;
}


/** ============================================================================
 *   @n@b Switch_update_addr
 *
 *   @b Description
 *   @n This API add/delete entries in the Address Lookup Engine (ALE) in "Switch" mode.
 *
 *   @param[in]  
 *   @n portNum         Switch port number.
 
 *   @param[in]  
 *   @n macAddress      MAC address to configure on the switch.
 * 
 *   @param[in]  
 *   @n add             0:add; 1:delete.
 *
 *   @return
 *   @n None
 *
 *   @Note  It supports "add" operation only now.           
 * =============================================================================
 */
int Switch_update_addr (uint32_t portNum, uint8_t macAddress[6], uint16_t add)
{
    CSL_CPSW_3GF_ALE_PORTCONTROL        alePortControlCfg;

    /* Configure the address in "Learning"/"Forward" state */
    alePortControlCfg.portState             =   ALE_PORTSTATE_FORWARD;
    alePortControlCfg.dropUntaggedEnable    =   0;
    alePortControlCfg.vidIngressCheckEnable =   0;
    alePortControlCfg.noLearnModeEnable     =   0;
    alePortControlCfg.mcastLimit            =   0;
    alePortControlCfg.bcastLimit            =   0;

    CSL_CPSW_3GF_setAlePortControlReg (portNum, &alePortControlCfg);
    
    /* Done with upading address */
    return 0;
}


/** ============================================================================
 *   @n@b Init_SGMII_SERDES
 *
 *   @b Description
 *   @n This API sets up the configuration for the SGMII SERDES. Assumes a 125 MHz
 *       reference clock.
 *
 *   @param[in]  
 *   @n None
 * 
 *   @return
 *   @n None
 * =============================================================================
 */
int32_t Init_SGMII_SERDES(void)
{
	/* SGMII SERDES Configuration complete. Return. */
	return 0;
}
/** ============================================================================
 *   @n@b Init_Cpsw
 *
 *   @b Description
 *   @n This API sets up the entire ethernet subsystem and all its associated
 *      components.
 *
 *   @param[in]  
 *   @n None
 * 
 *   @return
 *   @n None
 * =============================================================================
 */
int32_t Init_Cpsw (uint8_t* host_mac, uint8_t* amc_mac, uint8_t* wire_mac)
{                      
    uint32_t     macPortNum, mtu = 1518;
    uint8_t*     macSrcAddress [2] =  {amc_mac, wire_mac};
    uint8_t*     macAddress0 = host_mac;  /* MAC address for (CPPI) Port 0 */
    uint8_t*     macAddress1 = amc_mac;   /* MAC address for (EMAC1) Port 1 */
    uint8_t*     macAddress2 = wire_mac;  /* MAC address for (EMAC2) Port 2 */
    
	/* Initialize the SERDES modules */
	Init_SGMII_SERDES();
    
    /* Initialize the SGMII/Sliver submodules for the 
     * two corresponding MAC ports.
     */
    for (macPortNum = 0; macPortNum < NUM_MAC_PORTS; macPortNum++)
    {
    	if (Init_SGMII (macPortNum))
		  return -1;
        Init_MAC (macPortNum, &macSrcAddress[macPortNum][0], mtu);
    }

    /* Setup the Phys by initializing the MDIO */
    Init_MDIO ();

    /* Setup the Ethernet switch finally. */
    Init_Switch (mtu);
    
    Switch_update_addr(0, macAddress0, 0);
    Switch_update_addr(1, macAddress1, 0);
    Switch_update_addr(2, macAddress2, 0);

    /* CPSW subsystem setup done. Return success */
    return 0;    
}
