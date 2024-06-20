#include "game.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "buttons.h"
#include "buzzer.h"
#include "display.h"
#include "init.h"
#include "lfsr.h"
#include "timer.h"
#include "types.h"

// stores the current sequence; we init with a
// buffer for 255 bytes/255 steps
u8 sequence[SEQUENCE_MAX];
bool_t peripheral_activity = false;
u32 lfsr_state = LFSR_SEED;

// volatile u8 round_result;

volatile u8 button_debounce = 0xff;
volatile u8 button_state_prev;
volatile u8 button_state_curr;
volatile u8 button_fall;
volatile u8 button_rise;

// handle general display/buzzer setting and activation
// calls as required by the current step in the sequence
void peripheral_on(u8 *step) {
    update_playback_delay();
    peripheral_activity = true; // activate state-tracking flags,
    play_tone(*step);           // write a pointer to step's tone CLK count
                                // into TCA0.CMP0BUF & set to 50% duty cycle,

    display_step(*step);        // pass the step's display data to SPI0.DATA;
                                //  -> interrupt-driven, so the ISR sends a
                                //      pulse to the DISP LATCH as required

}

// set buzzer to 0% duty cycle, set all segment bits on the display
void peripheral_off(void) {
    peripheral_activity = false; // clear state flag
    stop_tone();
    update_display(DISP_OFF, DISP_OFF);
}

u8 fail_routine(u8 reps) {
    // similar to the main game loop state machine, we return the
    // next expected value to proceed into the following case,
    // or idle and hit the default return case while we wait
    switch (reps) {
    case 0:
        // wait the TCB0 timer before continuing each step
        if (elapsed_time >= playback_delay) {
            display_score(sequence_length);
            return 1;
        }
        break;

    case 1:
        if (elapsed_time >= playback_delay) {
            peripheral_off();
            return 2;
        }
        break;

    case 2:
        if (elapsed_time >= playback_delay) {

            // set head index of sequence to <sequence_length + 1> and
            // incrementally generate the next round over any existing steps
            sequence[0] = set_step(&lfsr_state);

            // call the game init function to set indexing variables to their
            // initial state
            game_reset(&sequence_length, &sequence_index, &user_index);
            elapsed_time = 0;
            return 3;
        }
        break;

    default:
        return reps;
        break;
    }

    return reps;
}

// main game loop statem machine
void game_loop(void) {

    // create the first item in the sequence and set indexing variables to
    // indicate our starting point to the gamestate
    sequence[0] = set_step(&lfsr_state);
    game_reset(&sequence_length, &sequence_index, &user_index);
    u8 button_press, button_release = 0;
    u8 reps; // repetition counter for the FAIL case

    game_phase_t game_phase = START;
    while (1) {
        /*
         *  THE GAME LOOP:
         *
         *           ---- if more notes,
         *          |     loop back to
         *          v     start case                     ---<----<---- if yes, loop back to
         *          |       ^                           |                wait for input
         *          |       |                           v                    ^
         *          v       |                  our turn ends,                |
         *|--> [start]-> play item in sequence ->  wait for user input  if ok, check if  ->-->  if no, flash OK segment  -->-|
         *        ^                                                     notes to play           generate a new tone and
         *        |                                    |                there are more          put it at the end of the
         *        |                                    v                   ^                    sequence. loop back to
         *        |                           play the input               |                    starting state
         *        ^                           tone and display  ->    check their note
         *        |                                                  matched the respective
         *        |                                                  note in our sequence
         *        ^                                                        |
         *        |                          if no, reset game             v
         *         -------<-------<-------   and loop back to    <------<--
         *                                   start
         */

        switch (game_phase) {

        case START:
            if (sequence_index < sequence_length) {
                // turn buzzer & display on (the function below has a number of
                // side effects so timer updates are handled by `update_display()`
                //  -> display_step/results/score similarly set the `peripheral_activity`
                //      to true.
                peripheral_on(&sequence[sequence_index]);
                sequence_index += 1;
                game_phase = SEQUENCE; // set playback state
            } else {
                // if there are no more items left to play, move to wait for
                // user input
                game_phase = AWAIT_INPUT;
            }

            break;

        // our turn: playback state
        case SEQUENCE:
            // wait until 50% of the playback delay has elapsed, then set the displays to
            // 0b01111111 (i.e, set all bits to clear the segments) and set buzzer duty
            // cycle to 0%
            if (peripheral_activity && elapsed_time > (playback_delay >> 1)) {
                peripheral_off();

            } else if (!peripheral_activity && elapsed_time > (playback_delay >> 1)) {
                // the peripheral_off() call above sets the peripheral_activity and
                // elapsed time
                game_phase = START;
            }

            break;

        case AWAIT_INPUT:
            // user turn: idle while wait input
            button_release = 1;
            // if a falling edge is detected on one of the 4 buttons,
            // read its bit position
            if (button_fall & (PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm)) {
                button_press = get_falling_pin(button_fall);
                peripheral_on(&button_press);
                button_release = 0; // button is held, peripheral_activity stays on
                                    // until released.

                game_phase = RESULT;
            }

            break;

        case RESULT:
            // user turn: wait for the timer to hit 50% of its playback delay,
            // or for the user to release the button (whichever comes first)
            if (!button_release) {
                if (get_rising_pin(button_rise) == button_press + 1)
                    button_release = 1;

            // once button is released, check that we have waited out the minimum
            // time period before we deactivate peripheral activity
            } else if (elapsed_time >= (playback_delay >> 1) && button_release) {
                peripheral_off();

                if (sequence[user_index] == button_press) {
                    user_index += 1;

                    // check that the sequence is complete
                    if (user_index < sequence_length) {
                        elapsed_time = 0;
                        game_phase = AWAIT_INPUT;

                    } else {
                        display_result(1);
                        game_phase = OK;
                    }

                // if the user's input doesnt match the item in our sequence,
                // move to FAIL state immediately
                } else if (sequence[user_index] != button_press) {
                    display_result(0);
                    game_phase = FAIL;
                }
            }

            break;

        case OK:
            // display OK segments and deactivate peripherals after the fully
            // playback delay time
            if (peripheral_activity && elapsed_time >= (playback_delay)) {
                peripheral_off();
            }

            // wait another 50% delay period, generate a new step and add it to
            // the sequence, and finally loop back to start
            else if (!peripheral_activity && elapsed_time >= (playback_delay >> 1)) {
                sequence[sequence_length] = set_step(&lfsr_state);
                sequence_length += 1;
                sequence_index = 0;
                user_index = 0;
                game_phase = START;
            }

            break;

        case FAIL:
            // control the peripheral activity and other waiting periods through a
            // second 'fail_routine' state machine function
            reps = fail_routine(reps);
            if (reps > 2) {
                reps = 0;
                game_phase = START;
            }

            break;

        default:
            // catch a weird state path and loop back to the machine's start
            game_phase = START;
            break;
        }
    }
}
