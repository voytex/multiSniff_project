/*
 * log.c
 *
 *  Created on: 2. 10. 2023
 *      Author: Administrator
 */

#include <xdc/runtime/System.h>
#include "ti_radio_config.h"
#include "log.h"

#define LOG_ENABLED


/*
 * === Log_print
 * Prints (using System_printf) log message formatted by
 * passed LogEntity_t type.
 *
 * Parameters:
 *      pEntity[in]             - pointer to entity to be printed
 *      entityType[in]          - type of entity
 * Returns:
 *      N/A
 */
void Log_print(void* pEntity, LogEntity_t entityType)
{
#ifdef LOG_ENABLED

    RF_Stat* eStat = (RF_Stat*)pEntity;

    switch (entityType)
    {

    case RfStat:
        switch (*eStat)
        {
        case RF_StatSuccess:
            System_printf("<RF_Stat> Stat Succcess!\n");
            break;
        case RF_StatCmdDoneError:
            System_printf("<RF_Stat> Cmd Done, Error\n");
            break;
        case RF_StatCmdDoneSuccess:
            System_printf("<RF_Stat> Cmd Done, Success\n");
        default:
            System_printf("<RF_Stat> Code 0x%x\n", (uint8_t)(*eStat));
            break;
        }
        break;

    case CmdStatus:
        pEntity += 1 * sizeof(uint16_t);
        // todo: according to docs, insert


    }
#endif

    return;
}

