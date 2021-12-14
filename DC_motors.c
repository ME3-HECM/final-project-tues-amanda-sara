#include <xc.h>             // Include processor file
#include "DC_motors.h"      // Include corresponding header file
#include "ADC.h"            // Include header file to check battery level
#include "buttons_n_LEDs.h" // Include header file to use buttons and LEDs on the clicker 2 board

/*************************************************************
 * DCmotors_init
 * Function used to initialise T2 and PWM for DC motor control
 *************************************************************/
void DCmotors_init(unsigned char PWMperiod) {    
	// timer 2 config
    T2CONbits.CKPS=0b100;   // 1:16 prescaler
    T2HLTbits.MODE=0b00000; // Free Running Mode, software gate only
    T2CLKCONbits.CS=0b0001; // Fosc/4

    // Tpwm*(Fosc/4)/prescaler - 1 = PTPER
    T2PR=PWMperiod;         // Period reg, calculate value for 10kHz base period
    T2CONbits.ON=1;         // Turn the timer on
    
    //initialise your TRIS and LAT registers for PWM
    TRISEbits.TRISE2=0;     // set RE2 TRIS for Output
    TRISEbits.TRISE4=0;     // set RE3 TRIS for Output
    TRISCbits.TRISC7=0;     // set RC7 TRIS for Output
    TRISGbits.TRISG6=0;     // set RG6 TRIS for Output
    
    LATEbits.LATE2=0;       // set RE2 output to 0
    LATEbits.LATE4=0;       // set RE4 output to 0
    LATCbits.LATC7=0;       // set RC7 output to 0
    LATGbits.LATG6=0;       // set RG6 output to 0

    PWM6DCH=0;              // 0% power
    PWM7DCH=0;              // 0% power
    
    PWM6CONbits.EN = 1;     // enable PWM generation
    PWM7CONbits.EN = 1;     // enable PWM generation
    
    RE2PPS=0x0A;            // PWM6 on RE2
    RC7PPS=0x0B;            // PMW7 on RC7
    
    clicker2buttons_init(); // Initialise buttons on clicker 2 board
    clicker2LEDs_init();    // Initialise LEDs on click 2 board
    buggyLEDs_init();       // Initialise LEDs on buggy
}

/************************************************************************
 * DCmotors_setPWM
 * Function used to set PWM output from the values in the motor structure
 ************************************************************************/
void DCmotors_setPWM(DC_motor *m) {
	int PWMduty;                                                                           // Temporary variable to store PWM duty cycle
    
	if (m->direction){PWMduty = (m->PWMperiod)-((int)(m->power)*(m->PWMperiod))/100;}      // If forward, low time increases with power
	else {PWMduty=((int)(m->power)*(m->PWMperiod))/100;}                                   // If reverse, high time increases with power
    
	*(m->dutyHighByte) = (unsigned char)(PWMduty);                                         // Set high duty cycle byte
    
	if (m->direction){*(m->dir_LAT) = (unsigned char)(*(m->dir_LAT) | (1<<(m->dir_pin)));} // If direction is high, set dir_pin bit in LAT to high without changing other bits
	else {*(m->dir_LAT) = (unsigned char)(*(m->dir_LAT) & (~(1<<(m->dir_pin))));}          // If direction is low, set dir_pin bit in LAT to low without changing other bits
}

/***************************************************
 * checkbatterylevel
 * Function used to check battery level on the buggy
 ***************************************************/
void checkbatterylevel(void) {
    unsigned char batterylevel;             // Initialise variable to store battery level
    batterylevel = ADC_getval();            // Use ADC to get battery voltage value
    if (batterylevel<200) {                  // If battery is low
        while(1) { // Unless user presses any button to dismiss the warnings
            RD7_LED = !RD7_LED;             // Keep flashing the RD7_LED,
            RH3_LED = !RH3_LED;             // RH3_LED,
            MAINBEAM_LED = !MAINBEAM_LED;   // MAINBEAM_LED,
            BRAKE_LED = !BRAKE_LED;         // BRAKE_LED,
            TURNLEFT_LED = !TURNLEFT_LED;   // TURNLEFT_LED,
            TURNRIGHT_LED = !TURNRIGHT_LED; // TURNRIGHT_LED,
            colourclickLEDs_RGB();          // and cycle the RGB LED on the colour click module
            __delay_ms(5);                  // Use a short delay so human eye can see change
        }
    }
}

