#include "display.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "game.h"
#include "types.h"
#include "timer.h"
#include "serial.h"

volatile u8 left_byte = DISP_OFF | DISP_LHS_BIT;
volatile u8 right_byte = DISP_OFF;
// volatile digit_side_t display_side = left;

const u8 segment_map[10] = {
//  00SFABGCDE
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

void display_score(u8 score) {
    // u8 range = score % 100;

    u8 tens_segment = DISP_OFF;
    u8 tens;
    u8 ones = score % 10;
    u8 ones_segment = segment_map[ones];

    if (score >= 10) {
        tens = score / 10;
        tens_segment = segment_map[tens];
    }

    update_display(tens_segment, ones_segment);
    elapsed_time = 0;
    peripheral_activity = true;
}

void update_display(const u8 left, const u8 right) {
    left_byte = left | DISP_LHS_BIT;
    right_byte = right;

    // elapsed_time = 0;
}

void display_step(u8 step) {
    static u8 pattern_map[4][2] = {
        {DISP_BITS_FE, DISP_OFF}, // segments E & F => left bar
        {DISP_BITS_BC, DISP_OFF}, // segments C & C => right bar
        {DISP_OFF, DISP_BITS_FE},
        {DISP_OFF, DISP_BITS_BC},
    };

    update_display(pattern_map[step][0], pattern_map[step][1]);
}


// SPI INT latch pulse handler
ISR(SPI0_INT_vect) {

    // latch data on PIN1
    PORTA.OUTCLR = PIN1_bm; // drive high - pulse start
    PORTA.OUTSET = PIN1_bm;  // drive low - pulse end

    SPI0.INTFLAGS = SPI_IF_bm; // interrupt ack
}

ISR(TCB1_INT_vect) {

    static volatile digit_side_t current_side = left;

    if (current_side == left) {

        SPI0.DATA = left_byte;
    } else {

        SPI0.DATA = right_byte;
    }

    current_side ^= 1;
    TCB1.INTFLAGS = TCB_CAPT_bm;
}
