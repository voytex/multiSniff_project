/*
 * main.c
 *
 *  Created on: 12. 12. 2023
 *      Author: vojtechlukas
 */

// === INCLUDES =================================================================================================

#include <stdint.h>

#include <xdc/std.h>

#include <xdc/runtime/System.h>

#include <xdc/runtime/Memory.h>

#include <xdc/runtime/Error.h>

#include <ti/sysbios/knl/Task.h>

#include <ti/sysbios/BIOS.h>

#include <ti/drivers/Board.h>

#include <ti/sysbios/knl/Semaphore.h>

#include "ti_drivers_config.h"

#include "ti_radio_config.h"

#include <ti/drivers/GPIO.h>

#include <ti/drivers/I2C.h>

#include <source/radio_api/radio_api.h>

#include <source/oled_gui/gui.h>

#include <source/ethernet/Ethernet.h>

#include <dashboard_task.h>

#include <source/html/html.h>

#include <dashboard_task.h>

#include <init_task.h>
#include <source/utils/log.h>

// ==============================================================================================================


// === DEFINES ==================================================================================================

#define MAC_ADDRESS_STV     (0x50000)
#define DVC_IP_ADD_STV      (MAC_ADDRESS_STV + 6)
#define TGT_IP_ADD_STV      (DVC_IP_ADD_STV + 4)

// ==============================================================================================================


// === INTERNAL FUNCTIONS =======================================================================================



// ==============================================================================================================
Task_Params dashboardTaskParams;
Task_Params initTaskParams;
Semaphore_Handle Init_SemaphoreHandle;


int main()
{
    Board_init();



    Task_Handle initTaskHandle;
    Task_Handle dashboardTaskHandle;

    Init_SemaphoreHandle = SemaphoreP_createBinary(0);

    Task_Params_init(&initTaskParams);
    initTaskParams.stackSize = 1024;
    initTaskParams.priority  = 3;
    initTaskHandle = Task_create((Task_FuncPtr)Init_Main, &initTaskParams, Error_IGNORE);

    Task_Params_init(&dashboardTaskParams);
    dashboardTaskParams.stackSize = 1024;
    dashboardTaskParams.priority  = 2;
    dashboardTaskHandle = Task_create((Task_FuncPtr)Dashboard_Main, &dashboardTaskParams, Error_IGNORE);

    BIOS_start();

    return 0;
}
