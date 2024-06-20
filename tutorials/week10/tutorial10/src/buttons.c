#include "buttons.h"

#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t pb_debounced_state = 0xFF;

void buttons_init(void)
{
    // Enable pull-up resistors for PBs
    PORTA.PIN4CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN5CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN6CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN7CTRL = PORT_PULLUPEN_bm;

    // Configure timer for PB sampling
    cli();
    TCB0.CTRLB = TCB_CNTMODE_INT_gc; // Configure TCB0 in periodic interrupt mode
    TCB0.CCMP = 3333;                // Set interval for 1 ms (3333 clocks @ 3.333 MHz)
    TCB0.INTCTRL = TCB_CAPT_bm;      // CAPT interrupt enable
    TCB0.CTRLA = TCB_ENABLE_bm;      // Enable
    sei();
}

// Timer ISR; samples pushbuttons
ISR(TCB0_INT_vect) {
    static uint8_t count = 20;

    uint8_t pb_sample = PORTA.IN;
    uint8_t pb_changed = pb_sample ^ pb_debounced_state; // Detect change to PB
    if (pb_changed) {
        count--;
        if (!count) {
            pb_debounced_state = pb_sample;
            count = 20;
        }
    } else {
        count = 20;
    }



    TCB0.INTFLAGS = TCB_CAPT_bm; // Acknowledge interrupt
}
