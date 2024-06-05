#include "types.h"

#define LFSR_SEED 0x09669396
#define LFSR_MASK 0xE2023CAB

#define SEG_STATE_FE 0b00111110
#define SEG_STATE_BC 0b01101011
#define SEG_DISP_LHS 0b10000000

void play(u8 *result);
u8 cross_ref(volatile u8 *step, u8 *btn);


void lfsr(volatile u32 *state, volatile u8 *step);
void check_step(volatile u8 *step);

static volatile u32 rand_state;
static volatile u8 step_state;
