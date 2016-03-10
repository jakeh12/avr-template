#include <avr/io.h>

#define F_CPU 16000000UL
#include <util/delay.h>

//  -U lfuse:w:0x6F:m // use transceiver oscillator

int main()
{

    // set clock prescaler to 0
    CLKPR = _BV(CLKPCE);
    CLKPR = 0x00;

    DDRB = 0xFF;

    while(1)
    {
        PORTB = 0xFF;
        _delay_ms(250);
        PORTB = 0x00;
        _delay_ms(250);
    }

}