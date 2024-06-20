#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "game.h"
#include "debug.h"
#include "init.h"
#include "isr.h"
#include "serial.h"
#include "types.h"


// static volatile u8 prev_pot = 0;
// static volatile u8 curr_ints = 0;
volatile u16 required_ints = 12;
volatile u8 my_turn = 1;

// implemented through UART `INC/DEC FREQ` function;
// called via an ISR??
//
void freq(u8 oct, u32 *step) {
    // check that the requested octave is `20Hz <= OCT <= 20kHz`
    if (oct > 3) {
        oct = 3;
    }
    /*
     * 0 -> E (hi)  -> BTN_1
     * 1 -> C#      -> BTN_2
     * 2 -> A       -> BTN_3
     * 3 -> E (lo)  -> BTN_4
     */
    switch (*step) {
        case 0:
            TCA0.SINGLE.PERBUF = Eh_CCNT;
            disp_seg = SEG_STATE_FE | SEG_DISP_LHS;
            break;
        case 1:
            TCA0.SINGLE.PERBUF = C_CCNT;
            disp_seg = SEG_STATE_BC | SEG_DISP_LHS;
            break;
        case 2:
            TCA0.SINGLE.PERBUF = A_CCNT;
            disp_seg = SEG_STATE_FE;
            break;
        case 3:
            TCA0.SINGLE.PERBUF = El_CCNT;
            disp_seg = SEG_STATE_BC;
            break;
    }
}

void lfsr(volatile u32 *state, volatile u8 *step) {
    u8 lsb = (*state & 1);
    *state = *state >> 1;
    if (lsb) {
        *state = *state ^ LFSR_MASK;
    }

    *step = *state & 0b11;
}

void check_step(volatile u8 *step) {
    switch (*step) {
    case 0:
        disp_seg = SEG_STATE_FE | SEG_DISP_LHS;
        break;
    case 1:
        disp_seg = SEG_STATE_BC | SEG_DISP_LHS;
        break;
    case 2:
        disp_seg = SEG_STATE_FE;
        break;
    default:
        disp_seg = SEG_STATE_BC;
        break;
    }
}

void play(u8 *result) {

    PORTB.OUTCLR = PIN0_bm | PIN1_bm;
    TCA0.SINGLE.CMP1BUF = TCA0.SINGLE.PER + 1;
    steps_correct = 1;

        if (bzr_state) {
            TCA0.SINGLE.CMP0BUF = TCA0.SINGLE.PER / 2;
            TCA0.SINGLE.CMP1BUF = TCA0.SINGLE.PER + 1;
            write_spi(disp_seg);
            prev_step_state = step_state;
            printf("next: %u\n", step_state);
        } else {
            TCA0.SINGLE.CMP0BUF = 0;
            TCA0.SINGLE.CMP1BUF = 0;
            write_spi(0xFF);
            printf("in NOT ACTIVE BZR state!!");
        }
}
