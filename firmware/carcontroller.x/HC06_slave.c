#include "xc.h"
#include "MotorDirection.h"
#include "HC06_slave.h"

volatile unsigned char buffer[64];
volatile unsigned char front = 0;
volatile unsigned char back = 0;

#define FORWARD  'F'
#define BACK     'B'
#define LEFT     'L'
#define RIGHT    'R'
#define STOP     'S'

#define DEFAULT_SPEED 2500

void bluetooth_setup(void)
{
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0x9FFF;

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

// interrupt version
void __attribute__((__interrupt__,__auto_psv__)) _U1RXInterrupt(void)
{
    IFS0bits.U1RXIF = 0;
    while(U1STAbits.URXDA)
    {
        buffer[front++] = U1RXREG;
        front &= 63;
    }
}

unsigned char uart1_getc(void)
{
    unsigned char x;

    while (front == back);
    x = buffer[back++];
    back &= 63;
    return x;
}

void uart1_putc(const unsigned char c)
{
    while (!IFS0bits.U1TXIF);
    IFS0bits.U1TXIF = 0;
    U1TXREG = c;
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

void Move_cmd(char direction){
    switch(direction){
        case FORWARD:
            car_forward(DEFAULT_SPEED);
            uart1_puts("Moving Forward...\r\n"); 
            break;
        case BACK:
            car_backward(DEFAULT_SPEED);
            uart1_puts("Moving Backward...\r\n");
            break;
        case LEFT:
            car_turn_left(DEFAULT_SPEED);
            uart1_puts("Turning Left...\r\n");
            break;
        case RIGHT:
            car_turn_right(DEFAULT_SPEED);
            uart1_puts("Turning Right...\r\n");
            break;
        case STOP:
            car_stop(DEFAULT_SPEED);
            uart1_puts("Stopping...\r\n");
            break;
        default:
             uart1_putc('?');
            // do nothing
            break;
    }
}

int uart1_available(void) {
    return (front != back); 
}

//interrupt version 
//int main(void) {
//    
//    setup(); 
//    
//    __builtin_enable_interrupts();
//    
//// transmit data on UART
//    uart1_puts("SYSTEM_READY\r\n");
//    while (1) {
//        while(front != back){
//            
//            char D = uart1_getc();
//            
//            Move_cmd(D);
//        }
//    }
//    return 0;
//    }

//// polling version
//int main(void)
//{
//    setup();
//    
//// transmit data on UART
//    uart1_puts("SYSTEM_READY\r\n");
//
//    while (1)
//    {
//        char D = uart1_getc();
//        Move_cmd(D);
//    }
//}