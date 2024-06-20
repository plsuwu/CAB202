#include "buzzer.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "types.h"

volatile u8 is_playing = 0;
static i8 selected_tone = 0;
static i8 octave = 0;

void increase_octave(void) {
    if (octave < MAX_OCTAVE) {
        ++octave;
        if (is_playing) {
            play_tone(selected_tone);
        }
    }
}

void decrease_octave(void) {
    if (octave > MIN_OCTAVE) {
        --octave;
        if (is_playing) {
            play_tone(selected_tone);
        }
    }
}

void play_tone(u8 tone) {
    // frequency-to-clock period:
    // -------------------------------------------
    // E (Hi)   -> 371.580 Hz : ~4485.349 cycles : E_HI_CLK_BASE
    // C#       -> 312.460 Hz : ~5334.009 cycles : C_CLK_BASE
    // A        -> 496.000 Hz : ~3360.215 cycles : A_CLK_BASE
    // E (Low)  -> 185.790 Hz : ~8970.699 cycles : E_LO_CLK_BASE
    static const u16 period_map[4] = {
        E_HI_CLK_BASE,
        C_CLK_BASE,
        A_CLK_BASE,
        E_LO_CLK_BASE,
    };

    u16 tone_period = period_map[tone];
    TCA0.SINGLE.PERBUF = tone_period;
    TCA0.SINGLE.CMP0BUF = tone_period >> 1;
    is_playing = 1;
}

void stop_tone(void) {
    TCA0.SINGLE.CMP0BUF = 0;
    is_playing = 0;
}
