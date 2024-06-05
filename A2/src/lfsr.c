#include "lfsr.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#include "game.h"
#include "types.h"

// return a u8 reflecting the next note + button in the sequence
u8 lfsr_next(u32 *current_state, u8 *sequence_length) {
    // get the least significant bit from the current_state, shift the lsb off current_state
    u8 lsb = (*current_state & 1);
    *current_state = *current_state >> 1;

    // if lsb == 1, XOR current_state with the mask constant
    if (lsb) {
        *current_state ^= LFSR_MASK;
    }

    // get the least significant two bits in current_state (will represent a value from 0-3)
    // and return that value as a u8 integer to determine the next step in the game's sequence
    return *current_state & 0b11;
};
