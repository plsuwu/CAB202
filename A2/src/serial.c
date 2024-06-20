#include "serial.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#include "types.h"

u8 read_spi(void) {
    return SPI0.DATA;
}

void write_spi(u8 b) {
    SPI0.DATA = b;
}

char uart_rx(void) {
    while (!(USART0.STATUS & USART_RXCIF_bm)) {
        ;;
    }

    return USART0.RXDATAL;
}

void uart_tx(char c) {
    while (!(USART0.STATUS & USART_DREIF_bm)) {
        ;;
    }

    USART0.TXDATAL = c;
}

void uart_puts(char *s) {
    char *c = s;

    while (*c != '\0') {
        uart_tx(*c);
        c++;
    }
}
