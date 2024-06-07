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
#include "timer.h"
#include "types.h"

u8 user_input[SEQUENCE_MAX];
u8 sequence[SEQUENCE_MAX];

u32 lfsr_state = LFSR_SEED;
volatile u8 round_result;
// u8 sequence_idx;
// u8 user_idx;
bool_t peripheral_activity = false;

void display_result(u8 result) {
    if (result) {
        // user input matched our sequence, show OK segments
        update_display(0,0);
    } else {
        update_display(DISP_BITS_FL, DISP_BITS_FL);
    }

    peripheral_activity = true;
    elapsed_time = 0;

    // turn off display after waiting delay
}

void peripheral_on(u8 *step) {
    peripheral_activity = true;
    play_tone(*step);
    display_step(*step);
    elapsed_time = 0;
}

void peripheral_off(void) {
    peripheral_activity = false;
    stop_tone();
    update_display(DISP_OFF, DISP_OFF);
    elapsed_time = 0;
}

void game_tick(void) {
    game_phase_t game_phase = START;

    u8 button_state_prev = 0xff;
    u8 button_state_curr = 0xff;
    u8 button_current = 0;
    u8 button_fall, button_rise, button_released = 0;

    sequence_idx = 0;
    sequence_length = 1;
    user_idx = 0;
    sequence[0] = set_step(&lfsr_state);

    while (1) {
        button_state_prev = button_state_curr;
        button_state_curr = debounce;

        button_rise = ~button_state_prev & button_state_curr;
        button_fall = button_state_prev & ~button_state_curr;

        switch (game_phase) {
            case START:
                if (sequence_idx < sequence_length) {
                    peripheral_on(&sequence[sequence_idx]);
                    sequence_idx += 1;
                    // peripheral_activity = true;
                    game_phase = SEQUENCE;
                } else {
                    game_phase = AWAIT_INPUT;
                }

                break;

            case SEQUENCE:

                if (peripheral_activity && elapsed_time > (playback_delay >> 1)) {
                    peripheral_off();
                } else if (!peripheral_activity && elapsed_time > (playback_delay >> 1)) {
                    game_phase = START;
                }

                break;

            case AWAIT_INPUT:

                button_released = 1;
                if (button_fall & (PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm)) {
                    if (button_fall & PIN4_bm) {
                        button_current = 0;
                    } else if (button_fall & PIN5_bm) {
                        button_current = 1;
                    } else if (button_fall & PIN6_bm) {
                        button_current = 2;
                    } else if (button_fall & PIN7_bm) {
                        button_current = 3;
                    }

                    peripheral_on(&button_current);
                    button_released = 0;

                    game_phase = RESULT;
                }

                break;

            case RESULT:

                if (!button_released) {
                    if (button_rise & PIN4_bm && button_current == 0) {
                        button_released = 1;
                    } else if (button_rise & PIN5_bm && button_current == 1) {
                        button_released = 1;
                    } else if (button_rise & PIN6_bm && button_current == 2) {
                        button_released = 1;
                    } else if (button_rise & PIN7_bm && button_current == 3) {
                        button_released = 1;
                    }
                } else if (elapsed_time >= (playback_delay >> 1) && button_released) {
                    peripheral_off();

                    if (sequence[user_idx] == button_current) {
                        user_idx += 1;
                        if (user_idx < sequence_length) {
                            elapsed_time = 0;
                            game_phase = AWAIT_INPUT;
                        } else {
                            display_result(1);
                            game_phase = OK;
                        }
                    } else if (sequence[user_idx] != button_current) {
                        display_result(0);
                        game_phase = FAIL_SEGMENT;
                    }
                }

                break;

            case OK:

                if (peripheral_activity && elapsed_time >= (playback_delay >> 1)) {
                    peripheral_off();
                } else if (!peripheral_activity && elapsed_time >= (playback_delay >> 1)) {
                    sequence[sequence_length] = set_step(&lfsr_state);
                    sequence_length += 1;
                    sequence_idx = 0;
                    user_idx = 0;
                    game_phase = START;
                }

                break;

            case FAIL_SEGMENT:

                if (peripheral_activity && elapsed_time >= (playback_delay)) {
                    display_score(sequence_length);
                    game_phase = FAIL_SCORE;
                }

                break;

            case FAIL_SCORE:
                if (peripheral_activity && elapsed_time >= (playback_delay)) {
                    peripheral_off();
                    game_phase = FAIL_SCORE;
                } else if (!peripheral_activity && elapsed_time >= (playback_delay)) {

                    sequence_length = 1;
                    sequence_idx = 0;
                    user_idx = 0;
                    elapsed_time = 0;
                    game_phase = START;
                }

                break;


            default:
                game_phase = START;
                break;
        }
    }
}
