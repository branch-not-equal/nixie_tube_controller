#include "uart.h"

void USART_Init(unsigned int baud)
{
	 UBRR0H = (unsigned char)(baud>>8);	 /* BAUDRATE UPPER BYTE */
	 UBRR0L = (unsigned char)baud;	 /* BAUDRATE LOWER BYTE */
	 UCSR0C = (1<<USBS0)|(3<<UCSZ00);	 /* FRAME FORMAT(8bit,2stop bit) */
	 UCSR0B = (1<<RXEN0)|(1<<TXEN0);	 /* Enable send/recv */
}


void USART_Transmit(unsigned char data)
{
	 while ( !(UCSR0A & (1<<UDRE0)) );	 /* wait for send buffer avail */
	 UDR0 = data;	 /* start send data */
}
