/*
 * gui.c
 *
 *  Created on: 25. 11. 2023
 *      Author: vojtechlukas
 */

// === INCLUDES =================================================================================================

#include <unistd.h>

#include <stdbool.h>

#include <string.h>

#include <source/driverlib/ssd1306/ssd1306.h>

#include <source/driverlib/ssd1306/font.h>

#include <source/driverlib/oled_gui/gui.h>

// ==============================================================================================================



// === INTERNAL FUNCTIONS =======================================================================================

#define HORIZ_CHARS_TO_PIX(ch) ((ch*CHARS_COLS_LENGTH) + ch)

#define VERTI_CHARS_TO_PIX(ch) ((ch*8) - 1)

// ==============================================================================================================



// === PUBLISHED FUNCTIONS ======================================================================================

/*
 * === SSD1306_Init
 * Initializes GPIO, I2C and SSD1306 itself by sending sequence of commands.
 * Also sets the zeroth byte of Lcd Buffer Stream to 0x40 for easier sending.
 *
 * Parameters:
 *      N/A
 * Returns:
 *      SSD1306_SUCCES, SSD1306_ERROR or I2C_ERROR
 */
void GUI_Init(void)
{
    SSD1306_Init();

    SSD1306_SetPosition(0, 0);

    SSD1306_DrawString("multiSniff           ", true);

    SSD1306_SetPosition(0, 1);

    SSD1306_DrawString("device IP:", false);

    SSD1306_DrawLine(0, HORIZ_CHARS_TO_PIX(15), VERTI_CHARS_TO_PIX(3), VERTI_CHARS_TO_PIX(3));

    SSD1306_SetPosition(0, 3);

    SSD1306_DrawString("target IP:", false);

    SSD1306_DrawLine(0, HORIZ_CHARS_TO_PIX(15), VERTI_CHARS_TO_PIX(5), VERTI_CHARS_TO_PIX(5));

    SSD1306_SetPosition(0, 5);

    SSD1306_DrawString("proto:", false);

    SSD1306_SetPosition(HORIZ_CHARS_TO_PIX(9), 5);

    SSD1306_DrawString("RX:", false);

    SSD1306_SetPosition(HORIZ_CHARS_TO_PIX(9), 6);

    SSD1306_DrawString("CH:", false);

    SSD1306_DrawLine(HORIZ_CHARS_TO_PIX(15), HORIZ_CHARS_TO_PIX(15), 0, 63);

    return;
}


/*
 * === SSD1306_Init
 * Initializes GPIO, I2C and SSD1306 itself by sending sequence of commands.
 * Also sets the zeroth byte of Lcd Buffer Stream to 0x40 for easier sending.
 *
 * Parameters:
 *      N/A
 * Returns:
 *      SSD1306_SUCCES, SSD1306_ERROR or I2C_ERROR
 */
void GUI_ChangeDeviceIp(char* deviceIp)
{
    SSD1306_SetPosition(0, 2);

    SSD1306_DrawString(deviceIp, false);

    SSD1306_UpdateScreen();

    return;
}


/*
 * === SSD1306_Init
 * Initializes GPIO, I2C and SSD1306 itself by sending sequence of commands.
 * Also sets the zeroth byte of Lcd Buffer Stream to 0x40 for easier sending.
 *
 * Parameters:
 *      N/A
 * Returns:
 *      SSD1306_SUCCES, SSD1306_ERROR or I2C_ERROR
 */
void GUI_ChangeTargetIp(char* targetIp)
{
    SSD1306_SetPosition(0, 4);

    SSD1306_DrawString(targetIp, false);

    SSD1306_UpdateScreen();

    return;
}


/*
 * === SSD1306_Init
 * Initializes GPIO, I2C and SSD1306 itself by sending sequence of commands.
 * Also sets the zeroth byte of Lcd Buffer Stream to 0x40 for easier sending.
 *
 * Parameters:
 *      N/A
 * Returns:
 *      SSD1306_SUCCES, SSD1306_ERROR or I2C_ERROR
 */
void GUI_ChangeProto(uint8_t proto)
{
    SSD1306_SetPosition(0, 6);

    switch (proto)
    {
    case 0:
        SSD1306_DrawString("BLE", false);
        break;

    case 1:
        SSD1306_DrawString("IEEE", false);
        break;

    default:
        SSD1306_DrawString("N/A", false);
        break;
    }

    SSD1306_UpdateScreen();

    return;
}


/*
 * === SSD1306_Init
 * Initializes GPIO, I2C and SSD1306 itself by sending sequence of commands.
 * Also sets the zeroth byte of Lcd Buffer Stream to 0x40 for easier sending.
 *
 * Parameters:
 *      N/A
 * Returns:
 *      SSD1306_SUCCES, SSD1306_ERROR or I2C_ERROR
 */
void GUI_ChangeRx(bool rx)
{
    SSD1306_SetPosition(HORIZ_CHARS_TO_PIX(12), 5);

    SSD1306_DrawString((rx ? "ON" : "OFF"), !rx);

    SSD1306_UpdateScreen();

    return;
}


/*
 * === SSD1306_Init
 * Initializes GPIO, I2C and SSD1306 itself by sending sequence of commands.
 * Also sets the zeroth byte of Lcd Buffer Stream to 0x40 for easier sending.
 *
 * Parameters:
 *      N/A
 * Returns:
 *      SSD1306_SUCCES, SSD1306_ERROR or I2C_ERROR
 */
void GUI_ChangeChannel(uint8_t ch)
{
    SSD1306_SetPosition(HORIZ_CHARS_TO_PIX(12), 6);

    char channel[2];

    //TODO: SSD1306_DrawString(itoa(ch, channel, "10"), false);

    return;
}
