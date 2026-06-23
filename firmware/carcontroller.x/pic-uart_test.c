//#include "xc.h"
//#pragma config ICS = PGx1 
//#pragma config FWDTEN = OFF 
//#pragma config GWRP = OFF 
//#pragma config GCP = OFF 
//#pragma config JTAGEN = OFF 
//#pragma config I2C1SEL = PRI 
//#pragma config IOL1WAY = OFF 
//#pragma config OSCIOFNC = ON 
//#pragma config FCKSM = CSECME 
//#pragma config FNOSC = FRCPLL 
//
//void setup(void)
//{
//    CLKDIVbits.RCDIV = 0;
//    AD1PCFG = 0x9FFF;
//
//    _TRISB6 = 0;   // U1TX  
//    _TRISB10 = 1;  // U1RX
//
//    __builtin_write_OSCCONL(OSCCON & 0xBF); // unlock PPS
//    _RP6R = 3;     // RB6 -> U1TX
//    _U1RXR = 10;   // RB10 -> U1RX
//    __builtin_write_OSCCONL(OSCCON | 0x40); // lock PPS
//
//    U1MODE = 0;
//    U1STA = 0;
//
//    U1MODEbits.BRGH = 0;
//    U1BRG = 103;   // 9600 baud, FCY = 16MHz
//
//    U1MODEbits.UEN = 0;
//    U1MODEbits.UARTEN = 1;
//    U1STAbits.UTXEN = 1;
//
//    IFS0bits.U1RXIF = 0;
//    IEC0bits.U1RXIE = 1;
//
//}
//
//
//void uart1_putc(char c)
//{
//    while (U1STAbits.UTXBF);
//    U1TXREG = c;
//}
//
//char uart1_getc(void)
//{
//    while (!U1STAbits.URXDA);
//    return U1RXREG;
//}
//
//int main(void)
//{
//    char x;
//
//    setup();
//
//    while (1)
//    {
//        x = uart1_getc();
//        uart1_putc(x);   // echo back
//    }
//
//    return 0;
//}
//#include "xc.h"
//
//#pragma config ICS = PGx1
//#pragma config FWDTEN = OFF
//#pragma config GWRP = OFF
//#pragma config GCP = OFF
//#pragma config JTAGEN = OFF
//#pragma config I2C1SEL = PRI
//#pragma config IOL1WAY = OFF
//#pragma config OSCIOFNC = ON
//#pragma config FCKSM = CSECME
//#pragma config FNOSC = FRCPLL
//
//volatile char tx_char = 0;
//volatile char rx_char = 0;
//volatile unsigned int ok_count = 0;
//volatile unsigned int err_count = 0;
//
//void uart1_init_loopback(void)
//{
//    CLKDIVbits.RCDIV = 0;
//    AD1PCFG = 0x9FFF;
//
//    _TRISB6 = 0;    // U1TX
//    _TRISB10 = 1;   // U1RX
//
//    __builtin_write_OSCCONL(OSCCON & 0xBF);   // unlock PPS
//    _RP6R = 3;      // RB6 -> U1TX
//    _U1RXR = 10;    // RB10 -> U1RX
//    __builtin_write_OSCCONL(OSCCON | 0x40);   // lock PPS
//
//    U1MODE = 0;
//    U1STA = 0;
//
//    U1MODEbits.BRGH = 0;      // standard speed
//    U1BRG = 103;              // 9600 baud if FCY = 16 MHz
//    U1MODEbits.LPBACK = 1;    // ???????
//    U1MODEbits.UARTEN = 1;
//    U1STAbits.UTXEN = 1;
//}
//
//int main(void)
//{
//    uart1_init_loopback();
//
//    while (1)
//    {
//        tx_char = 'A';
//
//        while (U1STAbits.UTXBF);   // ???????
//        U1TXREG = tx_char;         // ???
//
//        while (!U1STAbits.URXDA);  // ???????
//        rx_char = U1RXREG;         // ???
//
//        if (rx_char == tx_char)
//            ok_count++;
//        else
//            err_count++;
//
//        for (volatile long i = 0; i < 50000; i++);
//    }
//
//    return 0;
//}

//#include "xc.h"
//
//#pragma config ICS = PGx1
//#pragma config FWDTEN = OFF
//#pragma config GWRP = OFF
//#pragma config GCP = OFF
//#pragma config JTAGEN = OFF
//#pragma config I2C1SEL = PRI
//#pragma config IOL1WAY = OFF
//#pragma config OSCIOFNC = ON
//#pragma config FCKSM = CSECME
//#pragma config FNOSC = FRCPLL
//
//void uart1_init(void)
//{
//    CLKDIVbits.RCDIV = 0;
//    AD1PCFG = 0x9FFF;
//
//    _TRISB6 = 0;
//    _TRISB10 = 1;
//
//    __builtin_write_OSCCONL(OSCCON & 0xBF);
//    _RP6R = 3;      // RB6 -> U1TX
//    _U1RXR = 10;    // RB10 -> U1RX
//    __builtin_write_OSCCONL(OSCCON | 0x40);
//
//    U1MODE = 0;
//    U1STA = 0;
//
//    U1MODEbits.BRGH = 0;
//    U1BRG = 103;    // 9600 @ FCY=16MHz
//    U1MODEbits.UARTEN = 1;
//    U1STAbits.UTXEN = 1;
//}
//
//void uart1_putc(char c)
//{
//    while (U1STAbits.UTXBF);
//    U1TXREG = c;
//}
//
//int main(void)
//{
//    uart1_init();
//
//    while (1)
//    {
//        uart1_putc('U');
//        for (volatile long i = 0; i < 200000; i++);
//    }
//
//    return 0;
//}

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

volatile unsigned char last_rx = 0;
volatile unsigned int rx_count = 0;

void uart1_init(void)
{
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0x9FFF;

    _TRISB6 = 0;    // U1TX
    _TRISB10 = 1;   // U1RX

    __builtin_write_OSCCONL(OSCCON & 0xBF);
    _RP6R = 3;      // RB6 -> U1TX
    _U1RXR = 10;    // RB10 -> U1RX
    __builtin_write_OSCCONL(OSCCON | 0x40);

    U1MODE = 0;
    U1STA = 0;

    U1MODEbits.BRGH = 0;
    U1BRG = 103;    // 9600 @ FCY = 16MHz
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;
}

int main(void)
{
    uart1_init();

    while (1)
    {
        if (U1STAbits.URXDA)
        {
            last_rx = U1RXREG;
            rx_count++;
        }
    }

    return 0;
}