#include <avr/interrupt.h>
#include <avr/io.h>
#include "types.h"

#ifndef SERIAL_H
#define SERIAL_H


u8 uart_getc(void);
void uart_putc(u8 c);
void uart_puts(char *s);

#endif // SERIAL_H
