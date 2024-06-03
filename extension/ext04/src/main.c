#include <stdint.h>
#include <stdio.h>

#include "extension04.h"
#include "qutyserial.h"

/** Extension 04

Use this file to test the functions you have developed in extension04.c
by declaring the appropriate prototype functions in extension04.h.

This file will be replaced on Gradescope, so feel free to modify
anything in this file.
*/

/**
 * @brief Set all elements of a matrix to zero.
 *
 * @param matrix The matrix to be zeroed.
 * @param elements The number of elements in the matrix.
 */
void matrix_zero(int16_t *matrix, uint8_t elements)
{
    for (uint8_t i = 0; i < elements; i++)
    {
        matrix[i] = 0;
    }
}

/**
 * @brief Print a matrix via the serial interface.
 *
 * @param matrix The matrix to be printed.
 * @param dim An array of the dimensions of the matrix.
 */
void matrix_print(int16_t *matrix, uint8_t *dim)
{
    for (uint8_t i = 0; i < dim[0]; i++)
    {
        for (uint8_t j = 0; j < dim[1]; j++)
        {
            printf("% 6d ", matrix[i * dim[1] + j]);
        }
        printf("\n");
    }
}

int main(void)
{
    serial_init();

    // Define some matrices (2d arrays)
    int16_t matrix_a[2][2] = {{1, 2},
                              {3, 4}};

    int16_t matrix_b[2][3] = {{1, 2, 3},
                              {4, 5, 6}};

    int16_t matrix_identity[2][2] = {{1, 0},
                                     {0, 1}};

    // Get a pointer to the first element of each array
    int16_t *a = (int16_t *)matrix_a;
    int16_t *b = (int16_t *)matrix_b;
    int16_t *identity = (int16_t *)matrix_identity;

    // Define a matrix to hold the result of an operation
    int16_t result[6] = {0}; // initially zero

    // Define some scalars
    int16_t addend = 2;
    int16_t multiplicand = 2;

    // Define some dimensions
    uint8_t dim_2x2[2] = {2, 2};
    uint8_t dim_2x3[2] = {2, 3};

    // Press any key after opening the serial monitor to exit this loop
    while (!serial_bytes_available())
        ;

    // Uncomment the lines below once you have finished implementing the
    // functions in extension04.c.

    printf("A =\n");
    matrix_print(a, dim_2x2);

    printf("B =\n");
    matrix_print(b, dim_2x3);

    matrix_zero(result, 6);
    matrix_add(a, dim_2x2, addend, result);
    printf("A + %d =\n", addend);
    matrix_print(result, dim_2x2);

    matrix_zero(result, 6);
    matrix_sum(a, dim_2x2, identity, result);
    printf("A + I =\n");
    matrix_print(result, dim_2x2);

    matrix_zero(result, 6);
    matrix_scale(a, dim_2x2, multiplicand, result);
    printf("A * %d =\n", multiplicand);
    matrix_print(result, dim_2x2);

    matrix_zero(result, 6);
    matrix_mul(identity, dim_2x2, a, dim_2x2, result);
    printf("I * A =\n");
    matrix_print(result, dim_2x2);

    matrix_zero(result, 6);
    matrix_mul(identity, dim_2x2, b, dim_2x3, result);
    printf("I * B =\n");
    matrix_print(result, dim_2x3);

    while (1)
        ; // Loop indefinitely
}
