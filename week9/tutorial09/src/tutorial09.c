#include "timer.h"
#include "uart.h"
#include "spi.h"
#include <stdint.h>

/** Tutorial 09

INTRODUCTION:

In this week's tutorial you will work with the USART0 and SPI0
peripherals to read/write data via the USB-UART interface and control
the 7-segment display.

Interfacing with these hardware devices on the QUTy is a core
requirement for Assessment 2.
*/

/** EX: 9.0

The following functions have been defined in "uart.c" and included from
"uart.h":

    void uart_init(void);   // Initialise the UART peripheral to 9600 baud, 8N1
    char uart_getc(void);   // Blocking read of byte c from UART
    void uart_putc(char c); // Blocking write of byte c to UART

TASK: Implement the function "void uart_puts(char* s)" in "uart.c"
which takes, as an argument, a pointer to a character (string),
and transmits this string via UART using uart_putc().
*/

/** CODE: Write your code for Ex 9.0 in uart.c. */

int main(void)
{
    uart_init();

    /** EX: 9.1

    TASK: Write code below that will call uart_puts() to print your
    student number via the UART interface.

    The string should begin with a leading 'n' and should be terminated
    with a newline character '\n'.
    */

    char *student_number = "n9669396\n\0";
    uart_puts(student_number);

    /** CODE: Write your code for Ex 9.1 above this line. */

    /** EX: 9.2

    Create two new files:

        src/spi.c
        include/spi.h

    TASK:

    1. In "spi.c", declare and implement a function spi_init() that will
       initialise SPI0 in unbuffered mode, such that data can be written
       to the shift register that controls the 7-segment display.

       This function should also enable the SPI interrupt via the IE bit
       in INTCTRL.
    2. Declare and implement the function spi_write(uint8_t b) which
       will write the byte b out via the SPI interface.
    3. Declare and implement an ISR that handles the SPI interrupt, and
       also creates a rising edge on the DISP LATCH net.

       You will need to add some code to spi_init() to enable the
       required pins as outputs.

    Declare prototypes for these functions in "spi.h" and include
    "spi.h" at the top of "tutorial09.c".

    Once this exercise is complete, uncomment the lines below. The
    7-segment display should be blank after this code executes.
    */

    /** CODE: Write your code for Ex 9.2 in spi.c and spi.h. */

    /** TODO: Uncomment the lines below after Ex 9.2 is completed. */
    spi_init();       // Initialise SPI
    spi_write(0xFF);  // Clear display

    /** EX: 9.3

    The following function has been defined in "timer.c" and included
    from "timer.h":

        void timer_init(void); // Initialise TCB0 to produce interrupts every 1 ms

    TASK:

    Complete the ISR declared in "timer.c" to display simultaneously:

    - the first digit of your student number on the LHS of the 7-segment display
    - the second digit of your student number on the RHS of the 7-segment display

    after timer_init() is called.

    TIP: Swap which digit is displayed upon each interrupt using a
    static variable.

    TIP: The segments corresponding to digits 0-9 are outlined in the
    truth table below. See the QUTy schematic to determine the pin to
    segment mapping.

       ABCDEFG
    0: 0000001
    1: 1001111
    2: 0010010
    3: 0000110
    4: 1001100
    5: 0100100
    6: 0100000
    7: 0001111
    8: 0000000
    9: 0000100

     -- A --
    |       |
    F       B
    |       |
     -- G --
    |       |
    E       C
    |       |
     -- D --

    l = "9" =>
    r = "6"



    */

    /** CODE: Write your code for Ex 9.3 in timer.c. */

    /** EX: 9.4

    TASK: Write code below that will call timer_init() only after the
    character 'a' has been received via the UART interface.
    */

    char is_a = 'a';
    char uart_in = uart_getc();
    char writer[2];
    writer[0] = uart_in;
    writer[1] = '\0';
    uart_puts(writer);

    if (uart_in == is_a) {
        timer_init();
    }


    /** CODE: Write your code for Ex 9.4 above this line. */

    while (1)
        ; // Loop indefinitely
}

