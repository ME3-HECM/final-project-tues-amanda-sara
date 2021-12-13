#include <xc.h>
#include "timers.h"

/********************************************************************************
 * Timer0_init
 * Function used to set up Timer 0
 * Remember to write to High reg first, update happens when low reg is written to
 ********************************************************************************/
 void Timer0_init(void) {
    T0CON1bits.T0CS=0b010;          // Fosc/4 (see datasheet pg 354)
    T0CON1bits.T0ASYNC=1;           // Needed to ensure correct operation when Fosc/4 used as clock source (see datasheet errata)
    T0CON1bits.T0CKPS=PRESCALER;    // Adjust prescaler (see datasheet pg 369)
    T0CON0bits.T016BIT=1;           // Set to 16-bit mode (see datasheet pg 358)
    
    TMR0H=TMR0H_BITS;               // Set initial value for 16-bit Timer 0 most significant byte
    TMR0L=TMR0L_BITS;               // Set initial value for 16-bit Timer 0 least significant byte
    T0CON0bits.T0EN=1;              // Start the timer
    
    // Note for "testing" mode
    // Calculate required prescaler using T_int=(1/3600)/65535=4*PS/64000000
    // Thus, we need a prescaler 1:0.0678178751... to get an overflow every 0.000277777 ... second
    // However, such a prescaler does not exist, so the closest one is 1:1, which will cause an overflow every 0.004095937... seconds
    // It's a good idea to initialise the timer registers at a fine-tuned value to correct our error of 0.003818159... seconds when the timer overflows
    // We need to start the timer at a higher value to reduce the time period before the next overflow occurs by 0.00381816... seconds
    // Thus, we start the timer at n=65535-((1/3600)/(4*1/64000000))=61090.55556 ~ 61091
    
    // Note for "normal" mode
    // Calculate required prescaler using T_int=1/65535=4*PS/64000000
    // Thus, we need a prescaler 1:244.1443503... to get an overflow every second
    // However, such a prescaler does not exist, so the closest one is 1:256, which will cause an overflow every 1.04856 seconds
    // It's a good idea to initialise the timer registers at a fine-tuned value to correct our error of 0.04856 when the timer overflows
    // We need to start the timer at a higher value to reduce the time period by 0.04856 seconds before the next overflow occurs
    // Thus, we start the timer at n=65535-(1/(4*256/64000000))=3035
}