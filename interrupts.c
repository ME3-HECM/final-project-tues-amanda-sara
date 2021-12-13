#include <xc.h>
#include "interrupts.h"
#include "color_click.h"
#include "i2c.h"
#include "dc_motor.h"

#define AILTL 0x04
#define AILTH 0x05
#define AIHTL 0x06
#define AIHTH 0x07

volatile unsigned char read_val;
volatile unsigned int clear_lower;
volatile unsigned int clear_upper;

/****************************************************************************************************
 * Interrupts_init
 * Function to turn on interrupts and set if priority is used
 * Turn on the interrupt sources, peripheral interrupts and global interrupts
 * It's a good idea to turn on global interrupts last, once all other interrupt configuration is done
 ****************************************************************************************************/

void Timer0_init(void)
{
    T0CON1bits.T0CS=0b010; // Fosc/4
    T0CON1bits.T0ASYNC=1; // see datasheet errata - needed to ensure correct operation when Fosc/4 used as clock source
    T0CON1bits.T0CKPS=0b1000; // 1:256 count every 1.6ms
    T0CON0bits.T016BIT=1;	//16bit mode	
	
    // it's a good idea to initialise the timer registers so we know we are at 0
    TMR0H=0xFD;            // should be exactly 10ms to overflow 
    TMR0L=0x8F;
    T0CON0bits.T0EN=1;	//start the timer
}


void interrupts_init(void){
//    TRISBbits.TRISB1 = 1;
//    ANSELBbits.ANSELB1 = 0;
//    INT1PPS=0x09;
    
//    PIE0bits.INT1IE = 1; //enable external interrupt source
    PIE0bits.TMR0IE=1; 
    //PIE?bits.? = 1; //enable ? interrupt source
    
    IPR0bits.INT1IP = 1; //set clear channel interrupt to high priority 
    //IPR?bits.? = 0; //set ? interrupt to low priority
    
//    interrupts_clear();
    
    INTCONbits.IPEN = 1; // Enable priority levels on interrupts
//    INTCONbits.INT1EDG = 0; //falling edge
    INTCONbits.PEIE = 1;                        // Enable peripheral interrupts
    INTCONbits.GIE = 1;                         // Enable global interrupts (when this is off, all interrupts are deactivated)
}

/******************
 * Interrupts_clear
 ******************/
void interrupts_clear(void){
    I2C_2_Master_Start();         //Start condition
    I2C_2_Master_Write(0x52 | 0x00);     //7 bit device address + Write mode
    I2C_2_Master_Write(0b11100110);    //command + register address  
    I2C_2_Master_Stop();
    colour_int_init();
}

void colour_int_init(void){
    colorclick_writetoaddr(0x00, 0b10011);
    __delay_ms(3);
    colorclick_writetoaddr(0x0C, 0b0100);
    colorclick_writetoaddr(AILTL, (clear_lower&0b11111111));
    colorclick_writetoaddr(AILTH, (clear_lower>>8));
    colorclick_writetoaddr(AIHTL, (clear_upper&0b11111111));
    colorclick_writetoaddr(AIHTH, (clear_upper>>8));
}

/****************************************************************
 * High ISR
 * Special function for high priority interrupt service routine
 * Make sure all enabled interrupts are checked and flags cleared
 * Approaching card
 ****************************************************************/
void __interrupt(high_priority) HighISR() {
    if (PIR0bits.TMR0IF) {                        // Check the interrupt source 
        read_val = 1;                            // Toggle variable to run read card routine 
        TMR0H=0xFD;                               // should be exactly 10ms to overflow 
        TMR0L=0x8F;
//        RD7_LED = !RD7_LED;                       // Testing
//        interrupts_clear();
        PIR0bits.TMR0IF = 0;                      // Clear the interrupt flag
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
