#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>

#include "timer.h"

const uint32_t  MAX_TIMER       = (15.9375 * 64);
uint8_t         timer_enable    = 0;
uint32_t        curr_time       = 0;

ISR(TCB1_INT_vect) {
    if (timer_enable == 1 && curr_time <= MAX_TIMER) {
        curr_time++;
    }

    // do timer stuff
    TCB1.INTFLAGS = TCB_CAPT_bm;
}
