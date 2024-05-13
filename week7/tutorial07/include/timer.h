#include <stdint.h>

/** EX: 7.5

To allow the variables "elapsed_time" and "is_counting" to be shared
between compilation units (i.e., src/timer.c and src/buttons.c), we must
declare them as external variables.

TASK: Declare "elapsed_time" and "is_counting" as external variables.

NOTE: An external declaration must not provide an initialisation.
*/

extern uint32_t elapsed_time;
extern uint8_t is_counting;

/** CODE: Write your code for Ex 7.5 above this line. */
