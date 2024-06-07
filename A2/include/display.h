#ifndef DISPLAY_H
#define DISPLAY_H

#include "game.h"
#include "types.h"
#include <avr/interrupt.h>
#include <avr/io.h>

// void display_score(u8 *n, u8 *side);
// void display_tone(u8 *seg, u8 *side);
// void display_segment(CurrentState *curr);
void update_display(const u8 left, const u8 right);
void display_step(u8 step);
void display_score(u8 score);

// void display_init(void);
// void spi_disp_init(void);
// void tcb_disp_init(void);
extern volatile u8 left_byte;
extern volatile u8 right_byte;


typedef enum {
    left,
    right
} digit_side_t;

#endif // DISPLAY_H
