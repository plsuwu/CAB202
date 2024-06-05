#ifndef LFSR_H
#define LFSR_H

#include <avr/interrupt.h>
#include <avr/io.h>

#include "types.h"

u8 lfsr_next(u32 *current_state, u8 *sequence_length);

#endif  // LFSR_H
