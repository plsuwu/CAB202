#include "serial.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#include "types.h"

u8 read_spi(void) {
    return SPI0.DATA;
}

void write_spi(u8 byte) {
    SPI0.DATA = byte;
}

// void uart_init(void) {
//     USART0.BAUD = 1389;                            // 9600 baud
//     PORTB.DIRSET = PIN2_bm;                        // output enable TX pin
//     USART0.CTRLA = USART_RXCIE_bm;                 // enable RX interrupt
//     USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;  // enable RX and TX
// }

char uart_rx(void) {
    while (!(USART0.STATUS & USART_RXCIF_bm));  // Wait for data
    return USART0.RXDATAL;
}

void uart_tx(char c) {
    while (!(USART0.STATUS & USART_DREIF_bm));  // Wait for TXDATA empty
    USART0.TXDATAL = c;
}

void uart_puts(char *s) {
    char *c = s;

    while (*c != '\0') {
        uart_tx(*c);
        c++;
    }
}
