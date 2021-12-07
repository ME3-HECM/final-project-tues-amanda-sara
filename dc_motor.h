#ifndef _DC_MOTOR_H
#define _DC_MOTOR_H

#include <xc.h>

#define _XTAL_FREQ 64000000

// Clicker2 buttons
#define RF2_BUTTON LATFbits.LATF2
#define RF3_BUTTON LATFbits.LATF3

// Clicker2 LEDs
#define RD7_LED LATDbits.LATD7
#define RH3_LED LATHbits.LATH3

// Buggy LEDs
#define HEADLAMPS_LED LATHbits.LATH1 //H.LAMPS turns on the front white LEDs and rear red LEDs, at a reduced brightness.
#define MAINBEAM_LED LATDbits.LATD3 //M.BEAM and BRAKE enable you to turn these LEDs on at full brightness.
#define BRAKE_LED LATDbits.LATD4
#define TURNLEFT_LED LATFbits.LATF0 //The turn signals have not hardware based brightness control.
#define TURNRIGHT_LED LATHbits.LATH0

typedef struct { //definition of DC_motor structure
    char power;         //motor power, out of 100
    char direction;     //motor direction, forward(1), reverse(0)
    unsigned char *dutyHighByte; //PWM duty high byte address
    unsigned char *dir_LAT; //LAT for dir pin
    char dir_pin; // pin number that controls direction on LAT
    int PWMperiod; //base period of PWM cycle
} DC_motor;

//function prototypes
void DCmotors_init(int PWMperiod); // function to setup PWM
void clicker2buttons_init(void);
void clicker2LEDs_init(void);
void buggyLEDs_init(void);
unsigned char check_battery_level(void);
void setMotorPWM(DC_motor *m);
void forward(DC_motor *mL, DC_motor *mR);
void reverse(DC_motor *mL, DC_motor *mR);
void stop(DC_motor *mL, DC_motor *mR);
void turnLeft(DC_motor *mL, DC_motor *mR);
void turnRight(DC_motor *mL, DC_motor *mR);

#endif
