/*
 * html.c
 *
 *  Created on: 5. 1. 2024
 *      Author: vojtechlukas
 */


// === INCLUDES =================================================================================================

#include <source/html/html.h>

#include <stdint.h>

#include <stdbool.h>

#include <unistd.h>

// ==============================================================================================================


// === DEFINES ==================================================================================================

#define HTML_MEM_START    (0x20010000)

#define SUBSTITUTE_TOKEN  ('$')

#define MAXLEN            (17)

#define ERASE_SEQUENCE    ("                 ")

// ==============================================================================================================


// === STATIC VARIABLES =========================================================================================

typedef struct ValueBuffer
{
    char buf[17];
    char* pos;
    bool used;
} ValueBuffer_t;

static ValueBuffer_t valueBuffer[26];

const char eraseSequence[] = ERASE_SEQUENCE;

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


void Html_ScanAndRegisterTokens()
{
    char* pHtml = (char*)HTML_MEM_START;

    while ( *pHtml != '\0' )
    {
        if ( *pHtml == SUBSTITUTE_TOKEN )
        {
            valueBuffer[*(pHtml + 1) - 'a'].pos = pHtml;
        }

        pHtml++;
    }

    return;
}


// ==============================================================================================================


// === PUBLISHED FUNCTIONS ======================================================================================¨

void Html_AddKeyValueToBuffer(char key, char* value)
{
    Html_memcpy(valueBuffer[key - 'a'].buf, value, MAXLEN);

    valueBuffer[key - 'a'].used = true;

    return;
}


void Html_Init()
{
    uint8_t i;

    for (i = 0; i < 26; i++)
    {
        valueBuffer[i].used = false;
        valueBuffer[i].pos  = NULL;
    }

    Html_ScanAndRegisterTokens();

    return;
}

void Html_UpdateHtml()
{
    uint8_t i;
    char *pTmp;

    for ( i = 0; i < 26; i++ )
    {
        if ( valueBuffer[i].used )
        {
            pTmp = valueBuffer[i].pos;
            Html_memcpy(pTmp, eraseSequence, MAXLEN);
            Html_memcpy(pTmp, valueBuffer[i].buf, MAXLEN);
        }
    }
    return;
}





