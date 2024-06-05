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

u8 cross_ref(volatile u8 *s, u8 *btn) {
    // if (*btn == *s) {
    if (*btn == 0) { // change this to above!!
        return 1;
    }

    return 0;
}




void play(u8 *result) {

    PORTB.OUTCLR = PIN0_bm | PIN1_bm;
    TCA0.SINGLE.CMP1BUF = TCA0.SINGLE.PER + 1;
    steps_correct = 1;

    while (1) {

        // *result = ADC0.RESULT0;
        // if (prev_pot != *result) {
        //     prev_pot = *result;
        // }
        if (steps_correct == 0 && my_turn) {
            // display fail + set
            //          step_state = prev_step_state;
            //          step_sequence = 1;
            printf("shouldnt hit the fail check but we have/.....");
            printf("steps_correct: %u, my_turn? %u", steps_correct, steps_correct);


        }

        // if (steps_played && steps_played == step_sequence) {
        //     my_turn = 0;
        //     steps_played = 0;
        //     steps_correct_prev = steps_correct;
        //     step_sequence++;
        // }

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
}
