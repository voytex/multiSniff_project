// SPIfg.h


// PINs in use:
/*
 * SPI0 (W5500)
 * SCLK    DIO10
 * MOSI    DIO9
 * MISO    DIO8
 * CS      DIO26 (not controlled by SPI driver, but setSS() & resetSS())
 *
 * SPI1 (SD card reader?)
 * SCLK    DIO14
 * MOSI    DIO13
 * MISO    DIO12
 * CS      ???
 */
#ifndef _SPIFG_h
#define _SPIFG_h

#ifdef ARDUINO
#include "Arduino.h"
#endif

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
	uint8_t SPI_transf(uint8_t data);

	void SPI_begin();

	void  initSS();
void setSS();
void resetSS();

#ifdef __cplusplus
}
#endif
#endif

