#include <avr/interrupt.h>
#include <avr/io.h>
// #include <stdint.h>
#include <stdio.h>
#include "typing.h"
#include "init.h"
#include "com.h"

/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------- STDIO DEBUG USART ----------------------------------------- */
static int stdio_putchar(char c, FILE *stream) {
    uart_putc(c);
    return c;
}

static int stdio_getchar(FILE *stream) {
    return uart_getc();
}

static int stdio_putchar(char c, FILE *stream);
static int stdio_getchar(FILE *stream);
static FILE stdio = FDEV_SETUP_STREAM(stdio_putchar, stdio_getchar, _FDEV_SETUP_RW);

void stdio_init() {
    stdout = &stdio;
    stdin = &stdio;
}
/* ---------------------------------------- STDIO DEBUG USART ----------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */

typedef enum { PER_OFF, PER_ON } per_state_t;
volatile per_state_t per_state = PER_OFF;

volatile i8 DISP_LBYTE = 0b10000001;              // all LED segs + set 7th bit   -> LHS
volatile i8 DISP_RBYTE = 0b00000001;              // all LED segs + unset 7th bit -> RHS
volatile u8 DISP_CSIDE = 0;                       // bool to alternate byte to send
volatile u8 debounce = 0xFF;
volatile u8 fall = 0;
volatile u8 rise = 0;

void check_edge(u8 *sample) {
    static u8 count_a = 0;
    static u8 count_b = 0;

    u8 edge = *sample ^ debounce;
    u8 prev = debounce;

    count_a = ~count_a & edge;
    count_b = (count_b ^ count_a) & edge;

    debounce ^= (count_b & count_a) | (edge & prev);

    fall = edge & prev;
    rise = edge & debounce;

    return;
}

ISR(TCB1_INT_vect) {
    if (DISP_CSIDE == 0) {
        SPI0.DATA = DISP_LBYTE;
    } else {
        SPI0.DATA = DISP_RBYTE;
    }

    DISP_CSIDE ^= 1;
    TCB1_INTFLAGS = TCB_CAPT_bm;
}

ISR(TCB0_INT_vect) {
    // sample buttons & mutate `fall`/`rise`
    u8 sample = PORTA.IN;
    check_edge(&sample);

    // set peripheral state (ON/OFF)
    switch (per_state) {
    case PER_OFF:
        if (fall & PIN4_bm) {
            per_state = PER_ON;
            break;
        }
    case PER_ON:
        if (rise & PIN4_bm) {
            per_state = PER_OFF;
            break;
        }
    }


    TCB0_INTFLAGS = TCB_CAPT_bm;
}

ISR(SPI0_INT_vect) {
    // latch data

    PORTA.OUT &= ~PIN1_bm;                          // drive high to begin pulse
    PORTA.OUT |= PIN1_bm;                           // drive back low to finish pulse

    SPI0.INTFLAGS = SPI_IF_bm;                      // interrupt ack
}

int main(void) {

    /*
     * CONFIGURE AND ENABLE PERIPHERALS
     */
    cli();
    tcb_init();                                         // configure + enable TCB0/1 counter
    pwm_init();                                         // configure + enable PWM (buzzer & disp)
    spi_init();                                         // configure + enable PORTC peripherals (SPI stuff)
    button_init();                                      // configure + enable PORTA buttons
    adc_init();                                         // configure + enable ADC

    /* uart + stdio_dbg config */
    uart_init();
    stdio_init();

    // reenable interrupts
    sei();

    u8 result;

    while (1) {

        // read POT into result
        result = ADC0.RESULT0;

        // DISP DP OUTPUT EN.
        if (per_state) {

            // clr PORTB output bits,
            // buzzer duty to 50%   (?)
            // disp en. to 50%      (?)
            PORTB.OUTCLR = PIN5_bm | PIN0_bm | PIN1_bm;
            TCA0.SINGLE.CMP0BUF = TCA0.SINGLE.PER / 3;                          // buzzer
            TCA0.SINGLE.CMP1BUF = (u32)result * TCA0.SINGLE.PER / 255;          // disp
        } else {

            // set PORTB output bits
            // buzzer duty to 0%
            // disp en. to 0%
            PORTB.OUTSET = PIN5_bm | PIN0_bm | PIN1_bm;
            TCA0.SINGLE.CMP0BUF = 0;
            TCA0.SINGLE.CMP1BUF = 0;
        }

        ;;
    }
}
