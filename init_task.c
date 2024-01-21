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

// ==============================================================================================================


// === EXTERNAL SYMBOLS =========================================================================================

extern Semaphore_Handle Init_SemaphoreHandle;

// ==============================================================================================================


// === MAIN TASK FUNCTION =======================================================================================

void Init_Main(UArg a0, UArg a1)
{
    uint8_t     retVal = 0;
    IPAddress   dvcIp;
    IPAddress   tgtIp;
    char        pDvcIpBuf[16] = {0};
    char        pTgtIpBuf[16] = {0};

    ///////////////////////////
    // Driver Initialization:
    // Initialization functions are called
    // and startup vectors copied
    // from flash to RAM
    //
    SPI_begin();

    GUI_Init();

    Stv_CopyStvFromFlashIfNotYet();

    ///////////////////////////
    // Ethernet initialization:
    // According to STV, use either DHCP or static address.
    // Note, that if DHCP fails (request timeout occurs),
    // device is restarted with STV_DHCP flag set to STV_DHCP_FALSE.
    // Device then sets itself static IP address.
    //
    IPAddress_Init_str(&dvcIp, (uint8_t*)STVW_DEVICE_IP_ADDRESS);

    if ( Stv_ReadFromAddress(STVW_USING_DHCP) == STV_DHCP_TRUE )
    {
        GUI_ChangeDeviceIp("DHCP pending...");

        Log_print("Waiting for DHCP...", NULL, None);

        retVal = Ethernet_begin_mac((uint8_t*)STVR_MAC_ADDRESS);

        if ( retVal != 1)
        {
            Log_print("DHCP failed, switching to static", NULL, None);

            Stv_WriteAtAddress(STVW_USING_DHCP, STV_DHCP_FALSE);

            RestartMCU();
        }
    }
    else if ( Stv_ReadFromAddress(STVW_USING_DHCP) == STV_DHCP_FALSE )
    {
        Ethernet_begin_mac_ip((uint8_t*)STVR_MAC_ADDRESS, dvcIp);
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
    GUI_ChangeRx(false);

    GUI_ChangeProto(Stv_ReadFromAddress(STVW_RF_PROTOCOL) == STV_RF_PROTO_BLE ? 0 : 1);

    Semaphore_post(Init_SemaphoreHandle);

    return;
}
