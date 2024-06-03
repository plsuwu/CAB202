#include <avr/interrupt.h>
#include <avr/io.h>

#include "typing.h"


u8 uart_getc(void);
void uart_putc(u8 c);
void uart_puts(char *s);
