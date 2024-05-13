#include "timer.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>

#include "qutyio.h"

/** EX: 7.2

To display the elapsed time on the 7-segment display, we need to count
the number of times the TCB1 CAPT interrupt is invoked.

TASK: Declare a global variable called "elapsed_time" that will be used
to store the number of 1/64 seconds that have elapsed.

Ensure this variable is sufficiently large to store the largest time
that can be displayed on the 7-segment display (0xFF sixteenths of a
second).

The stopwatch should initially display 0x00.
*/

uint32_t elapsed_time = 0;

/** CODE: Write your code for Ex 7.2 above this line. */

/** EX: 7.3

To allow pushbuttons S1 and S2 to control when the stopwatch is
counting, we need to keep track of the current state of the stopwatch.

TASK: Declare a global variable called "is_counting" that keeps track
of the stopwatch's state.

The stopwatch should initially be stopped.
*/

uint8_t is_counting = 0;

/** CODE: Write your code for Ex 7.3 above this line. */

/** EX: 7.4

TASK: Declare an interrupt service routine (ISR) for the TCB1 peripheral
that was configured in Ex 7.0.

1. Find the vector number and name that correspond to the TCB1 CAPT
   interrupt.
   Refer to datasheet Section 8.2 Interrupt Vector Mapping on p. 63.
2. Find the corresponding vector definition in "iotn1626.h". This will
   look like *_vect and should correspond to the vector number
   identified in step 1.
3. Declare an ISR using the ISR(<vector>) macro syntax, where <vector>
   is the vector definition identified in step 2.
4. If the stopwatch is currently counting, increment the "elapsed_time"
   variable by 1. Ensure that this variable does not exceed the value
   which represents 15.9375 s (i.e., 0xFF * 1/16 s).
5. Update the 7-segment display to reflect the value of "elapsed_time".
6. Acknowledge that the interrupt has been handled by clearing the CAPT
   bit in the TCB1.INTFLAGS register.
   Refer to datasheet Section 22.5.5 Interrupt Flags on p. 260 for
   information on how to clear this bit.

NOTE: You may use display_hex() to display a number in hexadecimal on
the 7-segment display. This function accepts an unsigned 8-bit integer.

NOTE: As this interrupt is invoked every 64th of a second, the value in
"elapsed_time" will not represent a count of 16ths of a second.

You will need to use the following relationship to display the correct
value on the 7-segment display:

    1/16 second = 4 * 1/64 second
*/

// elapsed_time represents a count of 64ths of a second
// the 7-segment display displays a count of 16ths of a second

// what is the maximum value that can be displayed? 0xFF
// LHS: seconds
// RHS: 16ths of second

// maximum time:
//
// [0xFF] * 1/16 s
// [0xFF] * (4 * 1/64 s)
// (0xFF * 4) * 1/64 s

// TCB1 CAPT interrupt  -> `TCB1_INT`
// TCB1 VEC             -> `TCB1_INT_vect`
ISR(TCB1_INT_vect) {

    if (is_counting == 1 && elapsed_time <= (15.9375 * 64)) {
        elapsed_time++;
    }

    display_hex(elapsed_time >> 2);
    TCB1.INTFLAGS = TCB_CAPT_bm;
}

/** CODE: Write your code for Ex 7.4 above this line. */
