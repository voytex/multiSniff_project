
#include "Serial.h"

#include <ti/drivers/UART2.h>
#include "string.h"
#include "ti_drivers_config.h"
UART2_Handle uart;


void Serial_begin(void){

//    UART2_Params uartParams;    /* Create a UART with data processing off. */
//    UART2_Params_init(&uartParams);
////    uartParams.writeMode = CONFIG_UART2;
////    uartParams.readMode = UART_DATA_BINARY;
////    uartParams.readReturnMode = UART_RETURN_FULL;
////    uartParams.readReturnMode = UART_ECHO_OFF;
//    uartParams.baudRate = 115200;
//
//    uart = UART2_open(CONFIG_UART2_0_CONST, &uartParams);
//
//    if (uart == NULL) {
//        /* UART_open() failed */
//        while (1);
//    }

}



void Serial_print(char* txt){
    UART2_write(uart, txt, strlen(txt), NULL);
}
void Serial_println(char* txt){
    Serial_print(txt);
    Serial_print("\n\r");
}
void Serial_write(char c){
    UART2_write(uart, &c, 1, NULL);
}
