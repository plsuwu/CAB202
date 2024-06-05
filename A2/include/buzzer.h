#ifndef BUZZER_H
#define BUZZER_H

#include "game.h"
#include "types.h"
#include <avr/interrupt.h>
#include <avr/io.h>

/*
 * FRQ for each note is based on A = 496Hz:
 *
 *      frq = 496 * 2^(semitone / 12)
 *                                      ->
 *                                          up one semitone is (1/12),
 *                                          down one semitone is (-1/12).
 * -----------------------------------------------------------------------------
 * CCNT = (1 / FRQ) / (1 / (CPU_FRQ / PRESCALER)) -> where base prescaler is 6 (i think
 * this is how that works). = (1 / F)   / (1 / (( 20MHz * 10^6 ) / 12)
 */
#define MIN_CLK         33333  // 50Hz     @ div 2
#define MAX_CLK         83     // 20kHz    @ div 2

#define A_CLK_BASE      3360   // 496Hz    @ div 2
#define C_CLK_BASE      5341   // 312Hz    @ div 2
#define E_HI_CLK_BASE   4480   // 372Hz    @ div 2
#define E_LO_CLK_BASE   8960   // 186Hz    @ div 2

void play_tone(CurrentState *curr, u8 prev_period);
void buzzer_init(void);

#endif // BUZZER_H
