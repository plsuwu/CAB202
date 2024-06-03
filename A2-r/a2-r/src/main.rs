
pub const MASK: u32 = 0xE2023CAB;
pub const SEED: u32 = 0x09669396;
pub const A_BASE_TONE:  u32 = 496;

fn lf_shift_register(state: u32) -> u32 {
    let lsb = state & 1;
    let mut n_state = state >> 1;
    if lsb == 1 {
        n_state ^= MASK;
    }

    return n_state & 0b11;
}

fn check_input() {

}

fn tone_values(mod_freq: &str) -> Vec<u32> {
}

fn tone_generator() {

}

fn main() {
    // init
    let mut rng_state = SEED;
    let mut sequence = 1;
    let mut step = sequence & 1;
    let simon: u32;



    println!("Hello, world!");
}
