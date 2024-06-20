#include <stdint.h>
#include <stdio.h>

uint32_t state = 0x09669396;

void next() {

    // printf("prev state: %032b", state);

    uint32_t state_lsb = state & 0x01;
    state = state >> 1;
    if (state_lsb == 1) {
        state = state ^ 0xA8E831E0;
    }

    // printf("new state: %032b\n", state);
}

void print_rand(uint32_t *num) {
  uint32_t test = 0x00000003; // bitmask 0b0..011

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
    putchar('0');
    break;
  }
}

int main(void) {
    for (int i = 0; i < 32; ++i) {
        next();
        print_rand(&state);
    }
    printf("\n");

    // for (int i = 0; i < 32; ++i) {
    //     if (i < 10) {
    //         printf(" %u", i);
    //     } else {
    //         printf("  %u", i);
    //     }
    // }
    printf("\n");
}
