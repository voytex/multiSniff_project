/*
  IPAddress.cpp - Base class that provides IPAddress
  Copyright (c) 2011 Adrian McEwen.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write_byte to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifdef ARDUINO
#include <Arduino.h>
#endif

#include "IPAddress.h"
#include <stdio.h>
#include <string.h>


void IPAddress_Init_str(IPAddress* ip, uint8_t* address)
{
    // memcpy(ip->bytes, address, sizeof(ip->bytes));
    ip->bytes[0] = address[0];
    ip->bytes[1] = address[1];
    ip->bytes[2] = address[2];
    ip->bytes[3] = address[3];


}

int IPAddress_fromString(IPAddress* ip, const char *address)
{
    // TODO: add support for "a", "a.b", "a.b.c" formats

    uint16_t acc = 0; // Accumulator
    uint8_t dots = 0;

    while (*address)
    {
        char c = *address++;
        if (c >= '0' && c <= '9')
        {
            acc = acc * 10 + (c - '0');
            if (acc > 255) {
                // Value out of [0..255] range
                return 0;
            }
        }
        else if (c == '.')
        {
            if (dots == 3) {
                // Too much dots (there must be 3 dots)
                return 0;
            }
			ip->bytes[dots++] = acc;
            acc = 0;
        }
        else
        {
            // Invalid char
            return 0;
        }
    }

    if (dots != 3) {
        // Too few dots (there must be 3 dots)
        return 0;
    }
	ip->bytes[3] = acc;
    return 1;
}

/*
 * @param strIp HAS TO BE at least 15 bytes long
 * since this func writes from right to left.
 */
void IPAddress_toString(IPAddress ip, char* strIP) {
//    char* write = *strIP + 14;
//    uint8_t offset = 0;
//	uint8_t i, j;
//
//	for (j = 4; j > 0; j--) {
//	    char c = ip.bytes[j-1];
//	    if (c < 10) {
//	        *(write--) = (uint8_t)(c + '0');
//	        *(write--) = '.';
//	        offset += 2;
//	    } else if (c < 100) {
//	        for (i = 0; i < 2; ++i) {
//	            *(write--) = (uint8_t)((c % 10) + '0');
//	            c /= 10;
//	        }
//	        offset += 1;
//	        *(write--) = '.';
//	    } else {
//	        for (i = 0; i < 3; ++i) {
//	            *(write--) = (uint8_t)((c % 10) + '0');
//	            c /= 10;
//	        }
//	        if (j != 0) *(write--) = '.';
//	    }
//    }
	//strIP = write + 2;
	//strIP += 3;
    sprintf(strIP, "%u.%u.%u.%u\0", ip.bytes[0], ip.bytes[1], ip.bytes[2], ip.bytes[3]);
}
