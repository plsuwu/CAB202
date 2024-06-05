#ifndef GAME_H
#define GAME_H

#include <avr/interrupt.h>
#include <avr/io.h>

#include "types.h"

#define DISP_LHS_BIT 0b10000000 // bit to set side to LHS

#define DISP_BITS_FE 0b00111110 // left segments F & E
#define DISP_BITS_BC 0b01101011 // right segments B & C
#define DISP_BITS_OK 0b00101010 // F, E, B, C segments (sequence OK)
#define DISP_BITS_FL 0b01110111 // G segment (sequence fail)

#define RESULT_OK 0x4
#define RESULT_FL 0X5
#define LFSR_SEED 0x09669396
#define LFSR_MASK 0xE2023CAB
#define SEQUENCE_MAX 255
#define DEBUG_DELAY 250        // ms
#define DEBUG_DELAY_HALF 125    // ms

typedef enum {
    IDLE,
    SEQUENCE,
    USER,
    RESULT
    // PASS,
    // FAIL
} game_phase_t;

typedef enum {
    FE = DISP_BITS_FE,
    BC = DISP_BITS_BC,
    OK = DISP_BITS_OK,
    FL = DISP_BITS_FL,
} segment_t;

typedef enum {
    LHS = DISP_LHS_BIT,
    RHS = 0,
    TGL = 0xFF
} disp_side_t;

typedef struct {
    // u16 tca_prev_per;
    u16 tca_perbuf;
    u16 tone_cmpbuf;
    u16 disp_cmpbuf;
    segment_t disp_segment;
    disp_side_t disp_side;
} CurrentState;

game_phase_t game_phase;

void game_init(void);
void game_start(void);
void game_tick(void);

#endif  // GAME_H
