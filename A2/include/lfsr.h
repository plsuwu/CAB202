#ifndef LFSR_H
#define LFSR_H

#include <avr/interrupt.h>
#include <avr/io.h>

#include "types.h"

u8 set_step(u32 *state);
// coid generate_step(void);
void reset_lfsr(void);

#endif  // LFSR_H
