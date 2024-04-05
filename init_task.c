/*
 * init_task.c
 *
 *  Created on: 7. 1. 2024
 *      Author: vojtechlukas
 */

// === INCLUDES =================================================================================================

#include <source/ethernet/IPAddress.h>

#include <source/ethernet/Ethernet.h>

#include <source/oled_gui/gui.h>

#include <source/utils/log.h>

#include <source/utils/restart.h>

#include <ti/drivers/GPIO.h>

#include <ti/drivers/SPI.h>

#include <ti/sysbios/knl/Task.h>

#include <ti/sysbios/knl/Semaphore.h>

#include <source/utils/stv.h>

#include <source/html/html.h>

#include <source/utils/handler_funcs.h>

// ===============================================================================================================


// === INTERNAL FUNCTIONS =======================================================================================

static inline void byte2hex(uint8_t byte, char* pHex)
{
    uint8_t nib = (byte & 0xF0) >> 4;

    *pHex++ = nib > 9 ? nib + 'A' : nib + '0';

    nib = (byte & 0xF);

    *pHex = nib > 9 ? nib + 'A' : nib + '0';

    return;
}

static void mac2string(uint8_t* pSrc, char* pDst)
{
    uint8_t i;
    for ( i = 0; i < 6; i++)
    {
        byte2hex(*pSrc, pDst);

        pDst += 2;
        pSrc += 1;

        if ( i <= 5 )
        {
            *pDst = '-';
            pDst++;
        }
    }
}

// ==============================================================================================================


// === EXTERNAL SYMBOLS =========================================================================================

extern Semaphore_Handle Init_SemaphoreHandle;

extern void Main_CreateDashboardTask();

extern void Main_CreateSniffingTask();

// ==============================================================================================================


// === MAIN TASK FUNCTION =======================================================================================

void Init_Main(UArg a0, UArg a1)
{
    uint8_t     retVal = 0;
    IPAddress   dvcIp, tgtIp, gtwIp, dnsIp, msk;
    char        pDvcIpBuf[16] = {0};
    char        pTgtIpBuf[16] = {0};
    char        pMacStr[17] = {0};

    ///////////////////////////
    // Driver Initialization:
    // Initialization functions are called
    // and startup vectors copied
    // from flash to RAM
    //
    SPI_begin();

    GUI_Init();

    STV_CopyStvFromFlashIfNotYet();

    ///////////////////////////
    // Ethernet initialization:
    // According to STV, use either DHCP or static address.
    // Note, that if DHCP fails (request timeout occurs),
    // device is restarted with STV_DHCP flag set to STV_DHCP_FALSE.
    // Device then sets itself static IP address.
    //

    if ( STV_ReadFromAddress(STVW_USING_DHCP) == STV_DHCP_TRUE )
    {
        GUI_ChangeDeviceIp("DHCP pending...");

        Log_print("Waiting for DHCP...", NULL, None);

        retVal = Ethernet_begin_mac((uint8_t*)STVR_MAC_ADDRESS);


        if ( retVal != 1 )
        {
            Log_print("DHCP failed, switching to static", NULL, None);

            STV_WriteAtAddress(STVW_USING_DHCP, STV_DHCP_FALSE);

            RestartMCU();
        }
    }
    else if ( STV_ReadFromAddress(STVW_USING_DHCP) == STV_DHCP_FALSE )
    {
        IPAddress_Init_str(&dvcIp, (uint8_t*)STVW_DEVICE_IP_ADDRESS);

        IPAddress_Init_str(&gtwIp, (uint8_t*)STVW_GATEWAY_IP_ADDRESS);

        IPAddress_Init_str(&msk, (uint8_t*)STVW_NETWORK_MASK);

        dnsIp.dword = 0x01010101;

        Ethernet_begin_mac_ip_dns_gateway_subnet((uint8_t*)STVR_MAC_ADDRESS,
                                                 dvcIp, dnsIp, gtwIp, msk);
    }

    dvcIp = Ethernet_localIP();

    IPAddress_toString(dvcIp, pDvcIpBuf);

    Log_print("Device IP: ", &dvcIp, IpAddress);

    GUI_ChangeDeviceIp(pDvcIpBuf);

    ///////////////////////////
    // Target IP Setting:
    //
    IPAddress_Init_str(&tgtIp, (uint8_t*)STVW_TARGET_IP_ADDRESS);

    Log_print("Target IP: ", &tgtIp, IpAddress);

    IPAddress_toString(tgtIp, pTgtIpBuf);

    GUI_ChangeTargetIp(pTgtIpBuf);

    ///////////////////////////
    // Other settings:
    //
    mac2string((uint8_t*)STVR_MAC_ADDRESS, pMacStr);

    Html_SetKeyValueInBuffer('m', pMacStr);

    GUI_ChangeRx(false);

    GUI_ChangeProto(STV_ReadFromAddress(STVW_RF_PROTOCOL) == STV_RF_PROTO_BLE ? 0 : 1);

    GUI_ChangeChannel(STV_ReadFromAddress(STVW_RF_CHANNEL));

    Semaphore_post(Init_SemaphoreHandle);

    Main_CreateDashboardTask();

    Main_CreateSniffingTask();

    return;
}
