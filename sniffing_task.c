/*
 * radio_task.c
 *
 *  Created on: 7. 2. 2024
 *      Author: vojtechlukas
 */

// === INCLUDES =================================================================================================

#include "ti_drivers_config.h"

#include "ti_radio_config.h"

#include <stdio.h>

#include <ti/sysbios/BIOS.h>

#include <ti/sysbios/knl/Task.h>

#include <ti/sysbios/knl/Semaphore.h>

#include <stdint.h>

#include <source/queue/radio_queue.h>

#include <source/utils/stv.h>

#include <source/ethernet/Ethernet.h>

#include <source/ethernet/EthernetUdp.h>

#include <source/radio_api/radio_api.h>

#include <source/utils/restart.h>

#include <sniffing_task.h>

//===============================================================================================================

extern Semaphore_Handle Dashboard_SemaphoreHandle;

void HandleIncomingRfPacket(uint8_t*, IPAddress, RF_Protocol_t, const uint8_t[]);

EthernetUDP   ethernetUdp;

rfc_bleGenericRxOutput_t bleStats;

rfc_ieeeRxOutput_t ieeeStats;

// === MAIN TASK FUNCTION =======================================================================================

void Sniffing_Main(UArg a0, UArg a1)
{
    RF_Object     rfObj;
    RF_Params     rfParams;
    RF_CmdHandle  rfCmdHnd;
    RF_Protocol_t currProto;
    RF_Handle     rfHnd;
    uint8_t       packetBuffer[2047];
    const uint8_t accessAddress[] = {0xD6, 0xBE, 0x89, 0x8E};
    IPAddress*    targetIp = (IPAddress*)STVW_TARGET_IP_ADDRESS;
    uint8_t*      bSniffing = (uint8_t*)STVW_RUNNING_STATUS;
    uint8_t*      bChange = (uint8_t*)STVW_SIGNAL_RF_CHANGE;


    RadioQueue_init();

    RadioQueue_reset();

    EthernetUDP_begin_init(&ethernetUdp);

    EthernetUDP_begin(&ethernetUdp, 2014, 3);

    //Semaphore_post(Dashboard_SemaphoreHandle);

    currProto = Radio_GetCurrentProtocol();

    Radio_initRXCmd(&bleStats, &ieeeStats);

    Radio_openRadioCore(&rfParams, &rfObj, currProto, &rfHnd);

    Radio_setFrequencySynthesizer(&rfHnd, currProto);

    Radio_beginRX(rfHnd, currProto, &Radio_HandleQueueOverflow, RF_EventRxBufFull);

    for (;;)
    {

        if (*bSniffing)
        {
            HandleIncomingRfPacket(packetBuffer, *targetIp, currProto, accessAddress);
        }

        if (*bChange)
        {
            *bChange = 0x0;

            Radio_stopRX(rfHnd);

            RestartMCU();
        }
        Task_yield();
    }
}


/*
 * === TODO
 * Stops listening to RF frames.
 * Main purpose is to shadow TI's API for better readability
 *
 * Parameters:
 *      pHandle[in]             - handle to Radio Core
 * Returns:
 *      RF_Stat                 - Enum signaling successful completition
 */
void HandleIncomingRfPacket(uint8_t* buffer, IPAddress targetIp, RF_Protocol_t proto, const uint8_t accessAddr[])
{
    //uint8_t buf[2047];

    uint8_t ret = 0;

    uint16_t packetLen = RadioQueue_takePacket(buffer, 2047);

    if (packetLen)
    {
        EthernetUDP_beginPacket_ip(&ethernetUdp, targetIp, 2014);

        EthernetUDP_write_byte(&ethernetUdp, (uint8_t)proto);

        if ( proto == BluetoothLowEnergy )
        {
            EthernetUDP_write(&ethernetUdp, (uint8_t*)accessAddr, 4);
        }

        EthernetUDP_write(&ethernetUdp, (uint8_t*)buffer, packetLen);

        ret = EthernetUDP_endPacket(&ethernetUdp);
    }

    return;
}

