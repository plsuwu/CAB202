#!/usr/bin/env python

port = 0x0

def set_port(p):
    p &= (0x02 | 0x20)
    return p

def out_port(p):
    p &= 0x20
    return p

def clr_port(p):
    p ^= 0x20
    return p

def pretty(val):
    pp = bin(val)[2:].zfill(16)
    print(pp)

pretty(port)
pretty(set_port(port))
pretty(out_port(set_port(port)))

# u8 pb_state_f = pb_prev_state & ~pb_state;
print('\n----------\n')

btn = 0xFF
prt = set_port(btn)
pretty(prt)
pretty(btn)


