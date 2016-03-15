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
    display_ciprintf("Initializing...");
    _delay_ms(1000);
    display_cnprintf("Listening...    ");

    char message[20] = "";
    uint8_t length = 0;
    while(1)
    {
        length = rf_available();
        if (length)
        {
            //display_cnprintf("RX P: %d%% CRC: %d >> ", link_power(), crc_res());
            rf_read(message);
            display_printf("%s", message);
        }
    }


}