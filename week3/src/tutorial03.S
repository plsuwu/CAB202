#include <avr/io.h> // Include register definitions

.section .init0

.include "include/buzzer.S" // Place the contents of buzzer.S here

entry:

    // Ex 3.0

    // Write assembly code such that when we enable PB5 as an output,
    // DS1-DP is dim.

    // DS1-DP active LOW
    ldi r21, PIN5_bm
    sts PORTB_OUTSET, r21 // set HIGH - off

    // Ex 3.1
    //
    // Write assembly code to enable the pin connected to the DISP DP
    // net as an output.

    sts PORTB_DIRSET, r21

    // Ex 3.2
    //
    // Write assembly code to enable the internal
    // pull-up resistors for the pins connected to these pushbuttons.

    ldi r20, PORT_PULLUPEN_bm
    sts PORTA_PIN4CTRL, r20
    sts PORTA_PIN5CTRL, r20
    sts PORTA_PIN6CTRL, r20
    sts PORTA_PIN7CTRL, r20

    // Ex 3.3
    //
    // Write assembly code to illuminate DS1-DP.

    sts PORTB_OUTCLR, r21

// The code between the "loop_100ms" and "end_loop_100ms" labels will
// execute approximately 10 times per second.
loop_100ms:

    // Ex 3.4
    //
    // Write assembly code to read the state of BUTTON0 and store the
    // result in R17.
    //
    // S1 is pressed: R17 should contain the value 0x00.
    // S1 is released: R17 should contain a non-zero value.

    in r17, VPORTA_IN
    andi r17, PIN4_bm

    // Ex 3.5
    //
    // The two instructions below test the value in R17 to determine
    // whether S1 is pressed.
    // If S1 is released:
    //   any code between the "brne" instruction and the
    //   "end_loop_100ms" label will be skipped.
    //
    // Write assembly code to toggle the state of DS1-DP.

    cpi r17, 0
    brne end_loop_100ms
    // Write your code for Ex 3.5 below this line.

    ldi r21, PIN5_bm
    sts PORTB_DIRSET, r21
    sts PORTB_OUTTGL, r21

    // Write your code for Ex 3.5 above this line.

end_loop_100ms:

// The code between the "loop_10ms" and "end_loop_10ms" labels will
// execute approximately 100 times per second.
loop_10ms:

    // Ex 3.6
    //
    // At the start of this program, some code was included to set up
    // the Timer Counter A 0 (TCA0) peripheral to synthesise a 200 Hz
    // waveform to drive the piezo buzzer connected to the BUZZER net.
    //
    // TCA0 will automatically override the output of the pin connected
    // to the BUZZER net, however, the pin needs to be configured as an
    // output for the waveform to define the state of the BUZZER net.
    //
    // Write assembly code such that the buzzer is driven (audible),
    // when S4 is pressed, and not driven (silent), when S4 is released.
    //
    // To assist you, we have written some code below that tests R18:
    // If R18 contains zero, then:
    //   any code between "ex36_zero" and "rjmp end_ex36" will execute.
    // If R18 does not contain zero, then:
    //   any code between "ex36_nonzero" and "end_ex36" will execute.

    // Write your code for Ex 3.6 anywhere below this line

    in r18, VPORTA_IN       // read input from pushbuttons
    andi r18, PIN7_bm       // get VPORT input data at PIN7 (S4)

    cpi r18, 0
    brne ex36_nonzero

    ex36_zero:
        ldi r19, PIN0_bm
        sts PORTB_DIRSET, r19

        rjmp end_ex36

    ex36_nonzero:
        ldi r19, PIN0_bm
        sts PORTB_DIRCLR, r19

    end_ex36:


    // Write your code for Ex 3.6 anywhere above this line

// END OF TUTORIAL03 EXERCISES //
// DO NOT EDIT BELOW THIS LINE //
end_loop_10ms:

// Let's implement a delay loop based on a 16-bit counter to execute the
// above loops at approximately 10 ms and 100 ms intervals.

delay_10ms: // (0x100 * 0x1A * 5 - 1) * 300 ns = 9.98 ms
    adiw r30, 1     // 2 cycles
    cpi r31, 0x1A   // 1 cycle
    brne delay_10ms // 2 cycles (1 cycle on final iteration)
end_delay_10ms:

// Reset the high byte of the delay loop counter.
ldi r31, 0

// Execute the 100 ms loop every 10 iterations of the 10 ms loop.
inc r29
cpi r29, 10
brne loop_10ms

ldi r29, 0
rjmp loop_100ms

// The program counter should never reach this point.
loop:
    rjmp loop
