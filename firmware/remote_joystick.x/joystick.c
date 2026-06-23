/*
 * File:   PIC_Joystick.c
 * Author: Tarakesh Rameshkumar
 * This program can read the values from a joy stick and control the LED based on the values
 * Created on 20 April, 2026, for EE2361 Final Project ;
 */

// CONFIG


#include <xc.h>
#include "final_project_asmLib.h"

#define SW          _RB4
#define LED_LEFT    _LATB5
#define LED_CENTER  _LATB6
#define LED_RIGHT   _LATB7
#define LED_TOP     _LATB8
#define LED_BOTTOM  _LATB9

void delay(int delay_in_ms) {
    for (int num_ms = delay_in_ms; num_ms > 0; num_ms--)
        wait_tr_1ms();
}

void initADC(void) {
    AD1PCFG = 0xFFFC;    // Start by setting all to digital
//    AD1PCFGbits.PCFG0 = 0; // Set AN0 (Pin 2) to Analog
//    AD1PCFGbits.PCFG1 = 0; // Set AN1 (Pin 3) to Analog
//    AD1PCFGbits.PCFG2 = 0; // Set AN2 (Pin 4) to Analog
//    AD1PCFGbits.PCFG3 = 0; // Set AN3 (Pin 5) to Analog
    
    AD1CON1 = 0x00E0; // Auto-convert mode
    AD1CON2 = 0x0000; // Use VDD/VSS as reference
    AD1CON3 = 0x1F02; // Sample time setup
    AD1CON1bits.ADON = 1; // Turn on ADC
}

unsigned int readADC(int channel) {
    AD1CHS = channel;
    AD1CON1bits.SAMP = 1;         // Start sampling
    while (!AD1CON1bits.DONE);    // Wait for conversion
    return ADC1BUF0;              // Return 10-bit value (0-1023)
}


char joystick_cmd(void){

        unsigned int xVal = readADC(0);
        unsigned int yVal = readADC(1);
        
        // Y-Logic
        if (yVal > 800)      {

            return 'R'; 

        }
        else if (yVal < 200) { 

            return 'L'; 

        }
//        else                 { 

//            return 'S'; 
//        }
        
        // X-Logic
        if (xVal > 800)      
        { 


        return 'F';
        }
        else if (xVal < 200) {

            return 'B'; 
        }
//        else                 { 

//            return 'F'; 
//        }

        

        // Switch Logic
        if (SW == 0) return 'S';
        
        
// change speed hadn't finish yet.        
//        unsigned int xVal2 = readADC(2);
//        unsigned int yVal2 = readADC(3);
//        
//        // X-Logic
//        if (xVal2 > 800)      { LED_RIGHT = 1; LED_LEFT = 0; }
//        else if (xVal2 < 200) { LED_RIGHT = 0; LED_LEFT = 1; }
//        else                 { LED_RIGHT = 0; LED_LEFT = 0; }
//
//        // Y-Logic
//        if (yVal2 > 800)      { LED_TOP = 1; LED_BOTTOM = 0; }
//        else if (yVal2 < 200) { LED_TOP = 0; LED_BOTTOM = 1; }
//        else                 { LED_TOP = 0; LED_BOTTOM = 0; }
//
//        // Switch Logic
//        if (SW == 0) LED_CENTER = 1;
//        else         LED_CENTER = 0;
        
        return 'S'; 
}



//int main(void) {
//    
//    initADC();
//    TRISB &= ~((1<<5) | (1<<6) | (1<<7) | (1<<8) | (1<<9));
//    
//    TRISBbits.TRISB4 = 1; 
//    CNPU1bits.CN1PUE = 1;
//    
//    while (1) {
//        unsigned int xVal = readADC(0);
//        unsigned int yVal = readADC(1);
//        
//        // X-Logic
//        if (xVal > 800)      { LED_RIGHT = 1; LED_LEFT = 0; }
//        else if (xVal < 200) { LED_RIGHT = 0; LED_LEFT = 1; }
//        else                 { LED_RIGHT = 0; LED_LEFT = 0; }
//
//        // Y-Logic
//        if (yVal > 800)      { LED_TOP = 1; LED_BOTTOM = 0; }
//        else if (yVal < 200) { LED_TOP = 0; LED_BOTTOM = 1; }
//        else                 { LED_TOP = 0; LED_BOTTOM = 0; }
//
//        // Switch Logic
//        if (SW == 0) LED_CENTER = 1;
//        else         LED_CENTER = 0;
//        
//        
//        
//        unsigned int xVal2 = readADC(2);
//        unsigned int yVal2 = readADC(3);
//        
//        // X-Logic
//        if (xVal2 > 800)      { LED_RIGHT = 1; LED_LEFT = 0; }
//        else if (xVal2 < 200) { LED_RIGHT = 0; LED_LEFT = 1; }
//        else                 { LED_RIGHT = 0; LED_LEFT = 0; }
//
//        // Y-Logic
//        if (yVal2 > 800)      { LED_TOP = 1; LED_BOTTOM = 0; }
//        else if (yVal2 < 200) { LED_TOP = 0; LED_BOTTOM = 1; }
//        else                 { LED_TOP = 0; LED_BOTTOM = 0; }
//
//        // Switch Logic
//        if (SW == 0) LED_CENTER = 1;
//        else         LED_CENTER = 0;
//    }
//}