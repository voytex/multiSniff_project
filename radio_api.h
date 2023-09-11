/*
 * radio_api.h
 *
 *  Created on: 11. 9. 2023
 *      Author: Administrator
 */

#ifndef RADIO_API_H_
#define RADIO_API_H_

// === ENUM DEFINITIONS =========================================================================================

typedef enum RF_Protocol {
    BluetoothLowEnergy = 0,
    IEEE_802_15_4      = 1
} RF_Protocol_t;

// ==============================================================================================================

// === PUBLISHED FUNCTIONS ======================================================================================

void         Radio_openRadioCore           (RF_Params* pParams, RF_Object* pObj, RF_Mode* pMode, RF_RadioSetup* pSetup, RF_Handle pHandle);

RF_EventMask Radio_setFrequencySynthesizer (RF_Handle pHandle,  RF_Op* pFsCmd);

void         Radio_initRXCmd               (void* pRXCmd, RF_Protocol_t proto);

RF_CmdHandle Radio_beginRX                 (RF_Handle* pHandle, RF_Op* pRXCmd, void* callbackFunction, RF_EventMask event);
// ==============================================================================================================
#endif /* RADIO_API_H_ */
