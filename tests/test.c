#include <stdint.h>
#include <stdio.h>

typedef uint8_t u8;

typedef enum {
    RED,
    YELLOW,
    GREEN,
} LED_COLOR;

static LED_COLOR led_color = RED;
// sets LED color
void set_led(LED_COLOR color) {
    led_color = color;
    return;
}

// get curr pushbutton state -
//      takes switch num as arg (1, 2, or 3)
//      ret 0 if not pressed,
//      ret (1/non-zero) otherwise
char c;
char prev_c;
u8 get_sw(u8 sw) {

    // printf("recv: %u\n", c);

    if (sw == 1 && c == '1') {
        return 1;
    }

    if (sw == 1 && c == '2' && prev_c == '1') {
        return 1;
    }

    if (sw == 3 && c == '3') {
        return 1;
    }

    // released
    if (sw == 2 && c == '2') {
        return 1;
    }

    return 0;
}

typedef enum {                      // Q21
    READY,
    SET,
    GO
} STATE;

STATE state;

int main(void) {

    state = READY;                  // Q22
    set_led(RED);                   // Q23

    c = getchar();
    while(1) {                      // Q24

        if (c == prev_c) {
            c = getchar();
        }

        printf("current state: %u\n", state);
        prev_c = c;
        printf("recv: %u (prev => %u)\n", c, prev_c);


        switch(state) {             // Q25
            case READY:
                if (get_sw(1)) {   // Q26
                    state = SET;
                    set_led(YELLOW);
                }
                break;

            case SET:
                if (!get_sw(1)) {
                    state = READY;
                    set_led(RED);
                } else if (get_sw(2)) {            // Q28
                    state = GO;
                    set_led(GREEN);
                }
                break;

            case GO:                // Q29
                if (get_sw(3)) {
                    state = READY;
                    set_led(RED);
                }
                break;

            default:                // Q30
                state = READY;
                set_led(RED);
                break;

        } // end switch
    }
}


