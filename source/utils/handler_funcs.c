/*
 * handler_funcs.c
 *
 *  Created on: 31. 1. 2024
 *      Author: vojtechlukas
 */

// === INCLUDES =================================================================================================

#include <stdlib.h>

#include <stdint.h>

#include <source/utils/handler_funcs.h>

// ==============================================================================================================


// === PUBLIC VARIABLES =========================================================================================

HttpHandlerFunction_t httpHandlerFunctions[26];

// ==============================================================================================================



// === FUNCTION DECLARATIONS ====================================================================================

void HandlerFuncs_RegisterHandlerFunction(char key, void* handlerFunction)
{
    httpHandlerFunctions[CHAR_KEY(key)].handlerFunc = handlerFunction;

    return;
}


void HandlerFuncs_UnregisterHandlerFunction(char key)
{
    httpHandlerFunctions[CHAR_KEY(key)].handlerFunc = NULL;

    return;
}
