#include "uart.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

void uart_init(void) {
    cli();
    // Output enable USART0 TXD (PB2)
    PORTB.DIRSET = PIN2_bm;
    USART0.BAUD = 1389;                           // 9600 baud @ 3.333 MHz
    USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm; // Enable Tx/Rx
                                                  //
    sei();
}

char uart_getc(void) {
    while (!(USART0.STATUS & USART_RXCIF_bm))
        ; // Wait for data
    return USART0.RXDATAL;
}

void uart_putc(char c) {
    while (!(USART0.STATUS & USART_DREIF_bm))
        ; // Wait for TXDATA empty
    USART0.TXDATAL = c;
}

/** CODE: Write your code for Ex 9.0 below this line. */
void uart_puts(char *s) {
    char *c = s;

    while (*c != '\0') {
        uart_putc(*c);
        c++;
    }
}
/** CODE: Write your code for Ex 9.0 above this line. */
