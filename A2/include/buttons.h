#ifndef BUTTONS_H
#define BUTTONS_H

#include "types.h"
#include <avr/interrupt.h>
#include <avr/io.h>

u8 read_input(void);
void button_init(void);
void tcb_button_init(void);

#endif // BUTTONS_H
