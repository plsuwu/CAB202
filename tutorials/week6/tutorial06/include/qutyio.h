#include <stdint.h>

// DISPLAY
//
// Uses
//   Pins: PA1, PB1, PC0, PC2, PC3
//   Peripherals: SPI0, TCB0
void display_init(void);                       // Initialise display
void display_on(void);                         // Turn on display
void display_off(void);                        // Turn off display
void display_hex(uint8_t value);               // Display unsigned integer in hexadecimal
void display_raw(uint8_t left, uint8_t right); // Set segment states for left and right digits
void display_brightness(uint8_t value);        // Set display brightness

// SERIAL
//
// Uses
//   Pins: PB2, PB3
//   Peripherals: USART0
void serial_init(void);               // Initialise UART as stdin/stdout
uint8_t serial_bytes_available(void); // Returns number of bytes in receive buffer
