/*
 * radio_proc.c
 *
 *  Created on: 14. 3. 2024
 *      Author: vojtechlukas
 */

// === INCLUDES =================================================================================================

#include <stdint.h>

#include <source/oled_gui/gui.h>

#include "ti_radio_config.h"

#include "ti_drivers_config.h"

#include <source/utils/stv.h>

#include <source/queue/radio_queue.h>

#include <source/utils/log.h>

#include <source/radio_proc/radio_proc.h>

#include <source/ethernet/EthernetUdp.h>

// ==============================================================================================================

// === GLOBAL VARIABLES ========================================================================================

RF_Protocol_t CurrentRfProtocol;

RF_Object rfObject;

RF_Params rfParams;

RF_Handle rfHandle;

RF_CmdHandle rfCmdHandle;

rfc_bleGenericRxOutput_t bleStats;

rfc_ieeeRxOutput_t ieeeStats;

const uint8_t accessAddress[] = {0xD6, 0xBE, 0x89, 0x8E};

static volatile uint16_t nRxBufFull;

// ==============================================================================================================

// === EXTERN VARS ==============================================================================================

extern EthernetUDP ethernetUdp;

// ==============================================================================================================

// === INTERNAL FUNCTIONS =======================================================================================

void initCmdObjects(uint8_t channel)
{
    nRxBufFull = 0;

    RFCMD_bleGenericRX.pParams->pRxQ = RadioQueue_getDQpointer();
    RFCMD_bleGenericRX.pParams->rxConfig.bAutoFlushCrcErr = 1;
    RFCMD_bleGenericRX.pParams->rxConfig.bIncludeLenByte = 1;
    RFCMD_bleGenericRX.pParams->rxConfig.bIncludeCrc = 1;
    RFCMD_bleGenericRX.pParams->rxConfig.bAppendRssi = 0;
    RFCMD_bleGenericRX.pParams->rxConfig.bAppendStatus = 0;
    RFCMD_bleGenericRX.whitening.bOverride = 0x0;
    RFCMD_bleGenericRX.pOutput = &bleStats;

    if (channel >= 37 && channel <= 39)
    {
        RFCMD_bleGenericRX.channel = channel;
        switch (channel)
        {
        case 37:
            RFCMD_bleFrequencySynthesizer.frequency = 2402; // MHz
            break;
        case 38:
            RFCMD_bleFrequencySynthesizer.frequency = 2426; // MHz
            break;
        case 39:
            RFCMD_bleFrequencySynthesizer.frequency = 2480; // MHz
            break;
        }
        RFCMD_bleFrequencySynthesizer.fractFreq = 0;
    }

    RFCMD_ieeeRX.pRxQ = RadioQueue_getDQpointer();
    RFCMD_ieeeRX.rxConfig.bAutoFlushCrc = 1;
    RFCMD_ieeeRX.rxConfig.bAutoFlushIgn = 0;
    RFCMD_ieeeRX.rxConfig.bIncludePhyHdr = 0;
    RFCMD_ieeeRX.rxConfig.bIncludeCrc = 1;
    RFCMD_ieeeRX.rxConfig.bAppendRssi = 0;
    RFCMD_ieeeRX.rxConfig.bAppendCorrCrc = 0;
    RFCMD_ieeeRX.rxConfig.bAppendSrcInd = 0;
    RFCMD_ieeeRX.rxConfig.bAppendTimestamp = 0;
    RFCMD_ieeeRX.pOutput = &ieeeStats;

    if (channel >= 11 && channel <= 26)
    {
        RFCMD_ieeeFrequencySynthesizer.frequency = 2405 + 5 * (channel - 11);
        RFCMD_ieeeFrequencySynthesizer.fractFreq = 0;
        RFCMD_ieeeRX.channel = channel;
    }

    return;
}

// ==============================================================================================================

// === FUNCTION DEFINITIONS =====================================================================================

/*
 * === Radio_SetUpAndBeginRx
 * Sets up Radio Core, initializes Radio Commands (Objects)
 * according to the channel and protocol selected and begins RXing.
 *
 * Parameters:
 *      proto[in]           - RF protocol in `RF_Protocol_t` format
 *      channel[in]         - RF channel - valid values are:
 *                              - BLE:              37 / 38 / 39
 *                              - IEEE 802.15.4:    11..26
 * Returns:
 *      N/A
 */
