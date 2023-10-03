/*
 * log.h
 *
 *  Created on: 2. 10. 2023
 *      Author: Administrator
 */

#ifndef SOURCE_LOG_LOG_H_
#define SOURCE_LOG_LOG_H_

// === ENUM DEFINITIONS =========================================================================================

typedef enum LogEntity {
    RfStat,
    CmdStatus,
    RfEvent,
    Integer,
    Buffer
} LogEntity_t;

// ==============================================================================================================

// === PUBLISHED FUNCTIONS ======================================================================================

void Log_print(const char pMsg[], void* pEntity, LogEntity_t entityType);

// ==============================================================================================================

#endif /* SOURCE_LOG_LOG_H_ */
