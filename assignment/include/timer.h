#ifndef TIMER_H
#define TIMER_H

#include "types.h"

extern volatile u16 elapsed_time;
extern u16 playback_delay;
void update_playback_delay(void);

#endif // !TIMER_H
