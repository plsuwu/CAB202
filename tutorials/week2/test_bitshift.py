#!/usr/bin/env python

init_val = -37
init_divisor = 4

def direct(n: int, d: int):
    remainder = n % d
    division_result = n / d

    print(bin(remainder))
    print(division_result)


direct(init_val, init_divisor)

# direct_calc = direct(init_val, init_divisor)
# print(direct_calc)
