#include <xc.h>
#include "interrupts.h"
#include "color_click.h"
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
    INT1PPS=0x09;
    
    PIE0bits.INT1IE = 1; //enable external interrupt source
    //PIE?bits.? = 1; //enable ? interrupt source
    
    IPR0bits.INT1IP = 1; //set clear channel interrupt to high priority 
    //IPR?bits.? = 0; //set ? interrupt to low priority
    
    colorclick_int_clear();
    
    INTCONbits.IPEN = 1; // Enable priority levels on interrupts
    INTCONbits.INT1EDG = 0; //falling edge
    INTCONbits.PEIE = 1;                        // Enable peripheral interrupts
    INTCONbits.GIE = 1;                         // Enable global interrupts (when this is off, all interrupts are deactivated)
}

/****************************************************************
 * High ISR
 * Special function for high priority interrupt service routine
 * Make sure all enabled interrupts are checked and flags cleared
 * Approaching card
 ****************************************************************/
void __interrupt(high_priority) HighISR() {
    if (PIR0bits.INT1IF) {                        // Check the interrupt source
        LATHbits.LATH3 = !LATHbits.LATH3;   // Toggle this variable to inform that ?
        colorclick_int_clear();
        PIR0bits.INT1IF = 0;                      // Clear the interrupt flag
    }
}

/****************************************************************
 * Low ISR
 * Special function for low priority interrupt service routine
 * Make sure all enabled interrupts are checked and flags cleared
 * Battery low
 ****************************************************************/
void __interrupt(low_priority) LowISR() {
//    if (PIR?bits.?) {                      // Check the interrupt source
//    battery_flag = 1;                          // Toggle this variable to inform that ?
//        PIR?bits.? = 0;                    // Clear the interrupt flag
//    }
}
