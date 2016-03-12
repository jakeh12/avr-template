#include "oled.h"
#include "font.h"

void spi_init(void)
{
    DDRB |= _BV(DISP_D_C) | _BV(DISP_SCK) | _BV(DISP_MOSI);
    DDRD |= _BV(DISP_RST);
    DDRG |= _BV(DISP_SEL);

    /*
    DDRB |= _BV(DISP_SCK) | _BV(DISP_MOSI);
    DDRD |= _BV(DISP_RST);
    DDRE |= _BV(DISP_D_C);
    DDRG |= _BV(DISP_SEL);
     */

    SPCR = _BV(SPE) | _BV(MSTR);
}

void spi_transmit(uint8_t data)
{
    SPDR = data;
    loop_until_bit_is_set(SPSR, SPIF);
}

void display_write_data(uint8_t data)
{
    PORTB |= _BV(DISP_D_C);
    PORTB &= ~_BV(DISP_SEL);
    spi_transmit(data);
    PORTB |= _BV(DISP_SEL);
}

void display_write_instruction(uint8_t data)
{
    PORTB &= ~(_BV(DISP_D_C) | _BV(DISP_SEL));
    spi_transmit(data);
    PORTB |= _BV(DISP_SEL);
}

void display_reset(void)
{
    PORTD |= _BV(DISP_RST);
    _delay_us (3);
    PORTD &= ~(_BV(DISP_RST));
    _delay_us (3);
    PORTD |= _BV(DISP_RST);
}

void display_reset_cursor(void)
{
    display_write_instruction(SSD1306_SET_PAGE_START_ADDR);
    display_write_instruction(SSD1306_SET_COL_HI_NIBBLE);
    display_write_instruction(SSD1306_SET_COL_LO_NIBBLE);
}


void display_clear(void)
{
    display_reset_cursor();
    for (uint16_t byte = 0; byte < SSD1306_PIXEL_BYTES; byte++)
    {
        display_write_data (0x00);
    }
    display_reset_cursor();
}

void display_init(void)
{
    spi_init();

    display_reset();

    display_write_instruction(SSD1306_DISP_SLEEP);

    display_write_instruction(SSD1306_SET_DISP_CLOCK);
    display_write_instruction(0x80);

    display_write_instruction(SSD1306_SET_MULTIPLEX_RATIO);
    display_write_instruction(0x1F);

    display_write_instruction(SSD1306_SET_VERTICAL_OFFSET);
    display_write_instruction(0x00);

    display_write_instruction(SSD1306_SET_DISP_START_LINE);

    display_write_instruction(SSD1306_CHARGE_PUMP_REGULATOR);
    display_write_instruction(0x14);

    display_write_instruction(SSD1306_SET_SEG_REMAP_127);

    display_write_instruction(SSD1306_SET_COM_SCAN_INVERTED);

    display_write_instruction(SSD1306_SET_WIRING_SCHEME);
    display_write_instruction(0x02);

    display_write_instruction(SSD1306_SET_CONTRAST);
    display_write_instruction(0x8F);

    display_write_instruction(SSD1306_SET_PRECHARGE_PERIOD);
    display_write_instruction(0xF1);

    display_write_instruction(SSD1306_SET_VCOM_DESELECT_LEVEL);
    display_write_instruction(0x40);
    display_write_instruction(SSD1306_MEM_ADDRESSING);
    display_write_instruction(0x00);

    display_write_instruction(SSD1306_RESUME_TO_RAM_CONTENT);


    display_write_instruction(SSD1306_DISP_NORMAL);

    display_clear();

    display_write_instruction(SSD1306_DISP_ON);

    //display_write_instruction(SSD1306_DISP_INVERTED);

}

void display_print_letter(char letter)
{
    if (letter < 32) { letter = 32; }
    else if (letter > 126) { letter = 126; }

    char render;

    for (uint8_t byte = 0; byte < 8; byte++)
    {
        render = font[letter - 32][byte];
        render = &(font[letter - 32][byte]);
        render = pgm_read_byte(&(font[letter - 32][byte]));
        display_write_data(render);
    }
}

void display_print_string(char* string)
{
    display_write_instruction(SSD1306_SET_PAGE_START_ADDR);
    while(*string != 0x00)
    {
        display_print_letter(*string++);
    }
}

void display_printf(const char* format, ...)
{
    char string[128];
    char* string_p = string;
    va_list args;
    va_start(args, format);
    vsprintf(string_p, format, args);
    va_end(args);

    //display_write_instruction(SSD1306_SET_PAGE_START_ADDR);
    while(*string_p != 0x00)
    {
        display_print_letter(*string_p++);
    }
}
