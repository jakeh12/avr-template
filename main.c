#include "config/config.h"
#include "oled/oled.h"
#include "rf/rf.h"
//#include "usart/usart.h"

int main()
{
    atmega_init();
    display_init();
    rf_init();
    _delay_ms(1000);

    display_cnprintf("Active");
    _delay_ms(500);

    uint8_t frame [4]= {0xA0, 0xEF, 0x04, 0x32};

    rf_transmit(frame, 4);

    while(1)
    {
        rf_transmit(frame, 4);
        _delay_ms(250);
    }

}