#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>

#include "qutyio.h"

/** Tutorial 06

INTRODUCTION:

In this week's tutorial you will implement a pseudorandom number
generator. This will be of particular relevance to Assessment 2.
*/

/** EX: 6.0

TASK: Define a global unsigned 32-bit integer called "state".

Initialise "state" with a value equal to your student number, where each
decimal digit in your student number represents a hexadecimal literal.

EXAMPLE: If your student number was n12345678 then you should assign the
value 0x12345678 to "state".

Once you have defined this variable uncomment the first printf()
statement in the main() function.
*/

uint32_t state = 0x9669396;

/** CODE: Write your code for Ex 6.0 above this line. */

/** EX: 6.1

TASK: Write the function prototype for a function called "next"
which takes no parameters and returns no result.
*/

void next(void);

/** CODE: Write your code for Ex 6.1 above this line. */

/** EX: 6.2

TASK: Write a function called "print_rand" which returns no result
and takes, as a single argument, a pointer to an unsigned 32-bit
integer.

The function should inspect the least significant two bits of the
integer pointed to by the argument, and depending on the value of the
bits, execute one of the following function calls:

    0b00 (0): putchar('1');
    0b01 (1): putchar('2');
    0b10 (2): putchar('3');
    0b11 (3): putchar('4');

Once you have written this function uncomment the second printf()
statement in the main() function.
*/

void print_rand(uint32_t *num) {
    uint32_t test = 0x00000003; 

    switch (*num & test) {
        case 1:
            putchar('2');
            break;
        case 2:
            putchar('3');
            break;
        case 3:
            putchar('4');
            break;
        default:
            putchar('1');
            break;
    }
}

/** CODE: Write your code for Ex 6.2 above this line. */

/** EX: 6.3

TASK: Complete the implementation for the next() function.

next() should:

1. Right shift the bits in "state" by 1 position.
2. If the bit shifted out of state in step 1 was set (1), take the XOR
   of "state" with 0xA8E831E0 and store the result in "state".

Once you have written this function uncomment the third printf()
statement in the main() function.
*/

void next(void)
{
    /** CODE: Write your code for Ex 6.3 within this function. */
    uint32_t state_lsb = state & 0x01;  // track state lsb
    state = state >> 1;                 // bitshift right 1

    if (state_lsb == 1) {
        state = state ^ 0xA8E831E0;     // xor if state lsb bit is set
    } 

}

int main(void)
{
    serial_init();

    /** TODO: Uncomment the line below after Ex 10.0 is completed. */
    printf("Ex 6.0: state = 0x%08lX\n", state);

    /** TODO: Uncomment the line below after Ex 6.2 is completed. */
    uint32_t test = 0b1100; printf("Ex 6.2: output = "); print_rand(&test); test >>= 1; print_rand(&test); test >>= 1; print_rand(&test); test >>= 1; print_rand(&test); printf("\n");

    /** TODO: Uncomment the line below after Ex 6.3 is completed. */
    next(); printf("Ex 6.3: state = 0x%08lX", state); next(); next(); next(); next(); printf(", state = 0x%08lX\n", state);

    /** EX: 6.4

    The variable "state" together with the next() function implement a
    linear-feedback shift register, which produces a pseudorandom number
    sequence.

    This sequence was seeded (initialised) with your student number.

    TASK: Write code below to print the next 32 digits derived from this
    pseudorandom number sequence using next() and print_rand().

    All digits should be printed on a single line without spaces.

    NOTE: Call next() to advance "state" prior to printing the first
    digit.

    TIP: Recall that the unary operator "&" can be used to get a
    reference (pointer) to some data.
    */

    printf("Ex 6.4: ");
    /** CODE: Write your code for Ex 6.4 below this line. */

    for (int i = 0; i < 32; ++i) {
        next();
        print_rand(&state);
    }

    /** CODE: Write your code for Ex 6.4 above this line. */
    printf("\n");

    // END OF TUTORIAL06 EXERCISES //
    // DO NOT EDIT BELOW THIS LINE //

    while (1)
        ; // Loop indefinitely
}
