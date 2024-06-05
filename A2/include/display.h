#ifndef DISPLAY_H
#define DISPLAY_H

#include "game.h"
#include "types.h"
#include <avr/interrupt.h>
#include <avr/io.h>

// void display_score(u8 *n, u8 *side);
// void display_tone(u8 *seg, u8 *side);
void display_segment(CurrentState *curr, u8 prev_period);
void display_score(u8 score);

// void display_init(void);
void spi_disp_init(void);
void tcb_disp_init(void);

#endif // DISPLAY_H
