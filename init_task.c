/*
 * init_task.c
 *
 *  Created on: 7. 1. 2024
 *      Author: Administrator
 */

#include <source/ethernet/IPAddress.h>

#include <source/ethernet/Ethernet.h>

#include <source/oled_gui/gui.h>

#include <source/log/log.h>

#include <ti/drivers/GPIO.h>

#include <ti/drivers/SPI.h>

#include <ti/sysbios/knl/Task.h>

#include <ti/sysbios/knl/Semaphore.h>

#define MAC_ADDRESS_STV     (0x50000)
#define DVC_IP_ADD_STV      (MAC_ADDRESS_STV + 6)
#define TGT_IP_ADD_STV      (DVC_IP_ADD_STV + 4)
extern Semaphore_Handle Init_SemaphoreHandle;


void Init_Main(UArg a0, UArg a1)
{
    uint8_t retVal = 0;
    IPAddress dvcIp;
    IPAddress tgtIp;
    char pDvcIpBuf[16] = {0};
    char pTgtIpBuf[16] = {0};

    GUI_Init();

    SPI_begin();

    retVal = 0; //Ethernet_begin_mac((uint8_t*)MAC_ADDRESS_STV);

    if ( retVal == 1 )
    {
        dvcIp = Ethernet_localIP();
    }
    else
    {
        IPAddress_Init_str(&dvcIp, (uint8_t*)DVC_IP_ADD_STV);

        Ethernet_begin_mac_ip((uint8_t*)MAC_ADDRESS_STV, dvcIp);

        dvcIp = Ethernet_localIP();
    }

    IPAddress_Init_str(&tgtIp, (uint8_t*)TGT_IP_ADD_STV);

    Log_print("Device: ", &dvcIp, IpAddress);

    Log_print("Target: ", &tgtIp, IpAddress);

    IPAddress_toString(dvcIp, pDvcIpBuf);

    IPAddress_toString(tgtIp, pTgtIpBuf);

    GUI_ChangeDeviceIp(pDvcIpBuf);

    GUI_ChangeTargetIp(pTgtIpBuf);

    Semaphore_post(Init_SemaphoreHandle);

    return;
}
