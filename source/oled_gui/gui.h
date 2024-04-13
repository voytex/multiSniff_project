/*
 * gui.h
 *
 *  Created on: 25. 11. 2023
 *      Author: vojtechlukas
 */

#include <stdbool.h>

#ifndef SOURCE_OLED_GUI_GUI_H_
#define SOURCE_OLED_GUI_GUI_H_

// === PUBLISHED FUNCTIONS ====================================================================================

void GUI_Init(void);

void GUI_ChangeDeviceIp(const char *);

void GUI_ChangeTargetIp(const char *);

void GUI_ChangeProto(uint8_t);

void GUI_ChangeRx(bool);

void GUI_ChangeChannel(uint8_t);

void GUI_PeriodicUpdate(void);

// ============================================================================================================

#endif /* SOURCE_OLED_GUI_GUI_H_ */
