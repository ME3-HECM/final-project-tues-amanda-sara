#include <xc.h>             // Include processor file
#include "interrupts.h"     // Include corresponding header file
#include "buttons_n_LEDs.h" // Include header file to use buttons and LEDs on the clicker 2 board for testing purposes
#include "colour_click.h"   // Include header file to use colour click module functions
#include "I2C.h"            // Include header file to use I2C for programming the colour click module
#include "timers.h"         // Include header file to use timer0 module

/************************************************************************************************
 * interrupts_init
 * Function used to turn on and set priority for interrupts to detect colour cards and track time
 ************************************************************************************************/
void interrupts_init(void){
    TRISBbits.TRISB1 = 1;   // Set TRIS value for pin RB1 (input)
    ANSELBbits.ANSELB1 = 0; // Turn off analogue input for pin RB1
    INT1PPS=0x09;           // Set peripheral pin select module
    
    PIE0bits.INT1IE = 1;    // Enable external interrupt source
    PIE0bits.TMR0IE = 1;    // Enable timer interrupt source
    
    IPR0bits.INT1IP = 1;    // Set clear channel interrupt to high priority 
    IPR0bits.TMR0IP = 0;    // Set timer interrupt to low priority
    
    interrupts_clear();     // Clear interrupts on the colour click module
    
    INTCONbits.INT1EDG = 0; // Set interrupt on falling edge
    INTCONbits.IPEN = 1;    // Enable priority levels on interrupts
    INTCONbits.PEIE = 1;    // Enable peripheral interrupts
    INTCONbits.GIE = 1;     // Enable global interrupts last, once all other interrupt configuration is done (when this is off, all interrupts are deactivated)
}

/**************************************************************
 * interrupts_clear
 * Function used to clear interrupts on the colour click module
 **************************************************************/
void interrupts_clear(void){
    I2C_2_Master_Start();            // Start condition
    I2C_2_Master_Write(0x52 | 0x00); // 7 bit device address + Write mode
    I2C_2_Master_Write(0b11100110);  // Command + Register address  
    I2C_2_Master_Stop();             // Send stop bit
    
    interrupts_colourclick();        // Initialise interrupts on colour click modules
}

/***************************************************************
 * interrupts_colour
 * Function used to set up interrupts on the colour click module
****************************************************************/
void interrupts_colourclick(void){
    colourclick_writetoaddr(0x00, 0b10011);                          // write to enable register and turn on interrupts
    __delay_ms(3);                                                   // delay before next write 
    colourclick_writetoaddr(0x0C, 0b0100);                           // write to persistence register and set to trigger interrupt after 5 readings outside range 
    colourclick_writetoaddr(0x04, (interrupts_lowerbound & 0x00FF)); // set low bits for low threshold
    colourclick_writetoaddr(0x05, (interrupts_lowerbound >> 8));     // set high bits for low threshold
    colourclick_writetoaddr(0x06, (interrupts_upperbound & 0x00FF)); // set low bits for high threshold
    colourclick_writetoaddr(0x07, (interrupts_upperbound >> 8));     // set high bits for high threshold
}

/********************************************************************************************
 * HighISR
 * Special function for high priority interrupt service routine when approaching colour cards
 ********************************************************************************************/
void __interrupt(high_priority) HighISR() {
    if (PIR0bits.INT1IF) {    // Check the interrupt source 
        colourcard_flag = 1;  // Toggle variable to run read card routine
//        RD7_LED = !RD7_LED; // Testing
        interrupts_clear();   // Force clear interrupt flag on colour click 
        PIR0bits.INT1IF = 0;  // Clear the interrupt flag
    }
}

/************************************************************************************
 * LowISR
 * Special function for low priority interrupt service routine when timer 0 overflows
 ************************************************************************************/
void __interrupt(low_priority) LowISR() {
    if (PIR0bits.TMR0IF) {  // Check the interrupt source
//        RD7_LED=!RD7_LED; // Testing
        timer++;             // increment timer 
        TMR0H=TMR0H_BITS;    // reset initial timer values (high)
        TMR0L=TMR0L_BITS;    //                             (low)
        PIR0bits.TMR0IF = 0; // Clear the interrupt flag
    }
}
