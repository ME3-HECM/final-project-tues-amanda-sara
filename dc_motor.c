#include <xc.h>
#include "dc_motor.h"

#define LOW 50
#define HIGH 50

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
void setMotorPWM(DC_motor *m)
{
	int PWMduty; //tmp variable to store PWM duty cycle

	if (m->direction){ //if forward
		PWMduty=m->PWMperiod - ((int)(m->power)*(m->PWMperiod))/100; // low time increases with power
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
 * clicker2buttons_init
 **********************/
void clicker2buttons_init(void)
{
    // setup pin for input (connected to button)
    TRISFbits.TRISF2=1; //set TRIS value for RF2 pin (input)
    TRISFbits.TRISF3=1; //set TRIS value for RF3 pin (input)
    ANSELFbits.ANSELF2=0; //turn off analogue input on RF2 pin
    ANSELFbits.ANSELF3=0; //turn off analogue RF3 input on pin
}

/*******************
 * clicker2LEDs_init
 *******************/
void clicker2LEDs_init(void)
{
    TRISDbits.TRISD7 = 0;
    TRISHbits.TRISH3 = 0;
    
    RD7_LED = 0;
    RH3_LED = 0;
}

/****************
 * buggyLEDs_init
 ****************/
void buggyLEDs_init(void)
{
    TRISHbits.TRISH1 = 0; // H.LAMPS
    TRISDbits.TRISD3 = 0; // M.BEAM
    TRISDbits.TRISD4 = 0; // BRAKE
    TRISFbits.TRISF0 = 0; // TURN-L
    TRISHbits.TRISH0 = 0; // TURN-R
    
    HEADLAMPS_LED = 0;
    MAINBEAM_LED = 0;
    BRAKE_LED = 0;
    TURNLEFT_LED = 0;
    TURNRIGHT_LED = 0;
}

/***************************************************************************
 * check_battery_level
 * function to check battery level
 * Monitor the battery voltage via an analogue input pin
 * The voltage at BAT-VSENSE will always be one third of that at the battery
 ***************************************************************************/
unsigned char check_battery_level(void)
{
    unsigned char tmp;
    //tmp = ADC_getval();
    return tmp;
}

/************************************************
 * forward
 * function to make the robot go straight forward
 ************************************************/
void forward(DC_motor *mL, DC_motor *mR)
{
    // Assume it was stationary before
    mL->direction = 1; // left wheels go forward
    mR->direction = 1; // right wheels go forward
    
    // make both motors accelerate to 100
    while(((mL->power)<20) && ((mR->power)<20)){    // will be True until both motors have 100 power
        mL->power+=5;
        mR->power+=5;
        // set PWM output
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(50);
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
    
    // make both motors accelerate to 100
    while(((mL->power)<100) && ((mR->power)<100)){    // will be True until both motors have 100 power
        mL->power+=1;
        mR->power+=1;
        // set PWM output
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(100);
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
    while(((mL->power)>0) && ((mR->power)>0)){    // will be True until both motors have 0 power
        mL->power = mL->power - 5;
        mR->power = mR->power - 5;
        
        // set PWM output
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(100);    // set a delay so that motor decelerates non-instantaneously
    }
    
    BRAKE_LED = 0;
}

/**************************************
 * turnLeft
 * function to make the robot turn left 
 **************************************/
void turnLeft(DC_motor *mL, DC_motor *mR, unsigned char deg)
{
    if (returnhome_flag==0) {
        // in order for it to make it turn on the spot: (Assume it was stationary before)
        mL->direction = 0; // left wheels go backward
        mR->direction = 1; // right wheels go forward

        // make both motors accelerate
        while(((mL->power)<LOW) || ((mR->power)<HIGH)){
            // flash left signal
            TURNLEFT_LED = !TURNLEFT_LED;
            
            // gradually turn left
            if (mL->power<LOW) {mL->power+=1;}
            if (mR->power<HIGH) {mR->power+=1;}
            
            // set PWM output
            setMotorPWM(mL);
            setMotorPWM(mR);
            __delay_ms(100);
        }
        // switch off left signal
        TURNLEFT_LED = 0;
        
    } else {
        // in order for it to make it turn on the spot: (Assume it was stationary before)
        mL->direction = 1; // left wheels go forward
        mR->direction = 0; // right wheels go backward

        // make both motors accelerate
        while(((mL->power)<HIGH) || ((mR->power)<LOW)){
            // flash right signal
            TURNRIGHT_LED = !TURNRIGHT_LED;
            
            // gradually turn right
            if (mL->power<HIGH) {mL->power+=1;}
            if (mR->power<LOW) {mR->power+=1;}
            
            // set PWM output
            setMotorPWM(mL);
            setMotorPWM(mR);
            __delay_ms(100);
        }
        // switch off right signal
        TURNRIGHT_LED = 0;
    }
}

/***************************************
 * turnRight
 * function to make the robot turn right 
 ***************************************/
void turnRight(DC_motor *mL, DC_motor *mR, unsigned char deg)
{
    if (returnhome_flag==0) {
        // in order for it to make it turn on the spot: (Assume it was stationary before)
        mL->direction = 1; // left wheels go forward
        mR->direction = 0; // right wheels go backward

        // make both motors accelerate
        while(((mL->power)<HIGH) || ((mR->power)<LOW)){
            // flash right signal
            TURNRIGHT_LED = !TURNRIGHT_LED;
            
            // gradually turn right
            if (mL->power<HIGH) {mL->power+=1;}
            if (mR->power<LOW) {mR->power+=1;}
            
            // set PWM output
            setMotorPWM(mL);
            setMotorPWM(mR);
            __delay_ms(100);
        }
        // switch off right signal
        TURNRIGHT_LED = 0;
        
    } else {
        // in order for it to make it turn on the spot: (Assume it was stationary before)
        mL->direction = 0; // left wheels go backward
        mR->direction = 1; // right wheels go forward

        // make both motors accelerate
        while(((mL->power)<LOW) || ((mR->power)<HIGH)){
            // flash left signal
            TURNLEFT_LED = !TURNLEFT_LED;
            
            // gradually turn left
            if (mL->power<LOW) {mL->power+=1;}
            if (mR->power<HIGH) {mR->power+=1;}
            
            // set PWM output
            setMotorPWM(mL);
            setMotorPWM(mR);
            __delay_ms(100);
        }
        // switch off left signal
        TURNLEFT_LED = 0;
    }
}
