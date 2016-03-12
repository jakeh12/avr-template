#include "oled.h"
//#include "io.h"

int main()
{
    // set clock prescale to 0 -> 16 MHz
    CLKPR = _BV(CLKPCE);
    CLKPR = 0x00;

    display_init();
    display_printf("The quick brown fox jumps over the lazy dog.");

}