#ifndef BUTTONS_H
#define BUTTONS_H

// #include "game.h"
#include "types.h"
#include <avr/interrupt.h>
#include <avr/io.h>

volatile u8 button_rise;
volatile u8 button_fall;
volatile u8 button_debounce;
// extern volatile u8 button_state_prev;
// extern volatile u8 button_state_curr;

u8 get_falling_pin(u8 button);
u8 get_rising_pin(u8 button);

u8 read_input(void);
void button_init(void);
void tcb_button_init(void);

#endif // BUTTONS_H
