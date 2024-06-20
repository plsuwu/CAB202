#include "initialisation.h"

#include <avr/io.h>

void button_init(void)
{
    // Enable pull-up resistors for PBs
    PORTA.PIN4CTRL = PORT_PULLUPEN_bm; // S1
    PORTA.PIN5CTRL = PORT_PULLUPEN_bm; // S2
    PORTA.PIN6CTRL = PORT_PULLUPEN_bm; // S3
    PORTA.PIN7CTRL = PORT_PULLUPEN_bm; // S4
}

void pwm_init(void)
{
    /** EX: 12.2

    TASK: Configure TCA0 for waveform output on PB0. The buzzer should
    be silent initially.
    */

    /** CODE: Write your code for Ex 12.2 within this function. */

}

void spi_init(void)
{
    // Configure pins
    // DISP LATCH (PA1)
    PORTA.OUTCLR = PIN1_bm;
    PORTA.DIRSET = PIN1_bm;
    // SPI CLK (PC0), SPI MOSI (PC2)
    PORTC.DIRSET = PIN0_bm | PIN2_bm;

    PORTMUX.SPIROUTEA = PORTMUX_SPI0_ALT1_gc;   // Select alternate pin configuration for SPI
    SPI0.CTRLB = SPI_SSD_bm;                    // Disable client select line
    SPI0.INTCTRL = SPI_IE_bm;                   // Enable interrupt
    SPI0.CTRLA = SPI_MASTER_bm | SPI_ENABLE_bm; // Enable SPI as master
}

void timer_init(void)
{
    // Interrupt every 5 ms
    TCB1.CCMP = 16667;
    TCB1.INTCTRL = TCB_CAPT_bm;
    TCB1.CTRLA = TCB_ENABLE_bm;
}
