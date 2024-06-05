#include "types.h"

#ifndef ISR_H
#define ISR_H

volatile per_state_t per_state;
volatile per_state_t bzr_state;
volatile u8 rise;
volatile u8 fall;
volatile u8 disp_seg;
static volatile u8 curr_ints;

void write_spi(u8 byte);

#endif // ISR_H
