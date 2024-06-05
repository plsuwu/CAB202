// include guard
#ifndef TYPES_H
#define TYPES_H
#include <stdint.h>

typedef     uint64_t        u64;
typedef     uint32_t        u32;
typedef     uint16_t        u16;
typedef     uint8_t         u8;

typedef     int64_t         i64;
typedef     int32_t         i32;
typedef     int16_t         i16;
typedef     int8_t          i8;

typedef enum {
    PER_OFF,
    PER_ON
} per_state_t;

typedef enum {
    false = 0,
    true = 1
} bool_t;

#endif // TYPES_H
