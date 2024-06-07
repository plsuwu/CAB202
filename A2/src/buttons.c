#include "buttons.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "timer.h"
#include "game.h"
#include "types.h"
#include "serial.h"
#include "display.h"

#define NUM_BUTTONS 4

u8 button[NUM_BUTTONS] = {
    PIN4_bm,
    PIN5_bm,
    PIN6_bm,
    PIN7_bm,
};

volatile u8 debounce = 0xff;
// volatile u8 fall = 0;
// volatile u8 rise = 0;

// debounce sampler
void update_edge(void) {

    static u8 count_a = 0;
    static u8 count_b = 0;

    u8 edge = debounce ^ PORTA.IN;
    u8 prev = debounce;

    count_a = ~count_a & edge;
    count_b = (count_b ^ count_a) & edge;
    debounce ^= (count_b & count_a) | (edge & prev);

    // mutate rise/fall to contain the debounced sample
    // fall = edge & prev;
    // rise = edge & debounce;
}

// u8 read_input(void) {
// }

ISR(TCB0_INT_vect) {

    // sample button press,
    // mutate `fall`/`rise` via pointers in `read_edge` func
    update_edge();
    ++elapsed_time;
    // ack int

    TCB0.INTFLAGS = TCB_CAPT_bm;
}
