#include "xc.h"


volatile unsigned char buffer[64];
volatile unsigned char front = 0;
volatile unsigned char back = 0;

#define FORWARD  'F'
#define BACK     'B'
#define LEFT     'L'
#define RIGHT    'R'
#define STOP     'S'



//void send_remote_cmd(char cmd) {
//    while(U1STAbits.UTXBF); 
//    U1TXREG = cmd;         
//    
//}

void __attribute__((__interrupt__, auto_psv)) _U1RXInterrupt(void) {
    IFS0bits.U1RXIF = 0;      
//    char data = U1RXREG; 
        while(U1STAbits.URXDA)
    {
        buffer[front++] = U1RXREG;
        front &= 63;
    }
//    send_remote_cmd(data);    
}



void bluetooth_setup(void)
{
    CLKDIVbits.RCDIV = 0;
//  AD1PCFG = 0x9FFF;
    AD1PCFG = 0xFFF0;
    _TRISB6 = 0;   // U1TX  
    _TRISB10 = 1;  // U1RX

    __builtin_write_OSCCONL(OSCCON & 0xBF); // unlock PPS
    _RP6R = 3;     // RB6 -> U1TX
    _U1RXR = 10;   // RB10 -> U1RX
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock PPS

    U1MODE = 0;
    U1STA = 0;

    U1MODEbits.BRGH =0;
    U1BRG =103;//9600 baud, FCY = 16MHz

    U1MODEbits.UEN = 0;
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;

    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1;
    
    IPC2bits.U1RXIP = 4;
}

void uart1_putc(const unsigned char c)
{
    while (!IFS0bits.U1TXIF);
    IFS0bits.U1TXIF = 0;
    U1TXREG = c;
}
unsigned char uart1_getc(void)
{
    unsigned char x;

    while (front == back);
    x = buffer[back++];
    back &= 63;
    return x;
}

void uart1_puts(char s[])
{
    int i = 0;

    while (s[i] != '\0')
    {
        uart1_putc(s[i]);
        i++;
    }
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