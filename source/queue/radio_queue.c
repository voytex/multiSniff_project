/*
 * Copyright (c) 2015-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The following code is nearly entirely made by TI. Author of Masters Project
 * edited names of functions to better match his intentions and naming schemes.
 * Author
 */

// === INCLUDES =================================================================================================

#include <ti/drivers/rf/RF.h>

#include DeviceFamily_constructPath(driverlib/rf_data_entry.h)

#include DeviceFamily_constructPath(driverlib/rf_mailbox.h)

#include "radio_queue.h"

// ==============================================================================================================


// === DEFINES ================================================================================================

#define RF_QUEUE_DE_HEADER_SIZE     8

#define RF_QUEUE_ALIGN_PADDING(length) (4-((length + RF_QUEUE_DE_HEADER_SIZE)%4))

#define RF_QUEUE_DE_BUFFER_SIZE(numEntries, dataSize, appendedBytes) \
    (numEntries*(RF_QUEUE_DE_HEADER_SIZE + dataSize + appendedBytes + RF_QUEUE_ALIGN_PADDING(dataSize + appendedBytes)))

#define MAX_LENGTH          2047

#define NUM_DE              3

#define NUM_APPENDED_BYTES  12

#define DE_CONFIG_SIZE      2

// ==============================================================================================================


// === STATIC VARIABLES =========================================================================================

static dataQueue_t RadioQueue_object;

static uint8_t RadioQueue_RXbuffer[RF_QUEUE_DE_BUFFER_SIZE(NUM_DE, MAX_LENGTH, NUM_APPENDED_BYTES)];

static rfc_dataEntryGeneral_t* RadioQueue_ReadEntry;

static inline uint16_t get16bitValue(uint8_t*);

// ==============================================================================================================


// === INTERNAL FUNCTIONS =======================================================================================

inline uint16_t get16bitValue(uint8_t* buffer)
{
    uint16_t value = 0;
    value |= (uint16_t)buffer[1] << 8;
    value |= (uint16_t)buffer[0];
    return value;
}

// ==============================================================================================================


// === PUBLISHED FUNCTIONS ======================================================================================

/*
 * === RadioQueue_init
 * TBD.
 *
 * Parameters:
 *      param[in]       - description
 * Returns:
 *      N/A
 *
 */
void RadioQueue_init(void)
{
    RadioQueue_create(&RadioQueue_object, RadioQueue_RXbuffer, sizeof(RadioQueue_RXbuffer), NUM_DE, MAX_LENGTH + NUM_APPENDED_BYTES);

    return;
}


/*
 * === RadioQueue_getDQpointer
 * TBD.
 *
 * Parameters:
 *      param[in]       - description
 * Returns:
 *      N/A
 *
 */
dataQueue_t* RadioQueue_getDQpointer(void)
{
    return &RadioQueue_object;
}


/*
 * === RadioQueue_create
 * TBD.
 *
 * Parameters:
 *      param[in]       - description
 * Returns:
 *      N/A
 *
 */
uint8_t RadioQueue_create(dataQueue_t* dataQueue, uint8_t* buf, uint16_t bufLength, uint8_t numEntries, uint16_t length)
{
    if (bufLength < (numEntries * (length + RF_QUEUE_DE_HEADER_SIZE + RF_QUEUE_ALIGN_PADDING(length))))
    {
        // queue wont fit into the buffer
        return 1;
    }

    // padding
    uint8_t pad = 4 - ((length + RF_QUEUE_DE_HEADER_SIZE) % 4);

    // configure each data entry
    uint8_t* firstEntry = buf;
    uint8_t i;
    for (i = 0; i < numEntries; i++)
    {
        buf = firstEntry + i * (RF_QUEUE_DE_HEADER_SIZE + length + pad);
        ((rfc_dataEntry_t*)buf)->status         = DATA_ENTRY_PENDING;
        ((rfc_dataEntry_t*)buf)->config.type    = DATA_ENTRY_TYPE_GEN;
        ((rfc_dataEntry_t*)buf)->config.lenSz   = DE_CONFIG_SIZE;
        ((rfc_dataEntry_t*)buf)->length         = length;

        ((rfc_dataEntryGeneral_t*)buf)->pNextEntry = &(((rfc_dataEntryGeneral_t*)buf)->data) + length + pad;
    }

    // Make circular Last.Next = first
    ((rfc_dataEntry_t*)buf)->pNextEntry = firstEntry;
    dataQueue->pCurrEntry = firstEntry;
    dataQueue->pLastEntry = NULL;

    // Set read pointer to first Entry
    RadioQueue_ReadEntry = (rfc_dataEntryGeneral_t*)firstEntry;

    return 0;
}


/*
 * === RadioQueue_hasPacket
 * TBD.
 *
 * Parameters:
 *      param[in]       - description
 * Returns:
 *      N/A
 *
 */
bool RadioQueue_hasPacket(void)
{
    return (RadioQueue_ReadEntry->status == DATA_ENTRY_FINISHED);
}


/*
 * === RadioQueue_reset
 * TBD.
 *
 * Parameters:
 *      param[in]       - description
 * Returns:
 *      N/A
 *
 */
void RadioQueue_reset(void)
{
    //TODO falls into error?
    uint8_t* firstEntry = RadioQueue_RXbuffer;

    // Set read pointer and DataQueue.CurrEntry both to firstEntry
    RadioQueue_object.pCurrEntry = firstEntry;
    RadioQueue_ReadEntry = (rfc_dataEntryGeneral_t*)firstEntry;

    // Reset status to all entries
    rfc_dataEntryGeneral_t* pEntry = RadioQueue_ReadEntry;
    uint8_t k;
    for (k = 0; k < NUM_DE; k++)
    {
        pEntry->status = DATA_ENTRY_PENDING;
        pEntry = (rfc_dataEntryGeneral_t*)pEntry->pNextEntry;
    }

    return;
}


/*
 * === RadioQueue_nextEntry
 * TBD.
 *
 * Parameters:
 *      param[in]       - description
 * Returns:
 *      N/A
 *
 */
void RadioQueue_nextEntry(void)
{
    RadioQueue_ReadEntry->status = DATA_ENTRY_PENDING;

    RadioQueue_ReadEntry = (rfc_dataEntryGeneral_t*)RadioQueue_ReadEntry->pNextEntry;

    return;
}


/*
 * === RadioQueue_takePacket
 * TBD.
 *
 * Parameters:
 *      param[in]       - description
 * Returns:
 *      N/A
 *
 */
uint16_t RadioQueue_takePacket(uint8_t* buffer, uint16_t maxlen)
{
    if (!RadioQueue_hasPacket()) return 0;

    uint16_t elementLength = get16bitValue(&RadioQueue_ReadEntry->data);

    // Discard packets larger than buffer
    if (elementLength <= maxlen)
    {
        memcpy(buffer, (uint8_t*)(&RadioQueue_ReadEntry->data + DE_CONFIG_SIZE), elementLength);
        // TODO 802.15.4g Packets special handling
    }

    RadioQueue_nextEntry();

    return elementLength;
}








