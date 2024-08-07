#ifndef SERIAL_H
#define SERIAL_H

#include "types.h"
#include <avr/interrupt.h>
#include <avr/io.h>


void uart_init(void);

void uart_tx(char data);
char uart_rx(void);

// u8 read_spi(void);
void write_spi(u8 data);

#endif // SERIAL_H
