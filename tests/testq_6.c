#include <stdint.h>
#include <stdio.h>

static int iter = 0;
uint64_t factorial(uint64_t n) {
    // iter++;
  if (n == 0) {
    // printf("%u", iter);
    return 1;
  } else
  return (n * factorial(n - 1));
}

int main(void) {

  // struct date {
  //   uint16_t year : 14;
  //   uint8_t month : 4;
  //   uint8_t day : 5;
  //   uint8_t hour : 5;
  //   uint8_t minute : 6;
  //   uint8_t second : 6;
  // };
  //
  // uint8_t a[] = {0x01, 0x10, 0x03};
  //
  // // int val = *(uint16_t *)&a;
  // // printf("%x", val);
  //
  // // uint16_t so_struct = sizeof(struct date);
  // printf("%lu\n", sizeof(struct date));

  // int8_t temperature_readings[24] = { 0, 1, 7, 3, 4, 5, 6 };
  // int8_t temp;
  //
  // //temp = *(temperature_readings + 2);
  uint16_t n = 333;
  //
  // int8_t *ptr = temperature_readings;
  // ptr += 2;
  // temp =*ptr;

  // printf("%u\n", temp);

  // uint16_t max_val, curr_val;
  // max_val = 1234;
  // uint32_t cycles_per_second = 3333333;
  //
  // uint32_t num_cycles = 42049;
  //
  // curr_val = (max_val * num_cycles) / cycles_per_second;

  uint64_t result = factorial(9);
  printf("%lu\n", result);

  return 0;
}
