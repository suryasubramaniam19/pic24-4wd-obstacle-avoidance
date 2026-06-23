#include "xc.h"
#include "JOYSTICK.h"
#include "HC05_MASTER.h"

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


int main(void) {

    initADC();
    bluetooth_setup();

    TRISB &= ~((1<<5) | (1<<6) | (1<<7) | (1<<8) | (1<<9));
  
    TRISBbits.TRISB4 = 1; 
    CNPU1bits.CN1PUE = 1;
    
    char last_cmd = ' ';
    
    while(1) { 
        
        char current_cmd = joystick_cmd();
        if (current_cmd != last_cmd) { // only transit when logic was changed.
            uart1_putc(current_cmd);
            last_cmd = current_cmd;
        }
        
        for (int i = 0; i < 20000; i++){ 
            i = i;
        }
    }
    return 0;
}
