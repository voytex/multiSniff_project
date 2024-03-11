/*
 * dashboard_task.c
 *
 *  Created on: 8. 12. 2023
 *      Author: vojtechlukas
 */

// === INCLUDES =================================================================================================

#include <stdint.h>

#include <source/radio_api/radio_api.h>

#include <stdbool.h>

#include <string.h>

#include <ti/drivers/SPI.h>

#include <ti/drivers/GPIO.h>

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

#include <ti/sysbios/family/arm/m3/Hwi.h>

#include "ti_radio_config.h"

#include "ti_drivers_config.h"

// ==============================================================================================================


// === DEFINES ==================================================================================================

#define PORT                (80)

#define INPUT_BUFFER_SIZE   (256)

#define READ_BUFFER_SIZE    (32)

extern Semaphore_Handle Init_SemaphoreHandle;
extern Semaphore_Handle Dashboard_SemaphoreHandle;

// ==============================================================================================================


// === GLOBAL VARIABLES =========================================================================================

EthernetServer ethernetServer;

// ==============================================================================================================


// === INTERNAL FUNCTIONS =======================================================================================

void SendHtmlToClient(EthernetClient*);

void UpdateDashboardInfo(void);

void HandleRestApi(char*);

void SetStatusProperty(const char, const char*);

char* StrTok(char*, const char);

// ==============================================================================================================


// === PUBLISHED FUNCTIONS ======================================================================================

void Dashboard_Main(UArg a0, UArg a1)
{
    static bool readDestination;

    char incomingBuffer[INPUT_BUFFER_SIZE];

    unsigned int key;

    EthernetServer_begin(&ethernetServer, PORT);

    Ethernet_SetConnectInterruptForAllSockets();

    GPIO_enableInt(CONFIG_GPIO_W5500_INT_CONST);

    for (;;)
    {
        Semaphore_pend(Dashboard_SemaphoreHandle, BIOS_WAIT_FOREVER);

        EthernetClient ethernetClient = EthernetServer_available(&ethernetServer);

        readDestination = false;

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
                    HandleRestApi(incomingBuffer);

                    SendHtmlToClient(&ethernetClient);

                    EthernetServer_begin(&ethernetServer, PORT);

                    break;
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


        }

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


void SendHtmlToClient(EthernetClient *pClient)
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
    extern rfc_bleGenericRxOutput_t bleStats;
    extern rfc_ieeeRxOutput_t ieeeStats;

    ///////////////////////////
    // Update Target IP address
    //
    IPAddress_Init_str(&tmpIp, (uint8_t*)STVW_TARGET_IP_ADDRESS);

    IPAddress_toString(tmpIp, tempBuf);

    Html_SetKeyValueInBuffer('t', tempBuf);

    ///////////////////////////
    // Update Device IP address
    //
    tmpIp = Ethernet_localIP();

    IPAddress_toString(tmpIp, tempBuf);

    Html_SetKeyValueInBuffer('d', tempBuf);

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
    // Update Network Mask
    //
    tempBuf[0] = STV_ReadFromAddress(STVW_USING_DHCP) == STV_DHCP_TRUE ? '1' : '0';

    tempBuf[1] = '\0';

    Html_SetKeyValueInBuffer('h', tempBuf);

    ///////////////////////////
    // Update 'sniffing' status
    //
    tempBuf[0] = STV_ReadFromAddress(STVW_RUNNING_STATUS) == 0x52 ? '1' : '0';

    tempBuf[1] = '\0';

    Html_SetKeyValueInBuffer('r', tempBuf);

    ///////////////////////////
    // Update Current Protocol
    // TODO
    Html_SetKeyValueInBuffer('p', "0");

    ///////////////////////////
    //  Update RXOK BLE Frames
    // TODO
    sprintf(tempBuf, "%d", bleStats.nRxOk);
    Html_SetKeyValueInBuffer('v', tempBuf);

    ///////////////////////////
    // Update RXNOK BLE Frames
    // TODO
    sprintf(tempBuf, "%d", bleStats.nRxNok);
    Html_SetKeyValueInBuffer('w', tempBuf);

    ///////////////////////////
    // Update RXOK 802_15_4 Frames
    // TODO
    sprintf(tempBuf, "%d", ieeeStats.nRxData + ieeeStats.nRxBeacon + ieeeStats.nRxMacCmd);
    Html_SetKeyValueInBuffer('x', tempBuf);

    ///////////////////////////
    // Update RXNOK 802_15_4 Frames
    // TODO
    sprintf(tempBuf, "%d", ieeeStats.nRxNok);
    Html_SetKeyValueInBuffer('y', tempBuf);

    ///////////////////////////
    // Update Last Frame's RSSI
    //
    if ( Radio_GetCurrentProtocol() == BluetoothLowEnergy )
    {
        sprintf(tempBuf, "%d", bleStats.lastRssi);
    }
    else
    {
        sprintf(tempBuf, "%d", ieeeStats.lastRssi);
    }
    Html_SetKeyValueInBuffer('z', tempBuf);

    return;
}


void HandleRestApi(char* buf)
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

        SetStatusProperty(*key, value);

        key = StrTok(NULL, '=');

        value = StrTok(NULL, '&');
    }

    return;
}


void SetStatusProperty(const char key, const char* value)
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
        RestartMCU(); // <== not working
        break;

    case 'h':
        // TODO make sure, DHCP changing is working
        STV_WriteAtAddress(STVW_USING_DHCP, *value == '1' ? 0x59 : 0x4E);
        break;

    case 'r':
        STV_WriteAtAddress(STVW_RUNNING_STATUS, *value == '1' ? 0x52 : 0x00);
        GUI_ChangeRx((bool)(*value - '0'));
        break;

    case 'p':
        // TODO Is this alright?
        STV_WriteAtAddress(STVW_RF_PROTOCOL, *value == '0' ? 0xB5 : *value == '1' ? 0x15 : 0x0);
        GUI_ChangeProto((uint8_t)(*value - '0'));
        STV_WriteAtAddress(STVW_SIGNAL_RF_CHANGE, 0xFF);
        break;

    case 'k':
        // TODO RX Channel!
        break;


    }
}


void HandleInterrupt(void)
{
    //uint16_t hwi = Hwi_disable();
    //uint16_t key = Task_disable();
    Ethernet_ClearConnectInterruptForAllSockets();
    Semaphore_post(Dashboard_SemaphoreHandle);
    //Task_restore(key);
    //Hwi_restore(hwi);
    return;
}
