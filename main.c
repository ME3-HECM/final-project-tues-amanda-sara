/**********************************************************
 * A definition calls for a macro substitution
 * See The C programming language, second edition, pp.89-91
 **********************************************************/
#define _XTAL_FREQ 64000000 // Note intrinsic _delay function is 62.5ns at 64,000,000Hz

/***************************************************
 * CONFIG1L (configuration word 1) - oscillators
 * See PIC18(L)F67K40 Data Sheet 40001841D, pp.28-29
 ***************************************************/
#pragma config FEXTOSC = HS         // FEXTOSC external oscillator mode selection bits (HS (crystal oscillator) above 8 MHz; PFM (program memory) set to high power)
#pragma config RSTOSC = EXTOSC_4PLL // Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

/***********************************************************
 * CONFIG3L (configuration word 3) - windowed watchdog timer
 * See PIC18(L)F67K40 Data Sheet 40001841D, pp.32-33
 ***********************************************************/
#pragma config WDTE = OFF // WDT operating mode bits (WDT enabled regardless of sleep)

/**************************************************************************************
 * File inclusion makes it easy to handle collections of #defines and declarations etc.
 * See The C programming language, second edition, pp.88-89
 **************************************************************************************/
#include <xc.h> // Include processor files
#include <stdio.h> // Include standard input output library
#include "ADC.h"
#include "color_card.h"
#include "color_click.h"
#include "dc_motor.h"
#include "i2c.h"
#include "serial.h"
#include "interrupts.h"

volatile unsigned int clear_lower = 0;
volatile unsigned int clear_upper = 0;
volatile unsigned char card_flag;

/***************
 * Main function
 ***************/
void main(void) {
    // Initialisation functions
    ADC_init();
    colorclick_init();
    interrupts_init();
    USART4_init();
    buggyLEDs_init();
    
    int PWMcycle = 99; //PWM period = 10kHz
    DCmotors_init(PWMcycle);
    
    DC_motor motorL, motorR; 		//declare two DC_motor structures 
    
    motorL.power=0; 						//set motor power to 0 at start
    motorL.direction=1; 					//set default motor direction
    motorL.dutyHighByte=(unsigned char *)(&PWM6DCH);	//store address of PWM duty high byte
    motorL.dir_LAT=(unsigned char *)(&LATE); 		//store address of LAT
    motorL.dir_pin=4; 						//pin RE4 controls direction
    motorL.PWMperiod=PWMcycle; 			//store PWMperiod for motor

    //same for motorR but different PWM register, LAT and direction pin
    motorR.power=0; 						//set motor power to 0 at start
    motorR.direction=1; 					//set default motor direction
    motorR.dutyHighByte=(unsigned char *)(&PWM7DCH);	//store address of PWM duty high byte
    motorR.dir_LAT=(unsigned char *)(&LATG); 		//store address of LAT
    motorR.dir_pin=6; 						//pin RG6 controls direction
    motorR.PWMperiod=PWMcycle; 		
    
    TRISHbits.TRISH3 = 0;
    LATHbits.LATH3 = 0;

    // Colour calibration routine
    
    colorclick_toggleClearLED(1);
    MAINBEAM_LED = 1;
    
    __delay_ms(500);
    
    RGB_val initial; 
    RGB_val current;
    initial = colorclick_readColour(initial); //read ambient light value
    
    clear_lower = initial.C - 100;
    clear_upper = initial.C + 300;
    
    // Motor calibration routine
    
    unsigned char halt = 0;
    unsigned char start = 0;
    
    while(1) {
        
        current = colorclick_readColour(current); //read ambient light value
//        
        // Testing using serial communication
        char buf[40];
        unsigned int tmpR = current.R;
        unsigned int tmpG = current.G;
        unsigned int tmpB = current.B;
        unsigned int tmpC = current.C;
        sprintf(buf,"%i %i %i %i\n",tmpR,tmpG,tmpB,tmpC);
        sendStringSerial4(buf);
        __delay_ms(500);
        sprintf(buf,"%i %i %i %i\n",clear_upper,clear_lower,card_flag,initial.C);
        sendStringSerial4(buf);
        __delay_ms(500);
        
        if(start==0 && card_flag){
            card_flag = 0;
            start = 1;
        }
                
        RH3_LED = card_flag;
        RD7_LED = start;
        
//        while(halt<1){
//            if (halt==0){
//                forward(&motorL,&motorR);
//                
//            }
////            else if(card_flag==1){
////                stop(&motorL,&motorR);
////                card_flag;
////                halt = 1;
////            }
//        }
        
        forward(&motorL,&motorR);
        
//        RH3_LED = !RH3_LED;
//        __delay_ms(200);
        
        
    }
}
