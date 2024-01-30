/*
 * dashboard_task.c
 *
 *  Created on: 8. 12. 2023
 *      Author: vojtechlukas
 */

// === INCLUDES =================================================================================================

#include <stdint.h>

#include <ti/drivers/SPI.h>

#include <source/ethernet/Ethernet.h>

#include <source/ethernet/EthernetServer.h>

#include <source/ethernet/IPAddress.h>

#include <stdbool.h>

#include <ti/sysbios/knl/Task.h>

#include <ti/sysbios/knl/Semaphore.h>

#include <ti/sysbios/BIOS.h>

#include <dashboard_task.h>

#include <source/utils/log.h>

#include <source/utils/restart.h>

#include <source/utils/stv.h>

#include <source/html/html.h>

// ==============================================================================================================


// === DEFINES ==================================================================================================

#define PORT                (80)

#define INPUT_BUFFER_SIZE   (128)

#define READ_BUFFER_SIZE    (32)

extern Semaphore_Handle Init_SemaphoreHandle;

// ==============================================================================================================


// === STATIC VARIABLES =========================================================================================

EthernetServer ethernetServer;

// ==============================================================================================================


// === INTERNAL FUNCTIONS =======================================================================================

inline bool clientConnected(EthernetClient *client)
{
    return ( client->_sock < 8 );
}

// ==============================================================================================================


// === PUBLISHED FUNCTIONS ======================================================================================

void Dashboard_Main(UArg a0, UArg a1)
{
    Semaphore_pend(Init_SemaphoreHandle, BIOS_WAIT_FOREVER);

    static bool readDestination;

    EthernetServer_begin(&ethernetServer, PORT);

    for (;;)
    {
        readDestination = false;

        EthernetClient ethernetClient = EthernetServer_available(&ethernetServer);

        if ( EthernetClient_connected(&ethernetClient) )
        {

            char incomingBuffer[INPUT_BUFFER_SIZE];

            bool currentLineIsBlank = true;

            while ( EthernetClient_connected(&ethernetClient) )
            {
                if ( EthernetClient_available(&ethernetClient) )
                {
                    if ( !readDestination )
                    {
                        EthernetClient_readBytesUntil(&ethernetClient, '\n', incomingBuffer, INPUT_BUFFER_SIZE);

                        readDestination = true;
                    }

                    uint8_t c = EthernetClient_read(&ethernetClient);

                    if ( c == '\n' && currentLineIsBlank )
                    {
                        Dashboard_SendHtml(&ethernetClient);
                    }

                    if ( c == '\n' )
                    {
                        currentLineIsBlank = true;
                    }
                    else if ( c != '\r' )
                    {
                        currentLineIsBlank = false;
                    }
                }

                Task_sleep(100);
            }
        }

        Task_sleep(100);

        Ethernet_maintain();
    }
}


void Dashboard_SendHtml(EthernetClient *client)
{
    int32_t offset = 0;

    Dashboard_UpdateDashboardInfo();

    for (;;)
    {
        offset = Html_CopyHtmlToMtuBuffer(offset);

        EthernetClient_print(client, (const char*)MTU_BUF_MEM_START);

        if (offset == -1)
        {
            break;
        }
    }

    EthernetClient_stop(client);

    return;
}

void Dashboard_UpdateDashboardInfo(void)
{
    char        tempBuf[17] = {0};
    IPAddress   tmpIp;

    ///////////////////////////
    // Update Device IP address
    //
    tmpIp = Ethernet_localIP();

    IPAddress_toString(tmpIp, tempBuf);

    Html_SetKeyValueInBuffer('d', tempBuf);

    ///////////////////////////
    // Update Target IP address
    //
    IPAddress_Init_str(&tmpIp, (uint8_t*)STVW_TARGET_IP_ADDRESS);

    IPAddress_toString(tmpIp, tempBuf);

    Html_SetKeyValueInBuffer('t', tempBuf);

    ///////////////////////////
    // Update Gateway IP address
    //
    tmpIp = Ethernet_gatewayIP();

    IPAddress_toString(tmpIp, tempBuf);

    Html_SetKeyValueInBuffer('g', tempBuf);

    ///////////////////////////
    // Update Network Mask
    //
    tmpIp = Ethernet_subnetMask();

    IPAddress_toString(tmpIp, tempBuf);

    Html_SetKeyValueInBuffer('s', tempBuf);

    ///////////////////////////
    // Update 'sniffing' status
    // TODO
    Html_SetKeyValueInBuffer('r', "1");

    ///////////////////////////
    // Update Current Protocol
    // TODO
    Html_SetKeyValueInBuffer('p', "0");

    ///////////////////////////
    //  Update RXOK BLE Frames
    // TODO
    Html_SetKeyValueInBuffer('v', "42");

    ///////////////////////////
    // Update RXNOK BLE Frames
    // TODO
    Html_SetKeyValueInBuffer('w', "42");

    ///////////////////////////
    // Update RXOK 802_15_4 Frames
    // TODO
    Html_SetKeyValueInBuffer('x', "69");

    ///////////////////////////
    // Update RXNOK 802_15_4 Frames
    // TODO
    Html_SetKeyValueInBuffer('y', "69");

    ///////////////////////////
    // Update Last Frame's RSSI
    // TODO
    Html_SetKeyValueInBuffer('z', "-19");

    return;
}
