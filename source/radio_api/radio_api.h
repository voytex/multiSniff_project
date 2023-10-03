/*
 * radio_api.h
 *
 *  Created on: 11. 9. 2023
 *      Author: Administrator
 */

#ifndef SOURCE_RADIO_API_RADIO_API_H_
#define SOURCE_RADIO_API_RADIO_API_H_

// === ENUM DEFINITIONS =========================================================================================

typedef enum RF_Protocol {
    BluetoothLowEnergy = 0,
    IEEE_802_15_4      = 1
} RF_Protocol_t;

// ==============================================================================================================

// === PUBLISHED FUNCTIONS ======================================================================================

void         Radio_openRadioCore           (RF_Params* pParams, RF_Object* pObj, RF_Protocol_t proto, RF_Handle pHandle);

RF_EventMask Radio_setFrequencySynthesizer (RF_Handle pHandle,  RF_Protocol_t proto);

void         Radio_initRXCmd               (RF_Protocol_t proto);

RF_CmdHandle Radio_beginRX                 (RF_Handle pHandle, RF_Protocol_t proto, void* callbackFunction, RF_EventMask events);

RF_Stat      Radio_stopRX                  (RF_Handle pHandle);

// ==============================================================================================================
#endif /* SOURCE_RADIO_API_RADIO_API_H_ */
