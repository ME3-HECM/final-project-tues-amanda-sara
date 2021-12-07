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
//    INT1PPS=0x09;
    
    colorclick_writetoaddr(0x04, 0x14);
    colorclick_writetoaddr(0x05, 0x05);
    colorclick_writetoaddr(0x06, 0x6C);
    colorclick_writetoaddr(0x07, 0x07);
    
    PIE0bits.INT1IE = 1; //enable external interrupt source
    //PIE?bits.? = 1; //enable ? interrupt source
    
    IPR0bits.INT1IP = 1; //set clear channel interrupt to high priority 
    //IPR?bits.? = 0; //set ? interrupt to low priority
    
    INTCONbits.IPEN = 1;                        // Enable priority levels on interrupts
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
//    if (PIR?bits.?) {                        // Check the interrupt source
//        card_flag=1;           // Toggle this variable to inform that ?
//        PIR?bits.? = 0;                      // Clear the interrupt flag
//    }
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
