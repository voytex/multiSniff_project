/*
 * log.c
 *
 *  Created on: 2. 10. 2023
 *      Author: vojtechlukas
 */

// === INCLUDES =================================================================================================

#include "ti_radio_config.h"

#include <xdc/runtime/System.h>

#include <source/ethernet/IPAddress.h>

#include <source/utils/log.h>

// ==============================================================================================================

#define LOG_ENABLED

// === PUBLISHED FUNCTIONS ======================================================================================

/*
 * === Log_print
 * Prints (using System_printf) log message formatted by
 * passed-in LogEntity_t type.
 *
 * Parameters:
 *      msg[in]                 - message that gets printed before
 *                                actual diagnostics data
 *      pEntity[in]             - pointer to entity to be printed
 *                                (diagnostics data)
 *      entityType[in]          - type of entity
 * Returns:
 *      N/A
 */
void Log_print(const char pMsg[], void *pEntity, LogEntity_t entityType)
{
#ifdef LOG_ENABLED

    RF_Stat *eStat = (RF_Stat *)pEntity;
    uint16_t *status = (uint16_t *)pEntity + 1;
    uint64_t *events = (uint64_t *)pEntity;
    uint32_t *integer = (uint32_t *)pEntity;
    IPAddress *pIp = (IPAddress*)pEntity;

    if (pMsg != NULL)
    {
        System_printf("%s\n", pMsg);
    }

    switch (entityType)
    {

    case RfStat:
        System_printf("<RF_Stat> ");
        switch (*eStat)
        {
        case RF_StatSuccess:
            System_printf("Stat Succcess!\n");
            break;
        case RF_StatCmdDoneError:
            System_printf("Cmd Done, Error\n");
            break;
        case RF_StatCmdDoneSuccess:
            System_printf("Cmd Done, Success\n");
        default:
            System_printf("Code 0x%x\n", (uint8_t)(*eStat));
            break;
        }
        break;

    case CmdStatus:
        System_printf("<Op_Status_Code> ");
        switch (*status)
        {
        case 0x0:
            System_printf("IDLE - Not started\n");
            break;
        case 0x2:
            System_printf("ACTIVE - Running\n");
            break;
        case 0x400:
            System_printf("DONE_OK - Ended normally\n");
            break;
        case 0x404:
            System_printf("DONE_STOPPED - Stopped after CMD_STOP\n");
            break;
        case 0x405:
            System_printf("DONE_ABORT - Aborted by CMD_ABORT\n");
            break;
        case 0x1407:
            System_printf("BLE_DONE_ABORT - Aborted by FLUSHCMD\n");
            break;
        case 0x1408:
            System_printf("BLE_DONE_STOPPED - Operation stopped after STOP cmd\n");
            break;
        case 0x2402:
            System_printf("IEEE_DONE_STOPPED - Operation stopped after STOP cmd\n");
            break;
        default:
            System_printf("0x%x\n", *status);
            break;
        }
        break;

    case RfEvent:
        System_printf("<RF_Events> [0x%x]\n", *events);
        if (*events & RF_EventCmdDone)
            System_printf("   CmdDone\n");
        if (*events & RF_EventLastCmdDone)
            System_printf("   LastCmdDone\n");
        if (*events & RF_EventRxOk)
            System_printf("   RxOk\n");
        if (*events & RF_EventRxEntryDone)
            System_printf("   RxEntryDone\n");
        if (*events & RF_EventRxBufFull)
            System_printf("   RxBufFull\n");
        if (*events & RF_EventCmdCancelled)
            System_printf("   CmdCancelled\n");
        if (*events & RF_EventCmdStopped)
            System_printf("   CmdStopped\n");
        break;

    case Integer:
        System_printf("<Integer> %d\n", *integer);
        break;

    case Buffer:
        System_printf("<Buffer> %s\n", (const char *)pEntity);
        break;

    case None:
        break;

    case SSD1306:
        if (*integer == 0)
            System_printf("<SSD1306> Success\n");
        if (*integer == 1)
            System_printf("<SSD1306> Error\n");
        break;

    case IpAddress:
        System_printf("<IPAddress>: %d.%d.%d.%d\n", pIp->bytes[0], pIp->bytes[1], pIp->bytes[2], pIp->bytes[3]);
        break;

    default:
        System_printf("Unhandeled Log Event...\n");
        break;
    }

#endif /* LOG_ENABLED */

    return;
}
// =============================================================================================================
