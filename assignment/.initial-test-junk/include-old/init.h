#include <avr/interrupt.h>
#include <avr/io.h>

#include "serial.h"
#include "types.h"

/*
 * FRQ for each note is based on A = 496Hz:
 *
 *      frq = 496 * 2^(semitone / 12)
 *                                      ->
 *                                          up one semitone is (1/12),
 *                                          down one semitone is (-1/12).
 * -----------------------------------------------------------------------------
 * CCNT = (1 / FRQ) / (1 / (CPU_FRQ / PRESCALER)) -> where base prescaler is 6 (i think this is how that works).
 *      = (1 / F)   / (1 / (( 20MHz * 10^6 ) / 12)
 */
#define MIN_CLK     33333   // 50Hz     @ div 2
#define MAX_CLK     83      // 20kHz    @ div 2
#define A_CCNT      3360    // 496Hz    @ div 2
#define Eh_CCNT     4480    // 372Hz    @ div 2
#define C_CCNT      5341    // 312Hz    @ div 2
#define El_CCNT     8960    // 186Hz    @ div 2

void tcb_init(void);
void pwm_init(void);
void adc_init(void);
void uart_init(void);
void spi_init(void);
void button_init(void);
void stdio_init(void);
