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

#include <stdio.h>

#include <string.h>

#include <ti/drivers/GPIO.h>

#include <ti/drivers/I2C.h>

#include "ti_drivers_config.h"

#include <source/driverlib/ssd1306/font.h>

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
char *const SSD1306_Lcd = SSD1306_LcdBufferInclStreamByte + 1;


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
uint8_t SSD1306_Init()
{
    SSD1306_LcdBufferInclStreamByte[0] = 0x40;

    uint8_t i;

    uint8_t status = INIT_STATUS;

    //
    // I2C Initialization
    //
    GPIO_init();

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
    memset((void *)SSD1306_Lcd, 0xFF, CACHE_SIZE_MEM);

    return;
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
void SSD1306_SetPosition(uint8_t x, uint8_t y)
{
    _counter = x + ( y << 7 );

    return;
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
uint8_t SSD1306_UpdatePosition()
{
    uint8_t y = _counter >> 7;

    uint8_t x = _counter - ( y << 7 );

    uint8_t xNew = x + CHARS_COLS_LENGTH + 1;

    if ( xNew > END_COLUMN_ADDR )
    {
        if ( y > END_PAGE_ADDR )
        {
            return SSD1306_ERROR;
        }
        else if ( y < ( END_PAGE_ADDR - 1))
        {
            _counter = (( ++y ) << 7 );
        }
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
uint8_t SSD1306_DrawChar(char character)
{
    uint8_t i = 0;

    uint8_t canDrawChar = SSD1306_UpdatePosition();

    if ( canDrawChar == SSD1306_ERROR )
    {
        return SSD1306_ERROR;
    }

    while ( i < CHARS_COLS_LENGTH )
    {
        SSD1306_Lcd[_counter++] = FONTS[character - 32][i++];
    }

    _counter++;

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
uint8_t SSD1306_DrawString(char *str)
{
    uint8_t i = 0;

    uint8_t status = SSD1306_SUCCESS;

    while (( str[i] != '\0' ) && ( status == SSD1306_SUCCESS ))
    {
        status = SSD1306_DrawChar(str[i++]);
    }

    return status;
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
uint8_t SSD1306_DrawPixel(uint8_t x, uint8_t y)
{
    uint8_t page = 0;
    uint8_t pixel = 0;

    if (( x > MAX_X ) || ( y > MAX_Y ))
    {
        return SSD1306_ERROR;
    }

    page = y >> 3;

    pixel = 1 << ( y - ( page << 3 ));

    _counter = x + ( page << 7 );

    SSD1306_Lcd[_counter++] |= pixel;

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
uint8_t SSD1306_DrawLine(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2)
{
    int16_t det;
    int16_t deltaX, deltaY;
    int16_t traceX = 1, traceY = 1;

    deltaX = x2 - x1;

    deltaY = y2 - y1;

    if ( deltaX < 0 )
    {
        deltaX = -deltaX;
        traceX = -traceX;
    }

    if ( deltaY < 0 )
    {
        deltaY = -deltaY;
        traceY = -traceY;
    }

    if ( deltaY < deltaX )
    {
        det = ( deltaY << 1 ) - deltaX;

        SSD1306_DrawPixel(x1, y1);

        while ( x1 != x2 )
        {
            x1 += traceX;

            if ( det >= 0 )
            {
                y1 += traceY;

                det -= 2 * deltaY;
            }

            det += 2 * deltaY;

            SSD1306_DrawPixel(x1, y1);
        }
    }
    else
    {
        det = deltaY - ( deltaX << 1 );

        SSD1306_DrawPixel(x1, y1);

        while ( y1 != y2 )
        {
            y1 += traceY;

            if ( det <= 0 )
            {
                x1 += traceX;

                det += 2 * deltaY;
            }

            det -= 2 * deltaX;

            SSD1306_DrawPixel(x1, y1);
        }
    }

    return SSD1306_SUCCESS;
}
