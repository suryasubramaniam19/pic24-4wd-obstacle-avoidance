#include <p24Fxxxx.h>
#include <xc.h>
#include <stdlib.h>
#include "MotorDirection.h"
#include "Final_Project_asmLib.h"
#include "HC06_slave.h"
#include "Subra_FinalProject_UltrasonicSensor.h"


#pragma config ICS = PGx1          
#pragma config FWDTEN = OFF        
#pragma config GWRP = OFF          
#pragma config GCP = OFF           
#pragma config JTAGEN = OFF        
#pragma config POSCMOD = NONE      
#pragma config I2C1SEL = PRI       
#pragma config IOL1WAY = OFF       
#pragma config OSCIOFNC = ON       
#pragma config FCKSM = CSECME      
#pragma config FNOSC = FRCPLL

typedef enum{
    Mode_Manual,
                Mode_auto_obstacle
            
}SystemMode;

struct VehicleState{
    SystemMode currentMode;
    int Obstacle;
    char lastcommand;
};

struct VehicleState mycar = {Mode_Manual,  0,'S'};


int main(void) { 
    

    motor_init();
    bluetooth_setup();
    ultra_initial();
    
    Ultrasonic_Reset();
    initEchoCapture();
    int uart1_available(void);
    
    __builtin_enable_interrupts();
    
    // transmit data on UARTs
//    uart1_puts("SYSTEM_READY\r\n");
        char D = 'S';
        
                

    
    while (1) {

        obstacle_scan();    
        mycar.Obstacle = Obstacle_Detection();  // 1 exists, 0 non.
           
//                    if (U1STAbits.URXDA){         
            if(uart1_available()){
            
                D = uart1_getc();
            
            }
        
        if (mycar.Obstacle == 1 && D == 'F') {
            Move_cmd('S');
            mycar.currentMode = Mode_auto_obstacle;
        }

        
                  
            switch(mycar.currentMode){
                case Mode_auto_obstacle:
//                uart1_puts("Entering auto avoiding obstacle mode");
                avoid_obstacles();
                if( D == 'F' || D == 'B'|| D == 'R' || D == 'L' || D == 'S'){
                    if(mycar.Obstacle == 0){
                        mycar.currentMode = Mode_Manual;
                    }
                }
                
                break;      
                
                case Mode_Manual:
                    
//                uart1_puts("Entering player operate mode");

                    if(mycar.Obstacle == 1 && D == 'F'){
                        Move_cmd('S');
                        mycar.currentMode = Mode_auto_obstacle;
                    }
                    else{
                        Move_cmd(D);
                    }

                break;
                default:
                    mycar.currentMode = Mode_Manual;
                break;
            }

//version idea
//        // if obstacle encountered,change to auto avoid_obstacles mode                      
//        if(Obstacle == 1){
//            uart1_puts("OBSTACLE!\r\n");
//            avoid_obstacles();
//            D = 'S';
//        }
//////                
//        else{
//////                   
//            if( D == 'F' || D == 'B'|| D == 'R' || D == 'L' || D == 'S'){ // check if the key pressed is operating key
//                Move_cmd(D);  
//            }
//            else{
//                autodrive(); // if you didn't press operating key, it goes automatically
//            }
//        }
////
//}
}
}