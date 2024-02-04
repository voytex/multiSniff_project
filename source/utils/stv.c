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
 * === STV_WriteStringAtAddress
 * Writes 8-bit values ('buf') starting at 'base'
 * continuing for 'len' bytes
 *
 * Parameters:
 *      base[in]     - memory address
 *      buf[in]      - pointer to the buffer to write
 *      len[in]      - length of the buffer
 * Returns:
 *      N/A
 */
inline void STV_WriteStringAtAddress(uint32_t base, uint8_t* buf, uint8_t len)
{
    uint8_t i;
    uint8_t* pDst = (uint8_t*)base;

    for ( i = 0; i < len; i++ )
    {
        pDst[i] = buf[i];
    }

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
    if (( STV_ReadFromAddress(STVW_USING_DHCP) == 0x0 ) && ( STV_ReadFromAddress(STVW_RF_PROTOCOL) == 0x0 ))
    {
        STV_WriteStringAtAddress(STVW_USING_DHCP, (uint8_t*)STVR_USING_DHCP, STV_SIZE - 6);
    }

    return;
}

// ==============================================================================================================
