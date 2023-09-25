/*
 *  ======== ti_radio_config.h ========
 *  Configured RadioConfig module definitions
 *
 *  DO NOT EDIT - This file is generated for the CC2652RB1FRGZ
 *  by the SysConfig tool.
 *
 *  Radio Config module version : 1.14
 *  SmartRF Studio data version : 2.27.0
 */
#ifndef _TI_RADIO_CONFIG_H_
#define _TI_RADIO_CONFIG_H_

#include <ti/devices/DeviceFamily.h>
#include DeviceFamily_constructPath(driverlib/rf_mailbox.h)
#include DeviceFamily_constructPath(driverlib/rf_common_cmd.h)
#include DeviceFamily_constructPath(driverlib/rf_ble_cmd.h)
#include DeviceFamily_constructPath(driverlib/rf_ieee_cmd.h)
#include <ti/drivers/rf/RF.h>

/* SmartRF Studio version that the RF data is fetched from */
#define SMARTRF_STUDIO_VERSION "2.27.0"

// *********************************************************************************
//   RF Frontend configuration
// *********************************************************************************
// RF design based on: LP_CC2652RB
#define LP_CC2652RB

// RF frontend configuration
#define FRONTEND_24G_DIFF_RF
#define FRONTEND_24G_INT_BIAS

// Supported frequency bands
#define SUPPORT_FREQBAND_2400

// TX power table size definitions
#define TXPOWERTABLE_2400_PA5_SIZE 16 // 2400 MHz, 5 dBm

// TX power tables
extern RF_TxPowerTable_Entry txPowerTable_2400_pa5[]; // 2400 MHz, 5 dBm



//*********************************************************************************
//  RF Setting:   BLE, 1 Mbps, LE 1M
//
//  PHY:          bt5le1m
//  Setting file: setting_bt5_le_1m.json
//*********************************************************************************

// PA table usage
#define BLE_TX_POWER_TABLE_SIZE TXPOWERTABLE_2400_PA5_SIZE

#define ble_txPowerTable txPowerTable_2400_pa5

// TI-RTOS RF Mode object
extern RF_Mode RFCMD_bleModeObject;

// RF Core API commands
extern rfc_CMD_BLE5_RADIO_SETUP_t RFCMD_bleRadioSetup;
extern rfc_CMD_FS_t RFCMD_bleFrequencySynthesizer;
extern rfc_CMD_BLE5_GENERIC_RX_t RFCMD_bleGenericRX;

// RF Core API overrides
extern uint32_t ble_pOverridesCommon[];
extern uint32_t ble_pOverrides1Mbps[];
extern uint32_t ble_pOverrides2Mbps[];
extern uint32_t ble_pOverridesCoded[];


//*********************************************************************************
//  RF Setting:   IEEE 802.15.4-2006, 250 kbps, OQPSK, DSSS = 1:8
//
//  PHY:          ieee154
//  Setting file: setting_ieee_802_15_4.json
//*********************************************************************************

// PA table usage
#define IEEE_TX_POWER_TABLE_SIZE TXPOWERTABLE_2400_PA5_SIZE

#define ieee_txPowerTable txPowerTable_2400_pa5

// TI-RTOS RF Mode object
extern RF_Mode RFCMD_ieeeModeObject;

// RF Core API commands
extern rfc_CMD_RADIO_SETUP_t RFCMD_ieeeRadioSetup;
extern rfc_CMD_FS_t RFCMD_ieeeFrequencySynthesizer;
extern rfc_CMD_IEEE_TX_t RFCMD_ieeeTX;
extern rfc_CMD_IEEE_RX_t RFCMD_ieeeRX;

// RF Core API overrides
extern uint32_t ieee_pOverrides[];

#endif // _TI_RADIO_CONFIG_H_
