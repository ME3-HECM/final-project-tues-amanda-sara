//#include <xc.h>
//#include "interrupts.h"
//#include "buttons_n_LEDs.h"
//#include "colour_click.h"
//#include "I2C.h"
//#include "DC_motors.h"
//
///****************************************************************************************************
// * Interrupts_init
// * Function to turn on interrupts and set if priority is used
// * Turn on the interrupt sources, peripheral interrupts and global interrupts
// * It's a good idea to turn on global interrupts last, once all other interrupt configuration is done
// ****************************************************************************************************/
//void interrupts_init(void){
//    TRISBbits.TRISB1 = 1;
//    ANSELBbits.ANSELB1 = 0;
//    INT1PPS=0x09;
//    
//    PIE0bits.INT1IE = 1; //enable external interrupt source
//    //PIE?bits.? = 1; //enable ? interrupt source
//    
//    IPR0bits.INT1IP = 1; //set clear channel interrupt to high priority 
//    //IPR?bits.? = 0; //set ? interrupt to low priority
//    
//    interrupts_clear();
//    colourclick_writetoaddr(0x00, 0b10011);
//    __delay_ms(3);
//    colourclick_writetoaddr(0x0C, 0b0100);
//    colourclick_writetoaddr(0x04, 0x14);
//    colourclick_writetoaddr(0x05, 0x05);
//    colourclick_writetoaddr(0x06, 0x6C);
//    colourclick_writetoaddr(0x07, 0x07);
//    
//    INTCONbits.IPEN = 1; // Enable priority levels on interrupts
//    INTCONbits.INT1EDG = 0; //falling edge
//    INTCONbits.PEIE = 1;                        // Enable peripheral interrupts
//    INTCONbits.GIE = 1;                         // Enable global interrupts (when this is off, all interrupts are deactivated)
//}
//
///******************
// * Interrupts_clear
// ******************/
//void interrupts_clear(void){
//    I2C_2_Master_Start();         //Start condition
//    I2C_2_Master_Write(0x52 | 0x00);     //7 bit device address + Write mode
//    I2C_2_Master_Write(0b11100110);    //command + register address  
//    I2C_2_Master_Stop();
//    
////    colourclick_writetoaddr(AILTL, (interrupts_lower & 0b11111111));
////    colourclick_writetoaddr(AILTH, (interrupts_lower >> 8));
////    colourclick_writetoaddr(AIHTL, (interrupts_upper & 0b11111111));
////    colourclick_writetoaddr(AIHTH, (interrupts_upper >> 8));
//}
//
///****************************************************************
// * High ISR
// * Special function for high priority interrupt service routine
// * Make sure all enabled interrupts are checked and flags cleared
// * Approaching card
// ****************************************************************/
//void __interrupt(high_priority) HighISR() {
//    if (PIR0bits.INT1IF) {                        // Check the interrupt source 
//        card_flag = 1;                            // Toggle variable to run read card routine
//        interrupts_clear();
//        PIR0bits.INT1IF = 0;                      // Clear the interrupt flag
//    }
//}
//
///****************************************************************
// * Low ISR
// * Special function for low priority interrupt service routine
// * Make sure all enabled interrupts are checked and flags cleared
// * Battery low
// ****************************************************************/
//void __interrupt(low_priority) LowISR() {
////    if (PIR?bits.?) {                      // Check the interrupt source
////        RD7_LED = 1;
////        RH3_LED = 1;
////        PIR?bits.? = 0;                    // Clear the interrupt flag
////        PIE?bits.? = 0;
////    }
//}
