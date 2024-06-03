#include <stdint.h>

void matrix_sum(int16_t* a, uint8_t dim_2x2[2], int16_t* b, int16_t* result);
void matrix_add(int16_t* a, uint8_t* dim_2x2, int16_t addend, int16_t* result);
void matrix_scale(int16_t* a, uint8_t* dim_2x2, int16_t multiplicand, int16_t* result);
void matrix_mul(int16_t *identity, uint8_t* dim_2x2a, int16_t* a, uint8_t* dim_2x2b, int16_t* result);
