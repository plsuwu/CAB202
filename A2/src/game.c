#include <avr/io.h>
#include <avr/interrupt.h>

#include "com.h"
#include "typing.h"

const u32 LFSR_SEED = 0x9669396;
const u32 LFSR_MASK = 0xE2023CAB;
const u16 TONE_BASE_A = 496;        // 0x96693[96] -> 4[96]

const u16 K_512     = 24548;
const u16 K_812     = 20643;
const u16 K_1712    = 12274;

// better definition for this i think
typedef enum u8 {
    ONE = 1,
    TWO = 2,
    THREE = 3,
    FOUR = 4
} step_state_t;

u32 gamestate = LFSR_SEED;
step_state_t stepstate = ONE;

// implemented through UART `INC/DEC FREQ` function;
// called via an ISR??
//
// also, this function also calculates the frequencies as if they
// are 1-to-1 - which i dont think they are.
void freq(u8 oct, u32 note, u32 *tone) {
    u32 tmp = TONE_BASE_A;

    /*
     * 1 -> E (hi)  -> BTN_1
     * 2 -> C#      -> BTN_2
     * 3 -> A       -> BTN_3
     * 4 -> E (lo)  -> BTN_4
     */

    switch (note) {
        case 1:
            tmp *= K_512;
            break;
        case 2:
            tmp *= K_812;
            break;
        case 4:
            tmp *= K_1712;
            break;
        default:
            break;
    };

    if (note != 3) {
        // the note isnt the base A tone
        tmp += (1 << 14);
        *tone = tmp >> 15;
    } else {
        // the note is the base A tone
        // (i.e, no scaling necessary)
        *tone = tmp;
    }

    *tone *= oct;
    // tone should be in range: `20hz <= tone <= 20Khz`
}

void lfsr(u32 *state, u8 *step) {
    u8 lsb = (*state & 1);
    *state = *state >> 1;
    if (lsb) {
        *state = *state ^ LFSR_MASK;
    }

    *step = *state & 0b11;
}








