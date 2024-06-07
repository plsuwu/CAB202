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
#include "init.h"
#include "types.h"


// static int stdio_putchar(char c, FILE *stream) {
//     uart_tx(c);
//     return c;
// }
//
// static int stdio_getchar(FILE *stream) { return uart_rx(); }
//
// static int stdio_putchar(char c, FILE *stream);
// static int stdio_getchar(FILE *stream);
// static FILE stdio = FDEV_SETUP_STREAM(stdio_putchar, stdio_getchar, _FDEV_SETUP_RW);
//
// void stdio_init(void) {
//     stdout = &stdio;
//     stdin = &stdio;
// }

// void state_machine(void) {
//     game_phase_t game_phase = IDLE;
//
//     u8 prev_button_state = 0xff;
//     u8 curr_button_state = 0xff;
//
//     u8 button_current = 0;
//
//     // button flags
//     u8 falling_edge, rising_edge, released = 0;
//
//     while (1) {
//         switch (game_phase) {
//             case IDLE:
//                 game_phase = SEQUENCE;
//                 break;
//
//             case SEQUENCE:
//                 game_phase = USER;
//                 break;
//
//             case USER:
//                 // stop_tone_disp();
//                 break;
//
//             case RESULT:
//                 break;
//         }
//     }
// }

void peripheral_init(void) {
    cli();

    button_init();
    pwm_init();
    spi_init();
    timer_init();

    sei();
}

int main(void) {
    peripheral_init();
    // game_init();

    // sequence[0] = generate_step(&lfsr_state);

    game_tick();
}
