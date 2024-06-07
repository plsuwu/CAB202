#include "init.h"
#include <avr/io.h>

void button_init(void) {
    PORTA.PIN4CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN5CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN6CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN7CTRL = PORT_PULLUPEN_bm;
}

void pwm_init(void) {

    // output enable
    PORTB.DIRSET = PIN0_bm;

    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV2_gc;
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc | TCA_SINGLE_CMP0EN_bm;
    TCA0.SINGLE.PER = 0;
    TCA0.SINGLE.CMP0 = 0;

    TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
}

void spi_init(void) {

    PORTA.OUTCLR = PIN1_bm;
    PORTA.DIRSET = PIN1_bm;
    PORTC.DIRSET = PIN0_bm | PIN2_bm;

    PORTMUX.SPIROUTEA = PORTMUX_SPI0_ALT1_gc;
    SPI0.CTRLB = SPI_SSD_bm;
    SPI0.INTCTRL = SPI_IE_bm;
    SPI0.CTRLA = SPI_MASTER_bm | SPI_ENABLE_bm;
}

void timer_init(void) {
    // 1ms interrupt (timer/button debounce)
    TCB0.CCMP = 3333;
    TCB0.INTCTRL = TCB_CAPT_bm; // enable periodic interrupt
    TCB0.CTRLA = TCB_ENABLE_bm;

    // 5ms interrupt (disp.)

    TCB1.CCMP = 16667;
    TCB1.INTCTRL = TCB_CAPT_bm; // period interrupt
    TCB1.CTRLA = TCB_ENABLE_bm;
}

// adc init

void uart_init(void) {
    // 9600 baud
    USART0.BAUD = 1389;

    // rx compl. interrupt enable
    USART0.CTRLA = USART_RXCIE_bm;
    USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;

}



