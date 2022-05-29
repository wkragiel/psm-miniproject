#ifndef _usart_h_
#define _usart_h_

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

void USART_Init( unsigned int baud );
void USART_Transmit( unsigned char data );
unsigned char USART_Receive( void );

int u_putc(char c, FILE *unused);
#endif