#include <xc.h>   // Include processor file
#include "main.h" // Include corresponding header file

/***************
 * Main function
 ***************/
void main(void) {
    /**************************
     * Initialisation functions
     **************************/
    RGBC_val current;             // Initialise structure to store current RGBC values
    unsigned char start = 0;      // Initialise variable to indicate whether it's the first loop 
    unsigned char PWMperiod = 99; // 0.0001s*(64MHz/4)/16 -1 = 99
    timer = 0;                    // Initilaise 
    instr_counter = 0;            //
    dur_counter = 0;              //
    turnleft_calangle = 360;      // Angle turned left by motor during calibration
    turnright_calangle = 360;     // Angle turned right by motor during calibration
    interrupts_lowerbound = 0;    // Lower clear threshold value to trigger interrupts when encounter colour cards
    interrupts_upperbound = 0;    // Upper clear threshold value to trigger interrupts when encounter colour cards
    colourcard_flag = 0;          // Toggled when buggy encounters a colour card
    unknowncard_flag = 0;         // Incremented each time the buggy fails to identify a colour card
    returnhome_flag = 0;          // Toggled when buggy has found the final white card or in exceptions
    
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
    
    ADC_init();               // Initialise ADC module to read buggy battery level
    colourclick_init();       // Initialise colour click module
    DCmotors_init(PWMperiod); // Initialise DC motors to run the buggy
    USART4_init();            // Initialise USART4 module to use serial communication for testing purposes
    checkbatterylevel();      // Check buggy battery level to see if we need charging before beginning the maze
    
    /***************************
     * Motor calibration routine
     ***************************/
    DCmotors_calibration(&motorL, &motorR); // Initiate motor calibration routine (see DC_motors.c)
//    DCmotors_testing(&motorL, &motorR);     // Test all DC motor movements
    
    /****************************
     * Colour calibration routine
     ****************************/
    colourclick_calibration();                           // Initiate colour calibration routine (see colour_click.c)
//    colourcards_testingRGBC(&current, &motorL, &motorR); // Test colour cards reading with actual motor movements
//    colourcards_testingRGBC2();                          // Test colour cards readung with serial communication output on screen
    
    /***************
     * Getting ready
     ***************/
    while(RF2_BUTTON && RF3_BUTTON); // Wait for button press
    MAINBEAM_LED = 1;                // Switch on front and rear headlights
    colourclickLEDs_C(1);            // Switch on clear light on colour click module to improve colour recognition
    __delay_ms(1000);                // Wait a short delay before starting the maze
    interrupts_init();               // Initialise interrupts
    timer0_init();                   // Initialise timer0 module
    
    /*****************
     * Maze navigation
     *****************/
    // Infinite while loop
    while(1) {
        checkbatterylevel();                                  // Step 1: Check current battery level (warn user to charge buggy if low battery)
        
        if (start<1 && colourcard_flag==1) {                  // Step 2: Prevent accidental trips at beginning of maze
            colourcard_flag = 0;                              // Reset colour card flag if buggy had a false alarm in the first loop
            start = 1;                                        // Indicate that the buggy has passed the initial hurdle
            
        } else if (start>0 && colourcard_flag==1) {           // Step 3: 
            stop(&motorL, &motorR);                           //
            RD7_LED = 1;                                      //
            RH3_LED = 1;                                      //
            reverse(&motorL, &motorR);                        //
            __delay_ms(100);                                  //
            stop(&motorL, &motorR);                           //
            RD7_LED = 0;                                      //
            RH3_LED = 0;                                      //
            __delay_ms(1000);                                 //
            
            colourcards_readRGBC(&current, &motorL, &motorR); // Read card colour and move accordingly
            
            __delay_ms(1000);                                 // 
            colourclick_readRGBC(&current);                   //
            interrupts_lowerbound = current.C - 150;          //
            interrupts_upperbound = current.C + 100;          //
            
            colourcard_flag = 0;                              //
            
        } else if (returnhome_flag==1) {                      // Step 4:
            returnhome(&motorL, &motorR);                     //
            returnhome_flag=2;                                //
            
        } else if (returnhome_flag==0) {                      // Step 5:
            forward(&motorL, &motorR);                        //
        }
    }
}