/*****************************************
 * forward
 * Function used to move the buggy forward
 *****************************************/
void forward(DC_motor *mL, DC_motor *mR) {       // Assume buggy was previously stationary
    mL->direction = 1;                           // Left wheels go forward
    mR->direction = 1;                           // Right wheels go forward
    
    while((mL->power < 20) && (mR->power < 20)){ // Will be true until both motors have 20 power
        mL->power += 10;                         // Gradually increment the left motor power by 10
        mR->power += 10;                         // Gradually increment the right motor power by 10
        DCmotors_setPWM(mL);                     // Set PWM output for left motor
        DCmotors_setPWM(mR);                     // Set PWM output for right motor
        __delay_us(50);                          // Add a short delay so that motors don't power to maximum instantaneously
    }
}

/*******************************************
 * reverse
 * Function used to move the buggy backwards
 *******************************************/
void reverse(DC_motor *mL, DC_motor *mR) {       // Assume buggy was previously stationary
    mL->direction = 0;                           // Left wheels go backwards
    mR->direction = 0;                           // Right wheels go backwards
    
    while((mL->power < 50) && (mR->power < 50)){ // Will be true until both motors have 50 power
        mL->power += 10;                         // Gradually increment the left motor power by 10
        mR->power += 10;                         // Gradually increment the right motor power by 10
        DCmotors_setPWM(mL);                     // Set PWM output for left motor
        DCmotors_setPWM(mR);                     // Set PWM output for right motor
        __delay_us(50);                          // Add a short delay so that motors don't power to maximum instantaneously
    }
}

/***************************************************************
 * clearance
 * Function used to create space clearance for the buggy to turn
 ***************************************************************/
void clearance(DC_motor *mL, DC_motor *mR) {
    MAINBEAM_LED = 0; // Step 1: Switch on the front and rear headlamps
    reverse(mL, mR);  // Step 2: Reverse the buggy
    __delay_ms(350);  //    for a short distance/time
    stop(mL, mR);     // Step 3: Stop the buggy
    __delay_ms(1000); //    and wait a short time
    MAINBEAM_LED = 0; // Step 4: Switch off the front and rear headlamps
}

/*******************************************
 * stop
 * Function used to gradually stop the buggy
 *******************************************/
void stop(DC_motor *mL, DC_motor *mR) {        // Assume buggy was previously stationary
    BRAKE_LED = 1;                             // Switch on the brake lights
    
    while((mL->power > 0) && (mR->power > 0)){ // Will be true until both motors have 0 power
        mL->power -= 10;                       // Gradually decrement the left motor power by 10
        mR->power -= 10;                       // Gradually decrement the right motor power by 10
        DCmotors_setPWM(mL);                   // Set PWM output for left motor
        DCmotors_setPWM(mR);                   // Set PWM output for right motor
        __delay_us(50);                        // Add a short delay so that motors don't stop instantaneously
    }
    __delay_ms(500);                           // Add a short delay
    BRAKE_LED = 0;                             // before switching off brake lights
}

/*********************************************
 * turnleft
 * Function used to turn the buggy to the left
 *********************************************/
