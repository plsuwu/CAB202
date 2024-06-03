#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
// DO NOT USE!!
// #include "qutyio.h"
// DISCLUDE IN SUBMISSION!!!!

// uintx_t is really annoying to type out so i cosplay rust types
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

u32 state = 0x09669396;
u32 sequence_length = 1;

const u32 MASK_XOR_STATE = 0xE2023CAB;
const u8 MASK_SEQ_STEP = 0x3;

const u16 A_BASE_TONE = 496; // this is supposed to be the highest tone!!
const u16 k_n5_12 = 24548;
const u16 k_n8_12 = 20643;
const u16 k_n17_12 = 12274;

u32 playback_freq(u8 oct, u32 note) {
    u32 tone = A_BASE_TONE;

    // 1 -> E (High),
    // 2 -> C#,
    // 3 -> A,
    // 4 -> E (Low),

    switch (note) {
    case 1:
        tone *= k_n5_12;
        break;
    case 2:
        tone *= k_n8_12;
        break;
    case 3:
        break;
    default:
        tone *= k_n17_12;
        break;
    };
    if (note == 3) {
        return tone;
    }
    tone += (1 << 14);
    return tone >> 15;
}

void linear_feedback_shift_reg(void) {
    u8 lsb = state & 1;
    state = state >> 1;
    if (lsb == 1) {
        state = state ^ MASK_XOR_STATE;
    }
}

u8 get_next_step() { return (state & MASK_SEQ_STEP) + 1; }

int main(void) {
    PORTB.OUT |= PIN5_bm;

    TCB0.CTRLB = TCB_CNTMODE_INT_gc; // Configure TCB0 in periodic interrupt mode
    TCB0.CCMP = 3333;                // Set interval for 1 ms (3333 clocks @ 3.333 MHz)
    TCB0.INTCTRL = TCB_CAPT_bm;      // CAPT interrupt enable
    TCB0.CTRLA = TCB_ENABLE_bm;      // Enable

    sei();
    while(1) {
        ;;
    }
}
