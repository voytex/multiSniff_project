/*
 * radio_api.h
 *
 *  Created on: 11. 9. 2023
 *      Author: vojtechlukas
 */

#ifndef RADIO_API_H_
#define RADIO_API_H_

#include <source/ethernet/EthernetUdp.h>

#include "ti_radio_config.h"

// === ENUM DEFINITIONS =========================================================================================

typedef enum RF_Protocol {
    BluetoothLowEnergy = 0,
    IEEE_802_15_4      = 1
} RF_Protocol_t;

// ==============================================================================================================

// === PUBLISHED FUNCTIONS ======================================================================================

void          Radio_openRadioCore           (RF_Params* pParams, RF_Object* pObj, RF_Protocol_t proto, RF_Handle* pHandle);

RF_EventMask  Radio_setFrequencySynthesizer (RF_Handle pHandle,  RF_Protocol_t proto);

void          Radio_initRXCmd               (rfc_bleGenericRxOutput_t*, rfc_ieeeRxOutput_t*);

RF_CmdHandle  Radio_beginRX                 (RF_Handle pHandle, RF_Protocol_t proto, void* callbackFunction, RF_EventMask events);

RF_Stat       Radio_stopRX                  (RF_Handle pHandle);

RF_Protocol_t Radio_GetCurrentProtocol      (void);

void          Radio_HandleQueueOverflow     (RF_Handle rfHnd, RF_CmdHandle rfCmdHnd, RF_EventMask eventMsk);

// ==============================================================================================================

#endif /* RADIO_API_H_ */
