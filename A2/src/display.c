#include "display.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "game.h"
#include "types.h"
#include "serial.h"

static bool_t display_side_toggle = false;
static u8 display_data = 0xFF;
static volatile u8 display_side = 1;
static volatile u8 display_lhs = 0b11111111;
static volatile u8 display_rhs = 0b01111111;

const u8 number_segment_map[10] = {
//  00SFABGCDE
    0b00001000, // 0
    0b01101011, // 1
    0b01000100, // 2
    0b01000001, // 3
    0b00100011, // 4
    0b00010001, // 5
    0b00010011, // 6
    0b01001011, // 7
    0b00000000, // 8
    0b00000001, // 9
};

void display_score(u8 score) {
    u8 in_range = score % 100;
    u8 tens = in_range / 10;
    u8 ones = in_range % 10;

    u8 tens_segment = (number_segment_map[tens] | 1);
    u8 ones_segment = number_segment_map[ones];

    display_lhs = tens_segment;
    display_rhs = ones_segment;
    display_side_toggle =  true;

    printf("lhs -> [%u] : [%u]\n", tens, tens_segment);
    printf("rhs -> [%u] : [%u]\n", ones, ones_segment);

    PORTB.OUTCLR = PIN0_bm | PIN1_bm;
    TCA0.SINGLE.CMP1BUF = TCA0.SINGLE.PER + 1;

    write_spi(display_lhs);
    _delay_ms(DEBUG_DELAY_HALF);
}

void display_segment(CurrentState *curr, u8 prev_period) {
    if (curr->disp_side == TGL) {
        printf("TGL DISP\n");
        u8 write_segment = (curr->disp_segment | display_side);
        display_data = write_segment;

    } else {
        printf("NOT TGL DISP\n");
        display_side_toggle = false;
        u8 write_segment = (curr->disp_segment | curr->disp_side);
        display_data = write_segment;
    }

    PORTB.OUTCLR = PIN0_bm | PIN1_bm;
    TCA0.SINGLE.CMP1BUF = TCA0.SINGLE.PER + 1;
    write_spi(display_data);
    _delay_ms(DEBUG_DELAY_HALF);
    // _delay_ms(DEBUG_DELAY);

}

// init SPI PORT & ROUTE to DISP, enable interrupt generation on data TX to SPI CLK/MOSI
void spi_disp_init(void) {

    // (PIN0 : SPI_CLK | PIN2 : MOSI)
    VPORTC.OUT |= PIN0_bm | PIN2_bm;                // drive LOW
    VPORTC.DIR |= PIN0_bm | PIN2_bm;                // enable CLK/MOSI as output

    PORTMUX.SPIROUTEA = PORTMUX_SPI0_ALT1_gc;       // set port multiplexer to ALT route
                                                    // ........... (-> PC0,PC1,PC2,PC3)

    SPI0.CTRLB = SPI_SSD_bm;                        // disable client select line
    SPI0.INTCTRL = SPI_IE_bm;                       // enable SPI interrupts (SPI DATA latch)

    SPI0.CTRLA |= SPI_MASTER_bm | SPI_ENABLE_bm;    // enable using SPI as master
}

// TCB1 timer/counters -> latch/MOSI/CLK
void tcb_disp_init(void) {

    // TCB1 in periodic interrupt mode
    TCB1.CTRLB = TCB_CNTMODE_INT_gc;
    TCB1.CTRLA = TCB_CLKSEL_DIV2_gc;
    TCB1.CCMP = 16667;
    TCB1.INTCTRL = TCB_CAPT_bm;
    TCB1.CTRLA |= TCB_ENABLE_bm;
}

// SPI INT latch pulse handler
ISR(SPI0_INT_vect) {

    // latch data on PIN1
    PORTA.OUT &= ~PIN1_bm; // drive high - pulse start
    PORTA.OUT |= PIN1_bm;  // drive low - pulse end

    SPI0.INTFLAGS = SPI_IF_bm; // interrupt ack
}

ISR(TCB1_INT_vect) {
    if (display_side_toggle) {
        if (display_side) {
            write_spi(display_lhs);
        } else {
            write_spi(display_rhs);
        }
        display_side ^= display_side;

    } else {
        write_spi(display_data);
    }

    TCB1.INTFLAGS = TCB_CAPT_bm;
}

