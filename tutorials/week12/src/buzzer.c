#include "buzzer.h"

#include <avr/io.h>
#include <stdint.h>

// Values used to write to the TCA0.SINGLE.PERBUF register.
#define TONE1_PER 0
#define TONE2_PER 0
#define TONE3_PER 0
#define TONE4_PER 0

/** EX: 12.3

TASK: Implement the functions below to turn the buzzer ON and OFF.

The "buzzer_on" function should take a single argument, "tone", which
ranges from 0 to 3, and sets the frequency of the waveform output
generated by TCA0 correspondingly.

Ensure that the above macros reflect the four frequencies required.
*/

void buzzer_on(const uint8_t tone)
{
    static const uint16_t periods[4] = {TONE1_PER, TONE2_PER, TONE3_PER, TONE4_PER};

    /** CODE: Write your code for Ex 12.3 within this function. */

}

void buzzer_off(void)
{
    /** CODE: Write your code for Ex 12.3 within this function. */

}