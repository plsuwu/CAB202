#include "display.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#include "timer.h"
#include "game.h"
#include "types.h"

volatile u8 left_byte = DISP_OFF | DISP_LHS_BIT;
volatile u8 right_byte = DISP_OFF;


void update_display(const u8 left, const u8 right) {
    left_byte = left | DISP_LHS_BIT;
    right_byte = right;

    elapsed_time = 0;
}

void display_score(u8 score) {
    // maps each number to a u8 bit string
    static const u8 segment_map[10] = {
    //  xxSFABGCDE
        0b00001000, // 0
        0b01101011, // 1
        0b01000100, // 2
        0b01000001, // 3
        0b00100011, // 4
        0b00010001, // 5
        0b00010011, // 6
        0b01001011, // 7
        0b00000000, // 8
        0b00000001, // 9
    };

    if (score < 10) {

        // do not allocate for the tens they are not needed
        update_display(DISP_OFF, segment_map[score]);
    } else {

        // approximation - not accurate for very large numbers,
        // but should be ok for a u8
        u8 tens = (segment_map[(score * 205) >> 11]);
        update_display(tens, segment_map[(score - 10 * tens)]);
    }

    // set state flags
    peripheral_activity = true;
}

void display_step(u8 step) {
    // map a step/note to their corresponding playback segment
    static const u8 pattern_map[4][2] = {
        {DISP_BITS_FE, DISP_OFF}, // segments E & F => left bar
        {DISP_BITS_BC, DISP_OFF}, // segments C & C => right bar
        {DISP_OFF, DISP_BITS_FE},
        {DISP_OFF, DISP_BITS_BC},
    };

    // send the bits & set state flags
    update_display(pattern_map[step][0], pattern_map[step][1]);
    peripheral_activity = true;
}

void display_result(u8 result) {
    if (result) {
        // user input matched our sequence, show OK segments
        update_display(0, 0);

    } else {
        update_display(DISP_BITS_FL, DISP_BITS_FL);
    }

    peripheral_activity = true;
}

// SPI interrupt handler
ISR(SPI0_INT_vect) {

    // send pulse to PORTA output on PIN1

    PORTA.OUTCLR = PIN1_bm; // drive high - pulse start
    PORTA.OUTSET = PIN1_bm; // drive low - pulse end

    SPI0.INTFLAGS = SPI_IF_bm; // ack interrupt
}

// TCB1 ----------------------------
// display refresh interrupt handler
ISR(TCB1_INT_vect) {
    // toggle sides with each interrupt
    static volatile digit_side_t current_side = left;

    if (current_side)
        SPI0.DATA = right_byte;
    else
        SPI0.DATA = left_byte;

    // XOR the displaying side to invert it, allowing the interrupt handler
    // to alternate sides each time it is raised
    current_side ^= 1;
    TCB1.INTFLAGS = TCB_CAPT_bm; // ack interrupt
}
