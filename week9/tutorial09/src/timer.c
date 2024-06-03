#include <avr/io.h>
#include <avr/interrupt.h>

#include "spi.h"

void timer_init(void)
{
    cli();
    TCB0.CTRLB = TCB_CNTMODE_INT_gc; // Configure TCB0 in periodic interrupt mode
    TCB0.CCMP = 3333;                // Set interval for 1 ms (3333 clocks @ 3.333 MHz)
    TCB0.INTCTRL = TCB_CAPT_bm;      // CAPT interrupt enable

    TCB0.CTRLA = TCB_ENABLE_bm;      // Enable
    sei();
}

ISR(TCB0_INT_vect) {
    /** CODE: Write your code for Ex 9.3 within this ISR. */

    static uint8_t disp_toggle = 0;

    if (disp_toggle == 1) {

        spi_write(0b1000010);
        disp_toggle = 0;

    } else {

        spi_write(0b00100000);
        disp_toggle = 1;

    }

    TCB0.INTFLAGS = TCB_CAPT_bm;
}
