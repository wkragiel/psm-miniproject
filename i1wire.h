#ifndef _i1wire_h_
#define _i1wire_h_

#include <avr/io.h>
#include <util/delay.h>

#define OW_PIN PB3
#define OW_DIR DDRB
#define OW_OUT PORTB
#define OW_IN PINB

#define OW_low OW_DIR |= 1 << OW_PIN
#define OW_high OW_DIR &= ~( 1 << OW_PIN )
#define OW_check ( OW_IN & ( 1 << OW_PIN ) )

uint8_t OW_reset(void); //przywitanie
void OW_send(uint8_t byte); //wysylanie bajt
uint8_t OW_recv(void); //otrzymywanie bajt
void OWWriteBit(uint8_t byte); //wysylanie bit
uint8_t OWReadBit(void); //otrzymywanie bit

#endif