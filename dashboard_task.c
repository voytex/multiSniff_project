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

#include <stdbool.h>

#include <ti/sysbios/knl/Task.h>

#include <ti/sysbios/knl/Semaphore.h>

#include <ti/sysbios/BIOS.h>

#include <dashboard_task.h>

#include <source/utils/log.h>

#include <source/utils/restart.h>


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

    RestartMCU();

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
    //EthernetClient_println(client, "HTTP/1.1 200 OK\nContent-Type: text/html\nConnection: close\n\n<html>Hello World!</html>");

    EthernetClient_print(client, (const char*)0x51000);
    EthernetClient_stop(client);
}