void turnleft(DC_motor *mL, DC_motor *mR, unsigned int deg) {    // Assume buggy was previously stationary
    mL->direction = 0;                                           // Left wheels go backwards
    mR->direction = 1;                                           // Right wheels go forward

    TURNLEFT_LED = 1;                                            // Turn on left signal
    double delay = ((deg*2.332)+31.506) * 360/turnleft_calangle; // Use linear function found from experimentations (y=mx+c adjusted by desired-angle/actual-angle-rotated-during-motor-calibration-routine)
    while((mL->power < 70) || (mR->power < 70)){                 // Will be true until both motors have 70 power
        if (mL->power < 70) {mL->power += 10;}                   // Gradually increment the left motor power by 10
        if (mR->power < 70) {mR->power += 10;}                   // Gradually increment the right motor power by 10
        DCmotors_setPWM(mL);                                     // Set PWM output for left motor
        DCmotors_setPWM(mR);                                     // Set PWM output for right motor
        __delay_us(50);                                          // Add a short delay so that motors don't spin out-of-control
    }
    unsigned int i;                                              // Declare a counter for our delay for-loop (note delay function can't take variables as input)
    for (i=0; i<delay; i++) {__delay_ms(1);}                     // Delay the required time to turn the required angle
    TURNLEFT_LED = 0;                                            // Turn off left signal
}

/**********************************************
 * turnright
 * Function used to turn the buggy to the right
 **********************************************/
void turnright(DC_motor *mL, DC_motor *mR, unsigned int deg) {     // Assume buggy was previously stationary
    mL->direction = 1;                                             // Left wheels go forward
    mR->direction = 0;                                             // Right wheels go backward

    TURNRIGHT_LED = 1;                                             // Turn on right signal
    double delay = ((deg*2.0303)+62.964) * 360/turnright_calangle; // Use linear function found from experimentations (y=mx+c adjusted by desired-angle/actual-angle-rotated-during-motor-calibration-routine)
    while((mL->power < 70) || (mR->power < 70)){                   // Will be true until both motors have 70 power
        if (mL->power < 70) {mL->power += 10;}                     // Gradually increment the left motor power by 10
        if (mR->power < 70) {mR->power += 10;}                     // Gradually increment the right motor power by 10
        DCmotors_setPWM(mL);                                       // Set PWM output for left motor
        DCmotors_setPWM(mR);                                       // Set PWM output for right motor
        __delay_us(50);                                            // Add a short delay so that motors don't spin out-of-control
    }
    unsigned int i;                                                // Declare a counter for our delay for-loop (note delay function can't take variables as input)
    for (i=0; i<delay; i++) {__delay_ms(1);}                       // Delay the required time to turn the required angle
    TURNRIGHT_LED = 0;                                             // Turn off right signal
}

/********************************************************************
 * instructions
 * Function used to store DC motors instructions for all colour cards
 ********************************************************************/
void instructions(DC_motor *mL, DC_motor *mR, unsigned char num) {
    if (returnhome_flag==0) {       // If we still have not found our final card
        clearance(mL, mR);          // Reverse the car to provide some clearance for turnings
        unknowncard_flag = 0;       // Reset any possible unknown card flags
        instr[instr_counter] = num; // Store the instruction number in a global array
        instr_counter++;            // Increment the global counter in this global array
    }
    
    // Red card - Turn right 90 degrees
    if (num==1) {                                                   // Instruction 1:
        turnright(mL, mR, 90);                                      // Turn right 90 degrees
        stop(mL, mR);                                               // Stop the buggy
        
    // Green card - Turn left 90 degrees
    } else if (num==2) {                                            // Instruction 2:
        turnleft(mL, mR, 90);                                       // Turn left 90 degrees
        stop(mL, mR);                                               // Stop the buggy
        
    // Blue card - Turn 180 degrees
    } else if (num==3) {                                            // Instruction 3:
        turnright(mL, mR, 180);                                     // Turn right 180 degrees
        stop(mL, mR);                                               // Stop the buggy
        
    // Yellow card - Reverse 1 square and turn right 90 degrees
    } else if (num==4) {                                            // Instruction 4:
        reverse(mL, mR);                                            // Reverse
        __delay_ms(1400);                                           // for 1 square (approx 450 mm)
        stop(mL, mR);                                               // Stop the buggy
        __delay_ms(100);                                            // Wait a short delay
        turnright(mL, mR, 90);                                      // Turn right 90 degrees
        stop(mL, mR);                                               // Stop the buggy
        
    // Pink card - Reverse 1 square and turn left 90 degrees
    } else if (num==5) {                                            // Instruction 5:
        reverse(mL, mR);                                            // Reverse
        __delay_ms(1400);                                           // for 1 square (approx 450 mm)
        stop(mL, mR);                                               // Stop the buggy
        __delay_ms(100);                                            // Wait a short delay
        turnleft(mL, mR, 90);                                       // Turn left 90 degrees
        stop(mL, mR);                                               // Stop the buggy
        
    // Orange card - Turn right 135 degrees
    } else if (num==6) {                                            // Instruction 6:
        turnright(mL, mR, 135);                                     // Turn right 135 degrees
        stop(mL, mR);                                               // Stop the buggy
        
    // Light blue card - Turn left 135 degrees
    } else if (num==7) {                                            // Instruction 7:
        turnleft(mL, mR, 135);                                      // Turn left 135 degrees
        stop(mL, mR);                                               // Stop the buggy
    }
}

