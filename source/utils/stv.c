/*
 * stv.c
 *
 *  Created on: 21. 1. 2024
 *      Author: vojtechlukas
 */

#include <source/utils/stv.h>

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
