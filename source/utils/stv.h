/*
 * stv.h
 *
 *  Created on: 20. 1. 2024
 *      Author: vojtechlukas
 */

#ifndef SOURCE_UTILS_STV_H_
#define SOURCE_UTILS_STV_H_

// === INCLUDES =================================================================================================

#include <stdint.h>

// ==============================================================================================================


// === PUBLISHED FUNCTIONS ======================================================================================

uint8_t STV_ReadFromAddress(uint32_t address);

void STV_WriteAtAddress(uint32_t, uint8_t);

void STV_WriteStringAtAddress(uint32_t, uint8_t*, uint8_t);

void STV_CopyStvFromFlashIfNotYet();

// ==============================================================================================================


// === DEFINES ==================================================================================================

//
// For more information, see
// 'status_vector_map.md'
//

#define STVR_MAC_ADDRESS         (0x50000)

#define STVR_USING_DHCP          (0x50006)

#define STV_DHCP_TRUE            (0x59)

#define STV_DHCP_FALSE           (0x4E)

#define STVR_DEVICE_IP_ADDRESS   (0x50007)

#define STVR_GATEWAY_IP_ADDRESS  (0x5000B)

#define STVR_NETWORK_MASK        (0x5000F)

#define STVR_TARGET_IP_ADDRESS   (0x50010)

#define STVR_RF_PROTOCOL         (0x50014)

#define STVR_RF_CHANNEL          (0x50015)

#define STV_RF_PROTO_BLE         (0xB5)

#define STV_RF_PROTO_IEEE        (0x15)

#define STVR_RUNNING_STATUS      (0x50016)

#define STVW_USING_DHCP          (0x20013500)

#define STVW_DEVICE_IP_ADDRESS   (0x20013501)

#define STVW_GATEWAY_IP_ADDRESS  (0x20013505)

#define STVW_NETWORK_MASK        (0x20013509)

#define STVW_TARGET_IP_ADDRESS   (0x2001350D)

#define STVW_RF_PROTOCOL         (0x20013511)

#define STVW_RF_CHANNEL          (0x20013512)

#define STVW_RUNNING_STATUS      (0x20013513)

#define STVW_SIGNAL_RF_CHANGE    (0x20013514)

#define STV_SIZE                 (26)

// ==============================================================================================================



#endif /* SOURCE_UTILS_STV_H_ */
