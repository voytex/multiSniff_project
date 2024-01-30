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

void STV_CopyStvFromFlashIfNotYet();

// ==============================================================================================================


// === DEFINES ==================================================================================================

#define STVR_MAC_ADDRESS         (0x50000)

#define STVR_DEVICE_IP_ADDRESS   (0x50006)

#define STVR_TARGET_IP_ADDRESS   (0x5000A)

#define STVR_USING_DHCP          (0x5000E)

#define STV_DHCP_TRUE            (0x59)

#define STV_DHCP_FALSE           (0x4E)

#define STVR_RF_PROTOCOL         (0x5000F)

#define STV_RF_PROTO_BLE         (0xB5)

#define STV_RF_PROTO_IEEE        (0x15)

#define STVW_DEVICE_IP_ADDRESS   (0x20013500)

#define STVW_TARGET_IP_ADDRESS   (0x20013504)

#define STVW_USING_DHCP          (0x20013508)

#define STVW_RF_PROTOCOL         (0x20013509)

// ==============================================================================================================



#endif /* SOURCE_UTILS_STV_H_ */
