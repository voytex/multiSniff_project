/*
 * Copyright (c) 2016-2020, Texas Instruments Incorporated
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
 */

/*
 *  ======== main_tirtos.c ========
 */
#include <source/radio_api/radio_api.h>
#include <stdint.h>

/*
 * Task.h for Task switching
 */
#include <ti/sysbios/knl/Task.h>

/* RTOS header files */
#include <ti/sysbios/BIOS.h>

#include <ti/drivers/Board.h>

#include "ti_drivers_config.h"

#include "ti_radio_config.h"



Task_Params testParams;
Task_Handle testHandle;

/* Stack size in bytes */
#define THREADSTACKSIZE 1024

/*
 *  ======== main ========
 */
int main(void)
{
    RF_Handle pHandle;
    RF_Object pObj;
    RF_Params pParams;

    Radio_openRadioCore(&pParams, &pObj, BluetoothLowEnergy, pHandle);

    Radio_initRXCmd(BluetoothLowEnergy);

    Radio_setFrequencySynthesizer(pHandle, BluetoothLowEnergy);

    Radio_beginRX(pHandle, BluetoothLowEnergy, NULL, 1);

    Radio_stopRX(pHandle);


    Task_Params_init(&testParams);
    testParams.priority = 2;


    BIOS_start();

    return (0);
}
