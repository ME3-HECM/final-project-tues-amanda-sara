#include <xc.h>          // Include processor file
#include "serial_comm.h" // Include corresponding header file

/***************************************************************
 * USART4_init
 * Function used to set up USART4 for reception and transmission
 ***************************************************************/
void USART4_init(void) {
    RC0PPS = 0x12;          // Map EUSART4 TX to RC0
    RX4PPS = 0x11;          // RX is RC1
    TRISCbits.TRISC1 = 1;   // The TRIS value for the RX pin must be set to input in your code (it is input by default)
    
    BAUD4CONbits.BRG16 = 0; // Set baud rate scaling
    TX4STAbits.BRGH = 0;    // High baud rate select bit
    SP4BRGL = 51;           // Set baud rate to 51 = 19200bps
    SP4BRGH = 0;            // Not used

    RC4STAbits.CREN = 1;    // Enable continuos reception
    TX4STAbits.TXEN = 1;    // Enable transmitter
    RC4STAbits.SPEN = 1;    // Enable serial port
}

/************************************************************************************
 * getCharSerial4
 * Function used to wait for a byte to arrive on serial port and read it once it does
 ************************************************************************************/
char getCharSerial4(void) {
    while (!PIR4bits.RC4IF); // Wait for the data to arrive
    return RC4REG;           // Return byte in RCREG
}

/*******************************************************************
 * sendCharSerial4
 * Function used to check whether the TX reg is free and send a byte
 *******************************************************************/
void sendCharSerial4(unsigned char charToSend) {
    while (!PIR4bits.TX4IF); // Wait for flag to be set
    TX4REG = charToSend;     // Transfer char to transmitter
}

/**********************************************************
 * sendStringSerial4
 * Function used to send a string over the serial interface
 **********************************************************/
void sendStringSerial4(char *string){
    while (*string != 0) {          // While the data pointed to isn't a 0x00 do below (strings in C must end with a NULL byte)
        sendCharSerial4(*string++); // Send out the current byte pointed to and increment the pointer
    } 
}
