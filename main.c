#include "oled.h"

int main()
{
    // set clock prescale to 0 -> 16 MHz
    CLKPR = _BV(CLKPCE);
    CLKPR = 0x00;

    display_init();

    lprintf("Pre-scale: 0x%02X", 0xA3);

}