/*************************************************************************************
 * reverseinstructions
 * Function used to inverse DC motors instructions for all colour cards to return home
 *************************************************************************************/
void reverseinstructions(DC_motor *mL, DC_motor *mR) {
    if (instr[instr_counter]==1) {instructions(mL, mR, 2);}      //
    else if (instr[instr_counter]==2) {instructions(mL, mR, 1);} //
    else if (instr[instr_counter]==3) {instructions(mL, mR, 3);} //
    else if (instr[instr_counter]==4) {                          //
        __delay_ms(2500);
        stop(mL, mR);
        __delay_ms(100);
        turnleft(mL, mR, 90);
        stop(mL, mR);
    }
    else if (instr[instr_counter]==5) {
        forward(mL, mR);
        __delay_ms(2500);
        stop(mL, mR);
        __delay_ms(100);
        turnright(mL, mR, 90);
        stop(mL, mR);
    }
    else if (instr[instr_counter]==6) {instructions(mL, mR, 7);}
    else if (instr[instr_counter]==7) {instructions(mL, mR, 6);}
}

/**************************************************************
 * returnhome
 * Function used to trace all previous movements to return home
 **************************************************************/
void returnhome(DC_motor *mL, DC_motor *mR) {
    INTCONbits.GIE = 1; // Turn off interrupts
    
    unsigned char i=instr_counter;
    unsigned char j,k;
    for (j=0; j<=i; j++) {
        reverse(mL, mR);
        __delay_ms(3000);
        stop(mL, mR);
        
        reverseinstructions(mL, mR);
        
        dur_counter--;
        instr_counter--;
    }
    reverse(mL, mR);
    __delay_ms(3000);
    stop(mL, mR);
}

/*************************************************************************
 * DCmotors_calibration
 * Function used to calibrate the DC motors to adapt to different surfaces
 *************************************************************************/
void DCmotors_calibration(DC_motor *mL, DC_motor *mR) {
    unsigned char okay = 0;
    while(RF2_BUTTON && RF3_BUTTON);
    while(okay<1){
        // First turning demonstration by buggy
        MAINBEAM_LED = 1;
        __delay_ms(100);
        turnright(mL, mR, 360);
        stop(mL, mR);
        
        while(RF2_BUTTON && RF3_BUTTON);
        MAINBEAM_LED = 0;
        __delay_ms(1000);
        adjdelay(1);
        __delay_ms(1000);
        
        MAINBEAM_LED = 1;
        __delay_ms(100);
        turnleft(mL, mR, 360);
        stop(mL, mR);
        
        while(RF2_BUTTON && RF3_BUTTON);
        MAINBEAM_LED = 0;
        __delay_ms(1000);
        adjdelay(2);
        __delay_ms(1000);
        
        // Second turning demonstration by buggy after adjustments of turning duration
        MAINBEAM_LED = 1;
        __delay_ms(100);
        turnright(mL, mR, 90);
        stop(mL, mR);
        __delay_ms(1000);
        
        turnleft(mL, mR, 90);
        stop(mL, mR);
        MAINBEAM_LED = 0;
        __delay_ms(1000);
        
        // Check whether motor calibration is now satisfactory
        RH3_LED = 1;
        RD7_LED = 1;
        while(RF2_BUTTON && RF3_BUTTON);
        if(!RF2_BUTTON && RF3_BUTTON){
            RD7_LED = 0;
            okay = 1;
        } else if(!RF3_BUTTON && RF2_BUTTON){
            RH3_LED = 0;
            okay = 0;
        }
        __delay_ms(200);
        RH3_LED = 0;
        RD7_LED = 0;
        __delay_ms(1000);
    }
}

