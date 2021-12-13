#include <xc.h>
#include "main.h"

/***************
 * Main function
 ***************/
void main(void) {
    /**************************
     * Initialisation functions
     **************************/
    unsigned char PWMperiod = 99; // 0.0001s*(64MHz/4)/16 -1 = 99
    interrupts_lowerbound = 0;
    interrupts_upperbound = 32767;
    turnleft_delay = 0;
    turnright_delay = 0;
    overtime_flag = 0;
    colourcard_flag = 0;
    unknowncard_flag = 0;
    returnhome_flag = 0;
    
    DC_motor motorL;                                 // Initialise DC_motor structure for motorL
    motorL.power=0;                                  // Set motor power to 0 at start
    motorL.direction=1;                              // Set default motor direction forward
    motorL.dutyHighByte=(unsigned char *)(&PWM6DCH); // PWM duty high byte address
    motorL.dir_LAT=(unsigned char *)(&LATE);         // LAT for direction pin address
    motorL.dir_pin=4;                                // Pin RE4 controls direction on LAT
    motorL.PWMperiod=PWMperiod;                      // Base period of PWM cycle
    
    DC_motor motorR;                                 // Initialise DC_motor structure for motorR 
    motorR.power=0;                                  // Set motor power to 0 at start
    motorR.direction=1;                              // Set default motor direction forward
    motorR.dutyHighByte=(unsigned char *)(&PWM7DCH); // PWM duty high byte address
    motorR.dir_LAT=(unsigned char *)(&LATG);         // LAT for direction pin address
    motorR.dir_pin=6;                                // Pin RG6 controls direction on LAT
    motorR.PWMperiod=PWMperiod;                      // Base period of PWM cycle
    
    ADC_init();
    colourclick_init();
    DCmotors_init(PWMperiod);
    USART4_init();
    checkbatterylevel();
    
    /****************************
     * Colour calibration routine
     ****************************/
    colourclick_calibration();
//    colourcards_testingRGBC();
    
    /***************************
     * Motor calibration routine
     ***************************/
    DCmotors_calibration(&motorL, &motorR);
//    DCmotors_testing(&motorL, &motorR);
    
    /***************
     * Getting ready
     ***************/
    while(RF2_BUTTON && RF3_BUTTON);
    MAINBEAM_LED = 1;
    colourclickLEDs_C(1);
    __delay_ms(1000);
    interrupts_init();
    
    unsigned char start = 0;
    /*****************
     * Maze navigation
     *****************/
    RGBC_val current;
    while(1) {
        if (start<1 && colourcard_flag>0){      //prevents accident trips at beginning
            colourcard_flag = 0;
            start = 1;
        }
        
        if (colourcard_flag==1 && start>0) {
            stop(&motorL, &motorR);
            MAINBEAM_LED = 0;
            TURNLEFT_LED = 1;
            TURNRIGHT_LED = 1;
            reverse(&motorL, &motorR);
            __delay_ms(100);
            stop(&motorL, &motorR);
            __delay_ms(1000);
            TURNLEFT_LED = 0;
            TURNRIGHT_LED = 0;
            MAINBEAM_LED = 1;
            
            TURNLEFT_LED = 1;
            TURNRIGHT_LED = 1;
            colourcards_readRGBC(&current, &motorL, &motorR);
            __delay_ms(1000);
            TURNLEFT_LED = 0;
            TURNRIGHT_LED = 0;
            
            colourclick_readRGBC(&current);
            interrupts_upperbound = current.C + 100;
            interrupts_lowerbound = current.C - 150;
            colourcard_flag = 0;
        } else forward(&motorL, &motorR);
    }
}
