#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#ifndef F_CPU
#define F_CPU 16000000
#endif

#ifndef BAUD
#define BAUD 19200
#endif

void atmega_init();
