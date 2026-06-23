
#ifndef HC05_MASTER_H
#define	HC05_MASTER_H

#include <xc.h> // include processor files - each processor file is guarded.  

void bluetooth_setup(void);
void send_remote_cmd(char cmd);
unsigned char uart1_getc(void);
void uart1_putc(const unsigned char c);
void uart1_puts(char s[]);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

