#include "config.h"

void atmega_init()
{
    cli();
    CLKPR = _BV(CLKPCE);
    CLKPR = 0x00;

    wdt_disable();

    sei();
}