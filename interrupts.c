#include <xc.h>
#include "interrupts.h"
#include "buttons_n_LEDs.h"
#include "colour_click.h"
#include "I2C.h"
#include "DC_motors.h"
#include "timers.h"

/****************************************************************************************************
 * interrupts_init
 * Function to turn on interrupts and set if priority is used
 * Turn on the interrupt sources, peripheral interrupts and global interrupts
 * It's a good idea to turn on global interrupts last, once all other interrupt configuration is done
 ****************************************************************************************************/
void interrupts_init(void){
    TRISBbits.TRISB1 = 1;
    ANSELBbits.ANSELB1 = 0;
    INT1PPS=0x09;
    
    PIE0bits.INT1IE = 1; //enable external interrupt source
    PIE0bits.TMR0IE = 1; //enable timer interrupt source
    
    IPR0bits.INT1IP = 1; //set clear channel interrupt to high priority 
    IPR0bits.TMR0IP = 0; //set timer interrupt to low priority
    
    interrupts_clear();
    
    INTCONbits.INT1EDG = 0; // Set interrupt on falling edge
    INTCONbits.IPEN = 1; // Enable priority levels on interrupts
    INTCONbits.PEIE = 1;                        // Enable peripheral interrupts
    INTCONbits.GIE = 1;                         // Enable global interrupts (when this is off, all interrupts are deactivated)
}

/******************
 * interrupts_clear
 ******************/
void interrupts_clear(void){
    I2C_2_Master_Start();         //Start condition
    I2C_2_Master_Write(0x52 | 0x00);     //7 bit device address + Write mode
    I2C_2_Master_Write(0b11100110);    //command + register address  
    I2C_2_Master_Stop();
    
    interrupts_colour();
}

/*****************
 * interrupts_colour
 ******************/
void interrupts_colour(void){
    colourclick_writetoaddr(0x00, 0b10011);
    __delay_ms(3);
    colourclick_writetoaddr(0x0C, 0b0100);
    colourclick_writetoaddr(AILTL, (interrupts_lowerbound & 0x00FF));
    colourclick_writetoaddr(AILTH, (interrupts_lowerbound >> 8));
    colourclick_writetoaddr(AIHTL, (interrupts_upperbound & 0x00FF));
    colourclick_writetoaddr(AIHTH, (interrupts_upperbound >> 8));
}

/****************************************************************
 * High ISR
 * Special function for high priority interrupt service routine
 * Make sure all enabled interrupts are checked and flags cleared
 * Approaching card
 ****************************************************************/
void __interrupt(high_priority) HighISR() {
    if (PIR0bits.INT1IF) {          // Check the interrupt source 
        colourcard_flag = 1;        // Toggle variable to run read card routine
        RD7_LED = !RD7_LED;         // Testing
        interrupts_clear();
        PIR0bits.INT1IF = 0;        // Clear the interrupt flag
    }
}

/****************************************************************
 * Low ISR
 * Special function for low priority interrupt service routine
 * Make sure all enabled interrupts are checked and flags cleared
 * Battery low
 ****************************************************************/
void __interrupt(low_priority) LowISR() {
    if (PIR0bits.TMR0IF) {                      // Check the interrupt source
        overtime_flag = 1;
        returnhome_flag = 1;
        TMR0H=TMR0H_BITS;
        TMR0L=TMR0L_BITS;
        PIR0bits.TMR0IF = 0;                    // Clear the interrupt flag
        PIE0bits.TMR0IE = 0;
    }
}
