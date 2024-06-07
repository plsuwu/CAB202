#include "buzzer.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "game.h"
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

// void buzzer_init(void) {
//     // waveform output controls:
//     // CMP0EN => BUZZER  on PB0 / TCA0WO0 / PORTB PIN0
//     // CMP1EN => DISP EN on PB1 / TCA1WO1 / PORTB PIN1
//
//     //         0: BUZZER | 1: DISP EN
//     PORTB.DIR |= PIN0_bm | PIN1_bm;
//     TCA0.SINGLE.CTRLB =
//         TCA_SINGLE_WGMODE_SINGLESLOPE_gc | TCA_SINGLE_CMP0EN_bm | TCA_SINGLE_CMP1_bm;
//     TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV2_gc;
//
//     TCA0.SINGLE.PER = E_HI_CLK_BASE;  // start with high E
//     TCA0.SINGLE.CMP1 = E_HI_CLK_BASE + 1;             // 0% duty cycle to start display disabled
//     TCA0.SINGLE.CMP0 = 0;             // 0% duty cycle to start buzzer disabled
//
//     TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;  // set TCA_SINGLE enable bit
// }

// void play_tone(CurrentState *curr) {
//     // save the current state of the TCA0 PERIOD
//     TCA0.SINGLE.PERBUF = curr->tca_perbuf;
//     TCA0.SINGLE.CMP0BUF = curr->tone_cmpbuf;        // activate @ 50% duty cycle
// };
