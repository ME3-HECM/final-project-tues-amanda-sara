#include <xc.h>     // Include processor file
#include "timers.h" // Include corresponding header file

/***********************************************
 * Timer0_init
 * Function used to set up Timer 0 to track time
 ***********************************************/
 void timer0_init(void) {           // Remember to write to High reg first, update happens when low reg is written to
    T0CON1bits.T0CS=0b010;          // Fosc/4 (see datasheet pg 354)
    T0CON1bits.T0ASYNC=1;           // Needed to ensure correct operation when Fosc/4 used as clock source (see datasheet errata)
    T0CON1bits.T0CKPS=PRESCALER;    // Adjust prescaler (see datasheet pg 369)
    T0CON0bits.T016BIT=1;           // Set to 16-bit mode (see datasheet pg 358)
    
    TMR0H=TMR0H_BITS;               // Set initial value for 16-bit Timer 0 most significant byte
    TMR0L=TMR0L_BITS;               // Set initial value for 16-bit Timer 0 least significant byte
    T0CON0bits.T0EN=1;              // Start the timer
    
    // Calculate required prescaler using T_int=0.1/65535=4*PS/64000000
    // Thus, we need a prescaler 1:24.41443503... to get an overflow every 0.1 s (i.e. 100 ms)
    // However, such a prescaler does not exist, so the closest one is 1:32, which will cause an overflow every 0.13107 seconds
    // It's a good idea to initialise the timer registers at a fine-tuned value to correct our error of 0.04856 when the timer overflows
    // We need to start the timer at a higher value to reduce the time period by 0.03107 seconds before the next overflow occurs
    // Thus, we start the timer at n=65535-(0.1/(4*32/64000000))=15535
}
 