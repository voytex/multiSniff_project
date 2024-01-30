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


// === STATIC VARIABLES =========================================================================================

typedef struct ValueBuffer
{
    char buf[MAXLEN];
} ValueBuffer_t;

/*
 * === valueBuffer
 * This struct is essentially a dictionary. Suppose that in HTML code of the dashboard, we want to 'inject'
 * IP address of the MCU device. Let's add a SUBSTITUE_TOKEN into the HTML followed by some key. Note that
 * the key could be any lower-case letter of the alphabet ('a'..'z'). Let's assign a key of 'd' to device IP
 * address. The HTML could then look like this: "...<label>$d</label>...". In the next step, add the value
 * to be 'injected' in HTML, into this valueBuffer by calling Html_SetKeyValueInBuffer().
 * ValueBuffer will then look for instance like this:
 *    valueBuffer['d'] = "192.168.0.1"
 * Then, right before sending HTML to the client, copy the HTML code from the flash to the MTU buffer
 * by calling Html_CopyHtmlToMtuBuffer(). While this function copies HTML code, it looks for the '$d' tokens.
 * When it finds one, it replaces it with the value stored under given key in valueBuffer. Therefore in the
 * MTU buffer, HTML code will look like this:
 *    "...<label>192.168.0.1</label>...".
 *
 * Parameters:
 *      key[in]      - key as which value is going to be changed
 *      value[in]    - value to be changed
 * Returns:
 *      N/A
 */
static ValueBuffer_t valueBuffer[26];

// ==============================================================================================================


// === INTERNAL FUNCTIONS =======================================================================================

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

void Html_strset(const char* pDst, char ch, uint16_t maxlen)
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


/*
 * === Html_SetKeyValueInBuffer
 * This functions sets value of given key in valueBuffer
 * dictionary. The buffer is then used when copying HTML from
 * its source to MTU buffer right before sending.
 *
 * Parameters:
 *      key[in]      - key as which value is going to be changed
 *      value[in]    - value to be changed
 * Returns:
 *      N/A
 */
void Html_SetKeyValueInBuffer(char key, char* value)
{
    Html_strset(valueBuffer[key - 'a'].buf, 0, MAXLEN);

    Html_strcpy(valueBuffer[key - 'a'].buf, value, MAXLEN);

    return;
}


/*
 * === Html_CopyHtmlToMtuBuffer
 * Copies HTML code from its source on the flash to the MTU
 * buffer. While copying, if it comes across SUBSTITUE_TOKEN,
 * it injects the value from valueBuffer dictionary.
 *
 * Parameters:
 *      offset[in]   - used to offset to start of HTML code. Usable
 *                     when splitting HTML code to more messages.
 * Returns:
 *      uint16_t     - number of copied characters (MTU_SIZE
 *                     when there are more data available; n < MTU_SIZE
 *                     when no additional data to be copied are available.)
 *
 */
int32_t Html_CopyHtmlToMtuBuffer(uint16_t offset)
{
    char *pHtml = (char*)(HTML_MEM_START + offset);
    char *pMtu  = (char*)MTU_BUF_MEM_START;
    uint16_t i, tmp;

    while ( (uint32_t)pMtu < (MTU_BUF_MEM_START + MTU_SIZE - 1) )
    {
        //
        // Source HTML ended, remove all remaining characters from MTU buffer
        // and cease copying.
        //
        if ( *pHtml == 255 )
        {
            *pMtu = 0;
            return -1;
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

    //
    // Make sure that the last character from MTU buffer is '\0'
    // so that other functions can print the buffer successfully.
    //
    *pMtu = 0;

    return ((uint32_t)pHtml - HTML_MEM_START);
}







