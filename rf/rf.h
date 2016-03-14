#include <avr/io.h>
#include <stdint.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define RF_BUFFER_SIZE 128

uint8_t buffer[RF_BUFFER_SIZE];

void rf_init();
void rf_transmit(uint8_t* frame, uint8_t length);
uint8_t rf_receive_available();
ISR(TRX24_TX_END_vect);