#ifndef GAME_H
#define GAME_H

#include <avr/interrupt.h>
#include <avr/io.h>

#include "types.h"


#define DISP_SEG_B 0b01101111
#define DISP_SEG_C 0b01111011
#define DISP_SEG_E 0b01111110
#define DISP_SEG_F 0b00111111
#define DISP_OFF 0b01111111
#define DISP_LHS_BIT (1 << 7) // bit to set side to LHS

#define DISP_BITS_FE (DISP_SEG_E & DISP_SEG_F) // left segments F & E
#define DISP_BITS_BC (DISP_SEG_B & DISP_SEG_C) // right segments B & C
#define DISP_BITS_OK ~DISP_OFF // F, E, B, C segments (sequence OK)
// #define DISP_BITS_OK (DISP_BITS_FE & DISP_BITS_BC) // F, E, B, C segments (sequence OK)
#define DISP_BITS_FL 0b01110111 // G segment (sequence fail)

#define RESULT_OK 0x4
#define RESULT_FL 0X5
#define LFSR_SEED 0x09669396
#define LFSR_MASK 0xE2023CAB
#define SEQUENCE_MAX 255

#define DEBUG_DELAY 250        // ms
#define DEBUG_DELAY_HALF 125    // ms

typedef enum {
    START,
    SEQUENCE,
    AWAIT_INPUT,
    RESULT,
    OK,
    FAIL_SEGMENT,
    FAIL_SCORE
} game_phase_t;

typedef enum {
    LHS = DISP_LHS_BIT,
    RHS = 0b00000000,
    TGL = 0xFF,
} disp_side_t;

bool_t peripheral_activity;
game_phase_t game_phase;
u8 sequence[SEQUENCE_MAX];
u8 user_input[SEQUENCE_MAX];
u8 sequence_length;
u8 sequence_idx;
u8 user_idx;
u32 lfsr_state;


void game_init(void);
// void game_start(void);
void game_tick(void);

#endif  // GAME_H
