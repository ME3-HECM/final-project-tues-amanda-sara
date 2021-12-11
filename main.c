#include <xc.h>
#include "main.h"

volatile unsigned int clear_lower = 0;
volatile unsigned int clear_upper = 0;
volatile unsigned char card_flag = 0;

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
    
    RD7_LED = 0;
    RH3_LED = 0;
    
    MAINBEAM_LED = 1;               //turn on lights for initial reading
    colorclick_toggleClearLED(1);
    
    __delay_ms(1000);               //delay so lights are on before reading is taken 
    
    RGB_val initial;
    initial = colorclick_readColour(initial); //read initial light value
//    __delay_ms(100);
    
    clear_lower = initial.C - 10;
    clear_upper = initial.C + 10;
    
    if(clear_lower<0) clear_lower = 0;
    
    __delay_ms(1000);
    
    interrupts_init();
    
    
//    
//    RD7_LED = 0;
//    RH3_LED = 0;
//    
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
    
    /**************************
     * Infinite while loop
     **************************/
    RGB_val current;
    
    unsigned char start = 0;
    unsigned char move = 0;
    
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
//        sprintf(buf,"%i %i %i %i\n",clear_lower,clear_upper,initial.C,tmpC);
//        sendStringSerial4(buf);
//        __delay_ms(200);
//        
        /*****************
         * Maze navigation
         *****************/
//        current = colorclick_readColour(current); //read current light value
//        read_card(initial, current, &motorL, &motorR);
        
//        }
//        
//        if (battery_flag==1) {
//            RD7_LED = 1;
//            RH3_LED = 1;
//            battery_flag = 0;
//        }
        
        RH3_LED = card_flag;
        RD7_LED = move;
        TURNLEFT_LED = start;
        
        
        //prevents false flag trips at start
        if(start==0 && card_flag){ 
            card_flag = 0;
            start = 1;
            __delay_ms(100);
        }

        if(move<1 && start>0){
            __delay_ms(100);
            forward(&motorL, &motorR);
            if(card_flag==1){
                stop(&motorL, &motorR);
                __delay_ms(60);
                move = 1;
            }
        }
        
        if (card_flag==1) {
            __delay_ms(100);
            current = colorclick_readColour(current); //read current light value
            read_card(initial, current, &motorL, &motorR);
            __delay_ms(500);
            current = colorclick_readColour(current);
            clear_lower = current.C - 10;
            clear_upper = current.C + 10;
            card_flag = 0;
            move = 0;
        }
                
        
    }
}
