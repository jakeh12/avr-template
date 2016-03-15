#include "config/config.h"
#include "oled/oled.h"
#include "rf/rf.h"
#include "debug/debug.h"

#include "usart/usart.h"

void append(char* s, char c)
{
    int len = strlen(s);
    s[len] = c;
    s[len+1] = '\0';
}


int main()
{
    atmega_init();
    display_init();
    usart_init();
    rf_init();
    display_ciprintf("Initializing...");
    _delay_ms(1000);
    display_cnprintf("Transmitting... ");

    char buffer[2] = "";

    while(1)
    {
        buffer[0] = usart_receive();
        rf_transmit(buffer, strlen(buffer));
    }
}