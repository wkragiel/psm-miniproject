#include <avr/io.h>
#include "lcd.h"
#include "i1wire.h"
#include "usart.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

FILE lcd_stream = FDEV_SETUP_STREAM(lcd_puts, NULL, _FDEV_SETUP_WRITE);
FILE u_stream = FDEV_SETUP_STREAM(u_putc, NULL, _FDEV_SETUP_WRITE);


uint8_t bufor[15];
uint8_t i;
uint8_t star;
uint16_t mlod;
double min = 24.0;
unsigned int tryb = 0;
bool is_changed =false;
int cycles = 0;

ISR(USART_RXC_vect){
        bufor[i]=UDR;
        if(bufor[i] == '\n' || bufor[i] == '\r'){
            if(!strncmp( (const char *) bufor, "temp", 4 )){
                fprintf(&u_stream,"T = %d.%04d  C\n", star, mlod);
            }
            else if(!strncmp( (const char *) bufor, "grzalka", 7 )){
                char tmp[5];
                memcpy(tmp,&bufor[9],4);
                tmp[5] = '\0';
                min = atof(tmp);
                memset(tmp,'/0',5);
            }
            else if(!strncmp( (const char *) bufor, "start", 5 )){
                OCR1A = 6000;

                // ODCZYTAJ ZMIENNA INT
                is_changed = true;
            }

                i=0;
        }
        else
        i++;

}
ISR (TIMER1_COMPA_vect){

    if(is_changed){
            cycles++;
            if ((tryb == 0 && cycles == 20) || (tryb == 1 && cycles == 40) || (tryb == 2 && cycles == 60)) {
                OCRI1 = 3000;
                cycles = 0;
                is_changed = false;
            }

    }

    }

int main(void){

    lcdinit();
    USART_Init(1); //500 000 baud
    sei();

    DDRC = 0xff; // ustawiamy, ze caly port C będzie wyjsciowy na diodę

    ICR1 = 39999;
    TCCR1A |= (1<<COM1A1);
    TCCR1A |= (1<<WGM11);
    TCCR1B |= (1<<WGM12) | (1<<WGM13);
    TCCR1B |= (1<<CS11);
    DDRD = 0xFF;
    DDRB = 0xFF;
    TCCR0 |= (1<<WGM01) | (1<<WGM00);
    TCCR0 |= (1<<COM01) | (1<<COM00);
    TCCR0 |= (1<<CS00);

    TCCR2 |= (1<<WGM21) | (1<<WGM20);
    TCCR2 |= (1<<COM21) | (1<<COM20);
    TCCR2 |= (1<<CS20);

    OCR1A = 3000; //pozycja 0
    _delay_ms(2000);

    while(1)
    {
        OW_reset();
        OW_send(0xCC);
        OW_send(0x44);
        _delay_ms(750);

        OW_reset();
        OW_send(0xCC);
        OW_send(0xBE);
        uint8_t mlodsze = OW_recv();
        uint8_t starsze = OW_recv();
        uint16_t temperatura = (starsze<<8)|(mlodsze);

        TCCR1B|=(1<<WGM12); //tryb pracy ctc
        TCCR1B|=(1<<CS12)|(1<<CS10); //prescaller =1024
        OCR1A=3125; //żeby dostac te 5 Hz
        TIMSK|=(1<<OCIE1A);

        star = temperatura >> 4;
        mlod = (temperatura & (0x0f))*625;

        lcd_set_xy(0,0);
        fprintf(&lcd_stream, "T = %d.%04d %c C", star, mlod, 0b11011111);

        if(temperatura < min){
            PORTC = 0x00;
        } else PORTC = 0xff;

    }
    return 0;
}