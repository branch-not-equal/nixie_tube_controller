#ifndef _UART_H_
#define _UART_H_

#include <avr/io.h>

#define FOSC 8000000	 /* MCU clock freq */
#define BAUD 9600	 	 /* USART baudrate */
#define MYUBRR FOSC/16/BAUD-1	 /* UBRRn */



void USART_Init(unsigned int baud);
void USART_Transmit(unsigned char data);



#endif


