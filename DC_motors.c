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
    T2CONbits.CKPS=0b100; // 1:16 prescaler
    T2HLTbits.MODE=0b00000; // Free Running Mode, software gate only
    T2CLKCONbits.CS=0b0001; // Fosc/4

    // Tpwm*(Fosc/4)/prescaler - 1 = PTPER
    T2PR=PWMperiod; // Period reg, calculate value for 10kHz base period
    T2CONbits.ON=1; // Turn the timer on
    
    //initialise your TRIS and LAT registers for PWM
    TRISEbits.TRISE2=0; //set RE2 TRIS for Output
    TRISEbits.TRISE4=0; //set RE3 TRIS for Output
    TRISCbits.TRISC7=0; //set RC7 TRIS for Output
    TRISGbits.TRISG6=0; //set RG6 TRIS for Output
    
    LATEbits.LATE2=0; // set RE2 output to 0
    LATEbits.LATE4=0; // set RE4 output to 0
    LATCbits.LATC7=0; // set RC7 output to 0
    LATGbits.LATG6=0; // set RG6 output to 0

    PWM6DCH=0; // 0% power
    PWM7DCH=0; // 0% power
    
    PWM6CONbits.EN = 1; // enable PWM generation
    PWM7CONbits.EN = 1; // enable PWM generation
    
    RE2PPS=0x0A; //PWM6 on RE2
    RC7PPS=0x0B; //PMW7 on RC7
    
    //
    clicker2buttons_init();
    clicker2LEDs_init();
    buggyLEDs_init();
}

/************************************************************************
 * DCmotors_setPWM
 * Function used to set PWM output from the values in the motor structure
 ************************************************************************/
void DCmotors_setPWM(DC_motor *m) {
	int PWMduty; //tmp variable to store PWM duty cycle

	if (m->direction){ //if forward
		PWMduty = (m->PWMperiod) - ((int)(m->power)*(m->PWMperiod))/100; // low time increases with power
	} else { //if reverse
		PWMduty=((int)(m->power)*(m->PWMperiod))/100; // high time increases with power
	}

	*(m->dutyHighByte) = (unsigned char)(PWMduty); //set high duty cycle byte
        
	if (m->direction){ // if direction is high
		*(m->dir_LAT) = (unsigned char)(*(m->dir_LAT) | (1<<(m->dir_pin))); // set dir_pin bit in LAT to high without changing other bits
	} else {
		*(m->dir_LAT) = (unsigned char)(*(m->dir_LAT) & (~(1<<(m->dir_pin)))); // set dir_pin bit in LAT to low without changing other bits
	}
}

/***************************************************
 * checkbatterylevel
 * Function used to check battery level on the buggy
 ***************************************************/
void checkbatterylevel(void) {
    unsigned char batterylevel;
    batterylevel = ADC_getval();
    if (batterylevel<100) {RD7_LED = 1;}
    else {RD7_LED = 0;}
}

/*****************************************
 * forward
 * Function used to move the buggy forward
 *****************************************/
void forward(DC_motor *mL, DC_motor *mR) {
    mL->direction = 1; // left wheels go forward
    mR->direction = 1; // right wheels go forward
    
    // make both motors accelerate to 40
    while((mL->power < 20) && (mR->power < 20)){    // will be True until both motors have 100 power
        mL->power += 10;
        mR->power += 10;
        // set PWM output
        DCmotors_setPWM(mL);
        DCmotors_setPWM(mR);
        __delay_us(50);
    }
}

/*******************************************
 * reverse
 * Function used to move the buggy backwards
 *******************************************/
void reverse(DC_motor *mL, DC_motor *mR) {
    // Assume it was stationary before
    mL->direction = 0; // left wheels go forward
    mR->direction = 0; // right wheels go forward
    
    // make both motors accelerate to 50
    while((mL->power < 50) && (mR->power < 50)){    // will be True until both motors have 100 power
        mL->power += 10;
        mR->power += 10;
        // set PWM output
        DCmotors_setPWM(mL);
        DCmotors_setPWM(mR);
        __delay_us(50);
    }
}

/***************************************************************
 * clearance
 * Function used to create space clearance for the buggy to turn
 ***************************************************************/
void clearance(DC_motor *mL, DC_motor *mR) {
    MAINBEAM_LED = 0;
    reverse(mL, mR);
    __delay_ms(350);
    stop(mL, mR);
    __delay_ms(1000);
    MAINBEAM_LED = 0;
}

/*******************************************
 * stop
 * Function used to gradually stop the buggy
 *******************************************/
void stop(DC_motor *mL, DC_motor *mR) {
    BRAKE_LED = 1;
    // need to slowly bring both motors to a stop
    while((mL->power > 0) && (mR->power > 0)){    // will be True until both motors have 0 power
        mL->power -= 10;
        mR->power -= 10;
        
        // set PWM output
        DCmotors_setPWM(mL);
        DCmotors_setPWM(mR);
        __delay_us(50);    // set a delay so that motor decelerates non-instantaneously
    }
    __delay_ms(500);
    BRAKE_LED = 0;
}

/****************************************************
 * left
 * Function used to rotate the buggy wheels leftwards
 ****************************************************/
