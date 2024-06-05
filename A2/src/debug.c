#include <avr/io.h>
#include <stdlib.h>

#include "isr.h"
#include "serial.h"
#include "debug.h"
#include "types.h"

// static u8 prev = 0xFF;

/*
 * dbg_per() => When debug is 'true' in main, this function allows btn_0 to be pressed and
 *              held to activate the following:
 *
 * 7-segment display - shows '9' + the DP on both sides, buzzer plays tone, Potentiometer controls display
 * brightness, UART prints test string to STDOUT (continuous)
 */
void dbg_per(u8 *result, volatile per_state_t *per_state) {
    // read POT into result pointer

    *result = ADC0.RESULT0;
    // char test_str[] = "stdout works!\n\0";
    // DISP DP OUTPUT EN.
    if (*per_state) {

        // write to stdout on a loop
        // clr PORTB output bits
        // buzzer duty to 50%                       (?)
        // disp en. to 50%                        (?)
        //
        u8 reversed = 255 - *result;
        PORTB.OUTCLR = PIN0_bm | PIN1_bm;                     // DP | BUZZER | DISP
        TCA0.SINGLE.CMP0BUF = TCA0.SINGLE.PER / 2;
        TCA0.SINGLE.PERBUF = ((u32) (*result + 1) * 41625 / 255);
        TCA0.SINGLE.CMP1BUF = ((u32) reversed * TCA0.SINGLE.PER / 255);
    } else {

        // set PORTB output bits
        // buzzer duty to 0%
        // disp en. to 0%
        PORTB.OUTSET = PIN0_bm | PIN1_bm;
        TCA0.SINGLE.CMP0BUF = 0;
        TCA0.SINGLE.CMP1BUF = 0;
    }
}
