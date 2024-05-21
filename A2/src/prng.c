#include "prng.h"
#include "init.h"

#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

uint32_t state = 0x09669396;

// p-rand number gen via linear feedback shift register
void lfsr(void) {
    uint8_t lsb = state & 1;
    state = state >> 1;

    if (lsb == 1) {
        state = state ^ MASK_XOR_STATE;
    }

    if (debug == 1) {
        printf("%lu\n", state);
    }

}

// get the following step in the sequence based on the value of the lfsr state
uint8_t get_next_step() {
    switch (state & MASK_SEQ_STEP) {
        case 1:
            // step = 1:
            return 2;
        case 2:
            // step = 2:
            return 3;
        case 3:
            // step = 3:
            return 4;
        default:
            // step = 0:
            return 1;
    }
}
