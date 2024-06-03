#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>

// `uint<n>_t` is really annoying to type out and we get where they
// come from, so i have shortened to this Rust-like syntax
typedef unsigned long long u64;
typedef unsigned long u32;
typedef unsigned int u16;
typedef unsigned char u8;

typedef long long i64;
typedef long i32;
typedef int i16;
typedef char i8;

/*
 * PER control state (debug)
 */
typedef enum { PER_OFF, PER_ON } per_state_t;
volatile per_state_t per_state = PER_OFF;

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
 * UART init (not correctly working)
 */
void uart_init(void) {

     // output enable TX pin
    PORTB.DIRSET = PIN2_bm;
    // 9600 baud
    USART0.BAUD = 1389;
    // enable RX interrupt
    USART0.CTRLA = USART_RXCIE_bm;
    // enable RX and TX
    USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;
}

/*
 * RW functions (blocking)
 */
u8 uart_getc(void) {
    while (!(USART0.STATUS & USART_RXCIF_bm)); // wait data

    return USART0.RXDATAL;
}

void uart_putc(u8 c) {
    while (!(USART0.STATUS & USART_DREIF_bm)); // wait for TX.DATA empty

    USART0.TXDATAL = c;
}

static int stdio_putchar(char c, FILE *stream);
static int stdio_getchar(FILE *stream);

// declare stream for stdio -> FDEV_SETUP_STREAM macro
static FILE stdio = FDEV_SETUP_STREAM(stdio_putchar, stdio_getchar, _FDEV_SETUP_RW);

static int stdio_putchar(char c, FILE *stream) {
    uart_putc(c);
    return c;
}

static int stdio_getchar(FILE *stream) {
    return uart_getc();
}

void stdio_init(void) {
    stdout = &stdio;
    stdin = &stdio;
}

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

// debug disp en. + spi
volatile i8 DISP_LBYTE = 0b10000001;              // all LED segs + set 7th bit   -> LHS
volatile i8 DISP_RBYTE = 0b00000001;              // all LED segs + unset 7th bit -> RHS
volatile u8 DISP_CSIDE = 0;                       // bool to alternate byte to send

void spi_init(void) {

    //  (PIN0: SPI_CLK | PIN2: MOSI)
    VPORTC.OUT |= PIN0_bm | PIN2_bm; // drive LOW
    VPORTC.DIR |= PIN0_bm | PIN2_bm; // enable CLK/MOSI as output

    PORTMUX.SPIROUTEA = PORTMUX_SPI0_ALT1_gc;

    SPI0.CTRLB = SPI_SSD_bm;                        // disable client select line
    SPI0.INTCTRL = SPI_IE_bm;                       // enable SPI interrupts (SPI DATA latch)
    SPI0.CTRLA |= SPI_MASTER_bm | SPI_ENABLE_bm;    // enable using SPI as master
}

ISR(SPI0_INT_vect) {

    // latch data

    PORTA.OUT &= ~PIN1_bm;                          // drive high to begin pulse
    PORTA.OUT |= PIN1_bm;                           // drive back low to finish pulse

    SPI0.INTFLAGS = SPI_IF_bm;                      // interrupt ack
}

/*
 * BUTTON state
 */
volatile u8 debounce = 0xFF;
volatile u8 fall = 0;
volatile u8 rise = 0;

void check_edge(u8 *sample) {
    static u8 count_a = 0;
    static u8 count_b = 0;

    u8 edge = *sample ^ debounce;
    u8 prev = debounce;

    count_a = ~count_a & edge;
    count_b = (count_b ^ count_a) & edge;

    debounce ^= (count_b & count_a) | (edge & prev);

    fall = edge & prev;
    rise = edge & debounce;

    return;
}

ISR(TCB1_INT_vect) {
    if (DISP_CSIDE == 0) {
        SPI0.DATA = DISP_LBYTE;
    } else {
        SPI0.DATA = DISP_RBYTE;
    }

    DISP_CSIDE ^= 1;
    TCB1_INTFLAGS = TCB_CAPT_bm;
}

ISR(TCB0_INT_vect) {
    // sample buttons & mutate `fall`/`rise`
    u8 sample = PORTA.IN;
    check_edge(&sample);

    // set peripheral state (ON/OFF)
    switch (per_state) {
    case PER_OFF:
        if (fall & PIN4_bm) {
            per_state = PER_ON;
            break;
        }
    case PER_ON:
        if (rise & PIN4_bm) {
            per_state = PER_OFF;
            break;
        }
    }


    TCB0_INTFLAGS = TCB_CAPT_bm;
}

/*
 * INIT - PORTA button configuration
 */
void button_init(void) {

    // pullup en. all buttons
    //
    PORTA.PIN4CTRL |= PORT_PULLUPEN_bm;
    PORTA.PIN5CTRL |= PORT_PULLUPEN_bm;
    PORTA.PIN6CTRL |= PORT_PULLUPEN_bm;
    PORTA.PIN7CTRL |= PORT_PULLUPEN_bm;
}

/*
 * INIT - TCB0 configuration
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

int main(void) {

    // disable interrupts globally while configuring interrupt-generating peripherals

    cli();
    uart_init();
    stdio_init();
    tcb_init();                                         // configure + enable TCB0/1 counter
    pwm_init();                                         // configure + enable PWM (buzzer & disp)
    spi_init();                                         // configure + enable PORTC peripherals (SPI stuff)
    button_init();                                      // configure + enable PORTA buttons
    adc_init();                                         // configure + enable ADC

    // reenable interrupts
    sei();

    u8 result;

    u8 putme = 'A';
    stdio_putchar(putme, stdout);

    while (1) {

        // read POT into result
        result = ADC0.RESULT0;

        // DISP DP OUTPUT EN.
        if (per_state) {

            uart_putc(putme);

            // clr PORTB output bits,
            // buzzer duty to 50%   (?)
            // disp en. to 50%      (?)
            PORTB.OUTCLR = PIN5_bm | PIN0_bm | PIN1_bm;
            TCA0.SINGLE.CMP0BUF = TCA0.SINGLE.PER / 3;                          // buzzer
            TCA0.SINGLE.CMP1BUF = (u32)result * TCA0.SINGLE.PER / 255;          // disp
        } else {

            // set PORTB output bits
            // buzzer duty to 0%
            // disp en. to 0%
            PORTB.OUTSET = PIN5_bm | PIN0_bm | PIN1_bm;
            TCA0.SINGLE.CMP0BUF = 0;
            TCA0.SINGLE.CMP1BUF = 0;
        }

        ;;
    }
}
