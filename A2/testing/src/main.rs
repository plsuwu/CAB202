const __SEED_INIT: u32 = 0x09669396;
const __MASK: u32 = 0xE2023CAB;

const __A_BASE: u32 = 4 * (9 * 6);

fn precompute_float(float: f32) -> u32 {
    let res = (float * 2.00_f32.powf(15.0)) as u32;
    println!("{}", res);

    return res;
}

fn set_tones(nums: Vec<u32>, oct: u32) -> Vec<u32> {
    let mut res: Vec<u32> = Vec::new();
    let numerator: Vec<f32> = vec![-5.00, -8.00, -17.00];
    let mut float_quant_precompute = Vec::new();

    for n in numerator {
        let power = 2.00_f32.powf(n / 12 as f32);

        let pqf = precompute_float(power);
        float_quant_precompute.push(pqf);
    }

    println!("pqfs -> {:?}", float_quant_precompute);
    println!("reading as -> {:?}", read_tones(float_quant_precompute));

    return res;
}

fn read_tones(k_q: Vec<u32>) -> Vec<u32> {
    let mut res: Vec<u32> = Vec::new();

    for t in k_q {
        let hz = (__A_BASE * t + (1 << 14)) >> 15;
        res.push(hz);
    }

    return res;
}

fn lfsr(seed: u32) -> u32 {
    let lsb = seed & 1;
    let mut state = seed >> 1;

    if lsb == 1 {
        state = state ^ __MASK;
    }

    return state;
}

fn game_loop(n: u32) -> u32 {
    let res = lfsr(n);
    let step = res & 0x3;

    println!("{:?} => step: {:?}", res, step);

    return res;
}

fn main() {
    // let mut curr = __SEED_INIT;
    // for _ in 0..32 {
    //     curr = game_loop(curr);
    //     println!("{:?}", curr);
    // }

    println!();
    println!("{:?}", set_tones(vec![9,6], 1));
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_tones() {
        assert_eq!(set_tones(vec![9,6], 1), vec![162, 136, 81]);
        assert_eq!(set_tones(vec![9,6], 0), vec![162, 136, 81]);
        assert_eq!(set_tones(vec![9,6], 3), vec![486, 408, 243]);
    }

}


