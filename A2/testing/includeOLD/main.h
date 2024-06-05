#include "types.h"

volatile u16 required_ints;
volatile u8 my_turn;

volatile u16 correct_step;
volatile static u16 steps_correct;

volatile static u16 step_sequence;
volatile static u16 steps_played;
volatile static u16 steps_correct_prev;
volatile static u16 prev_step_state;
