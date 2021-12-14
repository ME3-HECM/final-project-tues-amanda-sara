#ifndef _I2C_H // This is a guard condition so that contents of this file are not included more than once
#define _I2C_H // Beginning of _I2C_H

#include <xc.h> // Include processor file

#define _XTAL_FREQ 64000000 // Note intrinsic delay function is 62.5ns at 64,000,000Hz
#define _I2C_CLOCK 100000   // I2C clock is set at 100kHz

/*********************
 * Function prototypes
 *********************/
void I2C_2_Master_Init(void);                       // Function used to inialise I2C module and pins
void I2C_2_Master_Idle(void);                       // Function used to wait until I2C is idle
void I2C_2_Master_Start(void);                      // Function used to send start bit
void I2C_2_Master_RepStart(void);                   // Function used to send repeated start bit
void I2C_2_Master_Stop(void);                       // Function used to send stop bit
void I2C_2_Master_Write(unsigned char data_byte);   // Function used to send a byte on the I2C interface
unsigned char I2C_2_Master_Read(unsigned char ack); // Function used to read a byte on the I2C interface

#endif // End of _I2C_H
