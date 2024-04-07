/*
 * radio_proc.h
 *
 *  Created on: 14. 3. 2024
 *      Author: vojtechlukkas
 */

#ifndef SOURCE_RADIO_PROC_RADIO_PROC_H_
#define SOURCE_RADIO_PROC_RADIO_PROC_H_

// === INCLUDES =================================================================================================

#include "ti_radio_config.h"

// ==============================================================================================================


// === ENUM DEFS ================================================================================================

typedef enum RF_Protocol {
    BluetoothLowEnergy = 0,
    IEEE_802_15_4      = 1
} RF_Protocol_t;

// ==============================================================================================================

void Radio_SetUpAndBeginRx(RF_Protocol_t proto, uint8_t channel);

void Radio_CallbackFunction(RF_Handle rfHnd, RF_CmdHandle rfCmdHnd, RF_EventMask eventMsk);

RF_Protocol_t Radio_GetCurrentProtocol();

void Radio_SetCurrentProtocol(RF_Protocol_t proto);

uint16_t Radio_GetRxOk(void);

void Radio_StopRx(void);


#endif /* SOURCE_RADIO_PROC_RADIO_PROC_H_ */
