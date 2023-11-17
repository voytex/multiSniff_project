/*
 * ssd1306.c
 *
 *  Created on: 8. 11. 2023
 *      Author: vojtechlukas
 *
 *      Following code is heavily inspired by SSD1306 driver written by Marian Hrinko
 *      (mato.hrinko@gmail.com)
 *
 */

// === INCLUDES =================================================================================================

#include <unistd.h>

#include <xdc/runtime/Memory.h>

#include <ti/drivers/I2C.h>

#include "ti_drivers_config.h"

#include <source/driverlib/ssd1306/ssd1306.h>

// ==============================================================================================================

// === GLOBAL VARIABLES =========================================================================================

const uint8_t SSD1306_InitSequence[] = {
        SSD1306_DISPLAY_OFF,                                         // 0xAE = Set Display OFF
        SSD1306_DISPLAY_OFF,                                         // 0xAE = Set Display OFF (twice because first command always got damaged)
        SSD1306_SET_MUX_RATIO, 63,                                   // 0xA8 - 64MUX for 128 x 64 version
                                                                     //      - 32MUX for 128 x 32 version
        SSD1306_MEMORY_ADDR_MODE, 0x00,                              // 0x20 = Set Memory Addressing Mode
                                                                     // 0x00 - Horizontal Addressing Mode
                                                                     // 0x01 - Vertical Addressing Mode
                                                                     // 0x02 - Page Addressing Mode (RESET)
        SSD1306_SET_COLUMN_ADDR, START_COLUMN_ADDR, END_COLUMN_ADDR, // 0x21 = Set Column Address, 0 - 127
        SSD1306_SET_PAGE_ADDR, START_PAGE_ADDR, END_PAGE_ADDR,       // 0x22 = Set Page Address, 0 - 7
        SSD1306_SET_START_LINE,                                      // 0x40
        SSD1306_DISPLAY_OFFSET, 0x00,                                // 0xD3
        SSD1306_SEG_REMAP_OP,                                        // 0xA0 / remap 0xA1
        SSD1306_COM_SCAN_DIR_OP,                                     // 0xC0 / remap 0xC8
        SSD1306_COM_PIN_CONF, 0x12,                                  // 0xDA, 0x12 - Disable COM Left/Right remap, Alternative COM pin configuration
                                                                     //       0x12 - for 128 x 64 version
                                                                     //       0x02 - for 128 x 32 version
        SSD1306_SET_CONTRAST, 0x7F,                                  // 0x81, 0x7F - reset value (max 0xFF)
        SSD1306_DIS_ENT_DISP_ON,                                     // 0xA4
        SSD1306_DIS_NORMAL,                                          // 0xA6
        SSD1306_SET_OSC_FREQ, 0x80,                                  // 0xD5, 0x80 => D=1; DCLK = Fosc / D <=> DCLK = Fosc
        SSD1306_SET_PRECHARGE, 0xc2,                                 // 0xD9, higher value less blinking
                                                                     // 0xC2, 1st phase = 2 DCLK,  2nd phase = 13 DCLK
        SSD1306_VCOM_DESELECT, 0x20,                                 // Set V COMH Deselect, reset value 0x22 = 0,77xUcc
        SSD1306_SET_CHAR_REG, 0x14,                                  // 0x8D, Enable charge pump during display on
        SSD1306_DISPLAY_ON                                           // 0xAF = Set Display ON
};

static char SSD1306_LcdBufferInclStreamByte[CACHE_SIZE_MEM + 1];
const char *SSD1306_Lcd = SSD1306_LcdBufferInclStreamByte + 1;


static I2C_Params _i2cParams;

static I2C_Handle _i2c;

// ==============================================================================================================


// === INTERNAL FUNCTIONS =======================================================================================

/*
 * === Template
 * Lorem Ipsum Dolor sit amet.
 *
 * Parameters:
 *      pParams[in]  - pointer to parameters structure
 *      pObj[in]     - pointer to object storing internal configuration
 *      proto[in     - RF protocol to which Radio Core should be opened
 *      pHandle[out] - pointer to RF_Handle to control Radio Core
 * Returns:
 *      N/A
 *
 */
uint8_t sendData(uint8_t address, void *data, uint8_t count)
{
    I2C_Transaction txn;

    txn.writeBuf      = data;
    txn.writeCount    = count;
    txn.readCount     = 0;
    txn.readBuf       = NULL;
    txn.slaveAddress  = address;

    return I2C_transfer(_i2c, &txn);
}

// ==============================================================================================================

// === FUNCTION DEFINITIONS =====================================================================================

