#include "init.h"
#include <avr/io.h>

#include "game.h"
#include "types.h"
#include "game.h"
#include "lfsr.h"
#include "timer.h"
#include "display.h"

void game_reset(u8 *sequence_length, u8 *sequence_index, u8 *user_index) {
    *sequence_length = 1;
    *sequence_index = 0;
    *user_index = 0;
}

void button_init(void) {
    // init all four buttons (4 = pb1, 5 = pb2, 6 = pb3, 7 = pb4)
    PORTA.PIN4CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN5CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN6CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN7CTRL = PORT_PULLUPEN_bm;
}

void pwm_init(void) {

    // output enable buzzer pin
    PORTB.DIRSET = PIN0_bm;

    // divide clock speed by 2
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV2_gc;
    // single slope PWM on default route (portb)
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc | TCA_SINGLE_CMP0EN_bm;
    TCA0.SINGLE.PER = 0;
    TCA0.SINGLE.CMP0 = 0;   // start buzzer disabled

    // OR-assign to enable TCA0 without overwriting CLKSEL bits
    TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
}

void spi_init(void) {

    // DISP LATCH on PA1
    PORTA.OUTCLR = PIN1_bm;     // drive high to avoid blocking data
    PORTA.DIRSET = PIN1_bm;

    // output enable SPI CLK (PC0) + MOSI (PC2)
    PORTC.DIRSET = PIN0_bm | PIN2_bm;

    PORTMUX.SPIROUTEA = PORTMUX_SPI0_ALT1_gc;   // use ALT port multiplex to use PORTC
    SPI0.CTRLB = SPI_SSD_bm;                    // disable client line
    SPI0.INTCTRL = SPI_IE_bm;                   // interrupt -enable
    SPI0.CTRLA = SPI_MASTER_bm | SPI_ENABLE_bm; // enable SPI0 in host mode
}

void timer_init(void) {
    // 1ms interrupt (timer/button debounce)
    TCB0.CCMP = 3333;           // ~1ms
    TCB0.INTCTRL = TCB_CAPT_bm; // enable periodic interrupt
    TCB0.CTRLA = TCB_ENABLE_bm; // enable

    // 5ms interrupt (disp.)
    TCB1.CCMP = 16667;          // ~5ms
    TCB1.INTCTRL = TCB_CAPT_bm; // period interrupt
    TCB1.CTRLA = TCB_ENABLE_bm; // enable
}

void adc_init(void) {
    ADC0.CTRLA = ADC_ENABLE_bm;
    ADC0.CTRLB = ADC_PRESC_DIV2_gc;
    ADC0.CTRLC = (4 << ADC_TIMEBASE_gp) | ADC_REFSEL_VDD_gc;
    ADC0.CTRLE = 64;
    ADC0.CTRLF = ADC_FREERUN_bm | ADC_LEFTADJ_bm;
    ADC0.MUXPOS = ADC_MUXPOS_AIN2_gc;
    ADC0.COMMAND = ADC_MODE_SINGLE_12BIT_gc | ADC_START_IMMEDIATE_gc;
}

void uart_init(void) {
    // 9600 baud
    USART0.BAUD = 1389;

    // rx compl. interrupt enable
    USART0.CTRLA = USART_RXCIE_bm;
    USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm; // enable RX + TX
}
