/** CODE: Include any header files and macro definitions here. */

/** EX: E3.0

In this week's tutorial, we used some functions from the libraries
"qutyio.h" and <stdio.h> to output data via the serial interface. This
can be a useful tool for debugging your program.

To use the serial interface you first need to initialise the required
hardware by calling serial_init(); this only needs to be done once at
the beginning of your program.

After this, you can use the function printf() from <stdio.h> to output
formatted strings via the serial interface.

You will need to use printf() in a few different ways in this extension
exercise:

    printf("foo")     - prints the word 'foo' to the serial output.
    printf("bar")     - prints the word 'bar' to the serial output.
    printf(" ")       - prints a space to the serial output.
    printf("%02X", x) - prints the 8-bit integer 'x' to the serial
                        output, formatted as two uppercase hexadecimal
                        digits, with 0 padding.
    printf("\n")      - prints a new line to the serial output.

Note that the format string can consist of multiple parts, for example:

    uint16_t x = 0xAB;
    printf("x: 0x%03X = %lu \n", x, x);

prints "x: 0x0AB = 171 " followed by a newline.

TASK: Write C code that performs the following steps:

1. Include "qutyio.h" and <stdio.h>, and initialise the serial interface
   using serial_init().
2. Create a variable "state" to store your student number as a decimal
   value. You should use the smallest fixed-width integer type from the
   <stdint.h> header file that can hold this value.
3. Iterate through all the numbers from 0 to 255 in sequence, and for
   each number, perform the following steps:
   a. Take the bitwise XOR of the number with the variable "state",
      storing the result back into "state".
   b. Rotate right the bits in "state" at least one time, and until the
      LSB of "state" is a zero. If there are no cleared bits in "state",
      do nothing.
   c. Print the least significant two bytes of "state" to the serial
      output as four uppercase hexadecimal digits, followed by a space.
      The prefix "0x" is not required.
   d. Inspect bits 11-4 of "state" (where bit 0 is the LSB).
      - If the most significant nibble of this byte is equal to the
        second last digit of your student number, print the word "foo"
        to the serial output.
      - If the least significant nibble of this byte is equal to the
        final digit of your student number print the word "bar" to the
        serial output.
      - If both conditions are satisfied, print "foobar".
   e. Print a newline character to the serial output.
4. After step 3, your program should have printed 256 lines to the
   serial output.

Program execution should then proceed to the infinite loop without
producing any further output.

EXAMPLES:

    Assume for all examples below the student number is n12345678

    If after step 3b "state" holds the value 0xE00B_C614,
        the program should print: "C614 "

    If after step 3b "state" holds the value 0xD76F_77F0,
        the program should print: "77F0 foo"

    If after step 3b "state" holds the value 0x7802_F184,
        the program should print: "F184 bar"

    If after step 3b "state" holds the value 0xAFB6_F784,
        the program should print: "F784 foobar"
*/

int main(void)
{
    /** CODE: Write your code for Ex E3.0 below this line. */

    serial_init();

    /** CODE: Write your code for Ex E3.0 above this line. */

    // END OF EXTENSION03 EXERCISES //
    // DO NOT EDIT BELOW THIS LINE  //

    while (1)
        ; // Loop indefinitely
}
