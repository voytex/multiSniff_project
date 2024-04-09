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

#include <source/utils/restart.h>

#include <sniffing_task.h>

#include <source/radio_proc/radio_proc.h>

#include <source/utils/log.h>

//===============================================================================================================


// === GLOBAL VARIABLES =========================================================================================

extern uint32_t nRxBufFull;

EthernetUDP ethernetUdp;

extern rfc_bleGenericRxOutput_t bleStats;

extern rfc_ieeeRxOutput_t ieeeStats;

// === MAIN TASK FUNCTION =======================================================================================


/*
 * === Sniffing_Main
 * This task ensures core functionality of the device. Firstly, it initializes
 * the RF Core and if Startup Vectors are configured that way, it starts sniffing.
 * Then it falls into infinite loop, where it periodically checks for new frames in
 * RF queue. If it founds one, it creates a UDP packet and fills it with appropriate
 * data.
 *
 * Parameters:
 *      a0                      - Not used
 *      a1                      - Not used
 * Returns:
 *      N/A
 */
void Sniffing_Main(UArg a0, UArg a1)
{
    const uint8_t accessAddress[] = {0xD6, 0xBE, 0x89, 0x8E};

    const uint8_t rxChannel = STV_ReadFromAddress(STVW_RF_CHANNEL);

    const RF_Protocol_t proto = STV_ReadFromAddress(STVW_RF_PROTOCOL) == STV_RF_PROTO_BLE ? BluetoothLowEnergy : IEEE_802_15_4;

    IPAddress* targetIp = (IPAddress*)STVW_TARGET_IP_ADDRESS;

    RadioQueue_init();

    RadioQueue_reset();

    EthernetUDP_begin_init(&ethernetUdp);

    EthernetUDP_begin(&ethernetUdp, 2014);

    if (STV_ReadFromAddress(STVW_RUNNING_STATUS) == 0x52)
    {
        Radio_SetUpAndBeginRx(proto, rxChannel);
    }

    for (;;)
    {
        if ( RadioQueue_hasPacket() )
        {
            uint8_t buffer[2047];

            uint16_t packetLen = RadioQueue_takePacket(buffer, 2047);

            EthernetUDP_beginPacket_ip(&ethernetUdp, *targetIp, 2014);

            EthernetUDP_write_byte(&ethernetUdp, (uint8_t)Radio_GetCurrentProtocol());

            if ( Radio_GetCurrentProtocol() == BluetoothLowEnergy )
            {
                EthernetUDP_write(&ethernetUdp, (uint8_t*)accessAddress, 4);
            }

            EthernetUDP_write(&ethernetUdp, (uint8_t*)buffer, packetLen);

            EthernetUDP_endPacket(&ethernetUdp);
        }

        Log_print("BLE RX CMD", &RFCMD_bleGenericRX, CmdStatus);

        Log_print("IEEE RX CMD", &RFCMD_ieeeRX, CmdStatus);

        Task_sleep(10);
    }

}

