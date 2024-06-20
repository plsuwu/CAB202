/** EX: E5.0

TASK:

Your task is to implement a two-tone siren using the QUTy. You must use
one of the Waveform Outputs of TCA0 to drive the buzzer with a 50 % duty
cycle signal.

Your siren must alternate between the following frequencies:

    f1 = 2AB0 Hz with f1 active for t1 = 3E0 ms
    f2 = 4CD0 Hz with f2 active for t2 = 6F0 ms

where A-F are the 2nd through 7th digits of your student number:

    n*ABCDEF*

EXAMPLE: If your student number was n12345678, then

A = 2, B = 3, C = 4, D = 5, E = 6, F = 7

f1 = 2230 Hz
f2 = 4450 Hz
t1 = 360 ms
t2 = 670 ms

NOTES:

Your program must use either TCA or TCB to time the duration of each
tone. Do not use software based delays.

Your program must be interrupt driven. The main() function expects
init() to return to demonstrate this.

You may define additional variables and functions that are used by
init(), in this file.
*/

// This function is called once by the main() function.
void init(void)
{

}

/** CODE: Write your code for Ex E5.0 above this line. */
