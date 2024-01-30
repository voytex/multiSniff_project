/*
 * html.h
 *
 *  Created on: 5. 1. 2024
 *      Author: vojtechlukas
 */


#ifndef SOURCE_HTML_HTML_H_
#define SOURCE_HTML_HTML_H_

// === DEFINES ==================================================================================================

#define HTML_MEM_START    (0x51000)

#define SUBSTITUTE_TOKEN  ('$')

#define MAXLEN            (17)

#define MTU_BUF_MEM_START (0x20012000)

#define MTU_SIZE          (1400)

// ==============================================================================================================


// === FUNCTION DECLARATIONS ====================================================================================

void Html_SetKeyValueInBuffer(char, char*);

int32_t Html_CopyHtmlToMtuBuffer(uint16_t);

// ==============================================================================================================

#endif /* SOURCE_HTML_HTML_H_ */
