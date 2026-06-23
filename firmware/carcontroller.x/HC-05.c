#include "xc.h"
#pragma config ICS = PGx1 
#pragma config FWDTEN = OFF 
#pragma config GWRP = OFF 
#pragma config GCP = OFF 
#pragma config JTAGEN = OFF 
#pragma config I2C1SEL = PRI 
#pragma config IOL1WAY = OFF 
#pragma config OSCIOFNC = ON 
#pragma config FCKSM = CSECME 
#pragma config FNOSC = FRCPLL 



void send_remote_cmd(char cmd) {
    while(U1STAbits.UTXBF); 
    U1TXREG = cmd;         
    
}

void __attribute__((__interrupt__, auto_psv)) _U1RXInterrupt(void) {
    IFS0bits.U1RXIF = 0;      
    char data = U1RXREG; 
    send_remote_cmd(data);    
}

void setup(){
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0x9fff;  // For digital I/O. 
    _TRISB6 = 0;  // U1TX output
    _TRISB10 = 1; // U1RX input

    U1MODE = 0;  // UEN<1:0> bits control the pins
 
    U1MODEbits.BRGH = 1;
    U1BRG = 416; // 9600 baud (check the calculation with the datasheet)
    U1MODEbits.UEN = 0;
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;

    // Peripheral Pin Select 
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    _RP6R = 0x0003;   //RB6->UART1:U1TX; See Table 10-3 on P109 of the datasheet
    _U1RXR = 10;   //RB10->UART1:U1RX;
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS

    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1;
}



//int main(void) {
//    setup();
//    
//    while(1) {
//        send_remote_cmd('H');  // 'H'
//        send_remote_cmd('E');  // 'E'
//        send_remote_cmd('L');  //'L'
//        send_remote_cmd('P');  //'P'
//        send_remote_cmd('\n'); // 
//        
//        
//        for(long i=0; i<500000; i++) {       
//            Nop(); 
//        } 
//    }
//}