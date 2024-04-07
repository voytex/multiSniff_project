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

#include <stdlib.h>

#include <stdio.h>

#include <source/driverlib/ssd1306/ssd1306.h>

#include <source/driverlib/ssd1306/font.h>

#include <source/radio_proc/radio_proc.h>

#include <source/oled_gui/gui.h>

// ==============================================================================================================

// === INTERNAL FUNCTIONS =======================================================================================

#define HORIZ_CHARS_TO_PIX(ch) ((ch * CHARS_COLS_LENGTH) + ch)

#define VERTI_CHARS_TO_PIX(ch) ((ch * 8) - 1)

static inline void ito2a(uint8_t i, char* ch)
{
    if (( i >= 0 ) && ( i < 100 ))
    {
        ch[0] = ( i / 10 ) + 48;
        ch[1] = ( i % 10 ) + 48;
    }
    else
    {
        ch[0] = 'o';
        ch[1] = '0';
    }

    return;
}

// ==============================================================================================================

// === PUBLISHED FUNCTIONS ======================================================================================

/*
 * === GUI_Init
 * Firstly calls SSD1306_Init() to perform SSD1306 initialization sequence.
 * Then renders basic GUI structure based on /design/ssd1306_design.xlsx.
 * Note: Does NOT update the screen (since it does not print any important
 * data). Just writes to LCD buffer.
 *
 * Parameters:
 *      N/A
 * Returns:
 *      N/A
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

    SSD1306_DrawString("RXOK frames:", false);

    SSD1306_DrawLine(HORIZ_CHARS_TO_PIX(15), HORIZ_CHARS_TO_PIX(15), 0, 63);

    return;
}

/*
 * === GUI_ChangeDeviceIp
 * Changes the `deice IP:` field in the GUI of the OLED.
 *
 * Parameters:
 *      deiceIp[in]   - pointer to new IP address
 *                      in const char* format
 * Returns:
 *      N/A
 */
void GUI_ChangeDeviceIp(const char *deviceIp)
{
    SSD1306_SetPosition(0, 2);

    SSD1306_DrawString("               ", false);

    SSD1306_SetPosition(0, 2);

    SSD1306_DrawString(deviceIp, false);

    SSD1306_DrawLine(0, HORIZ_CHARS_TO_PIX(15), VERTI_CHARS_TO_PIX(3), VERTI_CHARS_TO_PIX(3));

    SSD1306_UpdateScreen();

    return;
}

/*
 * === GUI_ChangeTargetIp
 * Changes the `target IP:` field in the GUI of the OLED.
 *
 * Parameters:
 *      targetIp[in]  - pointer to new IP address
 *                      in const char* format
 * Returns:
 *      N/A
 */
void GUI_ChangeTargetIp(const char *targetIp)
{
    SSD1306_SetPosition(0, 4);

    SSD1306_DrawString("               ", false);

    SSD1306_SetPosition(0, 4);

    SSD1306_DrawString(targetIp, false);

    SSD1306_DrawLine(0, HORIZ_CHARS_TO_PIX(15), VERTI_CHARS_TO_PIX(5), VERTI_CHARS_TO_PIX(5));

    SSD1306_UpdateScreen();

    return;
}

/*
 * === GUI_ChangeProto
 * Changes the `proto:` field in the GUI of the OLED.
 *
 * Parameters:
 *      proto[in]   - unsigned integer representing protocol
 *                    0 for BLE
 *                    1 for IEEE
 * Returns:
 *      N/A
 */
void GUI_ChangeProto(uint8_t proto)
{
    SSD1306_SetPosition(HORIZ_CHARS_TO_PIX(17), 1);

    SSD1306_DrawString("    ", false);

    SSD1306_SetPosition(HORIZ_CHARS_TO_PIX(17), 1);

    switch (proto)
    {
    case 0:
        SSD1306_DrawString("BLE:", false);
        break;

    case 1:
        SSD1306_DrawString("IEE:", false);
        break;

    default:
        SSD1306_DrawString("Err", false);
        break;
    }

    SSD1306_UpdateScreen();

    return;
}

/*
 * === GUI_ChangeRx
 * Changes the `RX:` field in the GUI of the OLED.
 *
 * Parameters:
 *      rx[in]  - bool value
 *                true = RX on, RX notification is visible
 *                false = no RX notification on the screen
 * Returns:
 *      N/A
 */
void GUI_ChangeRx(bool rx)
{
    if (rx)
    {
        SSD1306_SetPosition(HORIZ_CHARS_TO_PIX(17), 4);

        SSD1306_DrawString("    ", true);

        SSD1306_SetPosition(HORIZ_CHARS_TO_PIX(17), 5);

        SSD1306_DrawString(" RX ", true);

        SSD1306_SetPosition(HORIZ_CHARS_TO_PIX(17), 6);

        SSD1306_DrawString("    ", true);
    }
    else
    {
        SSD1306_SetPosition(HORIZ_CHARS_TO_PIX(17), 4);

        SSD1306_DrawString("    ", false);

        SSD1306_SetPosition(HORIZ_CHARS_TO_PIX(17), 5);

        SSD1306_DrawString("    ", false);

        SSD1306_SetPosition(HORIZ_CHARS_TO_PIX(17), 6);

        SSD1306_DrawString("    ", false);
    }

    SSD1306_UpdateScreen();

    return;
}

/*
 * === GUI_ChangeChannel
 * Changes the `CH:` field in the GUI of the OLED.
 *
 * Parameters:
 *      ch[in]   - unsigned int representing number of the channel
 * Returns:
 *      N/A
 */
void GUI_ChangeChannel(uint8_t ch)
{
    SSD1306_SetPosition(HORIZ_CHARS_TO_PIX(18), 2);

    SSD1306_DrawString("  ", false);

    char buf[3];

    ito2a(ch, buf);

    buf[2] = '\0';

    SSD1306_SetPosition(HORIZ_CHARS_TO_PIX(18), 2);

    SSD1306_DrawString(buf, false);

    SSD1306_UpdateScreen();

    return;
}


/*
 * === GUI_PeriodicUpdate
 * Function to be invoked periodically. It checks number of RX OK
 * frames (according to current RF Protocol being used) and refreshes
 * the OLED screen.
 *
 * Parameters:
 *      N/A
 * Returns:
 *      N/A
 */
void GUI_PeriodicUpdate()
{
    uint16_t nRxOk = Radio_GetRxOk();

    //
    // Since UINT16_MAX value is 65535, buffer of size 6
    // should be sufficient to hold the number.
    //
    char numBuf[6] = {0};

    ltoa(nRxOk, numBuf, 10);

    SSD1306_SetPosition(0, 6);

    SSD1306_DrawString("               ", false);

    SSD1306_SetPosition(0, 6);

    SSD1306_DrawString((const char*)numBuf, false);

    SSD1306_UpdateScreen();

    return;
}
