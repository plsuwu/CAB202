#ifndef GAME_H
#define GAME_H

#include <avr/interrupt.h>
#include <avr/io.h>

#include "buttons.h"
#include "types.h"

#define RESULT_OK 0x4
#define RESULT_FL 0X5
#define LFSR_SEED 0x09669396
#define LFSR_MASK 0xE2023CAB
#define SEQUENCE_MAX 255

typedef enum {
    START,
    SEQUENCE,
    AWAIT_INPUT,
    RESULT,
    OK,
    FAIL,
} game_phase_t;

u8 sequence[SEQUENCE_MAX];   // the current sequence
u8 user_input[SEQUENCE_MAX]; // tracks the user's input (do we use this?)
u32 lfsr_state;              // the state of the LFSR's seed
game_phase_t game_phase;     // describes the state machine's phase
bool_t peripheral_activity;  // descibes the state of the LED/buzzer

u8 sequence_length;          // user's score + the current 'level'
u8 sequence_index;           // our current position in the sequence
u8 user_index;               // user's current position in their sequence

extern volatile u8 button_press;
extern volatile u8 button_state_prev;
extern volatile u8 button_state_curr;

void game_loop(void);        // call the game loop frame

#endif // GAME_H
