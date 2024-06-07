#include "types.h"
#include "display.h"
#include <avr/io.h>
#include <avr/interrupt.h>

// typedef enum {
//     left,
//     right,
// } display_digit_t;

volatile u16 elapsed_time = 0;
u16 playback_delay = 250;

// ISR fires every ms
// ISR(TCB0_INT_vect) {
//     ++elapsed_time;
//     TCB0.INTFLAGS = TCB_CAPT_bm;
// }


// static void spi_write(void) {
// }
