#include "i1wire.h"

uint8_t OW_reset(void){ //przywitanie
    uint8_t result;
    _delay_us(0);
    //outp(PORTADDRESS,0x00); // Drives DQ low <- sprowadza magistrale w stan niski, odpowiednikiem jest nasze makro OWLoW
    OW_low;
    _delay_us(480);
    //outp(PORTADDRESS,0x01); // Releases the bus <-OWHigh robi to samo
    OW_high;
    _delay_us(70);
    //result = inp(PORTADDRESS) ^ 0x01; // Sample for presence pulse from slave <- owcheck
    result = OW_check;
    _delay_us(410); // Complete the reset sequence recovery
    return result; // Return sample presence pulse result
}

void OWWriteBit(uint8_t byte){ //wysylanie bit
    if(byte)
    {
        // Write '1' bit
        //outp(PORTADDRESS,0x00); // Drives DQ low
        OW_low;
        _delay_us(6);
        //outp(PORTADDRESS,0x01); // Releases the bus
        OW_high;
        _delay_us(64); // Complete the time slot and 10us recovery
    }
    else
    {
        // Write '0' bit
        //outp(PORTADDRESS,0x00); // Drives DQ low
        OW_low;
        _delay_us(60);
        //outp(PORTADDRESS,0x01); // Releases the bus
        OW_high;
        _delay_us(10);
    }
}
uint8_t OWReadBit(void){ //otrzymywanie bit
    uint8_t result;
    //outp(PORTADDRESS,0x00); // Drives DQ low
    OW_low;
    _delay_us(6);
    //outp(PORTADDRESS,0x01); // Releases the bus
    OW_high;
    _delay_us(9);
    //result = inp(PORTADDRESS) & 0x01; // Sample the bit value from the slave
    result = OW_check;
    _delay_us(55); // Complete the time slot and 10us recovery
    return result;
}

void OW_send(uint8_t byte){ //wysylanie bajt
    uint8_t loop;
    // Loop to write each bit in the byte, LS-bit first
    for (loop = 0; loop < 8; loop++)
    {
        OWWriteBit(byte & 0x01);
        // shift the data byte for the next bit
        byte >>= 1;
    }
}
uint8_t OW_recv(void){ //otrzymywanie bajt
    uint8_t loop, result=0;
    for (loop = 0; loop < 8; loop++)
    {
        // shift the result to get it ready for the next bit
        result >>= 1;
        // if result is one, then set MS bit
        if (OWReadBit())
            result |= 0x80; //wstawienie 1 do najstarszego bitu
    }
    return result;
}