void left(DC_motor *mL, DC_motor *mR, unsigned int deg) {
    // Calculations
    double delay = (deg*2.332) + 31.506 + ((turnleft_delay*deg)/90);
    
    // in order for it to make it turn on the spot: (Assume it was stationary before)
    mL->direction = 0; // left wheels go backward
    mR->direction = 1; // right wheels go forward

    // make both motors accelerate
    TURNLEFT_LED = 1;
    while((mL->power < 70) || (mR->power < 70)){
        // gradually turn left
        if (mL->power < 70) {mL->power += 10;}
        if (mR->power < 70) {mR->power += 10;}

        // set PWM output
        DCmotors_setPWM(mL);
        DCmotors_setPWM(mR);
        __delay_us(50);
    }
    
    unsigned int i;
    for (i=0; i<delay; i++) {__delay_ms(1);}
    TURNLEFT_LED = 0;
}

/*****************************************************
 * right
 * Function used to rotate the buggy wheels rightwards
 *****************************************************/
void right(DC_motor *mL, DC_motor *mR, unsigned int deg) {
    // Calculations
    double delay = (2.0303*deg) + 62.964 + ((turnright_delay*deg)/90);
    
    // in order for it to make it turn on the spot: (Assume it was stationary before)
    mL->direction = 1; // left wheels go forward
    mR->direction = 0; // right wheels go backward

    // make both motors accelerate
    TURNRIGHT_LED = 1;
    while((mL->power < 70) || (mR->power < 70)){
        // gradually turn right
        if (mL->power < 70) {mL->power += 10;}
        if (mR->power < 70) {mR->power += 10;}

        // set PWM output
        DCmotors_setPWM(mL);
        DCmotors_setPWM(mR);
        __delay_us(50);
    }
    
    unsigned int i;
    for (i=0; i<delay; i++) {__delay_ms(1);}
    // switch off right signal
    TURNRIGHT_LED = 0;
}

/*********************************************
 * turnleft
 * Function used to turn the buggy to the left
 *********************************************/
void turnleft(DC_motor *mL, DC_motor *mR, unsigned int deg) {
    if (returnhome_flag==0) {left(mL, mR, deg);}
    else {right(mL, mR, deg);}
}

/**********************************************
 * turnright
 * Function used to turn the buggy to the right
 **********************************************/
void turnright(DC_motor *mL, DC_motor *mR, unsigned int deg) {
    if (returnhome_flag==0) {right(mL, mR, deg);}
    else {left(mL, mR, deg);}
}

/*************************************************************************
 * DCmotors_calibration
 * Function used to calibrate the DC motors to adapt to different surfaces
 *************************************************************************/
void DCmotors_calibration(DC_motor *mL, DC_motor *mR) {
    unsigned char okay = 0;
    while(okay<1){
        while(RF2_BUTTON && RF3_BUTTON);
        MAINBEAM_LED = 1;
        __delay_ms(100);
        turnleft(mL, mR, 360);
        stop(mL, mR);

        while(RF2_BUTTON && RF3_BUTTON);
        adjdelay(1);
        MAINBEAM_LED = 0;
        __delay_ms(1000);

        MAINBEAM_LED = 1;
        __delay_ms(100);
        turnright(mL, mR, 360);
        stop(mL, mR);

        while(RF2_BUTTON && RF3_BUTTON);
        adjdelay(2);
        MAINBEAM_LED = 0;
        __delay_ms(1000);
        
        MAINBEAM_LED = 1;
        __delay_ms(100);
        turnleft(mL, mR, 90);
        stop(mL, mR);
        __delay_ms(1000);
        
        turnright(mL, mR, 90);
        stop(mL, mR);
        __delay_ms(1000);
        MAINBEAM_LED = 0;
        
        RH3_LED = 1;
        RD7_LED = 1;
        while(RF3_BUTTON && RF2_BUTTON);
        if(!RF2_BUTTON && RF3_BUTTON){
            RH3_LED = 0;
            okay = 1;
        } else if(!RF3_BUTTON && RF2_BUTTON){
            RD7_LED = 0;
            okay = 0;
        }
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
    __delay_ms(1000);
    unsigned char i;
    for (i=0; i<10; i++) {
        if(mode==2){
            BRAKE_LED = 1;
            if(!RF2_BUTTON && RF3_BUTTON){
                RD7_LED = 1;
                turnright_delay+=1;
                __delay_ms(800);
                RD7_LED = 0;               
            }
            else if(!RF3_BUTTON && RF2_BUTTON){
                RH3_LED = 1;
                turnright_delay-=1;
                __delay_ms(800);
                RH3_LED = 0;     
            }
            
            __delay_ms(500);
            BRAKE_LED = 0;

        } else if(mode==1){
            if(!RF2_BUTTON && RF3_BUTTON){
                RD7_LED = 1;
                turnleft_delay-=1;
                __delay_ms(800);
                RD7_LED = 0;               
            }
            else if(!RF3_BUTTON && RF2_BUTTON){
                RH3_LED = 1;
                turnleft_delay+=1;
                __delay_ms(800);
                RH3_LED = 0;               
            }
            
            __delay_ms(500);

        }        
    }  
}

/**************************************************
 * DCmotors_testing
 * Function used to test all the DC motor movements
 **************************************************/
void DCmotors_testing(DC_motor *mL, DC_motor *mR) {
    while (RF2_BUTTON && RF3_BUTTON);
    __delay_ms(500);
    forward(mL, mR);
    __delay_ms(1000);
    stop(mL, mR);
    
    while (RF2_BUTTON && RF3_BUTTON);
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
