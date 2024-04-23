#include <stdint.h>
#include <stdio.h>

int main() {
  // uint8_t a = 1;
  // const uint8_t *b = &a;
  // *b = 2;

  uint8_t a[] = {1, 2, 3, 4, 5, 6};

  uint8_t x = *(a + 2);
  uint8_t z = a[2];

  printf("%u\n", x);
  printf("%u\n", z);
  // a += 2;
  printf("%u\n", *a);


  uint8_t array[] = {3, 1, 5, 4, 6, 2, 3};
  // printf("%u", array[5]);

  uint32_t array_b[10];
  // printf("%lu\n", sizeof(array_b));

  // uint32_t *a = 0x0400;
  // a += 8;

  // printf("%x", *a);

  return 0;
}
