#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "buttons.h"
#include "buzzer.h"
#include "display.h"
#include "game.h"
#include "lfsr.h"
#include "serial.h"
#include "types.h"

/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------- STDIO DEBUG USART ----------------------------------------- */

static int stdio_putchar(char c, FILE *stream) {
    uart_tx(c);
    return c;
}

static int stdio_getchar(FILE *stream) { return uart_rx(); }

static int stdio_putchar(char c, FILE *stream);
static int stdio_getchar(FILE *stream);
static FILE stdio = FDEV_SETUP_STREAM(stdio_putchar, stdio_getchar, _FDEV_SETUP_RW);

void stdio_init(void) {
    stdout = &stdio;
    stdin = &stdio;
}

/* ---------------------------------------- STDIO DEBUG USART --------------------------------------- */
/* -------------------------------------------------------------------------------------------------- */
void peripheral_init(void) {
    cli();

    tcb_disp_init();
    tcb_button_init();
    spi_disp_init();
    buzzer_init();
    button_init();

    uart_init();
    stdio_init();

    sei();
}

int main(void) {
    peripheral_init();
    game_start();

    while (1) {
        if (game_phase != IDLE) {
            printf("[game phase: NOT [IDLE] -> %u]\n", game_phase);
            game_tick();
        } else {
            printf("[game phase: IS [IDLE] -> %u]\n", game_phase);
        }
        // _delay_ms(DEBUG_DELAY);
    }
}
