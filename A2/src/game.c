#include "game.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "buttons.h"
#include "buzzer.h"
#include "display.h"
#include "lfsr.h"
#include "serial.h"
#include "types.h"

game_phase_t game_phase = IDLE;
CurrentState current_state;

static u32 lfsr_state = LFSR_SEED;
volatile static u16 tca_prev_period = E_HI_CLK_BASE;
static u8 sequence_length = 0;
// static u8 current_index = 0;
// static u8 player_index = 0;

volatile static u8 user_input[SEQUENCE_MAX];
volatile static u8 sequence[SEQUENCE_MAX];

void generate_sequence(void) {
    // use sequence_length to get the current step in the sequence array,
    // then assign the output from `lfsr_next` to that index
    sequence[sequence_length - 1] = lfsr_next(&lfsr_state, &sequence_length);
}

void set_state_struct(u8 step, CurrentState* curr) {
    switch (step) {
        case 0:
            curr->tca_perbuf = E_HI_CLK_BASE;
            curr->tone_cmpbuf = 2240;
            curr->disp_cmpbuf = E_HI_CLK_BASE + 1;
            curr->disp_segment = FE;
            curr->disp_side = LHS;
            break;

        case 1:
            curr->tca_perbuf = C_CLK_BASE;
            curr->tone_cmpbuf = 2671;
            curr->disp_cmpbuf = C_CLK_BASE + 1;
            curr->disp_segment = BC;
            curr->disp_side = LHS;
            break;

        case 2:
            curr->tca_perbuf = A_CLK_BASE;
            curr->tone_cmpbuf = 1680;
            curr->disp_cmpbuf = A_CLK_BASE + 1;
            curr->disp_segment = FE;
            curr->disp_side = RHS;
            break;

        case 3:
            curr->tca_perbuf = E_LO_CLK_BASE;
            curr->tone_cmpbuf = 4480;
            curr->disp_cmpbuf = E_LO_CLK_BASE + 1;
            curr->disp_segment = BC;
            curr->disp_side = RHS;
            break;

        case 4:
            curr->tca_perbuf = MIN_CLK;
            curr->tone_cmpbuf = 0;
            curr->disp_cmpbuf = MIN_CLK + 1;
            curr->disp_segment = OK;
            curr->disp_side = TGL;
            break;

        case 5:
            curr->tca_perbuf = MIN_CLK;
            curr->tone_cmpbuf = 0;
            curr->disp_cmpbuf = MIN_CLK + 1;
            curr->disp_segment = FL;
            curr->disp_side = TGL;
            break;

    }
}

void stop_tone_disp(void) {
    // set buzzer & disp. duty cycle to 0%

    TCA0.SINGLE.CMP0BUF = 0;
    TCA0.SINGLE.CMP1BUF = 0;
}

void play_sequence(u8 length) {
    // run a round of the game

    printf("state_struct using segment(s): \n");
    for (u8 i = 0; i < length; ++i) {
        tca_prev_period = TCA0.SINGLE.PER;
        printf("\\%u        -> [%u]\n", i, current_state.disp_segment);
        set_state_struct(sequence[i], &current_state);
        play_tone(&current_state, tca_prev_period);
        display_segment(&current_state, tca_prev_period);

        // this is probably ok for debug but we ideally calculate clks
        _delay_ms(DEBUG_DELAY);
        stop_tone_disp();
    }
}

u8 compare_sequence() {
    for (u8 i = 0; i < sequence_length; ++i) {
        if (user_input[i] != sequence[i]) {
            return 0;
        }
    }

    return 1;
}

void get_user_input(void) {
    printf("need %u input(s), waiting... \n", sequence_length);
    for (u8 i = 0; i < sequence_length; ++i) {
        u8 button = 0xFF;

        button = read_input();
        while (button == 0xFF) {
            button = read_input();

            if (button != 0xFF) {
                break;
            }
        }

        printf("    ->[%u/%u]:          %u, \n", i + 1, sequence_length, button);

        set_state_struct(button, &current_state);

        play_tone(&current_state, tca_prev_period);
        display_segment(&current_state, tca_prev_period);
        _delay_ms(DEBUG_DELAY);
        stop_tone_disp();

        user_input[i] = button;
        _delay_ms(DEBUG_DELAY);
    }
}

void game_tick(void) {
    if (game_phase == SEQUENCE) {
        play_sequence(sequence_length);
        // _delay_ms(DEBUG_DELAY);
        printf("OK sequence: [ ");
        for (u8 i = 0; i < sequence_length; ++i) {
            printf("%u ", sequence[i]);
        }
        printf("]\n");
        game_phase = USER;
    }

    if (game_phase == USER) {
        get_user_input();
        game_phase = RESULT;
    }

    if (game_phase == RESULT) {
        printf("PLAYER sequence: [ ");
        for (u8 i = 0; i < sequence_length; ++i) {
            printf("%u, ", user_input[i]);
        }
        printf(" ]\n");



        u8 sequence_matches = compare_sequence();
        if (sequence_matches) {
            // ok -> incr. sequence length
            u8 okstate = 4;
            set_state_struct(okstate, &current_state);
            printf(">> disp_segment=> [%u]\n", current_state.disp_segment);

            display_segment(&current_state, tca_prev_period);

            PORTB.OUTCLR = PIN0_bm | PIN1_bm;
            TCA0.SINGLE.CMP1BUF = TCA0.SINGLE.PER + 1;
            _delay_ms(DEBUG_DELAY);

            stop_tone_disp();

            printf("OK\n\n");
            ++sequence_length;
            generate_sequence();
            game_phase = SEQUENCE;

        } else {
            // fail -> restart
            u8 failstate = 5;

            set_state_struct(failstate, &current_state);
            display_segment(&current_state, tca_prev_period);

            PORTB.OUTCLR = PIN0_bm | PIN1_bm;
            TCA0.SINGLE.CMP1BUF = TCA0.SINGLE.PER + 1;
            _delay_ms(DEBUG_DELAY_HALF);

            stop_tone_disp();
            _delay_ms(DEBUG_DELAY_HALF);

            display_score(sequence_length);
            PORTB.OUTCLR = PIN0_bm | PIN1_bm;
            TCA0.SINGLE.CMP1BUF = TCA0.SINGLE.PER + 1;
            _delay_ms(DEBUG_DELAY_HALF);

            stop_tone_disp();

            printf(">> disp_segment=> [%u]\n", current_state.disp_segment);
            printf("FAIL\n\n");

            game_start();
        }
    }
}

// game reset
void game_start(void) {
    sequence[0] = sequence[sequence_length -1];
    sequence_length = 1;

    game_phase = SEQUENCE;
}
