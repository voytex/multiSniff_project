/*
 * radio_api.h
 *
 *  Created on: 11. 9. 2023
 *      Author: Administrator
 */

#ifndef RADIO_API_H_
#define RADIO_API_H_

// === PUBLISHED FUNCTIONS ======================================================================================

void Radio_openRadioCore(RF_Params* pParams, RF_Object* pObj, RF_Mode* pMode, RF_RadioSetup* pSetup, RF_Handle pHandle);

RF_EventMask Radio_setFrequencySynthesizer(RF_Handle pHandle,  RF_Op* pFsCmd);

#endif /* RADIO_API_H_ */