/*
 * === Template
 * Lorem Ipsum Dolor sit amet.
 *
 * Parameters:
 *      pParams[in]  - pointer to parameters structure
 *      pObj[in]     - pointer to object storing internal configuration
 *      proto[in     - RF protocol to which Radio Core should be opened
 *      pHandle[out] - pointer to RF_Handle to control Radio Core
 * Returns:
 *      N/A
 *
 */
uint8_t SSD1306_Init(uint8_t address)
{
    SSD1306_LcdBufferInclStreamByte[0] = 0x40;

    uint8_t i;

    uint8_t status = INIT_STATUS;

    //
    // I2C Initialization
    //
    I2C_init();

    I2C_Params_init(&_i2cParams);

    _i2cParams.bitRate = I2C_400kHz;

    _i2cParams.transferMode = I2C_MODE_BLOCKING;

    _i2c = I2C_open(CONFIG_I2C, &_i2cParams);

    if ( _i2c == NULL )
    {
        return I2C_ERROR;
    }

    for ( i = 0; i < sizeof(SSD1306_InitSequence); i++)
    {
        status = SSD1306_SendCommand(SSD1306_InitSequence[i]);

//        if ( status != SSD1306_SUCCESS )
//        {
//            return status;
//        }
    }

    return SSD1306_SUCCESS;
}


/*
 * === Template
 * Lorem Ipsum Dolor sit amet.
 *
 * Parameters:
 *      pParams[in]  - pointer to parameters structure
 *      pObj[in]     - pointer to object storing internal configuration
 *      proto[in     - RF protocol to which Radio Core should be opened
 *      pHandle[out] - pointer to RF_Handle to control Radio Core
 * Returns:
 *      N/A
 *
 */
uint8_t SSD1306_SendCommand(uint8_t cmd)
{
    uint8_t buffer[2];

    buffer[0] = 0x0;

    buffer[1] = cmd;

    uint8_t status = sendData(SSD1306_ADDR, buffer, 2);

    if ( status == 1 )
    {
        return SSD1306_SUCCESS;
    }

    return SSD1306_ERROR;
}

/*
 * === Template
 * Lorem Ipsum Dolor sit amet.
 *
 * Parameters:
 *      pParams[in]  - pointer to parameters structure
 *      pObj[in]     - pointer to object storing internal configuration
 *      proto[in     - RF protocol to which Radio Core should be opened
 *      pHandle[out] - pointer to RF_Handle to control Radio Core
 * Returns:
 *      N/A
 *
 */
uint8_t SSD1306_NormalScreen()
{
    return SSD1306_SendCommand(SSD1306_DIS_NORMAL);
}


/*
 * === Template
 * Lorem Ipsum Dolor sit amet.
 *
 * Parameters:
 *      pParams[in]  - pointer to parameters structure
 *      pObj[in]     - pointer to object storing internal configuration
 *      proto[in     - RF protocol to which Radio Core should be opened
 *      pHandle[out] - pointer to RF_Handle to control Radio Core
 * Returns:
 *      N/A
 *
 */
uint8_t SSD1306_InverseScreen()
{
    return SSD1306_SendCommand(SSD1306_DIS_INVERSE);
}


/*
 * === Template
 * Lorem Ipsum Dolor sit amet.
 *
 * Parameters:
 *      pParams[in]  - pointer to parameters structure
 *      pObj[in]     - pointer to object storing internal configuration
 *      proto[in     - RF protocol to which Radio Core should be opened
 *      pHandle[out] - pointer to RF_Handle to control Radio Core
 * Returns:
 *      N/A
 *
 */
uint8_t SSD1306_UpdateScreen()
{
    uint8_t status = INIT_STATUS;

    status = sendData(SSD1306_ADDR, (void*)SSD1306_LcdBufferInclStreamByte, (uint8_t)(CACHE_SIZE_MEM + 1));

    if ( status == 1)
    {
        return SSD1306_SUCCESS;
    }

    return SSD1306_ERROR;
}


/*
 * === Template
 * Lorem Ipsum Dolor sit amet.
 *
 * Parameters:
 *      pParams[in]  - pointer to parameters structure
 *      pObj[in]     - pointer to object storing internal configuration
 *      proto[in     - RF protocol to which Radio Core should be opened
 *      pHandle[out] - pointer to RF_Handle to control Radio Core
 * Returns:
 *      N/A
 *
 */
void SSD1306_ClearScreenBuffer()
{
    memset((void*)SSD1306_Lcd, 0x00, (uint8_t)CACHE_SIZE_MEM);
}


