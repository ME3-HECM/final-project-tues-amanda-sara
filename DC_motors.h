#ifndef _DC_MOTORS_H // This is a guard condition so that contents of this file are not included more than once
#define _DC_MOTORS_H // Beginning of _DC_MOTORS_H

#include <xc.h> // Include processor file

#define _XTAL_FREQ 64000000 // Note intrinsic _delay function is 62.5ns at 64,000,000Hz

/*************
 * RGBC values
 *************/
typedef struct {                 // Define a structure
    char power;                  // Motor power, out of 100
    char direction;              // Motor direction, forward(1), reverse(0)
    unsigned char *dutyHighByte; // PWM duty high byte address
    unsigned char *dir_LAT;      // LAT for direction pin
    char dir_pin;                // Pin number that controls direction on LAT
    int PWMperiod;               // Base period of PWM cycle
} DC_motor;                      // This structure is named DC_motor

/******************
 * Global variables
 ******************/
extern volatile unsigned char DCmotors_lower;
extern volatile unsigned char DCmotors_upper;
extern volatile unsigned char returnhome_flag; // Toggled when the final card has been found (i.e. encountered the white card)
                                               // or in exceptional scenarios (i.e. cannot identify a card or stuck in the maze for too long)

/*********************
 * Function prototypes
 *********************/
void DCmotors_init(unsigned char PWMperiod);
void DCmotors_setPWM(DC_motor *m);
void checkbatterylevel(void);
void forward(DC_motor *mL, DC_motor *mR);
void reverse(DC_motor *mL, DC_motor *mR);
void stop(DC_motor *mL, DC_motor *mR);
void left(DC_motor *mL, DC_motor *mR, unsigned int deg);
void right(DC_motor *mL, DC_motor *mR, unsigned int deg);
void turnleft(DC_motor *mL, DC_motor *mR, unsigned int deg);
void turnright(DC_motor *mL, DC_motor *mR, unsigned int deg);
void DCmotors_calibration(DC_motor *mL, DC_motor *mR);
void DCmotors_adjustval(void);
void DCmotors_testing(DC_motor *mL, DC_motor *mR);

#endif // End of _DC_MOTOR_H
