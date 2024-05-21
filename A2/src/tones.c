#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "prng.h"
#include "init.h"

/* *
 *                  `tone = 4[xy] * 2^(-5/12)`
 *
 * - we avoid flops and division with scaling and quantizing:
 * ```
 *  k = 2^15                          // scale
 *  q = (2^(-5 / 12))) * k            // quant
 * ```
 * - q has been precalculated manually and rounded to a whole int and hardcoded.
 * - using the value of A_BASE_CONST, we use the below values + bitshifting in
 *      the func `set_tone()` which returns the tone value for any given note:
 *
 *              `tone = ((4[xy] * q) + (1 << 14)) >> 15`
 * */
const uint16_t k_e_hi               = 24548;
const uint16_t k_c                  = 20643;
const uint16_t k_e_lo               = 12274;
const uint16_t A_BASE_TONE          = 496; // this is supposed to be the highest tone!!

/*
 * reverse floating point value scaling to find the value for each tone in Hz
 * */
uint32_t parse_tone(uint8_t oct, uint32_t note) {
    uint32_t tone = A_BASE_TONE;

    switch (note) {
        case 1:
            tone *= k_e_hi;
            break;
        case 2:
            tone *= k_c;
            break;
        case 3:
            break;
        default:
            tone *= k_e_lo;
            break;
    };

    // required tone is A
    if (note == 3) {
        // do note ops here (e.g +/- octave) and return its result
        return tone;
    }

    // find required tone relative to A
    tone += (1 << 14);
    return tone >> 15;
}
