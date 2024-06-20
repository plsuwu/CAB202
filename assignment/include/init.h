#ifndef INIT_H
#define INIT_H

#include "types.h"

void game_reset(u8 *sequence_length, u8 *sequence_index, u8 *user_index);

void button_init(void);
void pwm_init(void);
void timer_init(void);
void spi_init(void);
void uart_init(void);

#endif // INIT_H
