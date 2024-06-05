#include "buttons.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "game.h"
#include "types.h"
#include "serial.h"

#define NUM_BUTTONS 4

u8 button[NUM_BUTTONS] = {
    PIN4_bm,
    PIN5_bm,
    PIN6_bm,
    PIN7_bm,
};

static volatile u8 debounce = 0xFF;
static volatile u8 fall = 0;
static volatile u8 rise = 0;

void tcb_button_init(void) {

    // TCB0 in periodic interrupt mode, with intent to read in button actuation
    TCB0.CTRLB = TCB_CNTMODE_INT_gc;                    // configure in periodic interrupt mode
    TCB0.CTRLA = TCB_CLKSEL_DIV2_gc;                    // CLKSEL / 2
    TCB0.CCMP = 4167;                                   // set period (~400Hz)
    TCB0.INTCTRL = TCB_CAPT_bm;                         // invoke CAPT ISR @ CCMP
    TCB0.CTRLA |= TCB_ENABLE_bm;                        // enable (avoid overwriting CLKSEL using `|=` )
}


// enables pullup resistors for all 4 buttons
void button_init(void) {
    PORTA.PIN4CTRL |= PORT_PULLUPEN_bm;
    PORTA.PIN5CTRL |= PORT_PULLUPEN_bm;
    PORTA.PIN6CTRL |= PORT_PULLUPEN_bm;
    PORTA.PIN7CTRL |= PORT_PULLUPEN_bm;
}

// debounce sampler
void read_edge(u8 *sample) {
    static u8 count_a = 0;
    static u8 count_b = 0;
    u8 edge = *sample ^ debounce;
    u8 prev = debounce;

    count_a = ~count_a & edge;
    count_b = (count_b ^ count_a) & edge;
    debounce ^= (count_b & count_a) | (edge & prev);

    // mutate rise/fall to contain the debounced sample
    fall = edge & prev;
    rise = edge & debounce;
}

u8 read_input(void) {
    // check if a bit representing a button is set in falling edge state
    // debounce in `read_edge()` inverts the value to be active HIGH
    for (u8 i = 0; i < NUM_BUTTONS; ++i) {
        if (fall & button[i]) {
            // return the current index if the bit in fall at that index's
            // bitmask is set (0-3)
            return i;
        }
    }

    return 0xFF;  // no button press, return 255
}

// maybe we want to generate this interrupt with PORTA input detection rather than a timer?
ISR(TCB0_INT_vect) {
    // sample button press,
    // mutate `fall`/`rise` via pointers in `read_edge` func
    u8 sample = PORTA.IN;
    read_edge(&sample);

    // ack int
    TCB0.INTFLAGS = TCB_CAPT_bm;
}
