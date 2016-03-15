#include <avr/io.h>
#include <stdint.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define RF_BUFFER_SIZE 128

void rf_clear_buffer();
void rf_init();
void rf_transmit(uint8_t* frame, uint8_t length);
uint8_t rf_available();
void rf_read(uint8_t* to);