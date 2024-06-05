#include "buzzer.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "game.h"
#include "types.h"

void buzzer_init(void) {
    // waveform output controls:
    // CMP0EN => BUZZER  on PB0 / TCA0WO0 / PORTB PIN0
    // CMP1EN => DISP EN on PB1 / TCA1WO1 / PORTB PIN1

    //         0: BUZZER | 1: DISP EN
    PORTB.DIR |= PIN0_bm | PIN1_bm;
    TCA0.SINGLE.CTRLB =
        TCA_SINGLE_WGMODE_SINGLESLOPE_gc | TCA_SINGLE_CMP0EN_bm | TCA_SINGLE_CMP1_bm;
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV2_gc;

    TCA0.SINGLE.PER = E_HI_CLK_BASE;  // start with high E
    TCA0.SINGLE.CMP1 = 0;             // display disabled
    TCA0.SINGLE.CMP0 = 0;             // buzzer disabled

    TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;  // set TCA_SINGLE enable bit
}

void play_tone(CurrentState *curr, u8 prev_period) {
    // save the current state of the TCA0 PERIOD
    TCA0.SINGLE.PERBUF = curr->tca_perbuf;

    while (prev_period == TCA0.SINGLE.PER)      // wait for buffer to hit PER
        ;                                       // (should prob be non-blocking)

    TCA0.SINGLE.CMP0BUF = curr->tone_cmpbuf;    // activate @ 50% duty cycle
};
