// This header file provides exact-width integer types that allow for
// cross-platform portability.
// #include <cstdint>
#include <stdatomic.h>
#include <stdint.h>

// This header file provides functions that interface with standard
// input and ouput.
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

// This header file provides functions to control serial output on the
// QUTy.
#include "../include/qutyserial.h"

// This is the main function. It is the entry point for a C program.
int main(void)
{
    // This function configures serial communication on the QUTy.
    serial_init();

    /** EX: 4.0

    C is a strongly typed language in which variables must be declared
    with a type.

    The syntax for declaring a variable is:

        <type> <name>;

    We can also initialise a variable with a value using the syntax:

        <type> <name> = <initial value>;

    TASK: Write C code below to declare a variable called "c_release"
    that represents an unsigned 16-bit integer, and initialise this
    variable with the year that the C programming language was released.
    */

    uint16_t c_release = 1972;

    /** CODE: Write your code for Ex 4.0 above this line. */
    /** TODO: Uncomment the line below after Ex 4.0 is completed. */
    printf("C was first released by Dennis Richie in the year %u.\n", c_release);

    /** EX: 4.1

    C provides a number of arithmetic, logical, and bitwise operators
    for performing operations on expressions.

    Below is a partially implemented block of code that assigns a value
    equal to the last two digits of your student number to the variable
    "result".

    TASK: Initialise the variables "x" and "parity" such that "result"
    is assigned a value equal to the last two digits of your student
    number.

    EXAMPLE: If your student number was n12345678 then "result" should
    be assigned the value 78.
    */

    int16_t a = 1;
    int16_t b = 3;
    int16_t c = -50;

    /** CODE: Write your code for Ex 4.1 below this line. */
    // n9669396

    int16_t x = 73;
    uint8_t parity = 0;
    /** CODE: Write your code for Ex 4.1 above this line. */

    uint8_t result;

    if (parity) {
        result = (b - a) * x - c - 1;
    } else {
        result = (b - a) * x + c;
    }

    printf("The result is %u.\n", result);

    /** EX: 4.2

    Loops can be used to repeat the execution of statements multiple
    times. The "for" loop is a common loop structure in C that uses the
    following syntax:

        for (initialisation; condition; update)
        {
            // statement
        }

    - "initialisation" is a statement that is executed once before
      the loop is executed.
    - "condition" is a Boolean expression that is evaluated before each
      iteration of the loop. If the condition is true, the loop is
      executed, otherwise the loop is terminated.
    - "update" is a statement that is executed after each iteration of
      the loop.

    TASK: Write C code below to print the following text to the console
    using a for loop:

        "1\n22\n333\n4444\n55555\n"

    where "\n" is the newline character.

    When printed to a console, this raw string produces the following
    output:

    1
    22
    333
    4444
    55555

    TIP: You may use the "printf" function to print formatted text to
    the console. To print an integer, you must use a compatible format
    specifier for integers, such as "%d"/"%i" for signed integers, and
    "%u" for unsigned integers.

    For example:

        printf("%u\n", 5);

    will print the string "5\n".

    RESOURCE: More information on the "printf" function can be found at
    the following link:

    https://learn.microsoft.com/en-us/cpp/c-runtime-library/format-specification-syntax-printf-and-wprintf-functions#type-field-characters
    */

    // This loop currently runs indefinitely. Make sure to add a
    // condition to terminate the loop after it has printed the
    // required text.

    int i = 0;

    do {
        i++;

        for (int j = 0; j < i; j++) {
            printf("%d", i);
        }
        printf("\n");
    } while (i < 5);

    /** CODE: Write your code for Ex 4.2 above this line. */

    /** EX: 4.3

    When working with microcontrollers, it is often necessary to work
    with individual bits of memory, where we often use one of the
    following bitwise operators (in order of precedence):

    - "~" (bitwise NOT)
    - "&" (bitwise AND)
    - "^" (bitwise XOR)
    - "|" (bitwise OR)

    TASK: Write C code below to print the value of the variable
    "c_release" defined in Ex 4.0 in binary.

    EXAMPLE: If "c_release" was assigned the value 10, your program
    should output:

        "0b00001010\n"

    You must include the "0b" prefix to indicate that the number is a
    binary literal, and include any leading zeros to ensure that the
    number of bits printed is equal to the size of the variable. You
    must also print a newline character at the end of the output.

    HINT: Consider using bitwise operators to isolate specific bits
    from the variable "c_release", and use bitshift operators to
    shift the bits into the correct position for printing.

    Recall bitshift operators have the following syntax:

        <value> << <number of positions> // left shift
        <value> >> <number of positions> // right shift
    */

    /** CODE: Write your code for Ex 4.3 below this line. */

    // c_release = 1972;
    // 0b0000011110110100\n

    uint16_t release_len = sizeof(c_release) * 8;

    printf("0b");
    for(int i = release_len - 1; i >= 0; i--) {
        if (c_release & (1 << i)) {
            printf("1");
        } else {
            printf("0");
        }
    }
    printf("\n");

    /** CODE: Write your code for Ex 4.3 above this line. */

    /** EX: 4.4

    UNIX time is a date and time representation used in many computer
    systems. It is defined as the number of seconds elapsed since
    January 1, 1970, 00:00:00 Coordinated Universal Time (UTC), not
    accounting for leap seconds.

    TASK: Modify the declaration of "seconds_since_epoch" below so that
    it represents the number of seconds that have elapsed since January
    1, 1970, 00:00:00 UTC+00:00, to the date of your submission at
    midnight, in your local timezone.

    NOTE: Modify the assignment to "timezone_offset_hours" and
    "timezone_offset_minutes" to reflect the timezone offset of your
    location. You may include daylight saving time offsets if you wish.
    */

    int64_t seconds_since_epoch = 1713276000;

    // Timezone offset from UTC+10:00
    int32_t timezone_offset_hours = 10;
    int32_t timezone_offset_minutes = 0;

    /** CODE: Write your code for Ex 4.4 above this line. */

    printf(
        "Timezone offset: UTC%+02d:%02d.\n",
        timezone_offset_hours,
        timezone_offset_minutes
    );

    printf(
        "Submission UNIX time: %lu%09lu.\n",
        (int32_t)(seconds_since_epoch / 1000000000),
        (int32_t)(seconds_since_epoch % 1000000000)
    );

    /** EX: 4.5

    In computer architecture, processors are designed to operate on
    specific data types that represent the number of bits that can be
    efficiently processed in a single operation. When UNIX time was
    first defined, it utilised a signed 32-bit integer to count the
    number of seconds since the UNIX epoch.

    TASK: Modify the assignments below to print the date and time when
    UNIX time will overflow on a 32-bit architecture.
    */

    int32_t overflow_day = 19;
    int32_t overflow_month = 1;
    int32_t overflow_year = 2038;
    int32_t overflow_hour = 3;
    int32_t overflow_minute = 14;
    int32_t overflow_second = 7;

    /** CODE: Write your code for Ex 4.5 above this line. */

    printf(
        "UNIX overflow date time: %02ld/%02ld/%04ld %02ld:%02ld:%02ld UTC+00:00.\n",
        overflow_day,
        overflow_month,
        overflow_year,
        overflow_hour,
        overflow_minute,
        overflow_second
    );

    /** EX: 4.6

    Neptune's orbital period around the Sun is approximately 60,190
    days, or 165 years.

    TASK: Modify the expression below to increment the variable
    "seconds_since_epoch" to the time of Neptune's next orbit after the
    UNIX epoch, using the approximate orbital period of 60,190 days.
    */

    // `seconds_since_epoch` was previously assigned as follows:
    // int32_t seconds_since_epoch = 1713276000
    // uint64_t neptune_orbital_seconds = 60190ul * 24ul * 60ul * 60ul;
    // seconds_since_epoch = 0;
    seconds_since_epoch += (5200416000ul - (uint64_t) seconds_since_epoch);

    /** CODE: Write your code for Ex 4.6 above this line. */

    printf(
        "Time of Neptune's next orbit after the UNIX epoch: %lu%09lu.\n",
        (int32_t)(seconds_since_epoch / 1000000000),
        (int32_t)(seconds_since_epoch % 1000000000)
    );

    // END OF TUTORIAL04 EXERCISES //
    // DO NOT EDIT BELOW THIS LINE //

    while (1)
        ; // Loop indefinitely
}
