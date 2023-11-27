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
#include <stdint.h>

#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/Error.h>
/*
 * Task.h for Task switching
 */
#include <ti/sysbios/knl/Task.h>

/* RTOS header files */
#include <ti/sysbios/BIOS.h>

#include <ti/drivers/Board.h>

#include <ti/sysbios/knl/Semaphore.h>

#include "ti_drivers_config.h"

#include "ti_radio_config.h"

#include <source/log/log.h>

#include <ti/drivers/GPIO.h>

#include <ti/drivers/I2C.h>

#include <source/radio_api/radio_api.h>

#include <source/driverlib/ssd1306/ssd1306.h>

#include <source/oled_gui/gui.h>

Task_Params testParams;
Task_Handle testHandle;

/* Stack size in bytes */
#define THREADSTACKSIZE 1024

/*
 *  ======== main ========
 */

void testTask(UArg a0, UArg a1)
{
       RF_Handle pHandle;
       RF_Object pObj;
       RF_Params pParams;

       // BLE ========

       Radio_openRadioCore(&pParams, &pObj, BluetoothLowEnergy, &pHandle);

       Radio_initRXCmd(BluetoothLowEnergy);

       Radio_setFrequencySynthesizer(pHandle, BluetoothLowEnergy);

       Radio_beginRX(pHandle, BluetoothLowEnergy, NULL, 0);

       Radio_stopRX(pHandle);

       // IEEE =======

       Radio_openRadioCore(&pParams, &pObj, IEEE_802_15_4, &pHandle);

       Radio_initRXCmd(IEEE_802_15_4);

       Radio_setFrequencySynthesizer(pHandle, IEEE_802_15_4);

       Radio_beginRX(pHandle, IEEE_802_15_4, NULL, 0);

       Radio_stopRX(pHandle);

       // BLE ========

       Radio_openRadioCore(&pParams, &pObj, BluetoothLowEnergy, &pHandle);

       Radio_initRXCmd(BluetoothLowEnergy);

       Radio_setFrequencySynthesizer(pHandle, BluetoothLowEnergy);

       Radio_beginRX(pHandle, BluetoothLowEnergy, NULL, 0);

       Radio_stopRX(pHandle);


}

uint8_t _send(I2C_Handle i2c, uint8_t data)
{
    uint8_t buf[2];
    buf[0] = 0;
    buf[1] = data;

    I2C_Transaction txn;
    txn.writeBuf = (void*)buf;
    txn.writeCount = 2;
    txn.readCount = 0;
    txn.readBuf = NULL;
    txn.slaveAddress = SSD1306_ADDR;
    return I2C_transfer(i2c, &txn);
}


void displayTask(UArg a0, UArg a1)
{
//    GPIO_init();
//    I2C_init();
//    I2C_Handle i2c;
//    I2C_Params i2cParams;
//    I2C_Params_init(&i2cParams);
//    i2cParams.bitRate = I2C_400kHz;
//    i2cParams.transferMode = I2C_MODE_BLOCKING;
//    i2c = I2C_open(CONFIG_I2C, &i2cParams);
//    if ( i2c == NULL ) Log_print("Failed to open I2C", NULL, None);
//
//    const uint8_t SSD1306_InitSequence[] = {
//            SSD1306_DISPLAY_OFF, SSD1306_DISPLAY_ON,
//            SSD1306_DISPLAY_OFF, SSD1306_DISPLAY_ON,// 0xAE = Set Display OFF
//            SSD1306_SET_MUX_RATIO, 63,                                   // 0xA8 - 64MUX for 128 x 64 version
//                                                                         //      - 32MUX for 128 x 32 version
//            SSD1306_MEMORY_ADDR_MODE, 0x00,                              // 0x20 = Set Memory Addressing Mode
//                                                                         // 0x00 - Horizontal Addressing Mode
//                                                                         // 0x01 - Vertical Addressing Mode
//                                                                         // 0x02 - Page Addressing Mode (RESET)
//            SSD1306_SET_COLUMN_ADDR, START_COLUMN_ADDR, END_COLUMN_ADDR, // 0x21 = Set Column Address, 0 - 127
//            SSD1306_SET_PAGE_ADDR, START_PAGE_ADDR, END_PAGE_ADDR,       // 0x22 = Set Page Address, 0 - 7
//            SSD1306_SET_START_LINE,                                      // 0x40
//            SSD1306_DISPLAY_OFFSET, 0x00,                                // 0xD3
//            SSD1306_SEG_REMAP_OP,                                        // 0xA0 / remap 0xA1
//            SSD1306_COM_SCAN_DIR_OP,                                     // 0xC0 / remap 0xC8
//            SSD1306_COM_PIN_CONF, 0x12,                                  // 0xDA, 0x12 - Disable COM Left/Right remap, Alternative COM pin configuration
//                                                                            //       0x12 - for 128 x 64 version
//                                                                            //       0x02 - for 128 x 32 version
//            SSD1306_SET_CONTRAST, 0x7F,                                  // 0x81, 0x7F - reset value (max 0xFF)
//            SSD1306_DIS_ENT_DISP_ON,                                     // 0xA4
//            SSD1306_DIS_NORMAL,                                          // 0xA6
//            SSD1306_SET_OSC_FREQ, 0x80,                                  // 0xD5, 0x80 => D=1; DCLK = Fosc / D <=> DCLK = Fosc
//            SSD1306_SET_PRECHARGE, 0xc2,                                 // 0xD9, higher value less blinking
//                                                                            // 0xC2, 1st phase = 2 DCLK,  2nd phase = 13 DCLK
//            SSD1306_VCOM_DESELECT, 0x20,                                 // Set V COMH Deselect, reset value 0x22 = 0,77xUcc
//            SSD1306_SET_CHAR_REG, 0x14,                                  // 0x8D, Enable charge pump during display on
//            SSD1306_DISPLAY_ON                                           // 0xAF = Set Display ON
//    };
//
//
//    uint8_t i;
//    uint32_t res;
//    for ( i = 0; i < sizeof(SSD1306_InitSequence); i++)
//    {
//        res = send(i2c, SSD1306_InitSequence[i]);
//        Log_print("I2C Transaction result:", &res, Integer);
//    }
//
//    res = send(i2c, 0x40);
//    Log_print("I2C data stream:", &res, Integer);
//    uint8_t buffer[1032];
//    buffer[0] = 0x40;
//    memset(buffer + 1, 127, 1031);
//
//    I2C_Transaction txn;
//    txn.writeBuf = buffer;
//    txn.writeCount = 1032;
//    txn.readCount = 0;
//    txn.readBuf = NULL;
//    txn.slaveAddress = SSD1306_ADDR;
//    res = I2C_transfer(i2c, &txn);
//    Log_print("I2C Draw", &res, Integer);

    GUI_Init();

    GUI_ChangeDeviceIp("192.168.1.1");

    GUI_ChangeTargetIp("192.168.1.2");

    GUI_ChangeProto(0);

    GUI_ChangeRx(false);

    GUI_ChangeChannel(27);

}


int main(void)
{
    Task_Params_init(&testParams);
    testParams.stackSize = 2048;
    testHandle = Task_create((Task_FuncPtr)displayTask, &testParams, NULL);

    BIOS_start();

    return (0);
}
