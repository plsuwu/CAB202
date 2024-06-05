// include guard
#ifndef TYPES_H
#define TYPES_H

typedef unsigned long long  u64;
typedef unsigned long       u32;
typedef unsigned int        u16;
typedef unsigned char       u8;

typedef long long           i64;
typedef long                i32;
typedef int                 i16;
typedef char                i8;

typedef enum {
    PER_OFF,
    PER_ON
} per_state_t;

typedef enum {
    false,
    true
} bool_t;

#endif // TYPES_H
