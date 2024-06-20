#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>

#include "main.h"
#include "debug.h"
#include "init.h"
#include "isr.h"
#include "serial.h"
#include "types.h"
#include "game.h"


volatile static u32 rand_state = LFSR_SEED;
volatile static u8 step_state = 1;
volatile static u16 prev_step_state = 1;
volatile static u16 step_sequence = 1;
volatile static u16 steps_played = 0;
volatile static u16 steps_correct = 1;
volatile static u16 steps_correct_prev = 1;

/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------- STDIO DEBUG USART ----------------------------------------- */

static int stdio_putchar(char c, FILE *stream) {
    uart_putc(c);
    return c;
}

static int stdio_getchar(FILE *stream) { return uart_getc(); }

static int stdio_putchar(char c, FILE *stream);
static int stdio_getchar(FILE *stream);
static FILE stdio = FDEV_SETUP_STREAM(stdio_putchar, stdio_getchar, _FDEV_SETUP_RW);

void stdio_init(void) {
    stdout = &stdio;
    stdin = &stdio;
}

/* ---------------------------------------- STDIO DEBUG USART --------------------------------------- */
/* -------------------------------------------------------------------------------------------------- */

const bool_t debug = false;

int main(void) {


    /*
     * CONFIGURE AND ENABLE PERIPHERALS
     */
    cli();
    tcb_init();    // configure + enable TCB0/1 counter
    pwm_init();    // configure + enable PWM (buzzer & disp)
    spi_init();    // configure + enable PORTC peripherals (SPI stuff)
    button_init(); // configure + enable PORTA buttons
    adc_init();    // configure + enable ADC

    /* uart + stdio_dbg config */
    uart_init();
    stdio_init();

    // reenable interrupts
    sei();
    // write_spi(0xFF); // clear 7-SEG

    u8 result;

    if (!debug) {
        while (1) {
            play(&result);
        }

    } else {

        while (1) {
            dbg_per(&result, &per_state);
        }
    }
}
