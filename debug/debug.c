#include "debug.h"

void print_hex_dump(uint8_t* array, uint8_t length)
{
    //display_clear();

    display_printf("[ ");

    for (uint8_t i = 0; i < length; i++)
    {
        display_printf(" 0x%02X", *(array+i));
        if (i != (length-1))
        {
            display_printf(", ");
        }
    }

    display_printf(" ]");

}