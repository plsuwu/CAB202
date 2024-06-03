#include "buttons.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>

#include "qutyserial.h"

/** Tutorial 10

INTRODUCTION:

In this week's tutorial you will implement pushbutton debouncing and
implement a state machine using enum and switch-case statements.

The Tutorial 10 exercises should implement the state machine depicted in
state_machine_tut10.png.

Pushbutton handling on the QUTy is a core requirement for Assessment 2.
*/

/** EX: 10.0

TASK: Declare an enumerated type (enum) that can hold the values of each
state required to implement the state machine in "state_machine_tut10.png".

Use meaningful names for each enum value.

TIP: Use the "typedef" keyword to create a type alias for your enum.
*/

typedef enum {
    CURR_OFF,
    CONFIRM_ON,
    CURR_ON,
    CONFIRM_OFF,
} led_state_t;

/** CODE: Write your code for Ex 10.0 above this line. */

int main(void)
{

    // serial_init();
    buttons_init();

    /** EX: 10.1

    TASK: Declare a variable of the enumerated type you declared above.

    Initialise this variable with the initial state of the state machine.
    */
    led_state_t led = CURR_OFF;

    PORTB.OUTSET = PIN5_bm;
    PORTB.DIRSET = PIN5_bm;

    static uint8_t pb_prev_state = 0xFF;

    /** CODE: Write your code for Ex 10.1 above this line. */

    while (1)
    {
        /** EX: 10.2

        The code in "buttons.c" uses periodic interrupts to sample the
        state of the pushbuttons into the variable "pb_debounced_state".

        TASK: Write code to detect pushbutton press events for S1-S4
        (i.e., detect the falling edge of the bits in "pb_debounced_state").

        Store the output of this detection in the variable "pb_falling_edge".

        HINT: To detect an edge, you will need to keep track of the
        previous state of the pushbuttons.

        This will require you to declare a variable whose value is
        preserved between iterations of this while loop.
        */

        uint8_t pb_fall = pb_prev_state & ~pb_debounced_state;
        pb_prev_state = pb_fall;
        /** CODE: Write your code for Ex 10.2 above this line. */

        // printf("PB-PREV: %u\nPB-FALL: %u\nPB-DBOUNCE: %u\n",pb_prev_state, pb_fall, pb_debounced_state);
        /** EX: 10.3

        TASK: Write a switch-case statement below that implements the
        state machine in "state_machine_tut10.png".

        The switch statement should test the value of the variable
        declared in Ex 10.1 and a case should be defined for every state
        in the state machine (every enumerator in the enum you declared
        in Ex 10.0).

        Use the "pb_falling_edge" variable from Ex 10.2 to trigger the
        appropriate transitions between states.
        */

        switch (led) {
            case CURR_OFF:
                PORTB.DIRSET = (PIN5_bm << 1);
                if ((pb_fall & PIN4_bm) == 0) {
                    led = CONFIRM_ON;
                    break;
                }

                break;

            case CONFIRM_ON:
                if ((pb_fall & PIN5_bm) == 0) {
                    led = CURR_ON;
                    break;
                } else {
                    led = CURR_OFF;
                    break;
                }
                break;

            case CURR_ON:
                PORTB.DIRSET = PIN5_bm;

                if ((pb_fall & PIN6_bm) == 0) {
                    led = CONFIRM_OFF;
                    break;
                }

                break;

            default:
                if ((pb_fall & PIN7_bm) == 0) {

                    led = CURR_OFF;
                    break;
                } else {
                    led = CURR_ON;
                }

                break;

        }

        /** CODE: Write your code for Ex 10.3 above this line. */
    }

    /** EX: 10.4

    TASK: Modify the code in "buttons.c" so that the variable
    "pb_debounced_state" holds a debounced version of the pushbutton's
    state, rather than a raw sample of PORTA.IN.
    */

    /** CODE: Write your code for Ex 10.4 in buttons.c. */
}
