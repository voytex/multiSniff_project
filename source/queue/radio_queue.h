/*
 * radio_queue.h
 *
 *  Created on: 17. 11. 2022
 *      Author: vojtechlukas
 */

#ifndef RADIO_QUEUE_H_
#define RADIO_QUEUE_H_

#include DeviceFamily_constructPath(driverlib/rf_mailbox.h)

// === PUBLISHED FUNCTIONS ======================================================================================

uint8_t RadioQueue_create(dataQueue_t *dataQueue, uint8_t *buf, uint16_t bufLength, uint8_t numEntries, uint16_t length);

void RadioQueue_init(void);

dataQueue_t *RadioQueue_getDQpointer(void);

bool RadioQueue_hasPacket(void);

void RadioQueue_reset(void);

uint16_t RadioQueue_takePacket(uint8_t *buffer, uint16_t maxlen);

void RadioQueue_nextEntry(void);

// ==============================================================================================================

#endif /* RADIO_QUEUE_H_ */
