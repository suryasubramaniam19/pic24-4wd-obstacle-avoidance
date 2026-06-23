#include "xc.h"
#include "Final_Project_asmLib.h"
#include "Subra_FinalProject_UltrasonicSensor.h"

#define AIN1 LATBbits.LATB0 
#define AIN2 LATBbits.LATB1
#define BIN1 LATBbits.LATB2
#define BIN2 LATBbits.LATB3



void motor_init(){
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    TRISAbits.TRISA2 = 0;
    TRISBbits.TRISB15 = 0; 
    TRISBbits.TRISB14 = 0; 
    
    LATAbits.LATA2 = 1;
    
    __builtin_write_OSCCONL(OSCCON & 0xbf); 
    _RP15R = 18;  
    _RP14R = 19;   
    __builtin_write_OSCCONL(OSCCON | 0x40); 
    
    //Timer3 take 4kHz
    TMR3 = 0;
    T3CON = 0;      
    T3CONbits.TCKPS = 0b00;//0b 
    PR3 = 3999;// = 1/(4000*62.5ns*1)-1
    T3CONbits.TON = 1;;
    
    //Output Compare1 50% dutyCycle 
    OC1CON = 0;    // turn off OC1 for now    PWM = (PR3+1)*Tcy*Prescaler, Dutycycle = OC1R*Tcy*Prescaler
    OC1R = 2000;   // OC1R/(PR3+1) = DutyCycle/PWM
    OC1RS = 2000;  // We will only change this once PWM is turned on
    OC1CONbits.OCTSEL = 1; // Use Timer 3 for compare source
    OC1CONbits.OCM = 0b110; // Output compare PWM w/o faults
    
    //Output Compare2 50% dutyCycle 
    OC2CON = 0;   
    OC2R = 2000;   
    OC2RS = 2000;  
    OC2CONbits.OCTSEL = 1; 
    OC2CONbits.OCM = 0b110; 
}

void set_car_speed(int left_speed, int right_speed){
    if( left_speed > 3999 ) left_speed = 3999;
    if( right_speed > 3999 ) right_speed = 3999;
    
    OC1RS = left_speed;
    OC2RS = right_speed;
 
}

void car_forward(int speed){
    AIN1 = 0;
    AIN2 = 1;
    BIN1 = 1;
    BIN2 = 0;
    
    set_car_speed(speed,speed);
}

void car_backward(int speed){
    AIN1 = 1;
    AIN2 = 0;
    BIN1 = 0;
    BIN2 = 1;
    
    set_car_speed(speed,speed);
}

void car_turn_left(int speed){
    AIN1 = 1;
    AIN2 = 0;
    BIN1 = 1;
    BIN2 = 0;
    
    set_car_speed( speed , speed );
}

void car_turn_right(int speed){
    AIN1 = 0;
    AIN2 = 1;
    BIN1 = 0;
    BIN2 = 1;
    
    set_car_speed( speed + 500 , speed );
}

void car_stop(int speed){
    AIN1 = 0;
    AIN2 = 0;
    BIN1 = 0;
    BIN2 = 0;
    
    set_car_speed( speed , speed );
}

//void autodrive(void){        
//        obstacle_scan();
//    if(Obstacle_Detection() == 0){        
//        car_forward(2000);  
//        
//        for(int i = 0; i < 3000; ++i){
//           yyw_wait_1ms();
//        }
//        
//        car_turn_left(2000);
//        
//        for(int i = 0; i < 3000; ++i){
//            yyw_wait_1ms();
//        }
//       
//        car_turn_right(2000);
//       
//        for(int i = 0; i < 3000; ++i){
//            yyw_wait_1ms();
//        }
//        
//        car_backward(2000);
//        
//        for(int i = 0; i < 3000; ++i){
//            yyw_wait_1ms();
//        }
//    }
//    else{
//        car_stop(0);  
//        for(int i = 0; i < 3000; ++i){
//            yyw_wait_1ms();
//        }    
//    }
//    
//}

void avoid_obstacles(void){
             
        car_stop(0);  
        for(int i = 0; i < 1000; ++i){
            yyw_wait_1ms();
        }
        
        car_backward(1500);  
        for(int i = 0; i < 1000; ++i){
           yyw_wait_1ms();    
        }

        car_turn_left(2500);
        for(int i = 0; i < 500; ++i){
            yyw_wait_1ms();
        }
        car_stop(0);  
        obstacle_scan();
        for(int i = 0; i < 500; ++i){
            yyw_wait_1ms();
        }
        if(Obstacle_Detection() == 1){
            return;
        }
        car_forward(1500);  
        for(int i = 0; i < 1000; ++i){
           yyw_wait_1ms();    
        }
        car_stop(0);
        for(int i = 0; i < 500; ++i){
            yyw_wait_1ms();
        }
        if(Obstacle_Detection() == 1){
            return;
        }
        
        obstacle_scan();
    if(Obstacle_Detection() == 1) {
        car_stop(0);
        return;
    }
}



void autodrive(void){        
        car_forward(2000);
        for(int i = 0; i < 3000; ++i){
           yyw_wait_1ms();
    }    
}