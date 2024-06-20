// include guard
#ifndef TYPES_H
#define TYPES_H
#include <stdint.h>

/*
 * Aliasing for often-used types that I found annoying to
 * read or write out.
 */

// alias integers for readability
// unsigned
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

// signed
typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

//  Enum to describe digit positioning
typedef enum {
    left,
    right
} digit_side_t;

// Boolean enum
typedef enum {
    false = 0,
    true = 1
} bool_t;

#endif // TYPES_H
