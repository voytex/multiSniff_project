/*
 * log.h
 *
 *  Created on: 2. 10. 2023
 *      Author: Administrator
 */

#ifndef LOG_H_
#define LOG_H_

typedef enum LogEntity {
    RfStat,
    CmdStatus
} LogEntity_t;

void Log_print(void* pEntity, LogEntity_t entityType);

#endif /* LOG_H_ */
