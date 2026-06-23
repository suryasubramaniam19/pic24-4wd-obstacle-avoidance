           //Author Block:
//Date: 04/19/2026
//Name: Surya Subramaniam
//Student ID Number: 6030087
//Course Number: EE 2361
//Term: Spring 2026
//Lab/Assignment Number: Final Project
//This library uses initializes the HC-SR04 Ultrasonic Sensor using the procedure given in the datasheet
//It contains a function handle the sensor's output using the PIC24's input capture

#include "xc.h"
#include "Subra_FinalProject_UltrasonicSensor.h"
#include "SUBRA_FINALPROJECT_ASMLIB_V001.h"
#include <stdio.h>

volatile int overflow = 0; //Counts overflows on timer 2
volatile static int currentState = 0; //Global variable to track current state in input capture ISR. If 0, we have received no edges and haven't sent the trigger yet
//If 1, we've sent the trigger and are waiting for a rising edge
//If 2, we have just received a rising edge and are now waiting for a negative edge.
//if 3, we have received a negative edge and can calculate the total echo time to see object distance, reset everything to wait for another rising edge
volatile unsigned long buf[2]; //Declare buffer array with size 2 to store rising and falling edge values measured on input capture
volatile int obstacle_flag = 0; 
unsigned long echoTime; //Stores total echo time based on difference between rising and falling edge


void ultra_initial(void){
    //Initialize PIC24
    CLKDIVbits.RCDIV = 0;  //Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    AD1PCFG = 0xffff; //All pins digital
    TRISBbits.TRISB7 = 0; //Set RB6 to output. Used for initial trigger input to HC-SR04

}

void triggerInput(){ //Sends initial trigger to sensor to start ultrasonic pulse
    LATBbits.LATB7 = 1; //Set initial high of trigger pulse
    delay(10); //Pulse should be high for 10 us to trigger HC-SR04
    LATBbits.LATB7 = 0; //Set pulse low
}

void initEchoCapture(){
    //Sets up timer 2 and input capture 1 for timestamps of edges
    TRISBbits.TRISB8 = 1; //RB8 takes the echo input from HC-SR04
    //CNPU2bits.CN22PUE = 1; //Enable pull up resistor for RB8
    
    //Set up timer 2 to overflow once per 10 milliseconds
    T2CON = 0;
    T2CONbits.TCKPS = 0b01; //Prescaler 8
    PR2 = 19999; //10 milliseconds
    TMR2 = 0;
    
    //Enable timer 2 interrupt
    _T2IF = 0; //Clear interrupt flag
    _T2IE = 1; //Enable interrupt
    T2CONbits.TON = 1;
    
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPINR7bits.IC1R = 8;  //Input Compare 1 to RB8
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
    
    //Input capture 1 setup
    IC1CON = 0; //Reset internal state of IC1
    IC1CONbits.ICTMR = 1; //Use Timer2 as capture timer
    IC1CONbits.ICM = 0b000; //Keep capture off initially
    _IC1IF = 0;
    _IC1IE = 1;
}

void Ultrasonic_Reset(void) {
    currentState = 0;
    buf[0] = 0;
    buf[1] = 889; 
}

void __attribute__((interrupt, auto_psv)) _T2Interrupt(void){
    //Clear timer 2 interrupt flag and increment and track overflows
    _T2IF = 0;
    overflow++;
}
void __attribute__((interrupt, auto_psv)) _IC1Interrupt(void){
    //Handles input capture events from the button
    unsigned int capture; //Captured TMR2 value at the edge
    unsigned long currentTime; //Extended time including overflows 
    _IC1IF = 0; //Reset flag
    
    if (currentState == 1){ //This was a rising edge
        capture = IC1BUF;
        if (_T2IF == 1){ //In case timer overflows right when input capture interrupts, must account for it
            _T2IF = 0;
            overflow++;
        }
        currentTime = (unsigned long)capture + ((unsigned long)(PR2 + 1) * (unsigned long)overflow);
        buf[0] = currentTime;
        currentState++;
    }
    
    else if(currentState == 2){ //This was a falling edge
        capture = IC1BUF;
        if (_T2IF == 1){ //In case timer overflows right when input capture interrupts, must account for it
            _T2IF = 0;
            overflow++;
        }
        currentTime = (unsigned long)capture + ((unsigned long)(PR2 + 1) * (unsigned long)overflow);
        buf[1] = currentTime;
        IC1CONbits.ICM = 0; //Stop capturing
        currentState++;
    }
}

void delay(int delay_in_us){
    //Calls assembly function that delays for a 1 us. Iterates depending on parameter
    while (delay_in_us > 0){
        wait1us();
        delay_in_us--;
    }
} 

void delayms(int delay_in_ms){
    //Calls assembly function that delays for a 1 ms. Iterates depending on parameter
    while (delay_in_ms > 0){
        wait1ms();
        delay_in_ms--;
    }
} 



int Obstacle_Detection(void) {
    return obstacle_flag;
}

void obstacle_scan(void){
    
        if (currentState == 0){ //Ready to sample new edges. 
            //Reset values
            overflow = 0;
            TMR2 = 0;
            _IC1IF = 0;
            triggerInput();
            IC1CONbits.ICM = 0b001; //Capture every edge, rising and falling
            currentState++;
        }
        
        else if (currentState == 3){ //All required edges have been sampled
            echoTime = buf[1] - buf[0];
            if (echoTime < 888){ //Datasheet says echo time (us)/148 gives total inches away from obstacle. 444 us = 3 inches of distance 7104 clock cycles = 888 timer ticks. Added lower bound of 50 for noise
                //Stop moving, object was detected
                obstacle_flag = 1;
                
            }
            
            else{
                obstacle_flag = 0; //safe zone means we can operate with manual mode
            }
            
            //Reset all values
            currentState = 0;
            buf[0] = 0;
            buf[1] = 889;
            overflow = 0;
            TMR2 = 0;
            delayms(10); //HC-SR04 requires 10 ms delay until next trigger pulse
           
        }
}
