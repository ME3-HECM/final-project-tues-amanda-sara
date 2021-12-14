#include <xc.h>  // Include processor file
#include "I2C.h" // Include corresponding header file

/***********************************************
 * I2C_2_Master_Init
 * Function used to inialise I2C module and pins
 ***********************************************/
void I2C_2_Master_Init(void) {
  //i2c config
  SSP2CON1bits.SSPM= 0b1000;    // i2c master mode
  SSP2CON1bits.SSPEN = 1;       //enable i2c
  SSP2ADD = (_XTAL_FREQ/(4*_I2C_CLOCK))-1; //Baud rate divider bits (in master mode)
  
  //pin configuration for i2c  
  TRISDbits.TRISD5 = 1;                   //Disable output driver
  TRISDbits.TRISD6 = 1;                   //Disable output driver
  ANSELDbits.ANSELD5=0;
  ANSELDbits.ANSELD6=0;
  SSP2DATPPS=0x1D;      //pin RD5
  SSP2CLKPPS=0x1E;      //pin RD6
  RD5PPS=0x1C;      // data output
  RD6PPS=0x1B;      //clock output
}

/*****************************************
 * I2C_2_Master_Idle
 * Function used to wait until I2C is idle
 *****************************************/
void I2C_2_Master_Idle(void) {
  while ((SSP2STAT & 0x04) || (SSP2CON2 & 0x1F)); // wait until bus is idle
}

/*********************************
 * I2C_2_Master_Start
 * Function used to send start bit
 *********************************/
void I2C_2_Master_Start(void) {
  I2C_2_Master_Idle();    
  SSP2CON2bits.SEN = 1;             //Initiate start condition
}

/******************************************
 * I2C_2_Master_RepStart
 * Function used to send repeated start bit
 ******************************************/
void I2C_2_Master_RepStart(void) {
  I2C_2_Master_Idle();
  SSP2CON2bits.RSEN = 1;           //Initiate repeated start condition
}

/********************************
 * I2C_2_Master_Stop
 * Function used to send stop bit
 ********************************/
void I2C_2_Master_Stop() {
  I2C_2_Master_Idle();
  SSP2CON2bits.PEN = 1;           //Initiate stop condition
}

/***************************************************
 * I2C_2_Master_Write
 * Function used to send a byte on the I2C interface
 ***************************************************/
void I2C_2_Master_Write(unsigned char data_byte) {
  I2C_2_Master_Idle();
  SSP2BUF = data_byte;         //Write data to SSPBUF
}

/***************************************************
 * I2C_2_Master_Read
 * Function used to read a byte on the I2C interface
 ***************************************************/
unsigned char I2C_2_Master_Read(unsigned char ack) {
  unsigned char tmp;
  I2C_2_Master_Idle();
  SSP2CON2bits.RCEN = 1;        // put the module into receive mode
  I2C_2_Master_Idle();
  tmp = SSP2BUF;                //Read data from SS2PBUF
  I2C_2_Master_Idle();
  SSP2CON2bits.ACKDT = !ack;     // 0 turns on acknowledge data bit
  SSP2CON2bits.ACKEN = 1;        //start acknowledge sequence
  return tmp;
}
