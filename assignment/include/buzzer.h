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


// E (Hi)   -> 371.580 Hz : ~4485.349 cycles : E_HI_CLK_BASE
// C#       -> 312.460 Hz : ~5334.009 cycles : C_CLK_BASE
// A        -> 496.000 Hz : ~3360.215 cycles : A_CLK_BASE
// E (Low)  -> 185.790 Hz : ~8970.699 cycles : E_LO_CLK_BASE
#define MAX_OCTAVE      2
#define MIN_OCTAVE      -2

#define E_HI_CLK_BASE   4485   // 372Hz    @ div 2
#define C_CLK_BASE      5334   // 312Hz    @ div 2
#define A_CLK_BASE      3360   // 496Hz    @ div 2
#define E_LO_CLK_BASE   8971   // 85 Hz    @ div 2

void play_tone(u8 tone);
void stop_tone(void);

// void buzzer_init(void);
void increase_octave(void);
void decrease_octave(void);

#endif // BUZZER_H
