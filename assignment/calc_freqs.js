const base_frq = 492;
const f_clk = (1/((20*(10**6))/12));

const t = ['Eh', 'C', 'El'];

const pow_nums = [
    -5, -8, -17
];

const freqs = [base_frq];
const clk_cnts = [];
console.log(`A: [${base_frq}]Hz | [${(1/base_frq) / f_clk}] cycles`);



let i = 0;

for (i = 0; i < 3; ++i) {

    let frq = base_frq * 2 ** (pow_nums[i]/12);
    freqs.push(frq);

    let clk = (1/frq) / f_clk;
    clk_cnts.push(clk);

    console.log(`${t[i]}: [${frq}]Hz | [${clk}] cycles`);
}

