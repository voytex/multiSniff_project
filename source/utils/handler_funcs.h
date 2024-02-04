/*
 * handler_funcs.h
 *
 *  Created on: 31. 1. 2024
 *      Author: vojtechlukas
 */

#ifndef SOURCE_UTILS_HANDLER_FUNCS_H_
#define SOURCE_UTILS_HANDLER_FUNCS_H_

#define CHAR_KEY(key)    ((key) - 'a')

typedef struct HttpHandlerFunction
{
    void* (*handlerFunc)();
} HttpHandlerFunction_t;

void HandlerFuncs_RegisterHandlerFunction(char, void*);

void HandlerFuncs_UnregisterHandlerFunction(char);



#endif /* SOURCE_UTILS_HANDLER_FUNCS_H_ */
