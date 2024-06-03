#include <avr/interrupt.h>
#include <avr/io.h>

#include "com.h"
#include "typing.h"


void tcb_init(void);
void pwm_init(void);
void adc_init(void);
void uart_init(void);
void spi_init(void);
void button_init(void);
void stdio_init();
