#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>

#include "game.h"
#include "main.h"
#include "types.h"

// we probably want to move these defs out of here
volatile per_state_t per_state = PER_OFF;
volatile per_state_t bzr_state = PER_OFF;
static volatile u16 curr_ints = 0;

volatile u8 DISP_CSIDE = 0; // bool to alternate byte to send
volatile u8 disp_seg = 0b11111111;

volatile u8 debounce = 0xFF;
volatile u8 fall = 0;
volatile u8 rise = 0;

void check_edge(u8 *sample) {
    static u8 count_a = 0;
    static u8 count_b = 0;

    u8 edge = *sample ^ debounce;
    u8 prev = debounce;

    count_a = ~count_a & edge;
    count_b = (count_b ^ count_a) & edge;

    debounce ^= (count_b & count_a) | (edge & prev);

    fall = edge & prev;
    rise = edge & debounce;

    return;
}

u8 read_spi(void) { return SPI0.DATA; }
// void write_spi(u8 byte) { SPI0.DATA = byte; }

ISR(TCB1_INT_vect) {
    write_spi(disp_seg);

    DISP_CSIDE ^= 1;
    TCB1_INTFLAGS = TCB_CAPT_bm;
}

ISR(TCB0_INT_vect) {
    // sample buttons & mutate `fall`/`rise`
    u8 sample = PORTA.IN;
    check_edge(&sample);

    // set peripheral state (ON/OFF)
    switch (per_state) {
    case PER_OFF:
        if ((fall & PIN4_bm) && !my_turn) {
            per_state = PER_ON;
            steps_correct++;
            my_turn = 1;
            break;
        }
    case PER_ON:
        if ((!fall || rise & PIN4_bm) && !my_turn) {
            per_state = PER_OFF;
            break;
        }
    }

    switch (bzr_state) {
    case PER_OFF:
        if (my_turn) {
            if (curr_ints == 400) {
                if (steps_played < step_sequence) {
                    steps_played--;
                    bzr_state = PER_ON;
                    lfsr(&rand_state, &step_state);
                    check_step(&step_state);
                    curr_ints = 0;
                } else {
                    my_turn = 0;
                }
            } else {
                curr_ints += 1;
            }
        }
        break;

    case PER_ON:
        if (my_turn) {
            if (curr_ints == 400 && my_turn) {
                steps_played++;

                bzr_state = PER_OFF;
                curr_ints = 0;
            } else {
                curr_ints += 1;
            }
        }

        break;
    }

    TCB0_INTFLAGS = TCB_CAPT_bm;
}

ISR(SPI0_INT_vect) {

    // latch data
    PORTA.OUT &= ~PIN1_bm; // drive high to begin pulse
    PORTA.OUT |= PIN1_bm;  // drive back low to finish pulse

    SPI0.INTFLAGS = SPI_IF_bm; // interrupt ack
}
