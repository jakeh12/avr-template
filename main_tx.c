#include "config/config.h"
#include "oled/oled.h"
#include "rf/rf.h"
#include "debug/debug.h"

//#include "usart/usart.h"

int main()
{
    atmega_init();
    display_init();
    rf_init();
    display_cnprintf("Initializing...");
    _delay_ms(1000);
    display_cnprintf("Transmitting...                 ");

    uint8_t message[4] = {0x10, 0x11, 0x12, 0x13};

    print_hex_dump(message, 4);

    while(1)
    {
        rf_transmit(message, 4);
        _delay_ms(1000);
    }
}