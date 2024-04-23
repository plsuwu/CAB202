#include <avr/io.h> // Include register definitions

.section .init0     // Places code at the top of program memory

/** EX: E2.0

U2 is an IC that controls the 2-digit, 7-segment display on the QUTy. It
is a shift register and is controlled by its three digital input pins
which are connected to nets SPI CLK, SPI MOSI, and DISP LATCH.

The shift register's outputs, Q0-Q6 drive the LED segments of the
display in an active-low configuration. Output Q7 selects the LHS or RHS
digit via U3 and U4.

We will learn how to elegantly drive the 7-segment display in future
weeks, but you now have enough tools to be able to gain rudimentary
control of the display using a process called "bit-banging". The term
"bit-banging" refers to the sequencing and control of general purpose
input/output (GPIO) pins to implement some protocol or interface to a
device. We would usually use a peripheral in preference to bit-banging.

The shift register has 8 bits of internal memory.

- When the input connected to the DISP LATCH net transitions from LOW to
  HIGH (rising edge), the state of this internal memory sets the state
  of outputs Q0-Q7.
- When the input connected to the SPI CLK net transitions from LOW to
  HIGH (rising edge), bit 0 of the internal memory is set to the current
  state of the input connected to the SPI MOSI net.

  Simultaneously, the data in internal memory is shifted by one bit
  position:

  - The previous value in bit 0 is shifted to bit 1.
  - The previous value in bit 1 is shifted to bit 2.
  - etc.

For example, if we wanted to turn all segments of the display off, we
would transmit the byte 0b1111 1111 (note the display side does not
matter), which corresponds to 8 pulses of the SPI CLK net with the SPI
MOSI net constantly driven HIGH.

Assuming all nets are initially driven LOW, we could:

1. Drive SPI MOSI HIGH
2. Create a rising edge on the SPI CLK net 8 times:
   a. Drive SPI CLK HIGH
   b. Drive SPI CLK LOW (to prepare for next rising edge)
3. Create a rising edge on the DISP LATCH:
   a. Drive DISP LATCH HIGH

If instead, you wanted to turn some segments on, you would need to drive
the SPI MOSI net to the appropriate state before creating each rising
edge on the SPI CLK net.

In this way, you can independently set each bit in the internal memory
of the shift register (and consequently, the corresponding output state
Q0-Q7).

TASK: Write assembly code below to configure the display on the QUTy to
display the final digit of your student number.

If the final digit of your student number is EVEN, it should be
displayed on the left-hand-side (LHS) digit of the display.

If the final digit of your student number is ODD, it should be displayed
on the right-hand-side (RHS) digit of the display.

EXAMPLE:

If your student number was n12345678, "8" should be displayed on the LHS
digit.

NOTE:

Efficiency is not assessed in this exercise and therefore it is not
necessary to implement loops or subroutines.
*/

entry:
    // Write your code for Ex E2.0 below this line.


    // Write your code for Ex E2.0 above this line.

// END OF EXTENSION02 EXERCISES //
// DO NOT EDIT BELOW THIS LINE  //
    break

loop:
    rjmp loop // Loop indefinitely

