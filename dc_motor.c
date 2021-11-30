#include <xc.h>
#include "dc_motor.h"

#define LOW 50
#define HIGH 50

// function initialise T2 and PWM for DC motor control
void initDCmotorsPWM(int PWMperiod){
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
}



// function to set PWM output from the values in the motor structure
void setMotorPWM(struct DC_motor *m)
{
	int PWMduty; //tmp variable to store PWM duty cycle

	if (m->direction){ //if forward
		// low time increases with power
		PWMduty=m->PWMperiod - ((int)(m->power)*(m->PWMperiod))/100;
	}
	else { //if reverse
		// high time increases with power
		PWMduty=((int)(m->power)*(m->PWMperiod))/100;
	}

	*(m->dutyHighByte) = (unsigned char)(PWMduty); //set high duty cycle byte
        
	if (m->direction){ // if direction is high
		*(m->dir_LAT) = (unsigned char)(*(m->dir_LAT) | (1<<(m->dir_pin))); // set dir_pin bit in LAT to high without changing other bits
	} else {
		*(m->dir_LAT) = (unsigned char)(*(m->dir_LAT) & (~(1<<(m->dir_pin)))); // set dir_pin bit in LAT to low without changing other bits
	}
}



//function to stop the robot gradually 
void stop(struct DC_motor *mL, struct DC_motor *mR)
{
    // need to slowly bring both motors to a stop
    while(((mL->power)!=0) && ((mR->power)!=0)){    // will be True until both motors have 0 power
        mL->power = mL->power - 10;
        mR->power = mR->power - 10;
        
        // set PWM output
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(100);    // set a delay so that motor decelerates non-instantaneously
    }
}



//function to make the robot turn left 
void turnLeft(struct DC_motor *mL, struct DC_motor *mR)
{
    // in order for it to make it turn on the spot: (Assume it was stationary before)
    // left wheels go backward
    mL->direction = 0;
    // right wheels go forward
    mR->direction = 1;
    
    // make both motors accelerate to 50
    while(((mL->power)!=LOW) || ((mR->power)!=HIGH)){
        if (mL->power<LOW) {mL->power+=5;}
        if (mR->power<HIGH) {mR->power+=5;}
        
        // set PWM output
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(100);  
    }
}



//function to make the robot turn right 
void turnRight(struct DC_motor *mL, struct DC_motor *mR)
{
    // in order for it to make it turn on the spot: (Assume it was stationary before)
    // left wheels go forward
    mL->direction = 1;
    // right wheels go backward
    mR->direction = 0;
    
    // make both motors accelerate to 70
    while(((mL->power)!=HIGH) || ((mR->power)!=LOW)){
        if (mL->power<HIGH) {mL->power+=5;}
        if (mR->power<LOW) {mR->power+=5;}
        
        // set PWM output
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(100);
    }
}



//function to make the robot go straight
void fullSpeedAhead(struct DC_motor *mL, struct DC_motor *mR)
{
    // Assume it was stationary before
    // left wheels go forward
    mL->direction = 1;
    // right wheels go forward
    mR->direction = 1;
    
    // make both motors accelerate to 100
    while(((mL->power)!=100) && ((mR->power)!=100)){    // will be True until both motors have 100 power
        mL->power+=10;
        mR->power+=10;
        // set PWM output
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(100);
    }
}