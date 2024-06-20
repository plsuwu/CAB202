#include "display.h"
#include "types.h"
#include <avr/interrupt.h>
#include <avr/io.h>

volatile u16 elapsed_time = 0;
u16 playback_delay = 250;

void update_playback_delay(void) {
    ADC0.COMMAND = ADC_START_IMMEDIATE_gc;

    // wait ADC conversion
    while (!(ADC0.STATUS & ADC_RESRDY_bm))
        ;

    // adc.result to get a time in range 250ms - 2000ms
    playback_delay = ADC0.RESULT;

    // interrupt ack
    ADC0.INTFLAGS = ADC_RESRDY_bm;
}
