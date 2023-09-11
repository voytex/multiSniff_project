/*
 * radio_api.c
 *
 *  Created on: 11. 9. 2023
 *      Author: Administrator
 */

// === INCLUDES =================================================================================================
#include "ti_drivers_config.h"

#include "ti_radio_config.h"

#include "radio_api.h"
// ==============================================================================================================


// === INTERNAL FUNCTIONS =======================================================================================

// ==============================================================================================================

/*
 * === Radio_openRadioCore
 * Initializes Radio Core of MCU according to parameters.
 *
 * Parameters:
 *      pParams[in]  - pointer to parameters structure
 *      pObj[in]     - pointer to object storing internal configuration
 *      pMode{in]    - pointer to global RF mode (e.g. BLE or IEEE)
 *      pSetup[in]   - pointer to radio setup structure
 *      pHandle[out] - pointer to RF_Handle to control Radio Core
 *
 */
void Radio_openRadioCore(RF_Params* pParams, RF_Object* pObj, RF_Mode* pMode, RF_RadioSetup* pSetup, RF_Handle pHandle)
{

}

/*
 * === Radio_setFrequencySynthesizer
 * Sets frequency synthesizer according in order to tune to a given
 * BLE or IEEE channel
 *
 * Parameters:
 *      pHandle[in]  - handle to Radio Core
 *      pFsCmd[in]   - pointer to Frequency Synthesizer Command
 * Returns:
 *      RF_EventMask - Event mask signaling status of the command
 *
 */
RF_EventMask Radio_setFrequencySynthesizer(RF_Handle pHandle,  RF_Op* pFsCmd)
{
    return RF_runCmd(pHandle, pFsCmd, RF_PriorityNormal, NULL, 0);
}


// === PUBLISHED FUNCTIONS ======================================================================================





// ==============================================================================================================
