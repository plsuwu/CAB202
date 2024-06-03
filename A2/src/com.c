#include <avr/interrupt.h>
#include <avr/io.h>

#include "typing.h"


u8 uart_getc(void) {
    while (!(USART0.STATUS & USART_RXCIF_bm));
    return USART0.RXDATAL;
}

void uart_putc(u8 c) {
    while (!(USART0.STATUS & USART_DREIF_bm));
    USART0.TXDATAL = c;
}

void uart_puts(char *s) {
    char *c = s;

    while (*c != '\0') {
        uart_putc(*c);
        c++;
    }
}
