/*
 * radio_proc.c
 *
 *  Created on: 14. 3. 2024
 *      Author: vojtechlukas
 */


// === INCLUDES =================================================================================================

#include <stdint.h>

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

RF_Object     rfObject;

RF_Params     rfParams;

RF_Handle     rfHandle;

RF_CmdHandle  rfCmdHandle;

rfc_bleGenericRxOutput_t bleStats;

rfc_ieeeRxOutput_t       ieeeStats;

const uint8_t accessAddress[] = {0xD6, 0xBE, 0x89, 0x8E};

uint32_t nRxBufFull;

// ==============================================================================================================


// === EXTERN VARS ==============================================================================================

extern EthernetUDP ethernetUdp;

// ==============================================================================================================


// === INTERNAL FUNCTIONS =======================================================================================

void initCmdObjects(uint8_t channel)
{
    nRxBufFull = 0;

    //RFCMD_bleGenericRX.status                             = 0x0;
    RFCMD_bleGenericRX.pParams->pRxQ                      = RadioQueue_getDQpointer(); //todo: rx queue
    RFCMD_bleGenericRX.pParams->rxConfig.bAutoFlushCrcErr = 1;
    RFCMD_bleGenericRX.pParams->rxConfig.bIncludeLenByte  = 1;
    RFCMD_bleGenericRX.pParams->rxConfig.bIncludeCrc      = 1;
    RFCMD_bleGenericRX.pParams->rxConfig.bAppendRssi      = 0;
    RFCMD_bleGenericRX.pParams->rxConfig.bAppendStatus    = 0;
    RFCMD_bleGenericRX.whitening.init                     = 0x65;
    RFCMD_bleGenericRX.pOutput                            = &bleStats; //todo: stats
    RFCMD_bleGenericRX.channel                            = channel;

    //RFCMD_bleFrequencySynthesizer.frequency

    //RFCMD_ieeeRX.status                                     = 0x0;
    RFCMD_ieeeRX.pRxQ                                     = RadioQueue_getDQpointer();
    RFCMD_ieeeRX.rxConfig.bAutoFlushCrc                   = 1;
    RFCMD_ieeeRX.rxConfig.bAutoFlushIgn                   = 0;
    RFCMD_ieeeRX.rxConfig.bIncludePhyHdr                  = 0;
    RFCMD_ieeeRX.rxConfig.bIncludeCrc                     = 1;
    RFCMD_ieeeRX.rxConfig.bAppendRssi                     = 0;
    RFCMD_ieeeRX.rxConfig.bAppendCorrCrc                  = 0;
    RFCMD_ieeeRX.rxConfig.bAppendSrcInd                   = 0;
    RFCMD_ieeeRX.rxConfig.bAppendTimestamp                = 0;
    RFCMD_ieeeRX.pOutput                                  = &ieeeStats;
    RFCMD_ieeeRX.channel                                  = channel;

    //RFCMD_ieeeFrequencySynthesizer.frequency

    return;
}

// ==============================================================================================================


// === FUNCTION DEFINITIONS =====================================================================================

void Radio_SetUpAndBeginRx(RF_Protocol_t proto, uint8_t channel)
{
    RF_Params_init(&rfParams);

    initCmdObjects(channel);

    if (proto == BluetoothLowEnergy)
    {
        rfHandle = RF_open(&rfObject, &RFCMD_bleModeObject, (RF_RadioSetup*)&RFCMD_bleRadioSetup, &rfParams);

        RF_postCmd(rfHandle, (RF_Op*)&RFCMD_bleFrequencySynthesizer, RF_PriorityNormal, NULL, 0);

        rfCmdHandle = RF_postCmd(rfHandle, (RF_Op*)&RFCMD_bleGenericRX, RF_PriorityNormal, (RF_Callback)Radio_HandlerFunc, RF_EventRxBufFull | RF_EventLastCmdDone);
    }

    if (proto == IEEE_802_15_4)
    {
        rfHandle = RF_open(&rfObject, &RFCMD_ieeeModeObject, (RF_RadioSetup*)&RFCMD_ieeeRadioSetup, &rfParams);

        RF_postCmd(rfHandle, (RF_Op*)&RFCMD_ieeeFrequencySynthesizer, RF_PriorityNormal, NULL, 0);

        rfCmdHandle = RF_postCmd(rfHandle, (RF_Op*)&RFCMD_ieeeRX, RF_PriorityNormal, (RF_Callback)Radio_HandlerFunc, RF_EventRxBufFull | RF_EventLastCmdDone);
    }

    return;
}


void Radio_HandlerFunc(RF_Handle rfHnd, RF_CmdHandle rfCmdHnd, RF_EventMask eventMsk)
{

    if (eventMsk & RF_EventRxBufFull)
    {
        nRxBufFull++;

        RadioQueue_reset();

        if (CurrentRfProtocol == BluetoothLowEnergy)
        {
            rfCmdHandle = RF_postCmd(rfHandle, (RF_Op*)&RFCMD_bleGenericRX, RF_PriorityNormal, (RF_Callback)Radio_HandlerFunc, RF_EventRxEntryDone | RF_EventRxBufFull | RF_EventLastCmdDone);
        }

        if (CurrentRfProtocol == IEEE_802_15_4)
        {
            rfCmdHandle = RF_postCmd(rfHandle, (RF_Op*)&RFCMD_ieeeRX, RF_PriorityNormal, (RF_Callback)Radio_HandlerFunc, RF_EventRxEntryDone | RF_EventRxBufFull | RF_EventLastCmdDone);
        }
    }

    return;
}

RF_Protocol_t Radio_GetCurrentProtocol()
{
    return CurrentRfProtocol;
}

void Radio_StopRx()
{
    RF_flushCmd(rfHandle, RF_CMDHANDLE_FLUSH_ALL, 0);

    RF_ratDisableChannel(rfHandle, RF_RatChannel0);
    RF_ratDisableChannel(rfHandle, RF_RatChannel1);
    RF_ratDisableChannel(rfHandle, RF_RatChannel2);

    RF_close(rfHandle);

    RadioQueue_reset();
}
