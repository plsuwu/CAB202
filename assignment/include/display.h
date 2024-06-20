#ifndef DISPLAY_H
#define DISPLAY_H

#include "types.h"
#include <avr/interrupt.h>
#include <avr/io.h>

#define DISP_SEG_B 0b01101111
#define DISP_SEG_C 0b01111011
#define DISP_SEG_E 0b01111110
#define DISP_SEG_F 0b00111111
#define DISP_OFF 0b01111111
#define DISP_LHS_BIT (1 << 7) // bit to set side to LHS

#define DISP_BITS_FE (DISP_SEG_E & DISP_SEG_F) // left segments F & E
#define DISP_BITS_BC (DISP_SEG_B & DISP_SEG_C) // right segments B & C
#define DISP_BITS_FL 0b01110111 // G segment (sequence fail)

void update_display(const u8 left, const u8 right);
void display_result(u8 result);
void display_step(u8 step);
void display_score(u8 score);

extern volatile u8 left_byte;
extern volatile u8 right_byte;

#endif // DISPLAY_H
