#include <avr/io.h>

#define F_CPU 250000UL
#include <util/delay.h>

int main()
{
    DDRB = 0xFF;

    while(1)
    {
        PORTB = 0xFF;
        _delay_ms(250);
        PORTB = 0x00;
        _delay_ms(250);
    }

}