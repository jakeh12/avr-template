#include "oled.h"

int main()
{
    display_init();
    display_printf("The quick brown fox jumps over the %s dog.", "lazy");
}