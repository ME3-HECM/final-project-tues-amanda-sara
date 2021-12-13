#include <xc.h>
#include "DC_motors.h"
#include "ADC.h"
#include "buttons_n_LEDs.h"

/********************************************************
 * DCmotors_init
 * function to initialise T2 and PWM for DC motor control
 ********************************************************/
void DCmotors_init(unsigned char PWMperiod)
{    
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

/*******************************************************************
 * setMotorPWM
 * function to set PWM output from the values in the motor structure
 *******************************************************************/
void DCmotors_setPWM(DC_motor *m)
{
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

/**********************
 * 
 **********************/
void checkbatterylevel(void)
{
    unsigned char batterylevel;
    batterylevel = ADC_getval();
    if (batterylevel<200) {
        RD7_LED = 1;
    } else {
        RD7_LED = 0;
    }
}

/************************************************
 * forward
 * function to make the robot go straight forward
 ************************************************/
void forward(DC_motor *mL, DC_motor *mR)
{
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

/************************************************
 * reverse
 * function to make the robot go straight reverse
 ************************************************/
void reverse(DC_motor *mL, DC_motor *mR)
{
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

/**************************************
 * stop
 * function to stop the robot gradually 
 **************************************/
void stop(DC_motor *mL, DC_motor *mR)
{
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
    
    BRAKE_LED = 0;
}

/******************
 * left
 *******************/
void left(DC_motor *mL, DC_motor *mR, unsigned int deg)
{
    // Calculations
    double delay = (deg*12.5) - 135 + turnleft_delay;
    
    // in order for it to make it turn on the spot: (Assume it was stationary before)
    mL->direction = 0; // left wheels go backward
    mR->direction = 1; // right wheels go forward

    // make both motors accelerate
    while((mL->power < LOW) || (mR->power < HIGH)){
        // flash left signal
        TURNLEFT_LED = !TURNLEFT_LED;

        // gradually turn left
        if (mL->power < LOW) {mL->power += 10;}
        if (mR->power < HIGH) {mR->power += 10;}

        // set PWM output
        DCmotors_setPWM(mL);
        DCmotors_setPWM(mR);
        __delay_us(50);
    }
    
    unsigned int i;
    for (i=0; i<delay; i++) {__delay_ms(1);}
    // switch off left signal
    TURNLEFT_LED = 0;
}

/**********
 * right
 ***********/
void right(DC_motor *mL, DC_motor *mR, unsigned int deg)
{
    // Calculations
    unsigned int delay = (8*deg) + 180 + turnright_delay;
    
    // in order for it to make it turn on the spot: (Assume it was stationary before)
    mL->direction = 1; // left wheels go forward
    mR->direction = 0; // right wheels go backward

    // make both motors accelerate
    while((mL->power < HIGH) || (mR->power < LOW)){
        // flash right signal
        TURNRIGHT_LED = !TURNRIGHT_LED;

        // gradually turn right
        if (mL->power < HIGH) {mL->power += 10;}
        if (mR->power < LOW) {mR->power += 10;}

        // set PWM output
        DCmotors_setPWM(mL);
        DCmotors_setPWM(mR);
        __delay_us(50);
    }
    
    unsigned int i;
    for (i=0; i<delay; i++) {__delay_ms(1);}
    __delay_ms(1);
    // switch off right signal
    TURNRIGHT_LED = 0;
}

/**************************************
 * turnLeft
 * function to make the robot turn left 
 **************************************/
void turnleft(DC_motor *mL, DC_motor *mR, unsigned int deg)
{
    if (returnhome_flag==0) {
        left(mL, mR, deg);
    } else {
        right(mL, mR, deg);
    }
}

/***************************************
 * turnRight
 * function to make the robot turn right 
 ***************************************/
void turnright(DC_motor *mL, DC_motor *mR, unsigned int deg)
{
    if (returnhome_flag==0) {
        right(mL, mR, deg);
    } else {
        left(mL, mR, deg);
    }
}

/*************************
 * 
 *************************/
void DCmotors_calibration(DC_motor *mL, DC_motor *mR)
{
    while(RF2_BUTTON && RF3_BUTTON);
    MAINBEAM_LED = 1;
    __delay_ms(200);
    turnleft(mL, mR, 360);
    __delay_ms(1000);
    stop(mL, mR);
    
    while(RF2_BUTTON && RF3_BUTTON);
    adjdelay(1);
    MAINBEAM_LED = 0;
    __delay_ms(1000);
    
    MAINBEAM_LED = 1;
    __delay_ms(200);
    turnright(mL, mR, 360);
    __delay_ms(1000);
    stop(mL, mR);
    
    while(RF2_BUTTON && RF3_BUTTON);
    adjdelay(2);
    MAINBEAM_LED = 0;
}

/******************
 * 
 ******************/
void adjdelay(unsigned char mode)
{
    __delay_ms(1000);
    unsigned char i;
    for (i=0; i<10; i++) {
        if(mode==1){
            if(!RF2_BUTTON && RF3_BUTTON){
                RD7_LED = 1;
                turnright_delay+=5;
                __delay_ms(800);
                RD7_LED = 0;               
            }
            if(!RF3_BUTTON && RF2_BUTTON){
                RH3_LED = 1;
                turnright_delay-=5;
                __delay_ms(800);
                RH3_LED = 0;               
            }

        } else if(mode==2){
            if(!RF2_BUTTON && RF3_BUTTON){
                RD7_LED = 1;
                turnleft_delay-=5;
                __delay_ms(800);
                RD7_LED = 0;               
            }
            if(!RF3_BUTTON && RF2_BUTTON){
                RH3_LED = 1;
                turnleft_delay+=5;
                __delay_ms(800);
                RH3_LED = 0;               
            }

        }        
        
}

/**********************
 * 
 **********************/
void DCmotors_testing(DC_motor *mL, DC_motor *mR)
{
    INTCONbits.GIE = 0;
    
    while (RF2_BUTTON && RF3_BUTTON);
    forward(mL, mR);
    __delay_ms(1000);
    stop(mL, mR);
    __delay_ms(1000);
    
    while (RF2_BUTTON && RF3_BUTTON);
    reverse(mL, mR);
    __delay_ms(2020);
    stop(mL, mR);
    __delay_ms(1000);
    
    while (RF2_BUTTON && RF3_BUTTON);
    turnright(mL, mR, 90);
//    __delay_ms(920);
    stop(mL, mR);
    __delay_ms(100);
    
    while (RF2_BUTTON && RF3_BUTTON);
    turnleft(mL, mR, 90);
//    __delay_ms(995);
    stop(mL, mR);
    __delay_ms(100);
    
    while (RF2_BUTTON && RF3_BUTTON);
    turnright(mL, mR, 180);
//    __delay_ms(1660);
    stop(mL, mR);
    __delay_ms(100);
    
    while (RF2_BUTTON && RF3_BUTTON);
    turnleft(mL, mR, 180);
//    __delay_ms(2115);
    stop(mL, mR);
    __delay_ms(100);
    
    while (RF2_BUTTON && RF3_BUTTON);
    turnright(mL, mR, 135);
//    __delay_ms(1240);
    stop(mL, mR);
    __delay_ms(100);
    
    while (RF2_BUTTON && RF3_BUTTON);
    turnleft(mL, mR, 135);
//    __delay_ms(1555);
    stop(mL, mR);
    __delay_ms(100);
    
    INTCONbits.GIE = 1;
}
