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


// === FUNCTION DEFINITIONS ====================================================================================

/*
 * === STV_ReadFromAddress
 * Returns 8-bit byte from given address
 *
 * Parameters:
 *      address[in]  - memory address
 * Returns:
 *      uint8_t      - value at given address
 */
inline uint8_t STV_ReadFromAddress(uint32_t address)
{
    return (uint8_t)(*((uint32_t*)address));
}


/*
 * === STV_WriteAtAddress
 * Writes 8-bit value to a given address
 *
 * Parameters:
 *      address[in]  - memory address
 *      value[in]    - value to write
 * Returns:
 *      N/A
 */
inline void STV_WriteAtAddress(uint32_t address, uint8_t value)
{
    uint8_t* pAddress = (uint8_t*)address;

    *pAddress = value;

    return;
}


/*
 * === STV_CopyStvFromFlashIfNotYet
 * If RAM region storing STVW values is empty,
 * this functions copies STVs from Flash to RAM.
 *
 * Parameters:
 *      N/A
 * Returns:
 *      N/A
 */
inline void STV_CopyStvFromFlashIfNotYet()
{
    uint32_t i;

    if ( STV_ReadFromAddress(STVW_USING_DHCP) == 0x0 )
    {
        for ( i = 0; i < 10; i++)
        {
            STV_WriteAtAddress(STVW_DEVICE_IP_ADDRESS + i, STV_ReadFromAddress(STVR_DEVICE_IP_ADDRESS + i));
        }
    }

    return;
}

// ==============================================================================================================

#endif /* SOURCE_UTILS_STV_H_ */
