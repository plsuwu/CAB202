#include <avr/interrupt.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

#include "spi.h"

//    1. In "spi.c", declare and implement a function spi_init() that will
//       initialise SPI0 in unbuffered mode, such that data can be written
//       to the shift register that controls the 7-segment display.
//
//       This function should also enable the SPI interrupt via the IE bit
//       in INTCTRL.
//    2. Declare and implement the function spi_write(uint8_t b) which
//       will write the byte b out via the SPI interface.
//    3. Declare and implement an ISR that handles the SPI interrupt, and
//       also creates a rising edge on the DISP LATCH net.
//
//       You will need to add some code to spi_init() to enable the
//       required pins as outputs.
//
//    Declare prototypes for these functions in "spi.h" and include
//    "spi.h" at the top of "tutorial09.c".
//
//    Once this exercise is complete, uncomment the lines below. The
//    7-segment display should be blank after this code executes.

void spi_init(void) {
    cli();
    // SPI pins => PCO..=3
    PORTMUX.SPIROUTEA = PORTMUX_SPI0_ALT1_gc;   // SCK on PC0 and MOSI on PC2 as outputs
    PORTC.DIRSET = (PIN0_bm | PIN2_bm);
    PORTA.DIRSET = PIN1_bm;
    SPI0.CTRLA = SPI_MASTER_bm;
    SPI0.CTRLB = SPI_SSD_bm;                    // host mode i think
    SPI0.INTCTRL = SPI_IE_bm;

    SPI0.CTRLA |= SPI_ENABLE_bm;                // enable
    sei();
}

// in host mode, writing to DATA register starts a tx
void spi_write(uint8_t b) {
    SPI0.DATA = b;                              // write data to SPI DATA register
}

uint8_t spi_read(void) {
    return SPI0.DATA;                           // read SPI DATA register
}

ISR(SPI0_INT_vect) {
    // uint8_t data = spi_read();
    // PORTC.OUTSET |= (PIN0_bm | PIN2_bm);
    PORTA.OUTCLR = PIN1_bm;
    SPI0.INTFLAGS |= SPI_IF_bm;
}


