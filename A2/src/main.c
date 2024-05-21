#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>

#include "init.h"
#include "prng.h"
#include "tones.h"

// DO NOT USE!!
#include "qutyio.h"
// DISCLUDE IN SUBMISSION!!!!

void test_tone_setter(void) {
    uint32_t ret;
    for (uint8_t i = 0; i < 4; i++) {
        ret = set_tone(1,1);
        printf("[%u] using E_HI_K: %lu\n", i, ret);
        ret = set_tone(1, 2);
        printf("[%u] using E_LO_K: %lu\n", i, ret);
        ret = set_tone(1, 3);
        printf("[%u] using TONE_BASE: %lu\n", i, ret);
        ret = set_tone(1, 4);
        printf("[%u] using C_K: %lu\n", i, ret);
    }
}

uint32_t test_state_mut() {
    printf("\n[*] RUNNING TESTS N THINGS:\n\n");

    for (uint8_t i = 0; i < 4; i++)  {
        lfsr();
        printf("%u\n",get_next_step());
    }

    printf("\n");
    return 0;
}

void test(void) {
    // test_state_mut();
    test_tone_setter();

}

int main() {

    serial_init();

    if (debug == 1) {
        test();
    }

    return 0;
}
