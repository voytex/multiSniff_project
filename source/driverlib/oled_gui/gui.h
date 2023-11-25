/*
 * gui.h
 *
 *  Created on: 25. 11. 2023
 *      Author: vojtechlukas
 */

#ifndef SOURCE_DRIVERLIB_OLED_GUI_GUI_H_
#define SOURCE_DRIVERLIB_OLED_GUI_GUI_H_

void GUI_Init(void);

void GUI_ChangeDeviceIp(char*);

void GUI_ChangeTargetIp(char*);

void GUI_ChangeProto(uint8_t);

void GUI_ChangeRx(bool);

void GUI_ChangeChannel(uint8_t);

#endif /* SOURCE_DRIVERLIB_OLED_GUI_GUI_H_ */
