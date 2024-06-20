#include "buttons.h"
#include <avr/interrupt.h>
#include <avr/io.h>

#include "display.h"
#include "game.h"
#include "timer.h"
#include "types.h"

// finds the switch that was released, if any
u8 get_rising_pin(u8 button) {
    if (button & PIN4_bm)
        return 1;

    else if (button & PIN5_bm)
        return 2;

    else if (button & PIN6_bm)
        return 3;

    else if (button & PIN7_bm)
        return 4;

    else
        return 0; // switch not released, return garbage
}

// find which button was pressed
u8 get_falling_pin(u8 button) {
    if (button & PIN4_bm)
        return 0;

    else if (button & PIN5_bm)
        return 1;

    else if (button & PIN6_bm)
        return 2;

    else if (button & PIN7_bm)
        return 3;

    else
        return 4; // we have pre-checked that there was an active pin,
                  // so we should never hit this state, but we wont
                  // get good data from returning a 4 and
                  // shoud just loop anyway regardless.
}

void update_edge(void) {
    static u8 count_a = 0;
    static u8 count_b = 0;

    button_state_curr = PORTA.IN ^ button_debounce;
    button_state_prev = button_debounce;

    count_b = (count_b ^ count_a) & button_state_curr;
    count_a = ~count_a & button_state_curr;

    button_debounce ^= (count_b & count_a) | (button_state_curr & button_state_prev);
    button_fall = button_state_curr & button_state_prev;
    button_rise = button_state_curr & button_debounce;
}

ISR(TCB0_INT_vect) {

    // samplet the state of the button pins on PORTA.IN and
    // alter `fall`/`rise` via pointers in `` func
    update_edge();
    elapsed_time += 1; // increment timer used for playback delay

    TCB0.INTFLAGS = TCB_CAPT_bm; // ack interrupt
}
