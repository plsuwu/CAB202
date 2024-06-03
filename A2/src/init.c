#include <avr/interrupt.h>
#include <avr/io.h>

#include "init.h"


/*
 * PWM init => TCA0
 */
void pwm_init(void) {
    PORTB.DIR = PIN0_bm | PIN5_bm | PIN1_bm;        // 0: BUZZER | 5: DISP DP | 1: DISP EN

    // waveform output controls:
    //
    //       CMP0EN =>  PA0 PWM (BUZZER         TCA0WO0 on      PORTB PIN0)
    //       CMP1EN =>  PA1 PWM (DISP EN.       TCA1WO1 on      PORTB PIN1)
    //
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc | TCA_SINGLE_CMP0EN_bm | TCA_SINGLE_CMP1_bm;
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV2_gc;
    TCA0.SINGLE.PER = 4281;
    TCA0.SINGLE.CMP1 = 0;                           // display disabled
    TCA0.SINGLE.CMP0 = 0;                           // buzzer disabled

    TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
}

/*
 * TCB init => TCB0/TCB1 configured in periodic interrupt mode
 */
void tcb_init(void) {

    // TCB0 in periodic interrupt mode -> button reading
    //
    TCB0.CTRLB = TCB_CNTMODE_INT_gc;                    // configure in periodic interrupt mode
    TCB0.CTRLA = TCB_CLKSEL_DIV2_gc;                    // CLKSEL / 2
    TCB0.CCMP = 4281;                                   // set period
    TCB0.INTCTRL = TCB_CAPT_bm;                         // invoke CAPT ISR @ CCMP
    TCB0.CTRLA |= TCB_ENABLE_bm;                        // enable (avoid overwriting CLKSEL using `|=` )


    // TCB1 in periodic interrupt mode -> display latching
    //
    TCB1.CTRLB = TCB_CNTMODE_INT_gc;
    TCB1.CTRLA = TCB_CLKSEL_DIV2_gc;
    TCB1.CCMP = 4281;                                   // -> basically a random number
    TCB1.INTCTRL = TCB_CAPT_bm;
    TCB1.CTRLA |= TCB_ENABLE_bm;
}

/*
 * ADC init => ADC0
 */
void adc_init(void) {

    // potentiometer config
    ADC0.CTRLA = ADC_ENABLE_bm;                                         // enable
    ADC0.CTRLB = ADC_PRESC_DIV2_gc;                                     // 2x prescaler
    ADC0.CTRLC = (4 << ADC_TIMEBASE_gp) | ADC_REFSEL_VDD_gc;            // 4 CLK_PER cycles @ 3.3MHz | VDD as ref
    ADC0.CTRLE = 64;                                                    // samp duration of 64
    ADC0.CTRLF = ADC_FREERUN_bm | ADC_LEFTADJ_bm;                       // freerun, l-adj res
    ADC0.MUXPOS = ADC_MUXPOS_AIN2_gc;                                   // AIN2 (POT R1) select
    ADC0.COMMAND = ADC_MODE_SINGLE_8BIT_gc | ADC_START_IMMEDIATE_gc;    // 8-bit single ended | start immediate
}

/*
 * UART init
 */
void uart_init(void) {
    USART0.BAUD = 1389;                             // 9600 baud
    PORTB.DIRSET = PIN2_bm;                         // output enable TX pin
    USART0.CTRLA = USART_RXCIE_bm;                  // enable RX interrupt
    USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;   // enable RX and TX
}

/*
 * SPI INIT => VPORTC.OUT on PIN0 (SPI_CLK) | PIN2 (MOSI)
 */
void spi_init(void) {

    //  (PIN0: SPI_CLK | PIN2: MOSI)
    VPORTC.OUT |= PIN0_bm | PIN2_bm; // drive LOW
    VPORTC.DIR |= PIN0_bm | PIN2_bm; // enable CLK/MOSI as output

    PORTMUX.SPIROUTEA = PORTMUX_SPI0_ALT1_gc;

    SPI0.CTRLB = SPI_SSD_bm;                        // disable client select line
    SPI0.INTCTRL = SPI_IE_bm;                       // enable SPI interrupts (SPI DATA latch)
    SPI0.CTRLA |= SPI_MASTER_bm | SPI_ENABLE_bm;    // enable using SPI as master
}

/*
 * PORTA init => PIN4/PIN5/PIN6/PIN7 -> pullup resistor-enabled
 */
void button_init(void) {

    // pullup en. all buttons
    //
    PORTA.PIN4CTRL |= PORT_PULLUPEN_bm;
    PORTA.PIN5CTRL |= PORT_PULLUPEN_bm;
    PORTA.PIN6CTRL |= PORT_PULLUPEN_bm;
    PORTA.PIN7CTRL |= PORT_PULLUPEN_bm;
}

