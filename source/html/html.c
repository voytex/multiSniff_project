/*
 * html.c
 *
 *  Created on: 5. 1. 2024
 *      Author: vojtechlukas
 */


// === INCLUDES =================================================================================================

#include <stdint.h>

#include <stdbool.h>

#include <unistd.h>

#include <source/html/html.h>

// ==============================================================================================================


// === DEFINES ==================================================================================================

#define HTML_MEM_START    (0x20010000)

#define SUBSTITUTE_TOKEN  ('$')

#define MAXLEN            (17)

#define MTU_BUF_MEM_START (0x20013000)

#define MTU_SIZE          (0x1000)

// ==============================================================================================================


// === STATIC VARIABLES =========================================================================================

typedef struct ValueBuffer
{
    char buf[17];
} ValueBuffer_t;

static ValueBuffer_t valueBuffer[26];

// ==============================================================================================================


// === INTERNAL FUNCTIONS =======================================================================================¨

uint8_t Html_strcpy(const char* pDst, const char* pSrc, uint8_t maxlen)
{
    uint8_t copied = 0;
    char* pD = (char*)pDst;
    char* pS = (char*)pSrc;

    while (( *pS != '\0' ) && ( maxlen-- ))
    {
        *pD = *pS;
        pS++;
        pD++;
        copied++;
    }

    return copied;
}

void Html_strset(const char* pDst, char ch, uint8_t maxlen)
{
    char* pD = (char*)pDst;
    while ( --maxlen )
    {
        *pD++ = ch;
    }

    return;
}

// ==============================================================================================================


// === PUBLISHED FUNCTIONS ======================================================================================

void Html_SetKeyValueInBuffer(char key, char* value)
{
    Html_strset(valueBuffer[key - 'a'].buf, 0, MAXLEN);

    Html_strcpy(valueBuffer[key - 'a'].buf, value, MAXLEN);

    return;
}

uint8_t Html_CopyHtmlToMtuBuffer(uint16_t offset)
{
    char *pHtml = (char*)(HTML_MEM_START + offset);
    char *pMtu  = (char*)MTU_BUF_MEM_START;
    uint16_t i, tmp;

    for ( i = 0; i < MTU_SIZE - 1; i++ )
    {
        if ( *pHtml == '\0' )
        {
            Html_strset(pMtu, 0, MTU_SIZE - i);
            return i;
        }

        if ( *pHtml == SUBSTITUTE_TOKEN )
        {
            tmp = Html_strcpy(pMtu, valueBuffer[*(pHtml + 1) - 'a'].buf, MAXLEN);
            pHtml += 2;
            pMtu += tmp;
        }
        else
        {
            *pMtu = *pHtml;
            pMtu++;
            pHtml++;
        }
    }

    *pMtu = 0;

    return i;
}







