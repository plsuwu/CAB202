#include "init.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define         STATE_INIT      0x09669396;
const uint8_t   debug           = 1;
const uint8_t   MASK_SEQ_STEP   = 0x3;
const uint32_t  MASK_XOR_STATE  = 0xE2023CAB;
uint32_t        sequence_length = 1;

void init(void) {
    if (debug == 1) {
        printf("doing initial setup");
    }


}

void clk_init(void) {
    cli();

    TCB1.CCMP = 52083;
    TCB1.CTRLB = TCB_CNTMODE_INT_gc;
    TCB1.INTCTRL = TCB_CAPT_bm;
    TCB1.CTRLA = TCB_ENABLE_bm;

    sei();
}

/*
 * enable pullup resistors on all buttons
 * sense falling edge on buttons for interrupt
 * */
void btn_init(void) {

    PORTA.PIN4CTRL |= PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
    PORTA.PIN5CTRL |= PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
    PORTA.PIN6CTRL |= PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
    PORTA.PIN7CTRL |= PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
}
