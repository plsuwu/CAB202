#include "lfsr.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#include "game.h"
#include "types.h"

// returns an unsigned 8-bit int reflecting the current step in the sequence
u8 generate_step(u32 *state) {
    // get the least significant bit from the lfsr_state, shift the lsb off
    // lfsr_state
    u8 lsb = (*state & 1);
    *state = *state >> 1;

    // if the least significant bit in the previous state was a 1, XOR
    // lfsr_state with the mask constant
    if (lsb) {
        *state ^= LFSR_MASK;
    }

    // return the last two bits from the shifted state value (will be a u8
    // in the inclusive range 0-3)
    return (*state & 0b11);
};

// takes a pointer to the current state.
// handles IO between the linear feedback shift register function,
// `generate_step` and the main game loop
u8 set_step(u32 *state) {
    return generate_step(state);
}
