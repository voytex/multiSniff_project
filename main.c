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

#include <sniffing_task.h>

#include <source/utils/log.h>


// ==============================================================================================================


// === TASK CREATE FUNCTIONS ====================================================================================


/*
 * === Initialization task
 */
Task_Handle initTaskHandle;
Task_Params initTaskParams;

void Main_CreateInitTask()
{
    Task_Params_init(&initTaskParams);
    initTaskParams.stackSize = 1024;
    initTaskParams.priority  = 3;
    initTaskHandle = Task_create((Task_FuncPtr)Init_Main, &initTaskParams, Error_IGNORE);

    return;
}


/*
 * === Dashboard Task
 */
Task_Handle dashboardTaskHandle;
Task_Params dashboardTaskParams;

void Main_CreateDashboardTask()
{
    Task_Params_init(&dashboardTaskParams);
    dashboardTaskParams.stackSize = 2048;
    dashboardTaskParams.priority  = 2;
    dashboardTaskHandle = Task_create((Task_FuncPtr)Dashboard_Main, &dashboardTaskParams, Error_IGNORE);

    return;
}


/*
 * === Sniffing Task
 */
Task_Handle sniffingTaskHandle;
Task_Params sniffingTaskParams;

void Main_CreateSniffingTask()
{
    Task_Params_init(&sniffingTaskParams);
    sniffingTaskParams.stackSize = 1024;
    sniffingTaskParams.priority  = 2;
    sniffingTaskHandle = Task_create((Task_FuncPtr)Sniffing_Main, &sniffingTaskParams, Error_IGNORE);

    return;
}

// ==============================================================================================================


// === SEMAPHORE STRUCTS ========================================================================================

Semaphore_Handle Init_SemaphoreHandle;
Semaphore_Params Init_SemaphoreParams;
Semaphore_Handle Dashboard_SemaphoreHandle; // <== TODO rename?
Semaphore_Params Dashboard_SemaphoreParams;

// ==============================================================================================================


// === PROGRAM ENTRY POINT ======================================================================================´

int main()
{
    Board_init();

    Semaphore_Params_init(&Init_SemaphoreParams);
    Init_SemaphoreParams.mode = 0x1;
    Init_SemaphoreHandle = Semaphore_create(0, &Init_SemaphoreParams, NULL);

    Semaphore_Params_init(&Dashboard_SemaphoreParams);
    Dashboard_SemaphoreParams.mode = 0x1;
    Dashboard_SemaphoreHandle = Semaphore_create(0, &Dashboard_SemaphoreParams, NULL);

    Main_CreateInitTask();

    BIOS_start();

    return 0;
}

// ==============================================================================================================´
