/*
 * main.c
 *
 *  Created on: 12. 12. 2023
 *      Author: vojtechlukas
 */

// === INCLUDES =================================================================================================

#include <stdint.h>

#include <xdc/std.h>

#include <xdc/runtime/System.h>

#include <xdc/runtime/Memory.h>

#include <xdc/runtime/Error.h>

#include <ti/sysbios/knl/Task.h>

#include <ti/sysbios/BIOS.h>

#include <ti/drivers/Board.h>

#include <ti/sysbios/knl/Semaphore.h>

#include "ti_drivers_config.h"

#include "ti_radio_config.h"

#include <ti/drivers/GPIO.h>

#include <ti/drivers/I2C.h>

#include <source/radio_api/radio_api.h>

#include <source/oled_gui/gui.h>

#include <source/ethernet/Ethernet.h>

#include <dashboard_task.h>

#include <source/log/log.h>

#include <source/html/html.h>

// ==============================================================================================================


// === DEFINES ==================================================================================================

#define MAC_ADDRESS_STV     (0x50000)
#define DVC_IP_ADD_STV      (MAC_ADDRESS_STV + 6)
#define TGT_IP_ADD_STV      (DVC_IP_ADD_STV + 4)

// ==============================================================================================================


// === INTERNAL FUNCTIONS =======================================================================================

Task_Params initTaskParams;
Task_Handle initTaskHandle;
void InitTask()
{
    uint8_t retVal = 0;
    IPAddress dvcIp;
    IPAddress tgtIp;
    char pDvcIpBuf[16] = {0};
    char pTgtIpBuf[16] = {0};

    GUI_Init();

    SPI_begin();

    retVal = Ethernet_begin_mac((uint8_t*)MAC_ADDRESS_STV);

    if ( retVal == 1 )
    {
        dvcIp = Ethernet_localIP();
    }
    else
    {
        IPAddress_Init_str(&dvcIp, (uint8_t*)DVC_IP_ADD_STV);

        Ethernet_begin_mac_ip((uint8_t*)MAC_ADDRESS_STV, dvcIp);
    }

    IPAddress_Init_str(&tgtIp, (uint8_t*)TGT_IP_ADD_STV);

    IPAddress_toString(dvcIp, pDvcIpBuf);

    IPAddress_toString(tgtIp, pTgtIpBuf);

    GUI_ChangeDeviceIp(pDvcIpBuf);

    GUI_ChangeTargetIp(pTgtIpBuf);

    //

    Html_SetKeyValueInBuffer('a', "testuju a");

    Html_SetKeyValueInBuffer('b', "Beta!");

    uint16_t tmp;
    tmp = Html_CopyHtmlToMtuBuffer(0);

    Html_SetKeyValueInBuffer('a', "IP: 19");

    Html_CopyHtmlToMtuBuffer(tmp);


    return;
}

// ==============================================================================================================

int main()
{
    Board_init();

    GPIO_init();

    Task_Params_init(&initTaskParams);
    initTaskParams.stackSize = 1024;
    initTaskHandle = Task_create((Task_FuncPtr)InitTask, &initTaskParams, NULL);

    BIOS_start();

    return 0;
}
