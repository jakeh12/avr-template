#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define DISP_D_C    PB7
#define DISP_RST    PD7
#define DISP_SCK    PB1
#define DISP_MOSI   PB2
#define DISP_SEL    PG2

#define SSD1306_X_PIXELS 		128
#define SSD1306_Y_PIXELS 		32
#define SSD1306_PIXEL_PAGES 		(SSD1306_Y_PIXELS / 8)
#define SSD1306_PIXEL_BYTES		(SSD1306_X_PIXELS * SSD1306_PIXEL_PAGES)


#define SSD1306_DEFAULT_CONTRAST 0x7F

// Fundamental commands
#define SSD1306_CHARGE_PUMP_REGULATOR 	0x8D
#define SSD1306_CHARGE_PUMP_ON   	0x14
#define SSD1306_SET_CONTRAST		0x81
#define SSD1306_RESUME_TO_RAM_CONTENT	0xA4
#define SSD1306_IGNORE_RAM_CONTENT	0xA5
#define SSD1306_DISP_NORMAL		0xA6
#define SSD1306_DISP_INVERTED		0xA7
#define SSD1306_DISP_SLEEP 		0xAE
#define SSD1306_DISP_ON			0xAF

// Scroll commands
#define SSD1306_SCROLL_RIGHT		0x26
#define SSD1306_SCROLL_LEFT		0x27
#define SSD1306_SCROLL_VERTICAL_RIGHT	0x29
#define SSD1306_SCROLL_VERTICAL_LEFT	0x2A
#define SSD1306_SCROLL_OFF		0x2E
#define SSD1306_SCROLL_ON   		0x2F
#define SSD1306_VERT_SCROLL_AREA 	0xA3

// Address setting commands
#define SSD1306_SET_COL_LO_NIBBLE	0x00
#define SSD1306_SET_COL_HI_NIBBLE	0x10
#define SSD1306_MEM_ADDRESSING 		0x20
#define SSD1306_SET_COL_ADDR		0x21
#define SSD1306_SET_PAGE_ADDR		0x22
#define SSD1306_SET_PAGE_START_ADDR	0xB0

// Hardware configuration
#define SSD1306_SET_DISP_START_LINE	0x40
#define SSD1306_SET_SEG_REMAP_0  	0xA0
#define SSD1306_SET_SEG_REMAP_127	0xA1
#define SSD1306_SET_MULTIPLEX_RATIO     0xA8
#define SSD1306_SET_COM_SCAN_NORMAL	0xC0
#define SSD1306_SET_COM_SCAN_INVERTED	0xC8
#define SSD1306_SET_VERTICAL_OFFSET	0xD3
#define SSD1306_SET_WIRING_SCHEME	0xDA
#define SSD1306_SET_DISP_CLOCK		0xD5
#define SSD1306_SET_PRECHARGE_PERIOD  	0xD9
#define SSD1306_SET_VCOM_DESELECT_LEVEL 0xDB
#define SSD1306_NOP			0xE3

void spi_init(void)
{
    DDRB |= _BV(DISP_D_C) | _BV(DISP_SCK) | _BV(DISP_MOSI);
    DDRD |= _BV(DISP_RST);
    DDRG |= _BV(DISP_SEL);
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

    display_write_instruction(SSD1306_SET_SEG_REMAP_0);

    display_write_instruction(SSD1306_SET_COM_SCAN_INVERTED);

    display_write_instruction(SSD1306_SET_WIRING_SCHEME);
    display_write_instruction(0x02);

    display_write_instruction(SSD1306_SET_CONTRAST);
    display_write_instruction(0x8F);

    display_write_instruction(SSD1306_SET_PRECHARGE_PERIOD);
    display_write_instruction(0xF1);

    display_write_instruction(SSD1306_SET_VCOM_DESELECT_LEVEL);
    display_write_instruction(0x40);

    display_write_instruction(SSD1306_RESUME_TO_RAM_CONTENT);

    display_write_instruction(SSD1306_DISP_NORMAL);

    display_clear();

    display_write_instruction(SSD1306_DISP_ON);

}

void display_reset_cursor(void)
{
    display_write_instruction(SSD1306_SET_PAGE_START_ADDR);
    display_write_instruction(SSD1306_SET_COL_HI_NIBBLE);
    display_write_instruction(SSD1306_SET_COL_LO_NIBBLE);
}

void display_test(void)
{
    display_reset_cursor();
    for (uint16_t byte = 0; byte < SSD1306_PIXEL_BYTES; byte++)
    {
        if (byte % 2 == 0)
        {
            display_write_data (0xFF);
        }
        else
        {
            display_write_data (0x00);
        }
    }
}

void display_clear(void)
{
    display_reset_cursor();
    for (uint16_t byte = 0; byte < SSD1306_PIXEL_BYTES; byte++)
    {
            display_write_data (0x00);
    }
}

int main()
{
    // set clock prescale to 0 -> 16 MHz
    CLKPR = _BV(CLKPCE);
    CLKPR = 0x00;

    display_init();

    display_test();

    DDRB |= _BV(PB4);

    while(1)
    {
        PORTB |= _BV(PB4);
        _delay_ms(100);
        PORTB &= ~_BV(PB4);
        _delay_ms(100);
    }



}