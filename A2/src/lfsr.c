#include "lfsr.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#include "game.h"
#include "types.h"

// return a u8 reflecting the next note + button in the sequence
u8 generate_step(u32 *state) {
    // get the least significant bit from the lfsr_state, shift the lsb off lfsr_state
    u8 lsb = (*state & 1);
    *state = *state >> 1;

    // if lsb == 1, XOR lfsr_state with the mask constant
    if (lsb) {
        *state ^= LFSR_MASK;
    }

    // get the least significant two bits in lfsr_state (will represent a value from 0-3)
    // and return that value as a u8 integer to determine the next step in the game's sequence
    return (*state & 0b11);
};

u8 set_step(u32 *state) {
    // use sequence_lengthgth to get the current step in the sequence array,
    // then assign the output from `lfsr_next` to that index
    return generate_step(state);
}

// void reset_lfsr(void) {
//     lfsr_state = LFSR_SEED;
//
// }

