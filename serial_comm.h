#ifndef _SERIAL_COMM_H // This is a guard condition so that contents of this file are not included more than once
#define _SERIAL_COMM_H // Beginning of _SERIAL_COMM_H

#include <xc.h> // Include processor file

#define _XTAL_FREQ 64000000 // Note intrinsic delay function is 62.5ns at 64,000,000Hz

/*********************
 * Function prototypes
 *********************/
void USART4_init(void);                         // Function used to set up USART4 for reception and transmission
char getCharSerial4(void);                      // Function used to wait for a byte to arrive on serial port and read it once it does
void sendCharSerial4(unsigned char charToSend); // Function used to check whether the TX reg is free and send a byte
void sendStringSerial4(char *string);           // Function used to send a string over the serial interface

#endif // End of _SERIAL_COMM_H
