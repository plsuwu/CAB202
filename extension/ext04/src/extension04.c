#include <stdint.h>

/** EX: E4.0

Matrix arithmetic operations are commonly required in many algorithms.
In this exercise you are required to write four functions which will
implement the following matrix operations:

    matrix_sum()   - Matrix addition: A + B
    matrix_add()   - Scalar addition: A + k
    matrix_scale() - Scalar multiplication: A * k
    matrix_mul()   - Matrix multiplication: A * B

A matrix of dimensions m x n has m rows and n columns. In C, this can be
represented as a 2-dimensional array of the form:

    matrix[m][n]

When passing a matrix as an argument to a function, a pointer to the
first element of the array must be used, to accommodate for different
dimension matrices. The dimensions of this matrix are supplied as an
additional argument.

For this exercise:

- The dimensions of a matrix must be stored in a two-element array of
  unsigned 8-bit integers, where:
  - The first element contains the number of rows
  - The second element contains the number of columns
  For example, {#rows, #cols}.
- Matrices must be stored as 2-dimensional arrays of signed 16-bit integers.
- Scalars must be stored as signed 16-bit integers.

You must define prototypes for your functions in extension04.h so that
they may be called by external code. The implementation of each function
should be placed in this file.

To test your functions, you may use the example code provided in main.c
and print matrices via the serial interface. main.c will be replaced
by our own test code on Gradescope.

For the following tasks, consider the 2 x 2 matrix A:

A = [ 1 2 ]
    [ 3 4 ]

and the 2 x 3 matrix B:

B = [ 1 2 3 ]
    [ 4 5 6 ]

*/

/** FUNCTION: matrix_sum() (Matrix Addition)

DESCRIPTION:

Computes the sum of two matrices of the same dimension, A and B, by
adding corresponding elements of A and B.

As this operation is element-wise, the resulting matrix will have the
same dimensions as A and B.

EXAMPLE:

B + B = [ 1 2 3 ] + [ 1 2 3 ]
        [ 4 5 6 ]   [ 4 5 6 ]

      = [ 1+1 2+2 3+3 ]
        [ 4+4 5+5 6+6 ]

      = [ 2  4  6 ]
        [ 8 10 12 ]

TASK: Write a function that computes the sum of two matrices.
ARGUMENTS: Arguments must be passed in the following order:
- matrix A
- dimensions of A or B
- matrix B
- resultant matrix
*/

/** CODE: Write your code for matrix_sum() below this line. */


/** CODE: Write your code for matrix_sum() above this line. */

/** FUNCTION: matrix_add() (Scalar Addition)

DESCRIPTION:

Computes the sum of a matrix, A, and a scalar, k, by adding k to each
element of A.

As this operation is element-wise, the resulting matrix will have the
same dimensions as A.

EXAMPLE:

B + 1 = [ 1 2 3 ] + 1
        [ 4 5 6 ]

      = [ 1+1 2+1 3+1 ]
        [ 4+1 5+1 6+1 ]

      = [ 2 3 4 ]
        [ 5 6 7 ]

TASK: Write a function that computes the sum of matrix and a scalar.
ARGUMENTS: Arguments must be passed in the following order:
- matrix A
- dimensions of A
- scalar addend k
- resultant matrix
*/

/** CODE: Write your code for matrix_add() below this line. */


/** CODE: Write your code for matrix_add() above this line. */

/** FUNCTION: matrix_scale() (Scalar Multiplication)

DESCRIPTION:

Computes the product of a matrix, A, and a scalar, k, by multiplying
each element of A by k.

As this operation is element-wise, the resulting matrix will have the
same dimensions as A.

EXAMPLE:

B * 2 = [ 1 2 3 ] * 2
        [ 4 5 6 ]

      = [ 1*2 2*2 3*2 ]
        [ 4*2 5*2 6*2 ]

      = [ 2  4  6 ]
        [ 8 10 12 ]

TASK: Write a function that computes the product of a matrix and a scalar.
ARGUMENTS: Arguments must be passed in the following order:
- matrix A
- dimensions of A
- scalar multiplicand k
- resultant matrix
*/

/** CODE: Write your code for matrix_scale() below this line. */


/** CODE: Write your code for matrix_scale() above this line. */

/** FUNCTION: matrix_mul() (Matrix Multiplication)

DESCRIPTION:

Computes the matrix product of the m x p matrix A and p x n matrix B,
by taking the product of the rows in A and the columns in B.

The resulting matrix will have dimensions m x n.

EXAMPLE:

By treating the rows of A as row vectors, and the columns of B as column
vectors, we can identify the products required to compute A * B:

A = [ --a1-- ]
    [ --a2-- ]
    [   ...  ]

B = [ |  |  |      ]
    [ b1 b2 b3 ... ]
    [ |  |  |      ]

A * B = [ (a1 * b1) (a1 * b2) (a1 * b3) ... ]
        [ (a2 * b1) (a2 * b2) (a2 * b3) ... ]
        [    ...       ...       ...    ... ]

The product of a row vector with a column vector is demonstrated below:

Consider the row vector a:

a = [ 1 2 ]

and the column vector b:

b = [ 1 ]
    [ 4 ]

The product of a and b is the sum of the element-wise products of a and b

a . b = 1*1 + 2*4
      = 1 + 8
      = 9

Therefore, by performing the product of each row vector in A with each
column vector in B, we can compute the matrix product of A and B:

A * B = [ 1 2 ] * [ 1 2 3 ]
        [ 3 4 ]   [ 4 5 6 ]

      = [ (1*1 + 2*4) (1*2 + 2*5) (1*3 + 2*6) ]
        [ (3*1 + 4*4) (3*2 + 4*5) (3*3 + 4*6) ]

      = [  9 12 15 ]
        [ 19 26 33 ]

Note that the inner dimension of A and B (p) must be equal for this
operation to be valid. This is because the product of row and column
vectors shown above is element-wise.

You do not need to perform any error handling for this, and can assume
that all tests on Gradescope use valid arguments.

TASK: Write a function that computes the product of two matrices.
ARGUMENTS: Arguments must be passed in the following order:
- matrix A
- dimensions of A
- matrix B
- dimensions of B
- resultant matrix
*/

/** CODE: Write your code for matrix_mul() below this line. */


/** CODE: Write your code for matrix_mul() above this line. */

/** RESOURCE: You can find many examples of these operations online. */
// https://online.stat.psu.edu/statprogram/reviews/matrix-algebra/arithmetic
