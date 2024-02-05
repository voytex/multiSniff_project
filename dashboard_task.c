/*
 * dashboard_task.c
 *
 *  Created on: 8. 12. 2023
 *      Author: vojtechlukas
 */

// === INCLUDES =================================================================================================

#include <stdint.h>

#include <stdbool.h>

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

#include <source/utils/handler_funcs.h>

#include <source/oled_gui/gui.h>

// ==============================================================================================================


// === DEFINES ==================================================================================================

#define PORT                (80)

#define INPUT_BUFFER_SIZE   (128)

#define READ_BUFFER_SIZE    (32)

extern Semaphore_Handle Init_SemaphoreHandle;

// ==============================================================================================================


// === GLOBAL VARIABLES =========================================================================================

EthernetServer ethernetServer;

extern HttpHandlerFunction_t httpHandlerFunctions[];

// ==============================================================================================================


// === INTERNAL FUNCTIONS =======================================================================================

void SendHtml(EthernetClient*);

void UpdateDashboardInfo(void);

void HandleChanges(char*);

void Change(const char, const char*);

char* StrTok(char*, const char);

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
                        HandleChanges(incomingBuffer);

                        SendHtml(&ethernetClient);
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


/*
 * === StrTok
 *
 *
 * Parameters:
 *      msg[in]                 - message that gets printed before
 *                                actual diagnostics data
 *      pEntity[in]             - pointer to entity to be printed
 *                                (diagnostics data)
 *      entityType[in]          - type of entity
 * Returns:
 *      N/A
 */
char* StrTok(char* string, const char token)
{
    static char* p;
    char* lastP = NULL;

    if ( string != NULL )
    {
        p = string;

        while ( *p != token )
        {
            if ( *p != '\0' )
            {
                p++;
            }
            else
            {
                return NULL;
            }
        }

        p++;

        return NULL;
    }
    else
    {
        if ( *p == '\0' )
        {
            return NULL;
        }

        lastP = p;

        while ( *p != token )
        {
            if ( *p != '\0' )
            {
                p++;
            }
            else
            {
                return lastP;
            }
        }

        *p = '\0';

        p++;

        return lastP;
    }
}


void SendHtml(EthernetClient *pClient)
{
    int32_t offset = 0;

    UpdateDashboardInfo();

    for (;;)
    {
        offset = Html_CopyHtmlToMtuBuffer(offset);

        EthernetClient_print(pClient, (const char*)MTU_BUF_MEM_START);

        if (offset == -1)
        {
            break;
        }
    }

    EthernetClient_stop(pClient);

    return;
}


void UpdateDashboardInfo(void)
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


void HandleChanges(char* buf)
{
    char* key;
    char* value;
    char* p;

    //
    // Replace 2nd ' ' (space) from
    // buffer, thus cutting the URL
    //
    p = buf;
    while ( *p != ' ') p++;
    p++;
    while ( *p != ' ') p++;
    *p = '\0';

    StrTok(buf, '?');
    key = StrTok(NULL, '=');
    value = StrTok(NULL, '&');

    while ( key != NULL )
    {
        Log_print("Key: ", key, Buffer);

        Log_print("Value: ", value, Buffer);

        Change(*key, value);

        key = StrTok(NULL, '=');

        value = StrTok(NULL, '&');
    }

    return;
}


void Change(const char key, const char* value)
{
    IPAddress tmpIp;
    IPAddress_fromString(&tmpIp, value);

    switch (key)
    {
    case 't':
        STV_WriteStringAtAddress(STVW_TARGET_IP_ADDRESS, (uint8_t*)&tmpIp, 4);
        GUI_ChangeTargetIp(value);
        break;

    case 'd':
        STV_WriteStringAtAddress(STVW_DEVICE_IP_ADDRESS, (uint8_t*)&tmpIp, 4);
        break;

    case 'g':
        STV_WriteStringAtAddress(STVW_GATEWAY_IP_ADDRESS, (uint8_t*)&tmpIp, 4);
        break;

    case 's':
        STV_WriteStringAtAddress(STVW_NETWORK_MASK, (uint8_t*)&tmpIp, 4);
        break;

    case 'h':
        // TODO make sure, DHCP changing is working
        STV_WriteAtAddress(STVW_RF_PROTOCOL, *value);
        break;

    case 'r':
        // TODO Is the device sniffing?
        GUI_ChangeRx((bool)(*value - '0'));
        break;

    case 'p':
        // TODO Is this alright?
        STV_WriteAtAddress(STVW_RF_PROTOCOL, *value == '0' ? 0xB5 : *value == '1' ? 0x15 : 0x0);
        GUI_ChangeProto((uint8_t)(*value - '0'));
        break;

    case 'k':
        // TODO RX Channel!
        break;


    }
}
