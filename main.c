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
    interrupts_lower = 0;
    interrupts_upper = 65535;
    DCmotors_lower = 50;
    DCmotors_upper = 50;
    colourcard_flag = 0;
    unknowncard_flag = 0;
    returnhome_flag = 0;
    
    ADC_init();
    colourclick_init();
    DCmotors_init(PWMperiod);
    USART4_init();
    checkbatterylevel();
    
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
    
    /****************************
     * Colour calibration routine
     ****************************/
    colourclick_calibration();
    
    /***************************
     * Motor calibration routine
     ***************************/
    DCmotors_calibration(&motorL, &motorR);
    
    /******************
     * 
     *****************/
    while(RF2_BUTTON && RF3_BUTTON);
    MAINBEAM_LED = 1;
    colourclickLEDs_C(1);
    __delay_ms(1000);
    interrupts_init();
    forward(&motorL, &motorR);
    
    /*********************
     * Infinite while loop
     *********************/
    RGBC_val current;
    while(1) {
        /*********
         * Testing
         *********/
        colourcards_testing();
        DCmotors_testing(&motorL, &motorR);
        
        /*****************
         * Maze navigation
         *****************/
        if (colourcard_flag==1) {
            colourclick_readRGBC(&current);
            colourcards_readRGBC(&current, &motorL, &motorR);
            card_flag = 0;
        }
    }
}
