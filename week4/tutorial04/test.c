#include <stdio.h>
#include <stdint.h>

int main(void) {

    uint16_t c_release = 1972;

    // 0b0000 0*->111 1011 0100
    unsigned char b[2];

    uint16_t byte;
    int i, j;

    b[0] = c_release >> 8;
    b[1] = c_release;

    printf("0b");
    for (i = 0; i <= 1; i++) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%b", byte);
        }
    }

    printf("\n");
}
