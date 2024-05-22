; Relevant pins:
; - Port A PIN 1: DISP LATCH
; - Port C PIN 0: SPI CLK
; - Port C PIN 2: SPI MOSI


; 1. Set the outputs of PC0, PC2, and PA1 to LOW,
;    so that their associated nets are driven LOW
;    when each pin is output enabled


; 2. Output enable the above pins to set the state of the nets


; 3. Determine the state of each segment on the 7-segment display
;    using the following segment mapping:
;
;     -- A --           Segment | State (ON/OFF) | State of MOSI net
;    |       |          ---------------------------------------------
;    F       B          A       |                |
;    |       |          B       |                |
;     -- G --           C       |                |
;    |       |          D       |                |
;    E       C          E       |                |
;    |       |          F       |                |
;     -- D --           G       |                |
;
;
; 4. Determine the value of the byte to be transmitted to the
;    shift register based on the desired state of each segment that you
;    determined above.
;
;    Q7 will control which digit is illuminated on the 7-segment display.
;
;    The byte should be ordered Q76543210, where Q7 corresponds to the MSB
;    and Q0 corresponds to the LSB
;
;    Hint: Refer to the QUTy schematic to determine how each pin Q0-Q7 maps
;          to the segments A-G.
;    Hint: The LEDs are active low.


; 5. Repeat the following for all 8 bits in the above byte, transmitting
;    the MSB first so that it is shifted to Q7 after 8 clocks:
;    a. If the bit is a 1:
;         Drive the SPI MOSI net HIGH
;       If the bit is a 0:
;         Drive the SPI MOSI net LOW
;    b. Drive the SPI CLK net HIGH (to create a rising edge)
;    c. Drive the SPI CLK net LOW (to prepare for the next rising edge)


; 6. a. Drive the DISP LATCH net HIGH (to create a rising edge)
// 9 -> ABCDGF ->