/**************************************************************************
 * adjdelay
 * Function used to adjust the turning duration and hence the turning angle
 **************************************************************************/
void adjdelay(unsigned char mode) {
    unsigned char i;
    for (i=0; i<10; i++) {
        if (mode==1){ // Turn right
            if (!RF2_BUTTON && RF3_BUTTON) {
                RD7_LED = 1;
                turnright_calangle -= 5;
                __delay_ms(800);
                RD7_LED = 0;               
            } else if (!RF3_BUTTON && RF2_BUTTON) {
                RH3_LED = 1;
                turnright_calangle += 5;
                __delay_ms(800);
                RH3_LED = 0;
            }
            __delay_ms(200);
            
        } else if (mode==2) { // Turn left
            if (!RF3_BUTTON && RF2_BUTTON) {
                RH3_LED = 1;
                turnleft_calangle -= 5;
                __delay_ms(800);
                RH3_LED = 0;               
            } else if (!RF2_BUTTON && RF3_BUTTON) {
                RD7_LED = 1;
                turnleft_calangle += 5;
                __delay_ms(800);
                RD7_LED = 0;               
            }
            __delay_ms(200);
        }        
    }  
}

/**************************************************
 * DCmotors_testing
 * Function used to test all the DC motor movements
 **************************************************/
void DCmotors_testing(DC_motor *mL, DC_motor *mR) {
    while (RF2_BUTTON && RF3_BUTTON); // Wait for button press
    __delay_ms(500);
    forward(mL, mR);
    __delay_ms(1000);
    stop(mL, mR);
    
    while (RF2_BUTTON && RF3_BUTTON); // Wait for button press
    __delay_ms(500);
    clearance(mL, mR);
    
    while (RF2_BUTTON && RF3_BUTTON);
    __delay_ms(500);
    reverse(mL, mR);
    __delay_ms(1400);
    stop(mL, mR);
    
    while (RF2_BUTTON && RF3_BUTTON);
    __delay_ms(500);
    turnright(mL, mR, 90);
    stop(mL, mR);
    
    while (RF2_BUTTON && RF3_BUTTON);
    __delay_ms(500);
    turnleft(mL, mR, 90);
    stop(mL, mR);
    
    while (RF2_BUTTON && RF3_BUTTON);
    __delay_ms(500);
    turnright(mL, mR, 135);
    stop(mL, mR);
    
    while (RF2_BUTTON && RF3_BUTTON);
    __delay_ms(500);
    turnleft(mL, mR, 135);
    stop(mL, mR);
    
    while (RF2_BUTTON && RF3_BUTTON);
    __delay_ms(500);
    turnright(mL, mR, 180);
    stop(mL, mR);
    
    while (RF2_BUTTON && RF3_BUTTON);
    turnleft(mL, mR, 180);
    stop(mL, mR);
    
    while (RF2_BUTTON && RF3_BUTTON);
    turnright(mL, mR, 360);
    stop(mL, mR);
    
    while (RF2_BUTTON && RF3_BUTTON);
    turnleft(mL, mR, 360);
    stop(mL, mR);
}
