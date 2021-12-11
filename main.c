#include <xc.h>
#include "main.h"

/***************
 * Main function
 ***************/
void main(void) {
    /**************************
     * Initialisation functions
     **************************/
    unsigned char PWMperiod = 99; // 0.0001s*(64MHz/4)/16 -1 = 99
    card_flag = 0;
    battery_flag = 0;
    returnhome_flag = 0;
    ADC_init();
    colorclick_init();
    interrupts_init();
    DCmotors_init(PWMperiod);
    USART4_init();

    /**************************
     * Motor structures
     **************************/
    DC_motor motorL;                                    //declare DC_motor structure
    motorL.power=0;                                     //set motor power to 0 at start
    motorL.direction=1;                                 //set default motor direction forward
    motorL.dutyHighByte=(unsigned char *)(&PWM6DCH);	//PWM duty high byte address
    motorL.dir_LAT=(unsigned char *)(&LATE);            //LAT for dir pin address
    motorL.dir_pin=4;                                   //pin RE4 controls direction on LAT
    motorL.PWMperiod=PWMperiod;                          //base period of PWM cycle

    // same for motorR but different PWM register, LAT and direction pin
    DC_motor motorR;                                    //declare DC_motor structure    
    motorR.power=0;                                     //set motor power to 0 at start
    motorR.direction=1;                                 //set default motor direction forward
    motorR.dutyHighByte=(unsigned char *)(&PWM7DCH);	//PWM duty high byte address
    motorR.dir_LAT=(unsigned char *)(&LATG);            //LAT for dir pin address
    motorR.dir_pin=6;                                   //pin RG6 controls direction on LAT
    motorR.PWMperiod=PWMperiod;                          //base period of PWM cycle
    
    /***************************
     * Colour calibration routine
     ***************************/
//    while(RF2_BUTTON && RF3_BUTTON);
//    RD7_LED = 1;
//    RH3_LED = 1;
//    
    RGB_val initial;
    initial = colorclick_readColour(initial); //read initial light value
    __delay_ms(100);
//    
//    RD7_LED = 0;
//    RH3_LED = 0;
    
    /***************************
     * Motor calibration routine
     ***************************/
//    while(RF2_BUTTON && RF3_BUTTON);
//    turnRight(&motorL, &motorR, 360);
//    motorL.power=0;
//    motorR.power=0;
//    
//    while(RF2_BUTTON && RF3_BUTTON);
//    if (RF2_BUTTON) {
//        RD7_LED = 1;
//        __delay_ms(100);
//        RD7_LED = 0;
//    } else if (RF3_BUTTON) {
//        RH3_LED = 1;
//        __delay_ms(100);
//        RH3_LED = 0;
//    }
//
//    while(RF2_BUTTON && RF3_BUTTON);
//    turnLeft(&motorL, &motorR, 360);
//    motorL.power=0;
//    motorR.power=0;
//    
//    while(RF2_BUTTON && RF3_BUTTON);
//    if (RF2_BUTTON) {
//        RD7_LED = 1;
//        __delay_ms(100);
//        RD7_LED = 0;
//    } else if (RF3_BUTTON) {
//        RH3_LED = 1;
//        __delay_ms(100);
//        RH3_LED = 0;
//    }
//    
//    while(RF2_BUTTON && RF3_BUTTON);
//    MAINBEAM_LED = 1;
//    colorclick_toggleClearLED(1);
//    __delay_ms(1000);
//    forward(&motorL, &motorR);
    
    /*********************
     * Check battery level
     *********************/
    unsigned char battery = ADC_getval();
    if (battery<100) {
        RD7_LED = 1;
        RH3_LED = 1;
    } else if (battery<200) {
        RD7_LED = 1;
        RH3_LED = 0;
    } else {
        RD7_LED = 0;
        RH3_LED = 0;
    }
    __delay_ms(1000);
    forward(&motorL, &motorR);
    
    /**************************
     * Infinite while loop
     **************************/
    RGB_val current;
    while(1) {
        /************************************
         * Testing using serial communication
         ************************************/
//        current = colorclick_readColour(current); //read current light value
//        char buf[10];
//        unsigned int tmpR = current.R;
//        unsigned int tmpG = current.G;
//        unsigned int tmpB = current.B;
//        unsigned int tmpC = current.C;
//        sprintf(buf,"%i %i %i %i\n\r",tmpR,tmpG,tmpB,tmpC);
//        sendStringSerial4(buf);
//        __delay_ms(500);
        
        /*****************
         * Maze navigation
         *****************/
        current = colorclick_readColour(current); //read current light value
        read_card(initial, current, &motorL, &motorR);
        
        if (card_flag==1) {
            current = colorclick_readColour(current); //read current light value
            read_card(initial, current, &motorL, &motorR);
            card_flag = 0;
        }
        
        if (battery_flag==1) {
            RD7_LED = 1;
            RH3_LED = 1;
            battery_flag = 0;
        }
    }
}
