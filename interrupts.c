#include <xc.h>
#include "interrupts.h"
#include "color.h"
#include "i2c.h"

/****************************************************************************************************
 * Interrupts_init
 * Function to turn on interrupts and set if priority is used
 * Turn on the interrupt sources, peripheral interrupts and global interrupts
 * It's a good idea to turn on global interrupts last, once all other interrupt configuration is done
 ****************************************************************************************************/

void interrupts_init(void){
    TRISBbits.TRISB1 = 1;
    ANSELBbits.ANSELB1 = 0;
//    INT1PPS=0x09;
    
    PIE0bits.INT1IE = 1; //enable external interrupt
    IPR0bits.INT1IP = 1; //set clear channel interrupt to high priority 
    
    color_writetoaddr(0x04, 0x14);
    color_writetoaddr(0x05, 0x05);
    color_writetoaddr(0x06, 0x6C);
    color_writetoaddr(0x07, 0x07);
    
    INTCONbits.IPEN = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
}

//void Interrupts_init(dateandtime current) {
//    if (current.hour<1 || current.hour>=5) {    // Check if start up timing is outside 1am-5am
//        PIE2bits.C1IE = 1;                      // If yes, enable comparator interrupt source
//    } else {PIE2bits.C1IE = 0;}                 // If no, disable comparator interrupt source initially (this will be turned on later when timing is right)
//    PIE0bits.TMR0IE = 1;                        // Enable timer interrupt source
//    
//    IPR2bits.C1IP = 1;                          // Set comparator interrupt to high priority
//    IPR0bits.TMR0IP = 0;                        // Set timer interrupt to low priority
//    
//    INTCONbits.IPEN = 1;                        // Enable priority levels on interrupts
//    INTCONbits.PEIE = 1;                        // Enable peripheral interrupts
//    INTCONbits.GIE = 1;                         // Enable global interrupts (when this is off, all interrupts are deactivated)
//}

/****************************************************************
 * High ISR
 * Special function for high priority interrupt service routine
 * Make sure all enabled interrupts are checked and flags cleared
 ****************************************************************/
//volatile unsigned char sunrise_flag=0;          // Declare global variable to toggle when sunrise occurs (to minimise code within interrupts)
//volatile unsigned char sunset_flag=0;           // Declare global variable to toggle when sunset occurs (to minimise code within interrupts)

void __interrupt(high_priority) HighISR() {
    if (PIR0bits.INT1IF) {                        // Check the interrupt source
        LATHbits.LATH3 = !LATHbits.LATH3;       // Toggle the LED1 when the LDR goes from light-to-dark (sunset) or dark-to-light (sunrise)
        PIR0bits.INT1IF = 0;                      // Clear the interrupt flag
    }
}

/****************************************************************
 * Low ISR
 * Special function for low priority interrupt service routine
 * Make sure all enabled interrupts are checked and flags cleared
 ****************************************************************/
//volatile unsigned char time_flag=0;             // Declare global variable to toggle when timer overflows (to minimise code within interrupts)

//void __interrupt(low_priority) LowISR() {
//    if (PIR0bits.TMR0IF) {                      // Check the interrupt source
//        TMR0H = TMR0H_BITS;                     // Write to TMR0H first (start the timer at a predetermined value to improve accuracy, see "timers.c" for details)
//        TMR0L = TMR0L_BITS;                     // Then write to TMR0L (start the timer at a predetermined value to improve accuracy, see "timers.c" for details)
//        time_flag = 1;                          // Toggle this variable to inform that timer has overflowed (1 second has passed)
//        PIR0bits.TMR0IF = 0;                    // Clear the interrupt flag
//    }
//}