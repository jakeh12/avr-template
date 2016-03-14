#include <avr/io.h>
#include <util/setbaud.h>
#include <stdint.h>

void usart_init(void);
void usart_transmit(uint8_t data);
uint8_t usart_receive(void);
void usart_transmit_string(char* string);
