/*
 * html.c
 *
 *  Created on: 5. 1. 2024
 *      Author: vojtechlukas
 */


// === INCLUDES =================================================================================================

#include <source/html/html.h>

#include <stdint.h>

// ==============================================================================================================


// === DEFINES ==================================================================================================

#define HTML_MEM_START    (0x20010000)

#define SUBSTITUTE_TOKEN  ('$')

#define MAXLEN            (17)

// ==============================================================================================================


// === STATIC VARIABLES =========================================================================================

typedef struct ValueBuffer
{
    char buf[17];
} ValueBuffer_t;

static ValueBuffer_t valueBuffer[26];

// ==============================================================================================================


// === INTERNAL FUNCTIONS =======================================================================================¨

void Html_memcpy(const char* pDst, const char* pSrc, uint8_t maxlen)
{
    char* pD = (char*)pDst;
    char* pS = (char*)pSrc;

    while (( *pS != '\0' ) && ( maxlen-- ))
    {
        *pD = *pS;
        pS++;
        pD++;
    }

    return;
}

// ==============================================================================================================


// === PUBLISHED FUNCTIONS ======================================================================================¨

void Html_AddKeyValueToBuffer(char key, char* value)
{
    Html_memcpy(valueBuffer[key - 'a'].buf, value, MAXLEN);

    return;
}


void Html_UpdateHtml()
{
    char *pHtml = (char*)HTML_MEM_START;
    char key;

    while ( *pHtml != '\0' )
    {
        if ( *pHtml == SUBSTITUTE_TOKEN )
        {
            key = *(++pHtml);

            Html_memcpy(pHtml - 1, valueBuffer[key - 'a'].buf, MAXLEN);
        }

        pHtml++;
    }

    return;
}





