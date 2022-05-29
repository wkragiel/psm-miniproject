#include "usart.h"
void USART_Init( unsigned int baud )
{
/* prę dkoś ć transmisji */
    UBRRH = (unsigned char)(baud>>8);
    UBRRL = (unsigned char)baud;
/* Format ramki: słowo=8bitów, 2 bity stopu */
    UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
/* Włą czenie odbiornika i nadajnika */
    UCSRB = (1<<RXEN)|(1<<TXEN);
/* Włą czenie przerwania, gdy przyszła dana */
    UCSRB |= (1<<RXCIE);
}

void USART_Transmit( unsigned char data )
{
/* Czekaj, aż zwolni się bufor nadajnika */
    while ( !( UCSRA & (1<<UDRE)) );
/* Umieś ć daną w buforze i ją wyś lij */
    UDR = data;
}

//Wysyłanie i odbiór danej (pooling)
unsigned char USART_Receive( void )
{
/* Czekaj, aż pojawi się dana do odbioru */
    while ( !(UCSRA & (1<<RXC)) ) ;
/* Odbierz daną */
    return UDR;

}

int u_putc(char c, FILE *unused)
{
    USART_Transmit(c);
    return 0;
}