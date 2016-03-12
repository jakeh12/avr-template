#include "oled/oled.h"

int main()
{
    display_init();
    char netid[] = "jhladik";
    display_printf("%s@iastate.edu", netid);
}