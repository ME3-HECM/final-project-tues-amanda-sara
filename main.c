#include <xc.h>   // Include processor file
#include "main.h" // Include corresponding header file

/***************
 * Main function
 ***************/
void main(void) {
    /**************************
     * Initialisation functions
     **************************/
    RGBC_val current;             // 
    unsigned char start = 0;      // 
    unsigned char PWMperiod = 99; // 0.0001s*(64MHz/4)/16 -1 = 99
    turnleft_calangle = 360;      // Angle turned left by motor during calibration
    turnright_calangle = 360;     // Angle turned right by motor during calibration
    interrupts_lowerbound = 0;    // Lower clear threshold value to trigger interrupts when encounter colour cards
    interrupts_upperbound = 0;    // Upper clear threshold value to trigger interrupts when encounter colour cards
    colourcard_flag = 0;          // Toggled when buggy encounters a colour card
    unknowncard_flag = 0;         // Incremented each time the buggy fails to identify a colour card
    returnhome_flag = 0;          // Toggled when buggy has found the final white card or in exceptions
    overtime_flag = 0;            // Toggled when buggy has been stuck in the maze for too long
    
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
    
    ADC_init();               //
    colourclick_init();       //
    DCmotors_init(PWMperiod); //
    timer0_init();            //
    USART4_init();            //
    checkbatterylevel();      //
    
    /***************************
     * Motor calibration routine
     ***************************/
    DCmotors_calibration(&motorL, &motorR); //
//    DCmotors_testing(&motorL, &motorR);     //
    
    /****************************
     * Colour calibration routine
     ****************************/
    colourclick_calibration(); //
//    colourcards_testingRGBC(); //
    
    /***************
     * Getting ready
     ***************/
    while(RF2_BUTTON && RF3_BUTTON); //
    MAINBEAM_LED = 1;                //
    colourclickLEDs_C(1);            //
    __delay_ms(1000);                //
    interrupts_init();               //
    
    /*****************
     * Maze navigation
     *****************/
    while(1) {
        if (start==0 && colourcard_flag==1) { // Prevents accidental trips at beginning
            colourcard_flag = 0;
            start = 1;
        } else if (start==1 && colourcard_flag==1) {
            stop(&motorL, &motorR);
            TURNLEFT_LED = 1;
            TURNRIGHT_LED = 1;
            reverse(&motorL, &motorR);
            __delay_ms(100);
            stop(&motorL, &motorR);
            TURNLEFT_LED = 0;
            TURNRIGHT_LED = 0;
            __delay_ms(1000);
            
            colourcards_readRGBC(&current, &motorL, &motorR);
            __delay_ms(1000);
            
            colourclick_readRGBC(&current);
            interrupts_upperbound = current.C + 100;
            interrupts_lowerbound = current.C - 150;
            
            colourcard_flag = 0;
        } else {forward(&motorL, &motorR);}
    }
}
