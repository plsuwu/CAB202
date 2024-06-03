#include <stdint.h>
#include <stdio.h>

int main() {
    uint8_t a = 0x0;
    uint8_t b = 0x0;
    uint8_t c = 0x0;
    uint8_t d = 0x0;

    a &= 0b11111100;
    b = b & 0b11111100 | 0b00001100;
    c = c | 0b11111100 & 0b00001100;
    d |= 0b00001100;

    printf("a: %b, b: %b, c: %b, d: %b\n", a,b,c,d);

    a = 0xFF;
    b = 0xFF;
    c = 0xFF;
    d = 0xFF;

    a &= 0b11111100;
    b = b & 0b11111100 | 0b00001100;
    c = c | 0b11111100 & 0b00001100;
    d |= 0b00001100;

    printf("a: %b, b: %b, c: %b, d: %b\n", a,b,c,d);

    return 0;
}