void Radio_SetUpAndBeginRx(RF_Protocol_t proto, uint8_t channel)
{
    nRxBufFull = 0;

    CurrentRfProtocol = proto;

    RF_Params_init(&rfParams);

    initCmdObjects(channel);

    if (proto == BluetoothLowEnergy)
    {
        rfHandle = RF_open(&rfObject, &RFCMD_bleModeObject, (RF_RadioSetup *)&RFCMD_bleRadioSetup, &rfParams);

        RF_postCmd(rfHandle, (RF_Op *)&RFCMD_bleFrequencySynthesizer, RF_PriorityNormal, NULL, 0);

        rfCmdHandle = RF_postCmd(rfHandle, (RF_Op *)&RFCMD_bleGenericRX, RF_PriorityNormal, (RF_Callback)Radio_CallbackFunction, RF_EventRxBufFull | RF_EventLastCmdDone);
    }

    if (proto == IEEE_802_15_4)
    {
        rfHandle = RF_open(&rfObject, &RFCMD_ieeeModeObject, (RF_RadioSetup *)&RFCMD_ieeeRadioSetup, &rfParams);

        RF_postCmd(rfHandle, (RF_Op *)&RFCMD_ieeeFrequencySynthesizer, RF_PriorityNormal, NULL, 0);

        rfCmdHandle = RF_postCmd(rfHandle, (RF_Op *)&RFCMD_ieeeRX, RF_PriorityNormal, (RF_Callback)Radio_CallbackFunction, RF_EventRxBufFull | RF_EventLastCmdDone);
    }

    return;
}

/*
 * === Radio_CallbackFunction
 * Callback function that is called every time RX buffer overfills or
 * last command in RF Core queue is done.
 *
 * Parameters:
 *      eventMsk[in]        - Event mask to distinguish among events
 *      (others not used)
 * Returns:
 *      N/A
 */
void Radio_CallbackFunction(RF_Handle rfHnd, RF_CmdHandle rfCmdHnd, RF_EventMask eventMsk)
{

    if (eventMsk & RF_EventRxBufFull)
    {
        nRxBufFull++;

        RadioQueue_reset();

        if (CurrentRfProtocol == BluetoothLowEnergy)
        {
            rfCmdHandle = RF_postCmd(rfHandle, (RF_Op *)&RFCMD_bleGenericRX, RF_PriorityNormal, (RF_Callback)Radio_CallbackFunction, RF_EventRxBufFull | RF_EventLastCmdDone);
        }

        if (CurrentRfProtocol == IEEE_802_15_4)
        {
            rfCmdHandle = RF_postCmd(rfHandle, (RF_Op *)&RFCMD_ieeeRX, RF_PriorityNormal, (RF_Callback)Radio_CallbackFunction, RF_EventRxBufFull | RF_EventLastCmdDone);
        }
    }

    return;
}

/*
 * === Radio_GetCurrentProtocol
 * Returns current protocol
 *
 * Parameters:
 *      N/A
 * Returns:
 *      RF_Protocol_t
 */
RF_Protocol_t Radio_GetCurrentProtocol()
{
    return CurrentRfProtocol;
}

/*
 * === Radio_SetCurrentProtocol
 * Sets current protocol
 *
 * Parameters:
 *      proto[in]           - RF protocol to set
 * Returns:
 *      N/A
 */
void Radio_SetCurrentProtocol(RF_Protocol_t proto)
{
    CurrentRfProtocol = proto;
}

/*
 * === Radio_StopRx
 * Stops RXing and returns Radio Core to initial state
 *
 * Parameters:
 *      N/A
 * Returns:
 *      N/A
 */
void Radio_StopRx()
{
    RF_flushCmd(rfHandle, RF_CMDHANDLE_FLUSH_ALL, 0);

    RF_ratDisableChannel(rfHandle, RF_RatChannel0);
    RF_ratDisableChannel(rfHandle, RF_RatChannel1);
    RF_ratDisableChannel(rfHandle, RF_RatChannel2);

    RF_close(rfHandle);

    RadioQueue_reset();

    return;
}

/*
 * === Radio_GetRxOk
 * Returns value of received RF frames from last RX start
 *
 * Parameters:
 *      N/A
 * Returns:
 *      uint16_t
 */
uint16_t Radio_GetRxOk(void)
{
    if (CurrentRfProtocol == BluetoothLowEnergy)
    {
        return bleStats.nRxOk;
    }

    if (CurrentRfProtocol == IEEE_802_15_4)
    {
        return ieeeStats.nRxData + ieeeStats.nRxBeacon + ieeeStats.nRxMacCmd;
    }

    return 0;
}
