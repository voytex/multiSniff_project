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


// === PUBLISHED FUNCTIONS ======================================================================================

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
 * Returns:
 *      N/A
 *
 */
void Radio_openRadioCore(RF_Params* pParams, RF_Object* pObj, RF_Mode* pMode, RF_RadioSetup* pSetup, RF_Handle pHandle)
{
    RF_Params_init(pParams);

    pHandle = RF_open(pObj, pMode, pSetup, pParams);

    return;
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


/*
 * === Radio_initRXCmd
 * Initializes RX commands of given protocol for sniffing.
 *
 * Parameters:
 *      pRXCmd[in,out]  - pointer to Command struct (BLE/IEEE)
 *      proto[in]       - protocol to determine typecasting
 * Returns:
 *      N/A
 */
void Radio_initRXCmd(void* pRXCmd, RF_Protocol_t proto)
{
    if ( proto == BluetoothLowEnergy )
    {
        ((rfc_CMD_BLE5_GENERIC_RX_t*)pRXCmd)->pParams->pRxQ                      = NULL; //todo: rx queue
        ((rfc_CMD_BLE5_GENERIC_RX_t*)pRXCmd)->pParams->rxConfig.bAutoFlushCrcErr = 1;
        ((rfc_CMD_BLE5_GENERIC_RX_t*)pRXCmd)->pParams->rxConfig.bIncludeLenByte  = 1;
        ((rfc_CMD_BLE5_GENERIC_RX_t*)pRXCmd)->pParams->rxConfig.bIncludeCrc      = 1;
        ((rfc_CMD_BLE5_GENERIC_RX_t*)pRXCmd)->pParams->rxConfig.bAppendRssi      = 0;
        ((rfc_CMD_BLE5_GENERIC_RX_t*)pRXCmd)->pParams->rxConfig.bAppendStatus    = 0;
        ((rfc_CMD_BLE5_GENERIC_RX_t*)pRXCmd)->whitening.init                     = 0x65;
        ((rfc_CMD_BLE5_GENERIC_RX_t*)pRXCmd)->pOutput                            = NULL; //todo: stats
        ((rfc_CMD_BLE5_GENERIC_RX_t*)pRXCmd)->channel                            = 0x66; //todo: initial channel?
    }

    if ( proto == IEEE_802_15_4 )
    {
        ((rfc_CMD_IEEE_RX_t*)pRXCmd)->pRxQ                                       = NULL; //todo rx queue
        ((rfc_CMD_IEEE_RX_t*)pRXCmd)->rxConfig.bAutoFlushCrc                     = 1;
        ((rfc_CMD_IEEE_RX_t*)pRXCmd)->rxConfig.bAutoFlushIgn                     = 0;
        ((rfc_CMD_IEEE_RX_t*)pRXCmd)->rxConfig.bIncludePhyHdr                    = 0;
        ((rfc_CMD_IEEE_RX_t*)pRXCmd)->rxConfig.bIncludeCrc                       = 1;
        ((rfc_CMD_IEEE_RX_t*)pRXCmd)->rxConfig.bAppendRssi                       = 0;
        ((rfc_CMD_IEEE_RX_t*)pRXCmd)->rxConfig.bAppendCorrCrc                    = 0;
        ((rfc_CMD_IEEE_RX_t*)pRXCmd)->rxConfig.bAppendSrcInd                     = 0;
        ((rfc_CMD_IEEE_RX_t*)pRXCmd)->rxConfig.bAppendTimestamp                  = 0;
        ((rfc_CMD_IEEE_RX_t*)pRXCmd)->pOutput                                    = NULL; //todo: stats¨
        ((rfc_CMD_IEEE_RX_t*)pRXCmd)->channel                                    = 0;
    }

    return;
}




// ==============================================================================================================
