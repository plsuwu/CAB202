#include <avr/interrupt.h>
#include <avr/io.h>

#include "buttons.h"
#include "game.h"
#include "init.h"

// initialize peripheral functionality
void peripheral_init(void) {
    cli(); // disable interrupts globally

    button_init();
    pwm_init();
    spi_init();
    timer_init();

    sei(); // re-enable interrupts globally
}

// entry point
int main(void) {

    // call hardware init functions
    peripheral_init();

    // set game variables and run the main game loop
    game_loop();
}
