#ifndef _I2C_H // This is a guard condition so that contents of this file are not included more than once
#define _I2C_H // Beginning of _I2C_H

#include <xc.h> // Include processor file

#define _XTAL_FREQ 64000000 // Note intrinsic _delay function is 62.5ns at 64,000,000Hz
#define _I2C_CLOCK 100000   // I2C clock is set at 100kHz

/*********************
 * Function prototypes
 *********************/
void I2C_2_Master_Init(void);
void I2C_2_Master_Idle(void);
void I2C_2_Master_Start(void);
void I2C_2_Master_RepStart(void);
void I2C_2_Master_Stop(void);
void I2C_2_Master_Write(unsigned char data_byte);
unsigned char I2C_2_Master_Read(unsigned char ack);

#endif // End of _I2C_H
